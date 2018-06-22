/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlegroundPackets.h"
#include "BattlegroundScore.h"
#include "BattlegroundScript.h"
#include "ChatTextBuilder.h"
#include "Creature.h"
#include "CreatureTextMgr.h"
#include "DatabaseEnv.h"
#include "Formulas.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Log.h"
#include "Map.h"
#include "MiscPackets.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "SpellAuras.h"
#include "Transport.h"
#include "Util.h"
#include "WorldStatePackets.h"
#include <cstdarg>

template<class Do>
void Battleground::BroadcastWorker(Do& _do)
{
    for (BattlegroundPlayerMap::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
        if (Player* player = GetPlayer(itr, "BroadcastWorker"))
            _do(player);
}

Battleground::Battleground(BattlegroundTemplate const* battlegroundTemplate, PVPDifficultyEntry const* bracket, uint32 instanceId, uint32 clientInstanceId, BattlegroundTypeId originalTypeId, bool rated) :
    Events(0), StartDelayTimes{ BG_START_DELAY_2M, BG_START_DELAY_1M, BG_START_DELAY_30S, BG_START_DELAY_NONE }, StartMessageIds{ BG_TEXT_START_TWO_MINUTES, BG_TEXT_START_ONE_MINUTE, BG_TEXT_START_HALF_MINUTE, BG_TEXT_BATTLE_HAS_BEGUN },
    HonorMode(BG_NORMAL), TeamScores{ 0, 0 }, _typeId(originalTypeId), _randomTypeId(battlegroundTemplate->Id), _instanceId(instanceId), _clientInstanceId(clientInstanceId), _status(STATUS_NONE), _startTime(0), _countdownTimer(0),
    _resetStatTimer(0), _validStartPositionTimer(0), _endTime(0), _lastResurrectTime(0), _prematureCountDown(false), _prematureCountDownTimer(0), _lastPlayerPositionBroadcast(0), _startDelayTime(0), _inBGFreeSlotQueue(false),
    _setDeleteThis(false), _winnerTeamId(BG_TEAM_NEUTRAL), _isRated(rated), _queueId(uint64(battlegroundTemplate->Id) | UI64LIT(0x1F10000000000000)), _invitedAlliance(0), _invitedHorde(0), _bgRaids{ nullptr, nullptr },
    _playersCount{ 0, 0 }, _battlegroundTemplate(battlegroundTemplate), _pvpDifficultyEntry(bracket), _map(nullptr) { }

Battleground::Battleground(BattlegroundTemplate const* battlegroundTemplate) :
    Battleground(battlegroundTemplate, nullptr, 0, 0, battlegroundTemplate->Id, false) { }

Battleground::~Battleground()
{
    sBattlegroundMgr->RemoveBattleground(GetTypeId(), GetInstanceId());
    // unload map
    if (_map)
    {
        _map->SetUnload();
        //unlink to prevent crash, always unlink all pointer reference before destruction
        _map->SetBG(nullptr);
        _map = nullptr;
    }
    // remove from bg free slot queue
    RemoveFromBGFreeSlotQueue();

    for (BattlegroundScoreMap::const_iterator itr = PlayerScores.begin(); itr != PlayerScores.end(); ++itr)
        delete itr->second;
}

void Battleground::Update(uint32 diff)
{
    if (!GetPlayersSize())
    {
        //BG is empty
        // if there are no players invited, delete BG
        // this will delete arena or bg object, where any player entered
        // [[   but if you use battleground object again (more battles possible to be played on 1 instance)
        //      then this condition should be removed and code:
        //      if (!GetInvitedCount(HORDE) && !GetInvitedCount(ALLIANCE))
        //          this->AddToFreeBGObjectsQueue(); // not yet implemented
        //      should be used instead of current
        // ]]
        // Battleground Template instance cannot be updated, because it would be deleted
        if (!GetInvitedCount(HORDE) && !GetInvitedCount(ALLIANCE))
            _setDeleteThis = true;
        return;
    }

    switch (GetStatus())
    {
        case STATUS_WAIT_JOIN:
            if (GetPlayersSize())
            {
                ProcessJoin(diff);
                CheckSafePositions(diff);
                GetBattlegroundScript()->OnUpdateWaitJoin(diff);
            }
            break;
        case STATUS_IN_PROGRESS:
            ProcessOfflineQueue();
            ProcessPlayerPositionBroadcast(diff);
            GetBattlegroundScript()->OnUpdateInProgress(diff);
            // after 47 minutes without one team losing, the arena closes with no winner and no rating change
            if (IsArena())
            {
                if (GetElapsedTime() >= 47 * MINUTE*IN_MILLISECONDS)
                {
                    EndBattleground(TEAM_OTHER);
                    return;
                }
            }
            else
            {
                ProcessResurrect(diff);
                if (sBattlegroundMgr->GetPrematureFinishTime() && (GetPlayersCountByTeam(ALLIANCE) < GetMinPlayersPerTeam() || GetPlayersCountByTeam(HORDE) < GetMinPlayersPerTeam()))
                    ProcessProgress(diff);
                else if (_prematureCountDown)
                    _prematureCountDown = false;
            }
            break;
        case STATUS_WAIT_LEAVE:
            ProcessLeave(diff);
            GetBattlegroundScript()->OnUpdateWaitLeave(diff);
            break;
        default:
            break;
    }

    // Update start time and reset stats timer
    //SetElapsedTime(GetElapsedTime() + diff); // TODO
    _startTime = GetElapsedTime() + diff;
    if (GetStatus() == STATUS_WAIT_JOIN)
    {
        _resetStatTimer += diff;
        _countdownTimer += diff;
    }

    GetBattlegroundScript()->OnUpdate(diff);
}

inline void Battleground::CheckSafePositions(uint32 diff)
{
    float const maxDist = _battlegroundTemplate->MaxStartDistSq;
    if (!maxDist)
        return;

    _validStartPositionTimer += diff;
    if (_validStartPositionTimer >= CHECK_PLAYER_POSITION_INVERVAL)
    {
        _validStartPositionTimer = 0;

        for (const auto& itr : GetPlayers())
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr.first))
            {
                if (player->IsGameMaster())
                    continue;

                Position pos = player->GetPosition();
                WorldSafeLocsEntry const* startLoc = _battlegroundTemplate->StartLocation[GetTeamIndexByTeamId(player->GetBGTeam())];
                if (pos.GetExactDistSq(startLoc->Loc.X, startLoc->Loc.Y, startLoc->Loc.Z) > maxDist)
                {
                    TC_LOG_DEBUG("bg.battleground", "BATTLEGROUND: Sending %s back to start location (map: %u) (possible exploit)", player->GetName().c_str(), GetMapId());
                    player->TeleportTo(startLoc);
                }
            }
        }
    }
}

void Battleground::ProcessPlayerPositionBroadcast(uint32 diff)
{
    _lastPlayerPositionBroadcast += diff;
    if (_lastPlayerPositionBroadcast >= PLAYER_POSITION_UPDATE_INTERVAL)
    {
        _lastPlayerPositionBroadcast = 0;

        WorldPackets::Battleground::BattlegroundPlayerPositions playerPositions;
        GetBattlegroundScript()->BuildPlayerPositionData(&playerPositions.FlagCarriers);
        SendPacketToAll(playerPositions.Write());
    }
}

