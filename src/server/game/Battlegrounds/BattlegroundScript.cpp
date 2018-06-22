/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
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

#include "BattlegroundScript.h"
#include "Arena.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "MiscPackets.h"
#include "PhasingHandler.h"
#include "SpellAuras.h"
#include "SpellInfo.h"
#include "TemporarySummon.h"
#include "Transport.h"
#include "WorldStatePackets.h"

BattlegroundScript::BattlegroundScript(BattlegroundMap* map) : Map(map), battleground(map->GetBG())
{
#ifdef TRINITY_API_USE_DYNAMIC_LINKING
    uint32 scriptId = sBattlegroundMgr->GetBattlegroundTemplateByMapId(map->GetId())->ScriptId;
    auto const scriptname = sObjectMgr->GetScriptName(scriptId);
    ASSERT(!scriptname.empty());
    // Acquire a strong reference from the script module
    // to keep it loaded until this object is destroyed.
    module_reference = sScriptMgr->AcquireModuleReferenceOfScriptName(scriptname);
#endif // #ifndef TRINITY_API_USE_DYNAMIC_LINKING
}

void BattlegroundScript::UpdateWorldState(uint32 variable, uint32 value, bool hidden)
{
    Worldstates[variable] = value;
    battleground->UpdateWorldState(variable, value, hidden);
}

void BattlegroundScript::ToggleGameobjectSpawn(ObjectGuid guid, uint32 respawntime) const
{
    if (GameObject* obj = Map->GetGameObject(guid))
    {
        if (respawntime)
            obj->SetLootState(GO_JUST_DEACTIVATED);

        obj->SetRespawnTime(respawntime);
    }
}

GameObject* BattlegroundScript::AddObject(uint32 entry, Position const& pos, QuaternionData const& rot, uint32 respawnTime, GOState goState) const
{
    GameObject* go = GameObject::CreateGameObject(entry, Map, pos, rot, 255, goState);
    if (!go)
    {
        TC_LOG_ERROR("bg.battleground", "BattlegroundScript::AddObject: cannot create gameobject (entry: %u) for BG (map: %u, instance id: %u)!", entry, Map->GetId(), Map->GetInstanceId());
        return nullptr;
    }

    if (!Map->AddToMap(go))
    {
        delete go;
        return nullptr;
    }

    if (respawnTime)
        go->SetRespawnTime(respawnTime);

    return go;
}

Creature* BattlegroundScript::AddCreature(uint32 entry, Position const& pos, uint32 respawntime, Transport* transport) const
{
    if (transport)
        return transport->SummonPassenger(entry, pos, TEMPSUMMON_MANUAL_DESPAWN);

    Creature* creature = Creature::CreateCreature(entry, Map, pos);

    if (!creature)
    {
        TC_LOG_ERROR("bg.battleground", "BattlegroundScript::AddCreature: cannot create creature (entry: %u) for BG (map: %u, instance id: %u)!", entry, Map->GetId(), Map->GetInstanceId());
        return nullptr;
    }

    creature->SetHomePosition(pos);

    if (!Map->AddToMap(creature))
    {
        delete creature;
        return nullptr;
    }

    if (respawntime)
        creature->SetRespawnDelay(respawntime);

    return creature;
}

Creature* BattlegroundScript::AddSpiritGuide(Position const& pos, TeamId teamId) const
{
    uint32 const entry = (teamId == TEAM_ALLIANCE) ? BG_CREATURE_ENTRY_A_SPIRITGUIDE : BG_CREATURE_ENTRY_H_SPIRITGUIDE;
    if (Creature* creature = AddCreature(entry, pos))
    {
        creature->setDeathState(DEAD);
        creature->AddChannelObject(creature->GetGUID());
        // aura
        /// @todo Fix display here
        // creature->SetVisibleAura(0, SPELL_SPIRIT_HEAL_CHANNEL);
        // casting visual effect
        creature->SetChannelSpellId(SPELL_SPIRIT_HEAL_CHANNEL);
        creature->SetChannelSpellXSpellVisualId(VISUAL_SPIRIT_HEAL_CHANNEL);
        // correct cast speed
        creature->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
        creature->SetFloatValue(UNIT_MOD_CAST_HASTE, 1.0f);
        //creature->CastSpell(creature, SPELL_SPIRIT_HEAL_CHANNEL, true);
        return creature;
    }

    return nullptr;
}

void BattlegroundScript::OnUpdate(uint32 diff)
{
    Scheduler.Update(diff);
}

void BattlegroundScript::OnStart()
{
    battleground->StartCriteriaTimer(CRITERIA_TIMED_TYPE_UNK_2, EVENT_START_BATTLE);
}

Team BattlegroundScript::CalculatePrematureWinner()
{
    Team winner = TEAM_OTHER;
    if (battleground->GetPlayersCountByTeam(ALLIANCE) >= battleground->GetMinPlayersPerTeam())
        winner = ALLIANCE;
    else if (battleground->GetPlayersCountByTeam(HORDE) >= battleground->GetMinPlayersPerTeam())
        winner = HORDE;

    return winner;
}

bool BattlegroundScript::CheckAchievementCriteriaMeet(uint32 criteriaId, Player const* /*player*/, Unit const* /*target*/, uint32 /*miscvalue*/)
{
    TC_LOG_ERROR("misc", "Achievement system call BattlegroundScript::CheckAchievementCriteriaMeet but Battleground script for map %u not have implementation for achievement criteria %u",
        Map->GetId(), criteriaId);
    return false;
}

bool BattlegroundScript::OnGameObjectClicked(Player* player, GameObject* gameObject)
{
    player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);
    return true;
}

void BattlegroundScript::OnQueryCountdownTimer(uint32 timerType)
{
    WorldPackets::Misc::StartTimer startTimer;
    startTimer.TimeRemaining = BATTLEGROUND_COUNTDOWN_MAX - battleground->GetElapsedTime() / 1000;
    startTimer.TotalTime = BATTLEGROUND_COUNTDOWN_MAX;
    startTimer.Type = timerType;

    battleground->SendPacketToAll(startTimer.Write());
}

void BattlegroundScript::HandleAreaTrigger(Player* player, uint32 triggerId, bool /*entered*/)
{
    auto const& itr = battleground->GetTemplate()->BuffSpawnTriggers.find(triggerId);
    if (itr != battleground->GetTemplate()->BuffSpawnTriggers.end())
        battleground->HandleTriggerBuff(player, triggerId);
}

void ArenaScript::FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet)
{
    packet.Worldstates.emplace_back(ARENA_WORLD_STATE_ALIVE_PLAYERS_GREEN, battleground->GetAlivePlayersCountByTeam(HORDE));
    packet.Worldstates.emplace_back(ARENA_WORLD_STATE_ALIVE_PLAYERS_GOLD, battleground->GetAlivePlayersCountByTeam(ALLIANCE));
}