inline void Battleground::ProcessOfflineQueue()
{
    // remove offline players from bg after 5 minutes
    if (!_offlineQueue.empty())
    {
        auto const itr = Players.find(*(_offlineQueue.begin()));
        if (itr != Players.end())
        {
            if (itr->second.OfflineRemoveTime <= sWorld->GetGameTime())
            {
                RemovePlayerAtLeave(itr->first, true, true);// remove player from BG
                _offlineQueue.pop_front();                 // remove from offline queue
                //do not use itr for anything, because it is erased in RemovePlayerAtLeave()
            }
        }
    }
}

inline void Battleground::ProcessResurrect(uint32 diff)
{
    // *********************************************************
    // ***        BATTLEGROUND RESURRECTION SYSTEM           ***
    // *********************************************************
    // this should be handled by spell system
    _lastResurrectTime += diff;
    if (_lastResurrectTime >= RESURRECTION_INTERVAL)
    {
        if (GetReviveQueueSize())
        {
            for (auto& itr : ReviveQueue)
            {
                Creature* sh = nullptr;
                for (GuidVector::const_iterator itr2 = (itr.second).begin(); itr2 != (itr.second).end(); ++itr2)
                {
                    Player* player = ObjectAccessor::FindPlayer(*itr2);
                    if (!player)
                        continue;

                    if (!sh && player->IsInWorld())
                    {
                        sh = player->GetMap()->GetCreature(itr.first);
                        // only for visual effect
                        if (sh)
                            // Spirit Heal, effect 117
                            sh->CastSpell(sh, SPELL_SPIRIT_HEAL, true);
                    }

                    // Resurrection visual
                    player->CastSpell(player, SPELL_RESURRECTION_VISUAL, true);
                    _resurrectQueue.push_back(*itr2);
                }
                (itr.second).clear();
            }

            ReviveQueue.clear();
            _lastResurrectTime = 0;
        }
        else
            // queue is clear and time passed, just update last resurrection time
            _lastResurrectTime = 0;
    }
    else if (_lastResurrectTime > 500)    // Resurrect players only half a second later, to see spirit heal effect on NPC
    {
        for (GuidVector::const_iterator itr = _resurrectQueue.begin(); itr != _resurrectQueue.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;
            player->ResurrectPlayer(1.0f);
            player->CastSpell(player, 6962, true);
            player->CastSpell(player, SPELL_SPIRIT_HEAL_MANA, true);
            player->SpawnCorpseBones(false);
        }
        _resurrectQueue.clear();
    }
}

BattlegroundScript* Battleground::GetBattlegroundScript() const
{
    ASSERT(GetBgMap()->GetBattlegroundScript());
    return GetBgMap()->GetBattlegroundScript();
}

inline void Battleground::ProcessProgress(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND BALLANCE SYSTEM            ***
    // *********************************************************
    // if less then minimum players are in on one side, then start premature finish timer
    if (!_prematureCountDown)
    {
        _prematureCountDown = true;
        _prematureCountDownTimer = sBattlegroundMgr->GetPrematureFinishTime();
    }
    else if (_prematureCountDownTimer < diff)
    {
        // time's up!
        EndBattleground(GetBattlegroundScript()->CalculatePrematureWinner());
        _prematureCountDown = false;
    }
    else if (!sBattlegroundMgr->isTesting())
    {
        uint32 const newtime = _prematureCountDownTimer - diff;
        // announce every minute
        if (newtime > (MINUTE * IN_MILLISECONDS))
        {
            if (newtime / (MINUTE * IN_MILLISECONDS) != _prematureCountDownTimer / (MINUTE * IN_MILLISECONDS))
                PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING, CHAT_MSG_SYSTEM, nullptr, (uint32)(_prematureCountDownTimer / (MINUTE * IN_MILLISECONDS)));
        }
        else
        {
            //announce every 15 seconds
            if (newtime / (15 * IN_MILLISECONDS) != _prematureCountDownTimer / (15 * IN_MILLISECONDS))
                PSendMessageToAll(LANG_BATTLEGROUND_PREMATURE_FINISH_WARNING_SECS, CHAT_MSG_SYSTEM, nullptr, (uint32)(_prematureCountDownTimer / IN_MILLISECONDS));
        }
        _prematureCountDownTimer = newtime;
    }
}

inline void Battleground::ProcessJoin(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND STARTING SYSTEM            ***
    // *********************************************************
    _startDelayTime -= diff;

    if (!IsArena())
        _endTime = 300000;

    if (_resetStatTimer > 5000)
    {
        _resetStatTimer = 0;
        for (const auto& itr : GetPlayers())
            if (Player* player = ObjectAccessor::FindPlayer(itr.first))
                player->ResetAllPowers();
    }

    if (!(Events & BG_STARTING_EVENT_1))
    {
        Events |= BG_STARTING_EVENT_1;

        if (!FindBgMap())
        {
            TC_LOG_ERROR("bg.battleground", "Battleground::_ProcessJoin: map (map id: %u, instance id: %u) is not created!", GetMapId(), _instanceId);
            EndNow();
            return;
        }

        GetBattlegroundScript()->InitStartMessageIds(StartMessageIds[BG_STARTING_EVENT_FIRST], StartMessageIds[BG_STARTING_EVENT_SECOND], StartMessageIds[BG_STARTING_EVENT_THIRD], StartMessageIds[BG_STARTING_EVENT_FOURTH]);

        // Setup here, only when at least one player has ported to the map
        if (!SetupBattleground())
        {
            EndNow();
            return;
        }

        _startDelayTime = StartDelayTimes[BG_STARTING_EVENT_FIRST];
        // First start warning - 2 or 1 minute
        if (StartMessageIds[BG_STARTING_EVENT_FIRST])
            SendBroadcastText(StartMessageIds[BG_STARTING_EVENT_FIRST], CHAT_MSG_BG_SYSTEM_NEUTRAL);
    }
    // After 1 minute or 30 seconds, warning is signaled
    else if (GetStartDelayTime() <= StartDelayTimes[BG_STARTING_EVENT_SECOND] && !(Events & BG_STARTING_EVENT_2))
    {
        Events |= BG_STARTING_EVENT_2;
        if (StartMessageIds[BG_STARTING_EVENT_SECOND])
            SendBroadcastText(StartMessageIds[BG_STARTING_EVENT_SECOND], CHAT_MSG_BG_SYSTEM_NEUTRAL);

        GetBattlegroundScript()->OnBattlegroundStartingEvent2();
    }
    // After 30 or 15 seconds, warning is signaled
    else if (GetStartDelayTime() <= StartDelayTimes[BG_STARTING_EVENT_THIRD] && !(Events & BG_STARTING_EVENT_3))
    {
        Events |= BG_STARTING_EVENT_3;
        if (StartMessageIds[BG_STARTING_EVENT_THIRD])
            SendBroadcastText(StartMessageIds[BG_STARTING_EVENT_THIRD], CHAT_MSG_BG_SYSTEM_NEUTRAL);

        GetBattlegroundScript()->OnBattlegroundStartingEvent3();
    }
    // Delay expired (after 2 or 1 minute)
    else if (GetStartDelayTime() <= 0 && !(Events & BG_STARTING_EVENT_4))
    {
        Events |= BG_STARTING_EVENT_4;

        if (StartMessageIds[BG_STARTING_EVENT_FOURTH])
            SendBroadcastText(StartMessageIds[BG_STARTING_EVENT_FOURTH], CHAT_MSG_BG_SYSTEM_NEUTRAL);
        SetStatus(STATUS_IN_PROGRESS);
        _startDelayTime = StartDelayTimes[BG_STARTING_EVENT_FOURTH];

        GetBattlegroundScript()->OnStart();
        for (ObjectGuid::LowType const& doorSpawnId : _battlegroundTemplate->Doors)
        {
            if (GameObjectData const* data = sObjectMgr->GetGOData(doorSpawnId))
            {
                sObjectMgr->RemoveGameobjectFromGrid(doorSpawnId, data);
                auto const bounds = _map->GetGameObjectBySpawnIdStore().equal_range(doorSpawnId);
                for (auto itr = bounds.first; itr != bounds.second;)
                {
                    GameObject* gameObject = itr->second;
                    ++itr;
                    gameObject->AddObjectToRemoveList();
                }
            }
        }

        // Remove preparation
        if (IsArena())
        {
            /// @todo add arena sound PlaySoundToAll(SOUND_ARENA_START);
            for (const auto& itr : GetPlayers())
            {
                if (Player* player = ObjectAccessor::FindPlayer(itr.first))
                {
                    // BG Status packet
                    BattlegroundQueueTypeId const bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(_typeId, GetArenaType());
                    uint32 const queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);

                    WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
                    sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, this, player, queueSlot, player->GetBattlegroundQueueJoinTime(bgQueueTypeId), GetArenaType());
                    player->SendDirectMessage(battlefieldStatus.Write());

                    // Correctly display EnemyUnitFrame
                    player->SetByteValue(PLAYER_BYTES_4, PLAYER_BYTES_4_OFFSET_ARENA_FACTION, player->GetBGTeam());

                    player->RemoveAurasDueToSpell(SPELL_ARENA_PREPARATION);
                    player->ResetAllPowers();
                    if (!player->IsGameMaster())
                    {
                        // remove auras with duration lower than 30s
                        player->RemoveAppliedAuras([](AuraApplication const* aurApp)
                        {
                            Aura* aura = aurApp->GetBase();
                            return !aura->IsPermanent()
                                && aura->GetDuration() <= 30 * IN_MILLISECONDS
                                && aurApp->IsPositive()
                                && !aura->GetSpellInfo()->HasAttribute(SPELL_ATTR0_UNAFFECTED_BY_INVULNERABILITY)
                                && !aura->HasEffectType(SPELL_AURA_MOD_INVISIBILITY);
                        });
                    }
                }
            }

            CheckWinConditions();
        }
        else
        {
            PlaySoundToAll(SOUND_BG_START);

            for (auto const& itr : GetPlayers())
            {
                if (Player* player = ObjectAccessor::FindPlayer(itr.first))
                {
                    player->RemoveAurasDueToSpell(SPELL_PREPARATION);
                    player->ResetAllPowers();
                }
            }
            // Announce BG starting
            if (sWorld->getBoolConfig(CONFIG_BATTLEGROUND_QUEUE_ANNOUNCER_ENABLE))
                sWorld->SendWorldText(LANG_BG_STARTED_ANNOUNCE_WORLD, GetName(), GetMinLevel(), GetMaxLevel());
        }
    }

    if (GetRemainingTime() > 0 && (_endTime -= diff) > 0)
        _endTime  = GetRemainingTime() - diff;
}

inline void Battleground::ProcessLeave(uint32 diff)
{
    // *********************************************************
    // ***           BATTLEGROUND ENDING SYSTEM              ***
    // *********************************************************
    // remove all players from battleground after 2 minutes
    _endTime = GetRemainingTime() - diff;
    if (GetRemainingTime() <= 0)
    {
        _endTime = 0;
        BattlegroundPlayerMap::iterator next;
        for (BattlegroundPlayerMap::iterator itr = Players.begin(); itr != Players.end(); itr = next)
        {
            next = itr;
            ++next;
            //itr is erased here!
            RemovePlayerAtLeave(itr->first, true, true);// remove player from BG
            // do not change any battleground's private variables
        }
    }
}

Player* Battleground::GetPlayer(ObjectGuid guid, bool offlineRemove, char const* context) const
{
    Player* player = nullptr;
    if (!offlineRemove)
    {
        // should this be ObjectAccessor::FindConnectedPlayer() to return players teleporting ?
        player = ObjectAccessor::FindPlayer(guid);
        if (!player)
            TC_LOG_ERROR("bg.battleground", "Battleground::%s: player (%s) not found for BG (map: %u, instance id: %u)!",
                context, guid.ToString().c_str(), GetMapId(), _instanceId);
    }
    return player;
}

Player* Battleground::GetPlayerForTeam(Team team, BattlegroundPlayerMap::const_iterator itr, char const* context) const
{
    Player* player = GetPlayer(itr, context);
    if (player)
    {
        Team localTeam = itr->second.Team;
        if (!localTeam)
            localTeam = player->GetTeam();
        if (localTeam != team)
            player = nullptr;
    }
    return player;
}

BattlegroundMap* Battleground::GetBgMap() const
{
    ASSERT(_map);
    return _map;
}

uint32 Battleground::GetMapId() const
{
    return _battlegroundTemplate->BattlemasterEntry->MapID[0];
}

void Battleground::SendPacketToAll(WorldPacket const* packet) const
{
    for (auto itr = Players.begin(); itr != Players.end(); ++itr)
        if (Player* player = GetPlayer(itr, "SendPacketToAll"))
            player->SendDirectMessage(packet);
}

void Battleground::SendPacketToTeam(Team team, WorldPacket const* packet, Player* except /*= nullptr*/) const
{
    for (auto itr = Players.begin(); itr != Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(team, itr, "SendPacketToTeam"))
            if (player != except)
                player->SendDirectMessage(packet);
}

void Battleground::SendChatMessage(Creature* source, uint8 textId, WorldObject* target /*= nullptr*/)
{
    sCreatureTextMgr->SendChat(source, textId, target);
}

void Battleground::SendBroadcastText(uint32 id, ChatMsg msgType, WorldObject const* target)
{
    if (!sBroadcastTextStore.LookupEntry(id))
    {
        TC_LOG_ERROR("bg.battleground", "Battleground::SendBroadcastText: `broadcast_text` (ID: %u) was not found", id);
        return;
    }

    Trinity::BroadcastTextBuilder builder(nullptr, msgType, id, GENDER_MALE, target);
    Trinity::LocalizedPacketDo<Trinity::BroadcastTextBuilder> localizer(builder);
    BroadcastWorker(localizer);
}

void Battleground::PlaySoundToAll(uint32 soundId, ObjectGuid sourceGuid) const
{
    SendPacketToAll(WorldPackets::Misc::PlaySound(sourceGuid, soundId).Write());
}

void Battleground::PlaySoundToTeam(uint32 soundId, Team team) const
{
    SendPacketToTeam(team, WorldPackets::Misc::PlaySound(ObjectGuid::Empty, soundId).Write());
}

void Battleground::CastSpellOnTeam(uint32 spellId, Team team)
{
    for (BattlegroundPlayerMap::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(team, itr, "CastSpellOnTeam"))
            player->CastSpell(player, spellId, true);
}

void Battleground::RemoveAuraOnTeam(uint32 spellId, Team team)
{
    for (BattlegroundPlayerMap::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(team, itr, "RemoveAuraOnTeam"))
            player->RemoveAura(spellId);
}

void Battleground::RewardHonorToTeam(uint32 honor, Team team)
{
    for (BattlegroundPlayerMap::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
        if (Player* player = GetPlayerForTeam(team, itr, "RewardHonorToTeam"))
            UpdatePlayerScore(player, SCORE_BONUS_HONOR, honor);
}

void Battleground::RewardReputationToTeam(uint32 factionId, uint32 reputation, Team team)
{
    FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionId);
    if (!factionEntry)
        return;

    for (BattlegroundPlayerMap::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
    {
        Player* player = GetPlayerForTeam(team, itr, "RewardReputationToTeam");
        if (!player)
            continue;

        uint32 repGain = reputation;
        AddPct(repGain, player->GetTotalAuraModifier(SPELL_AURA_MOD_REPUTATION_GAIN));
        AddPct(repGain, player->GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_FACTION_REPUTATION_GAIN, factionId));
        player->GetReputationMgr().ModifyReputation(factionEntry, repGain);
    }
}

void Battleground::UpdateWorldState(uint32 variable, uint32 value, bool hidden /*= false*/) const
{
    WorldPackets::WorldState::UpdateWorldState worldstate;
    worldstate.VariableID = variable;
    worldstate.Value = value;
    worldstate.Hidden = hidden;
    SendPacketToAll(worldstate.Write());
}

void Battleground::EndBattleground(Team winner)
{
    RemoveFromBGFreeSlotQueue();

    bool guildAwarded = false;

    if (winner == ALLIANCE)
    {
        if (IsBattleground())
            SendBroadcastText(BG_TEXT_ALLIANCE_WINS, CHAT_MSG_BG_SYSTEM_NEUTRAL);

        PlaySoundToAll(SOUND_ALLIANCE_WINS);                // alliance wins sound

        _winnerTeamId = BG_TEAM_ALLIANCE;
    }
    else if (winner == HORDE)
    {
        if (IsBattleground())
            SendBroadcastText(BG_TEXT_HORDE_WINS, CHAT_MSG_BG_SYSTEM_NEUTRAL);

        PlaySoundToAll(SOUND_HORDE_WINS);                   // horde wins sound

        _winnerTeamId = BG_TEAM_HORDE;
    }
    else
        _winnerTeamId = BG_TEAM_NEUTRAL;

    PreparedStatement* stmt = nullptr;
    uint64 battlegroundId = 1;
    if (IsBattleground() && sWorld->getBoolConfig(CONFIG_BATTLEGROUND_STORE_STATISTICS_ENABLE))
    {
        stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PVPSTATS_MAXID);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            Field* fields = result->Fetch();
            battlegroundId = fields[0].GetUInt64() + 1;
        }

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PVPSTATS_BATTLEGROUND);
        stmt->setUInt64(0, battlegroundId);
        stmt->setUInt8(1, GetWinner());
        stmt->setUInt8(2, GetUniqueBracketId());
        stmt->setUInt8(3, GetTypeId(true));
        CharacterDatabase.Execute(stmt);
    }

    SetStatus(STATUS_WAIT_LEAVE);
    //we must set it this way, because end time is sent in packet!
    _endTime = TIME_AUTOCLOSE_BATTLEGROUND;

    WorldPackets::Battleground::PVPLogData pvpLogData;
    BuildPvPLogDataPacket(pvpLogData);

    BattlegroundQueueTypeId bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeId(), GetArenaType());

    for (auto itr = Players.begin(); itr != Players.end(); ++itr)
    {
        Team const team = itr->second.Team;

        Player* player = GetPlayer(itr, "EndBattleground");
        if (!player)
            continue;

        // should remove spirit of redemption
        if (player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

        if (!player->IsAlive())
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }
        else
        {
            //needed cause else in av some creatures will kill the players at the end
            player->CombatStop();
            player->getHostileRefManager().deleteReferences();
        }

        // remove temporary currency bonus auras before rewarding player
        player->RemoveAura(SPELL_HONORABLE_DEFENDER_25Y);
        player->RemoveAura(SPELL_HONORABLE_DEFENDER_60Y);

        uint32 winnerKills = player->GetRandomWinner() ? sWorld->getIntConfig(CONFIG_BG_REWARD_WINNER_HONOR_LAST) : sWorld->getIntConfig(CONFIG_BG_REWARD_WINNER_HONOR_FIRST);
        uint32 loserKills = player->GetRandomWinner() ? sWorld->getIntConfig(CONFIG_BG_REWARD_LOSER_HONOR_LAST) : sWorld->getIntConfig(CONFIG_BG_REWARD_LOSER_HONOR_FIRST);

        if (IsBattleground() && sWorld->getBoolConfig(CONFIG_BATTLEGROUND_STORE_STATISTICS_ENABLE))
        {
            stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_PVPSTATS_PLAYER);
            BattlegroundScoreMap::const_iterator score = PlayerScores.find(player->GetGUID());

            stmt->setUInt32(0,  battlegroundId);
            stmt->setUInt64(1,  player->GetGUID().GetCounter());
            stmt->setBool  (2,  team == winner);
            stmt->setUInt32(3,  score->second->GetKillingBlows());
            stmt->setUInt32(4,  score->second->GetDeaths());
            stmt->setUInt32(5,  score->second->GetHonorableKills());
            stmt->setUInt32(6,  score->second->GetBonusHonor());
            stmt->setUInt32(7,  score->second->GetDamageDone());
            stmt->setUInt32(8,  score->second->GetHealingDone());
            uint8 i = 0;
            for (auto const& scores : score->second->ScoreTypes)
            {
                if (i > 5)
                    break;

                stmt->setUInt32(9 + i, scores.second);
                ++i;
            }

            CharacterDatabase.Execute(stmt);
        }

        // Reward winner team
        // TODO ENUMS + boxes
        if (team == winner)
        {
            if (IsRandom() || BattlegroundMgr::IsBGWeekend(GetTypeId()))
            {
                // todo needs research
                //UpdatePlayerScore(player, SCORE_BONUS_HONOR, GetBonusHonorFromKill(winnerKills));
                if (!player->GetRandomWinner())
                {
                    player->SetRandomWinner(true);
                    if (player->CanCompleteQuest(45435))
                    {
                        player->AddHonorXP(300);
                        player->AddItem(143680, 1);
                        player->CompleteQuest(45435);
                    }
                    else
                    {
                        player->AddHonorXP(150);
                        player->AddItem(138881, 1);
                    }
                }
            }
            else
            {
                // TODO: loss honor xp
            }

            player->UpdateCriteria(CRITERIA_TYPE_WIN_BG, 1);
            if (!guildAwarded)
            {
                guildAwarded = true;
                if (ObjectGuid::LowType guildId = GetBgMap()->GetOwnerGuildId(player->GetBGTeam()))
                {
                    if (Guild* guild = sGuildMgr->GetGuildById(guildId))
                        guild->UpdateCriteria(CRITERIA_TYPE_WIN_BG, 1, 0, 0, nullptr, player);
                }
            }
        }
        else
        {
            if (IsRandom() || BattlegroundMgr::IsBGWeekend(GetTypeId()))
                UpdatePlayerScore(player, SCORE_BONUS_HONOR, GetBonusHonorFromKill(loserKills));
        }

        player->ResetAllPowers();
        player->CombatStopWithPets(true);

        BlockMovement(player);

        player->SendDirectMessage(pvpLogData.Write());

        WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
        sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), player->GetBattlegroundQueueJoinTime(bgQueueTypeId), GetArenaType());
        player->SendDirectMessage(battlefieldStatus.Write());

        player->UpdateCriteria(CRITERIA_TYPE_COMPLETE_BATTLEGROUND, 1);
    }

    GetBattlegroundScript()->OnEnd(winner);
}

uint8 Battleground::GetMinLevel() const
{
    if (_pvpDifficultyEntry)
        return _pvpDifficultyEntry->MinLevel;

    return _battlegroundTemplate->BattlemasterEntry->MinLevel;
}

uint8 Battleground::GetMaxLevel() const
{
    if (_pvpDifficultyEntry)
        return _pvpDifficultyEntry->MaxLevel;

    return _battlegroundTemplate->BattlemasterEntry->MaxLevel;
}

uint8 Battleground::GetMaxPlayersPerTeam() const
{
    return _battlegroundTemplate->BattlemasterEntry->MaxPlayers;
}

uint8 Battleground::GetMinPlayersPerTeam() const
{
    return _battlegroundTemplate->BattlemasterEntry->MinPlayers;
}

uint32 Battleground::GetScriptId() const
{
    return _battlegroundTemplate->ScriptId;
}

uint32 Battleground::GetBonusHonorFromKill(uint32 kills) const
{
    //variable kills means how many honorable kills you scored (so we need kills * honor_for_one_kill)
    uint32 maxLevel = std::min<uint32>(GetMaxLevel(), 110U);
    return Trinity::Honor::hk_honor_at_level(maxLevel, float(kills));
}

void Battleground::BlockMovement(Player* player) const
{
    player->SetClientControl(player, false); // movement disabled NOTE: the effect will be automatically removed by client when the player is teleported from the battleground, so no need to send with uint8(1) in RemovePlayerAtLeave()
}

void Battleground::RemovePlayerAtLeave(ObjectGuid guid, bool transport, bool sendPacket)
{
    Team const team = GetPlayerTeam(guid);
    bool participant = false;
    // Remove from lists/maps
    auto const itr = Players.find(guid);
    if (itr != Players.end())
    {
        UpdatePlayersCountByTeam(team, true);               // -1 player
        Players.erase(itr);
        // check if the player was a participant of the match, or only entered through gm command (goname)
        participant = true;
    }

    auto const itr2 = PlayerScores.find(guid);
    if (itr2 != PlayerScores.end())
    {
        delete itr2->second;                                // delete player's score
        PlayerScores.erase(itr2);
    }

    RemovePlayerFromResurrectQueue(guid);

    Player* player = ObjectAccessor::FindPlayer(guid);

    if (player)
    {
        // should remove spirit of redemption
        if (player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
            player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);

        player->RemoveAurasByType(SPELL_AURA_MOUNTED);

        if (!player->IsAlive())                              // resurrect on exit
        {
            player->ResurrectPlayer(1.0f);
            player->SpawnCorpseBones();
        }
    }
    else
    {
        SQLTransaction trans(nullptr);
        Player::OfflineResurrect(guid, trans);
    }

    RemovePlayer(player, guid, team);                           // BG subclass specific code

    BattlegroundTypeId bgTypeId = GetTypeId();
    BattlegroundQueueTypeId const bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeId(), GetArenaType());

    if (participant) // if the player was a match participant, remove auras, calc rating, update queue
    {
        if (player)
        {
            player->ClearAfkReports();

            // if arena, remove the specific arena auras
            if (IsArena())
            {
                bgTypeId = BATTLEGROUND_AA;                   // set the bg type to all arenas (it will be used for queue refreshing)

                // unsummon current and summon old pet if there was one and there isn't a current pet
                player->RemovePet(nullptr, PET_SAVE_NOT_IN_SLOT);
                player->ResummonPetTemporaryUnSummonedIfAny();
            }

            if (sendPacket)
            {
                WorldPackets::Battleground::BattlefieldStatusNone battlefieldStatus;
                sBattlegroundMgr->BuildBattlegroundStatusNone(&battlefieldStatus, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), player->GetBattlegroundQueueJoinTime(bgQueueTypeId));
                player->SendDirectMessage(battlefieldStatus.Write());
            }

            // this call is important, because player, when joins to battleground, this method is not called, so it must be called when leaving bg
            player->RemoveBattlegroundQueueId(bgQueueTypeId);
        }

        // remove from raid group if player is member
        if (Group* group = GetBgRaid(team))
            if (!group->RemoveMember(guid))                // group was disbanded
                SetBgRaid(team, nullptr);

        DecreaseInvitedCount(team);
        //we should update battleground queue, but only if bg isn't ending
        if (IsBattleground() && GetStatus() < STATUS_WAIT_LEAVE)
        {
            // a player has left the battleground, so there are free slots -> add to queue
            AddToBGFreeSlotQueue();
            sBattlegroundMgr->ScheduleQueueUpdate(0, 0, bgQueueTypeId, bgTypeId, GetBracketId());
        }

        // Let others know
        WorldPackets::Battleground::BattlegroundPlayerLeft playerLeft;
        playerLeft.Guid = guid;
        SendPacketToTeam(team, playerLeft.Write(), player);
    }

    if (player)
    {
        // Do next only if found in battleground
        player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);  // We're not in BG.
        // reset destination bg team
        player->SetBGTeam(TEAM_OTHER);

        if (transport)
            player->TeleportToBGEntryPoint();

        GetBattlegroundScript()->OnPlayerLeave(player);
        TC_LOG_DEBUG("bg.battleground", "Removed player %s from Battleground.", player->GetName().c_str());
    }

    //battleground object will be deleted next Battleground::Update() call
}

bool Battleground::SetupBattleground()
{
    for (ObjectGuid::LowType const spawnId : _battlegroundTemplate->CreaturesToSpawn)
    {
        if (CreatureData const* data = sObjectMgr->GetCreatureData(spawnId))
        {
            sObjectMgr->AddCreatureToGrid(spawnId, data);
            Creature::CreateCreatureFromDB(spawnId, _map);
        }
    }

    for (ObjectGuid::LowType const spawnId : _battlegroundTemplate->GameObjectsToSpawn)
    {
        if (GameObjectData const* data = sObjectMgr->GetGOData(spawnId))
        {
            sObjectMgr->AddGameobjectToGrid(spawnId, data);
            GameObject::CreateGameObjectFromDB(spawnId, _map);
        }
    }

    if (BattlegroundScript* script = GetBattlegroundScript())
    {
        script->OnSetup();
        return true;
    }

    return false;
}

void Battleground::StartBattleground()
{
    // todo
    _startTime = 0; // still needed ?
    _lastResurrectTime = 0; // still needed ?

    // add BG to free slot queue
    AddToBGFreeSlotQueue();

    // add bg to update list
    // This must be done here, because we need to have already invited some players when first BG::Update() method is executed
    // and it doesn't matter if we call StartBattleground() more times, because m_Battlegrounds is a map and instance id never changes
    sBattlegroundMgr->AddBattleground(this);
}

void Battleground::TeleportPlayerToExploitLocation(Player* player) const
{
    if (WorldSafeLocsEntry const* loc = GetExploitTeleportLocation(player->GetBGTeam()))
        player->TeleportTo(loc);
}

void Battleground::AddPlayer(Player* player)
{
    // remove afk from player
    if (player->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_AFK))
        player->ToggleAFK();

    // score struct must be created in inherited class

    Team const team = player->GetBGTeam();

    BattlegroundPlayer bp;
    bp.OfflineRemoveTime = 0;
    bp.Team = team;
    bp.ActiveSpec = player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID);

    // Add to list/maps
    Players[player->GetGUID()] = bp;

    UpdatePlayersCountByTeam(team, false);                  // +1 player

    WorldPackets::Battleground::BattlegroundPlayerJoined playerJoined;
    playerJoined.Guid = player->GetGUID();
    SendPacketToTeam(team, playerJoined.Write(), player);

    // BG Status packet
    BattlegroundQueueTypeId bgQueueTypeId = sBattlegroundMgr->BGQueueTypeId(_typeId, GetArenaType());
    uint32 queueSlot = player->GetBattlegroundQueueIndex(bgQueueTypeId);

    WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
    sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, this, player, queueSlot, player->GetBattlegroundQueueJoinTime(bgQueueTypeId), GetArenaType());
    player->SendDirectMessage(battlefieldStatus.Write());

    player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    // add arena specific auras
    if (IsArena())
    {
        player->RemoveArenaEnchantments(TEMP_ENCHANTMENT_SLOT);
        player->DestroyConjuredItems(true);
        player->UnsummonPetTemporaryIfAny();

        if (GetStatus() == STATUS_WAIT_JOIN)                 // not started yet
        {
            player->CastSpell(player, SPELL_ARENA_PREPARATION, true);
            player->ResetAllPowers();
        }
    }
    else
        if (GetStatus() == STATUS_WAIT_JOIN)                 // not started yet
            player->CastSpell(player, SPELL_PREPARATION, true);   // reduces all mana cost of spells.

    player->ResetCriteria(CRITERIA_TYPE_KILL_CREATURE, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_WIN_BG, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_DAMAGE_DONE, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_BE_SPELL_TARGET, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_CAST_SPELL, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_HONORABLE_KILL_AT_AREA, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_HONORABLE_KILL, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_HEALING_DONE, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_GET_KILLING_BLOWS, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);
    player->ResetCriteria(CRITERIA_TYPE_SPECIAL_PVP_KILL, CRITERIA_CONDITION_BG_MAP, GetMapId(), true);

    // setup BG group membership
    PlayerAddedToBGCheckIfBGIsRunning(player);
    AddOrSetPlayerToCorrectBgGroup(player, team);
    PlayerScores[player->GetGUID()] = new BattlegroundScore(player->GetGUID(), player->GetBGTeam(), GetTemplate()->Scores);

    // Maybe we could cache this one
    WorldPackets::Battleground::BattlegroundInit packet;
    packet.MaxPoints = _battlegroundTemplate->MaxPoints;
    player->SendDirectMessage(packet.Write());

    if (_battlegroundTemplate->MaxPoints)
        SendBattlegroundPoints();

    GetBattlegroundScript()->OnPlayerEnter(player);
}

// this method adds player to his team's bg group, or sets his correct group if player is already in bg group
void Battleground::AddOrSetPlayerToCorrectBgGroup(Player* player, Team team)
{
    ObjectGuid playerGuid = player->GetGUID();
    Group* group = GetBgRaid(team);
    if (!group)                                      // first player joined
    {
        group = new Group;
        SetBgRaid(team, group);
        group->Create(player);
    }
    else                                            // raid already exist
    {
        if (group->IsMember(playerGuid))
        {
            uint8 subgroup = group->GetMemberGroup(playerGuid);
            player->SetBattlegroundOrBattlefieldRaid(group, subgroup);
        }
        else
        {
            group->AddMember(player);
            if (Group* originalGroup = player->GetOriginalGroup())
                if (originalGroup->IsLeader(playerGuid))
                {
                    group->ChangeLeader(playerGuid);
                    group->SendUpdate();
                }
        }
    }
}

// This method should be called when player logs into running battleground
void Battleground::EventPlayerLoggedIn(Player* player)
{
    ObjectGuid guid = player->GetGUID();
    // player is correct pointer
    for (auto itr = _offlineQueue.begin(); itr != _offlineQueue.end(); ++itr)
    {
        if (*itr == guid)
        {
            _offlineQueue.erase(itr);
            break;
        }
    }
    Players[guid].OfflineRemoveTime = 0;
    PlayerAddedToBGCheckIfBGIsRunning(player);
    // if battleground is starting, then add preparation aura
    // we don't have to do that, because preparation aura isn't removed when player logs out
}

// This method should be called when player logs out from running battleground
void Battleground::EventPlayerLoggedOut(Player* player)
{
    ObjectGuid guid = player->GetGUID();
    if (!IsPlayerInBattleground(guid))  // Check if this player really is in battleground (might be a GM who teleported inside)
        return;

    // player is correct pointer, it is checked in WorldSession::LogoutPlayer()
    _offlineQueue.push_back(player->GetGUID());
    Players[guid].OfflineRemoveTime = sWorld->GetGameTime() + MAX_OFFLINE_TIME;
    if (GetStatus() == STATUS_IN_PROGRESS)
    {
        // drop flag and handle other cleanups
        RemovePlayer(player, guid, GetPlayerTeam(guid));

        // 1 player is logging out, if it is the last, then end arena!
        if (IsArena())
            if (GetAlivePlayersCountByTeam(player->GetBGTeam()) <= 1 && GetPlayersCountByTeam(GetOtherTeam(player->GetBGTeam())))
                EndBattleground(GetOtherTeam(player->GetBGTeam()));
    }
}

// This method should be called only once ... it adds pointer to queue
void Battleground::AddToBGFreeSlotQueue()
{
    if (!_inBGFreeSlotQueue && IsBattleground())
    {
        sBattlegroundMgr->AddToBGFreeSlotQueue(_typeId, this);
        _inBGFreeSlotQueue = true;
    }
}

// This method removes this battleground from free queue - it must be called when deleting battleground
void Battleground::RemoveFromBGFreeSlotQueue()
{
    if (_inBGFreeSlotQueue)
    {
        sBattlegroundMgr->RemoveFromBGFreeSlotQueue(_typeId, _instanceId);
        _inBGFreeSlotQueue = false;
    }
}

// get the number of free slots for team
// returns the number how many players can join battleground to MaxPlayersPerTeam
uint32 Battleground::GetFreeSlotsForTeam(Team team) const
{
    // if BG is starting and CONFIG_BATTLEGROUND_INVITATION_TYPE == BG_QUEUE_INVITATION_TYPE_NO_BALANCE, invite anyone
    if (GetStatus() == STATUS_WAIT_JOIN && sWorld->getIntConfig(CONFIG_BATTLEGROUND_INVITATION_TYPE) == BG_QUEUE_INVITATION_TYPE_NO_BALANCE)
        return (GetInvitedCount(team) < GetMaxPlayersPerTeam()) ? GetMaxPlayersPerTeam() - GetInvitedCount(team) : 0;

    // if BG is already started or CONFIG_BATTLEGROUND_INVITATION_TYPE != BG_QUEUE_INVITATION_TYPE_NO_BALANCE, do not allow to join too much players of one faction
    uint32 otherTeamInvitedCount;
    uint32 thisTeamInvitedCount;
    uint32 otherTeamPlayersCount;
    uint32 thisTeamPlayersCount;

    if (team == ALLIANCE)
    {
        thisTeamInvitedCount  = GetInvitedCount(ALLIANCE);
        otherTeamInvitedCount = GetInvitedCount(HORDE);
        thisTeamPlayersCount  = GetPlayersCountByTeam(ALLIANCE);
        otherTeamPlayersCount = GetPlayersCountByTeam(HORDE);
    }
    else
    {
        thisTeamInvitedCount  = GetInvitedCount(HORDE);
        otherTeamInvitedCount = GetInvitedCount(ALLIANCE);
        thisTeamPlayersCount  = GetPlayersCountByTeam(HORDE);
        otherTeamPlayersCount = GetPlayersCountByTeam(ALLIANCE);
    }
    if (GetStatus() == STATUS_IN_PROGRESS || GetStatus() == STATUS_WAIT_JOIN)
    {
        // difference based on ppl invited (not necessarily entered battle)
        // default: allow 0
        uint32 diff = 0;

        // allow join one person if the sides are equal (to fill up bg to minPlayerPerTeam)
        if (otherTeamInvitedCount == thisTeamInvitedCount)
            diff = 1;
        // allow join more ppl if the other side has more players
        else if (otherTeamInvitedCount > thisTeamInvitedCount)
            diff = otherTeamInvitedCount - thisTeamInvitedCount;

        // difference based on max players per team (don't allow inviting more)
        uint32 diff2 = (thisTeamInvitedCount < GetMaxPlayersPerTeam()) ? GetMaxPlayersPerTeam() - thisTeamInvitedCount : 0;

        // difference based on players who already entered
        // default: allow 0
        uint32 diff3 = 0;
        // allow join one person if the sides are equal (to fill up bg minPlayerPerTeam)
        if (otherTeamPlayersCount == thisTeamPlayersCount)
            diff3 = 1;
        // allow join more ppl if the other side has more players
        else if (otherTeamPlayersCount > thisTeamPlayersCount)
            diff3 = otherTeamPlayersCount - thisTeamPlayersCount;
        // or other side has less than minPlayersPerTeam
        else if (thisTeamInvitedCount <= GetMinPlayersPerTeam())
            diff3 = GetMinPlayersPerTeam() - thisTeamInvitedCount + 1;

        // return the minimum of the 3 differences

        // min of diff and diff 2
        diff = std::min(diff, diff2);
        // min of diff, diff2 and diff3
        return std::min(diff, diff3);
    }
    return 0;
}

bool Battleground::IsArena() const
{
    return _battlegroundTemplate->IsArena();
}

bool Battleground::HasFreeSlots() const
{
    return GetPlayersSize() < GetMaxPlayers();
}

void Battleground::BuildPvPLogDataPacket(WorldPackets::Battleground::PVPLogData& pvpLogData) const
{
    if (GetStatus() == STATUS_WAIT_LEAVE)
        pvpLogData.Winner = GetWinner();

    pvpLogData.Players.reserve(GetPlayerScoresSize());
    for (auto const& score : PlayerScores)
    {
        WorldPackets::Battleground::PVPLogData::PlayerData playerData;
        score.second->BuildPvPLogPlayerDataPacket(playerData);

        if (Player* player = ObjectAccessor::GetPlayer(GetBgMap(), playerData.PlayerGUID))
        {
            playerData.IsInWorld = true;
            playerData.PrimaryTalentTree = player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID);
            playerData.PrimaryTalentTreeNameIndex = 0;
            playerData.Race = player->getRace();
            playerData.Prestige = player->GetPrestigeLevel();
        }

        pvpLogData.Players.push_back(playerData);
    }

    pvpLogData.PlayerCount[BG_TEAM_HORDE] = int8(GetPlayersCountByTeam(HORDE));
    pvpLogData.PlayerCount[BG_TEAM_ALLIANCE] = int8(GetPlayersCountByTeam(ALLIANCE));
}

bool Battleground::UpdatePlayerScore(Player* player, ScoreType type, uint32 value, bool doAddHonor)
{
    BattlegroundScoreMap::const_iterator itr = PlayerScores.find(player->GetGUID());
    if (itr == PlayerScores.end()) // player not found...
        return false;

    if (type == SCORE_BONUS_HONOR && doAddHonor && IsBattleground())
        player->RewardHonor(nullptr, 1, value); // RewardHonor calls UpdatePlayerScore with doAddHonor = false
    else
        itr->second->UpdateScore(type, value);

    GetBattlegroundScript()->OnUpdatePlayerScore(type, player);

    return true;
}

void Battleground::AddPlayerToResurrectQueue(ObjectGuid creatureGuid, ObjectGuid playerGuid)
{
    ReviveQueue[creatureGuid].push_back(playerGuid);

    Player* player = ObjectAccessor::FindPlayer(playerGuid);
    if (!player)
        return;

    player->CastSpell(player, SPELL_WAITING_FOR_RESURRECT, true);
}

void Battleground::RemovePlayerFromResurrectQueue(ObjectGuid playerGuid)
{
    for (auto& itr : ReviveQueue)
    {
        for (auto itr2 = itr.second.begin(); itr2 != itr.second.end(); ++itr2)
        {
            if (*itr2 == playerGuid)
            {
                itr.second.erase(itr2);
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                    player->RemoveAurasDueToSpell(SPELL_WAITING_FOR_RESURRECT);
                return;
            }
        }
    }
}

void Battleground::RelocateDeadPlayers(ObjectGuid spiritGuideGuid)
{
    // Those who are waiting to resurrect at this node are taken to the closest own node's graveyard
    auto itr = ReviveQueue.find(spiritGuideGuid);
    if (itr == ReviveQueue.end())
        return;

    GuidVector& ghostList = itr->second;
    if (!ghostList.empty())
    {
        WorldSafeLocsEntry const* closestGrave = nullptr;
        for (GuidVector::const_iterator itr = ghostList.begin(); itr != ghostList.end(); ++itr)
        {
            Player* player = ObjectAccessor::FindPlayer(*itr);
            if (!player)
                continue;

            if (!closestGrave)
                closestGrave = GetBattlegroundScript()->GetGraveYardForPlayer(player);

            if (closestGrave)
                player->TeleportTo(GetMapId(), closestGrave->Loc.X, closestGrave->Loc.Y, closestGrave->Loc.Z, player->GetOrientation());
        }

        ghostList.clear();
    }
}

void Battleground::SendMessageToAll(uint32 entry, ChatMsg msgType, Player const* source)
{
    if (!entry)
        return;

    Trinity::TrinityStringChatBuilder builder(nullptr, msgType, entry, source);
    Trinity::LocalizedPacketDo<Trinity::TrinityStringChatBuilder> localizer(builder);
    BroadcastWorker(localizer);
}

void Battleground::PSendMessageToAll(uint32 entry, ChatMsg msgType, Player const* source, ...)
{
    if (!entry)
        return;

    va_list ap;
    va_start(ap, source);

    Trinity::TrinityStringChatBuilder builder(nullptr, msgType, entry, source, &ap);
    Trinity::LocalizedPacketDo<Trinity::TrinityStringChatBuilder> localizer(builder);
    BroadcastWorker(localizer);

    va_end(ap);
}

void Battleground::EndNow()
{
    RemoveFromBGFreeSlotQueue();
    SetStatus(STATUS_WAIT_LEAVE);
    _endTime = 0;
}

void Battleground::HandleTriggerBuff(Player* player, uint32 areaTriggerId) const
{
    auto const& itr = GetTemplate()->BuffSpawnTriggers.find(areaTriggerId);
    if (itr == GetTemplate()->BuffSpawnTriggers.end())
        return;

    if (GetTemplate()->BuffSpawnTriggers.at(areaTriggerId).empty())
        return;

    GameObject* currentBuff = nullptr;
    std::vector<uint32> gameObjectIds = GetTemplate()->BuffSpawnTriggers.at(areaTriggerId);
    for (uint32 const entry : gameObjectIds)
    {
        if (GameObject* obj = player->FindNearestGameObject(entry, 10.0f))
        {
            if (obj->isSpawned())
            {
                currentBuff = obj;
                break;
            }
        }
    }

    if (!currentBuff || currentBuff->GetGoType() != GAMEOBJECT_TYPE_TRAP)
        return;

    currentBuff->CastSpell(player, currentBuff->GetGOInfo()->trap.spell);

    uint32 const newBuffEntry = Trinity::Containers::SelectRandomContainerElement(GetTemplate()->BuffSpawnTriggers.at(areaTriggerId));
    GameObject* newBuff = GameObject::CreateGameObject(newBuffEntry, GetBgMap(), currentBuff->GetPosition(), currentBuff->GetWorldRotation(), 255, GO_STATE_READY);
    if (!newBuff)
        return;

    newBuff->SetRespawnTime(BUFF_RESPAWN_TIME);

    if (!GetBgMap()->AddToMap(newBuff))
    {
        delete newBuff;
        return;
    }

    currentBuff->DespawnAndRemoveFromWorld(true);
}

void Battleground::HandleKillPlayer(Player* victim, Player* killer)
{
    // Keep in mind that for arena this will have to be changed a bit

    // Add +1 deaths
    UpdatePlayerScore(victim, SCORE_DEATHS, 1);
    // Add +1 kills to group and +1 killing_blows to killer
    if (killer)
    {
        // Don't reward credit for killing ourselves, like fall damage of hellfire (warlock)
        if (killer == victim)
            return;

        UpdatePlayerScore(killer, SCORE_HONORABLE_KILLS, 1);
        UpdatePlayerScore(killer, SCORE_KILLING_BLOWS, 1);

        for (BattlegroundPlayerMap::const_iterator itr = Players.begin(); itr != Players.end(); ++itr)
        {
            Player* creditedPlayer = ObjectAccessor::FindPlayer(itr->first);
            if (!creditedPlayer || creditedPlayer == killer)
                continue;

            if (creditedPlayer->GetTeam() == killer->GetTeam() && creditedPlayer->IsAtGroupRewardDistance(victim))
                UpdatePlayerScore(creditedPlayer, SCORE_HONORABLE_KILLS, 1);
        }

        GetBattlegroundScript()->OnPlayerKillsUnit(killer, victim);
    }

    if (!IsArena())
    {
        // To be able to remove insignia -- ONLY IN Battlegrounds
        victim->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SKINNABLE);
        RewardXPAtKill(killer, victim);
    }
}

void Battleground::HandleKillUnit(Creature* victim, Player* killer)
{
    if (victim && killer)
        GetBattlegroundScript()->OnPlayerKillsUnit(killer, victim);
}

// Return the player's team based on battlegroundplayer info
// Used in same faction arena matches mainly
Team Battleground::GetPlayerTeam(ObjectGuid guid) const
{
    auto const itr = Players.find(guid);
    if (itr != Players.end())
        return itr->second.Team;

    return TEAM_OTHER;
}

Team Battleground::GetOtherTeam(Team team)
{
    return team ? ((team == ALLIANCE) ? HORDE : ALLIANCE) : TEAM_OTHER;
}

bool Battleground::IsPlayerInBattleground(ObjectGuid guid) const
{
    return Players.find(guid) != Players.end();
}

void Battleground::PlayerAddedToBGCheckIfBGIsRunning(Player* player)
{
    if (GetStatus() != STATUS_WAIT_LEAVE)
        return;

    WorldPackets::Battleground::PVPLogData pvpLogData;
    BattlegroundQueueTypeId const bgQueueTypeId = BattlegroundMgr::BGQueueTypeId(GetTypeId(), GetArenaType());

    BlockMovement(player);

    BuildPvPLogDataPacket(pvpLogData);
    player->SendDirectMessage(pvpLogData.Write());

    WorldPackets::Battleground::BattlefieldStatusActive battlefieldStatus;
    sBattlegroundMgr->BuildBattlegroundStatusActive(&battlefieldStatus, this, player, player->GetBattlegroundQueueIndex(bgQueueTypeId), player->GetBattlegroundQueueJoinTime(bgQueueTypeId), GetArenaType());
    player->SendDirectMessage(battlefieldStatus.Write());
}

uint32 Battleground::GetAlivePlayersCountByTeam(Team team) const
{
    int count = 0;
    for (const auto& itr : Players)
    {
        if (itr.second.Team == team)
        {
            Player* player = ObjectAccessor::FindPlayer(itr.first);
            if (player && player->IsAlive())
                ++count;
        }
    }
    return count;
}

void Battleground::SetHoliday(bool is_holiday)
{
    HonorMode = is_holiday ? BG_HOLIDAY : BG_NORMAL;
}

void Battleground::SetBgRaid(Team team, Group* bgRaid)
{
    Group*& old_raid = team == ALLIANCE ? _bgRaids[TEAM_ALLIANCE] : _bgRaids[TEAM_HORDE];
    if (old_raid)
        old_raid->SetBattlegroundGroup(nullptr);
    if (bgRaid)
        bgRaid->SetBattlegroundGroup(this);
    old_raid = bgRaid;
}

WorldSafeLocsEntry const* Battleground::GetExploitTeleportLocation(Team team) const
{
    return GetTemplate()->ExploitLocation[GetTeamIndexByTeamId(team)];
}

void Battleground::StartCriteriaTimer(CriteriaTimedTypes type, uint32 entry) const
{
    for (auto const& itr : GetPlayers())
        if (Player* player = ObjectAccessor::FindPlayer(itr.first))
            player->StartCriteriaTimer(type, entry);
}

void Battleground::RewardXPAtKill(Player* killer, Player* victim)
{
    if (sWorld->getBoolConfig(CONFIG_BG_XP_FOR_KILL) && killer && victim)
        killer->RewardPlayerAndGroupAtKill(victim, true);
}

uint32 Battleground::GetTeamScore(TeamId teamId) const
{
    if (teamId == TEAM_ALLIANCE || teamId == TEAM_HORDE)
        return TeamScores[teamId];

    TC_LOG_ERROR("bg.battleground", "GetTeamScore with wrong Team %u for BG %u", teamId, GetTypeId());
    return 0;
}

char const* Battleground::GetName() const
{
    return _battlegroundTemplate->BattlemasterEntry->Name->Str[sWorld->GetDefaultDbcLocale()];
}

BattlegroundBracketId Battleground::GetBracketId() const
{
    return _pvpDifficultyEntry ? _pvpDifficultyEntry->GetBracketId() : BG_BRACKET_ID_FIRST;
}

uint8 Battleground::GetUniqueBracketId() const
{
    return uint8(GetMinLevel() / 5) - 1; // 10 - 1, 15 - 2, 20 - 3, etc.
}

void Battleground::SendBattlegroundPoints() const
{
    WorldPackets::Battleground::BattlegroundPoints packetAlliance;
    packetAlliance.Points = GetTeamScore(TEAM_ALLIANCE);
    packetAlliance.Team = 1;
    WorldPackets::Battleground::BattlegroundPoints packetHorde;
    packetHorde.Points = GetTeamScore(TEAM_HORDE);
    packetHorde.Team = 1;
    WorldPackets::Battleground::BattlegroundPoints packetAllianceForHorde;
    packetAllianceForHorde.Points = GetTeamScore(TEAM_ALLIANCE);
    packetAllianceForHorde.Team = 0;
    WorldPackets::Battleground::BattlegroundPoints packetHordeForAlliance;
    packetHordeForAlliance.Points = GetTeamScore(TEAM_HORDE);
    packetHordeForAlliance.Team = 0;

    SendPacketToTeam(HORDE, packetAllianceForHorde.Write());
    SendPacketToTeam(HORDE, packetHorde.Write());
    SendPacketToTeam(ALLIANCE, packetHordeForAlliance.Write());
    SendPacketToTeam(ALLIANCE, packetAlliance.Write());
}
