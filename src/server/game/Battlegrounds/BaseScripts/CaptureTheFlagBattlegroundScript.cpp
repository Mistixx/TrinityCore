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

#include "CaptureTheFlagBattlegroundScript.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"
#include "BattlegroundScore.h"
#include "BattlegroundScript.h"
#include "MiscPackets.h"
#include "SpellInfo.h"
#include "TemporarySummon.h"

CaptureTheFlagBattlegroundScript::CaptureTheFlagBattlegroundScript(BattlegroundMap* map) :
    BattlegroundScript(map), _flagStates{ IN_BASE, IN_BASE }, _lastFlagCapture(TEAM_OTHER)
{
    Worldstates[WORLD_STATE_FLAG_CAPTURES_ALLIANCE] = 0;
    Worldstates[WORLD_STATE_FLAG_CAPTURES_HORDE] = 0;
    Worldstates[WORLD_STATE_FLAG_STATE_ALLIANCE] = IN_BASE;
    Worldstates[WORLD_STATE_FLAG_STATE_HORDE] = IN_BASE;
    Worldstates[WORLD_STATE_FLAG_CAPTURES_MAX] = battleground->GetTemplate()->MaxPoints;
    Worldstates[WORLD_STATE_HORDE_FLAG_PICKED_UP] = 1;
    Worldstates[WORLD_STATE_ALLIANCE_FLAG_PICKED_UP] = 1;
}

// No more areatriggers in base...
void CaptureTheFlagBattlegroundScript::OnUpdateInProgress(uint32 diff)
{
    if (IsAllianceFlagInBase() && IsHordeFlagInBase())
        return;

    if (IsAllianceFlagInBase() && IsHordeFlagPickedUp())
        if (Player* ally = ObjectAccessor::FindConnectedPlayer(GetAllyThatCarriesHordeFlag()))
            if (GameObject* go = Map->GetGameObject(GetGuidData(DATA_ALLIANCE_FLAG_IN_BASE)))
                if (ally->IsInDist(go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), 5.0f))
                    CaptureHordeFlag(ally);

    if (IsHordeFlagInBase() && IsAllianceFlagPickedUp())
        if (Player* hordie = ObjectAccessor::FindConnectedPlayer(GetHordieThatCarriesAllianceFlag()))
            if (GameObject* go = Map->GetGameObject(GetGuidData(DATA_HORDE_FLAG_IN_BASE)))
                if (hordie->IsInDist(go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), 5.0f))
                    CaptureAllianceFlag(hordie);
}

void CaptureTheFlagBattlegroundScript::RespawnFlagOnCapture()
{
    // todo review
    //when map_update will be allowed for battlegrounds this code will be useless
    SetAllianceFlagState(IN_BASE);
    SetHordeFlagState(IN_BASE);
    ToggleGameobjectSpawn(GetGuidData(DATA_ALLIANCE_FLAG_IN_BASE), RESPAWN_IMMEDIATELY);
    ToggleGameobjectSpawn(GetGuidData(DATA_HORDE_FLAG_IN_BASE), RESPAWN_IMMEDIATELY);
    battleground->SendBroadcastText(TEXT_FLAGS_PLACED, CHAT_MSG_BG_SYSTEM_NEUTRAL);
    battleground->PlaySoundToAll(SOUND_FLAGS_RESPAWNED); // flag respawned sound...
}

void CaptureTheFlagBattlegroundScript::RespawnFlag(Team team)
{
    switch (team)
    {
        case ALLIANCE:
            if (GameObject* gameObject = Map->GetGameObject(_droppedFlagGuids[TEAM_ALLIANCE]))
            {
                gameObject->Delete();
                _droppedFlagGuids[TEAM_ALLIANCE] = ObjectGuid::Empty;
            }

            SetAllianceFlagState(IN_BASE);
            ToggleGameobjectSpawn(GetGuidData(DATA_ALLIANCE_FLAG_IN_BASE), RESPAWN_IMMEDIATELY);
            UpdateFlagWorldState(ALLIANCE);
            break;
        case HORDE:
            if (GameObject* gameObject = Map->GetGameObject(_droppedFlagGuids[TEAM_HORDE]))
            {
                gameObject->Delete();
                _droppedFlagGuids[TEAM_HORDE] = ObjectGuid::Empty;
            }

            SetHordeFlagState(IN_BASE);
            ToggleGameobjectSpawn(GetGuidData(DATA_HORDE_FLAG_IN_BASE), RESPAWN_IMMEDIATELY);
            UpdateFlagWorldState(HORDE);
            break;
        default:
            break;
    }

    OnRespawnFlag(team);
}

Team CaptureTheFlagBattlegroundScript::CalculatePrematureWinner()
{
    if (battleground->GetTeamScore(TEAM_ALLIANCE) > battleground->GetTeamScore(TEAM_HORDE))
        return ALLIANCE;

    if (battleground->GetTeamScore(TEAM_HORDE) > battleground->GetTeamScore(TEAM_ALLIANCE))
        return HORDE;

    return BattlegroundScript::CalculatePrematureWinner();
}

bool CaptureTheFlagBattlegroundScript::OnGameObjectClicked(Player* player, GameObject* gameobject)
{
    switch (gameobject->GetGoType())
    {
        case GAMEOBJECT_TYPE_NEW_FLAG_DROP:
            PlayerClickedOnDroppedFlag(player, gameobject);
            return true;
        case GAMEOBJECT_TYPE_NEW_FLAG:
            PlayerClickedOnFlag(player, gameobject);
            return true;
        default:
            return false;
    }
}

void CaptureTheFlagBattlegroundScript::PlayerClickedOnFlag(Player* player, GameObject* gameObject)
{
    uint32 const entry = gameObject->GetEntry();
    switch (player->GetTeam())
    {
        case HORDE:
            if (entry == ALLIANCE_FLAG_IN_BASE)
                AllianceFlagPickedUpFromBase(player);
            else
                TC_LOG_ERROR("battleground", "Player %s clicked on non-handled 'flag' %s!", player->GetGUID().ToString(), gameObject->GetGUID().ToString());
            break;
        case ALLIANCE:
            if (entry == HORDE_FLAG_IN_BASE)
                HordeFlagPickedUpFromBase(player);
            else
                TC_LOG_ERROR("battleground", "Player %s clicked on non-handled 'flag' %s!", player->GetGUID().ToString(), gameObject->GetGUID().ToString());
            break;
        default:
            break;
    }
}

void CaptureTheFlagBattlegroundScript::PlayerClickedOnDroppedFlag(Player* player, GameObject* gameObject)
{
    uint32 const entry = gameObject->GetEntry();
    switch (player->GetTeam())
    {
        case HORDE:
            if (entry == ALLIANCE_FLAG_ON_GROUND)
                AllianceFlagPickedUpFromGround(player);
            else if (entry == HORDE_FLAG_ON_GROUND)
                ReturnHordeFlag(player);
            else
                TC_LOG_ERROR("battleground", "Player %s clicked on non-handled 'dropped flag' %s!", player->GetGUID().ToString(), gameObject->GetGUID().ToString());
            break;
        case ALLIANCE:
            if (entry == HORDE_FLAG_ON_GROUND)
                HordeFlagPickedUpFromGround(player);
            else if (entry == ALLIANCE_FLAG_ON_GROUND)
                ReturnAllianceFlag(player);
            else
                TC_LOG_ERROR("battleground", "Player %s clicked on non-handled 'dropped flag' %s!", player->GetGUID().ToString(), gameObject->GetGUID().ToString());
            break;
        default:
            break;
    }
}

void CaptureTheFlagBattlegroundScript::OnPlayerDroppedFlag(Player* player)
{
    switch (player->GetTeam())
    {
        case ALLIANCE:
            if (!IsHordeFlagPickedUp())
                return;

            player->CastSpell(player, SPELL_RECENTLY_DROPPED_HORDE_FLAG, true);
            SetAllyThatCarriesHordeFlag(ObjectGuid::Empty);
            SetHordeFlagState(ON_GROUND);
            UpdateFlagWorldState(HORDE);
            battleground->SendBroadcastText(TEXT_HORDE_FLAG_DROPPED, CHAT_MSG_BG_SYSTEM_HORDE, player);

            OnHordeFlagDropped(player);
            if (GameObject* go = Map->GetGameObject(GetGuidData(DATA_HORDE_FLAG_IN_BASE)))
            {
                GameObject* droppedFlag = GameObject::CreateGameObject(go->GetGOInfo()->newflag.FlagDrop, Map, player->GetPosition(), QuaternionData::fromEulerAnglesZYX(player->GetOrientation(), 0.f, 0.f), 255, GO_STATE_READY);
                if (droppedFlag)
                    Map->AddToMap(droppedFlag);

                Scheduler.Schedule(Milliseconds(go->GetGOInfo()->newflag.RespawnTime), SCHEDULE_HORDE_FLAG_RESPAWN, [this](TaskContext)
                {
                    RespawnFlag(HORDE);
                });
            }
            break;
        case HORDE:
            if (!IsAllianceFlagPickedUp())
                return;

            player->CastSpell(player, SPELL_RECENTLY_DROPPED_ALLIANCE_FLAG, true);
            SetHordieThatCarriesAllianceFlag(ObjectGuid::Empty);
            SetAllianceFlagState(ON_GROUND);
            UpdateFlagWorldState(ALLIANCE);
            battleground->SendBroadcastText(TEXT_ALLIANCE_FLAG_DROPPED, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);

            OnAllianceFlagDropped(player);

            if (GameObject* go = Map->GetGameObject(GetGuidData(DATA_ALLIANCE_FLAG_IN_BASE)))
            {
                GameObject* droppedFlag = GameObject::CreateGameObject(go->GetGOInfo()->newflag.FlagDrop, Map, player->GetPosition(), QuaternionData::fromEulerAnglesZYX(player->GetOrientation(), 0.f, 0.f), 255, GO_STATE_READY);
                if (droppedFlag)
                    Map->AddToMap(droppedFlag);

                Scheduler.Schedule(Milliseconds(go->GetGOInfo()->newflag.RespawnTime), SCHEDULE_ALLIANCE_FLAG_RESPAWN, [this](TaskContext)
                {
                    RespawnFlag(ALLIANCE);
                });
            }
            break;
        default:
            break;
    }
}

void CaptureTheFlagBattlegroundScript::HordeFlagPickedUpFromBase(Player* ally)
{
    battleground->SendBroadcastText(TEXT_HORDE_FLAG_PICKED_UP, CHAT_MSG_BG_SYSTEM_ALLIANCE, ally);
    battleground->SendPacketToAll(WorldPackets::Misc::PlaySound(GetGuidData(DATA_HORDE_FLAG_IN_BASE), SOUND_HORDE_FLAG_PICKED_UP).Write());

    // todo review
    ToggleGameobjectSpawn(GetGuidData(DATA_HORDE_FLAG_IN_BASE), RESPAWN_ONE_DAY);
    SetAllyThatCarriesHordeFlag(ally->GetGUID());
    SetHordeFlagState(ON_PLAYER);
    UpdateFlagWorldState(HORDE);

    OnHordeFlagPickedUpFromBase(ally);
}

void CaptureTheFlagBattlegroundScript::AllianceFlagPickedUpFromBase(Player* hordie)
{
    battleground->SendBroadcastText(TEXT_ALLIANCE_FLAG_PICKED_UP, CHAT_MSG_BG_SYSTEM_HORDE, hordie);
    battleground->SendPacketToAll(WorldPackets::Misc::PlaySound(GetGuidData(DATA_ALLIANCE_FLAG_IN_BASE), SOUND_ALLIANCE_FLAG_PICKED_UP).Write());

    // todo review
    ToggleGameobjectSpawn(GetGuidData(DATA_ALLIANCE_FLAG_IN_BASE), RESPAWN_ONE_DAY);
    SetHordieThatCarriesAllianceFlag(hordie->GetGUID());
    SetAllianceFlagState(ON_PLAYER);
    UpdateFlagWorldState(ALLIANCE);

    OnAllianceFlagPickedUpFromBase(hordie);
}

void CaptureTheFlagBattlegroundScript::HordeFlagPickedUpFromGround(Player* player)
{
    Scheduler.CancelGroup(SCHEDULE_HORDE_FLAG_RESPAWN);

    battleground->SendBroadcastText(TEXT_HORDE_FLAG_PICKED_UP, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);
    battleground->PlaySoundToAll(SOUND_HORDE_FLAG_PICKED_UP);
    ToggleGameobjectSpawn(GetGuidData(DATA_HORDE_FLAG_ON_GROUND), RESPAWN_ONE_DAY);
    SetAllyThatCarriesHordeFlag(player->GetGUID());
    SetHordeFlagState(ON_PLAYER);

    UpdateFlagWorldState(HORDE);

    // They just add the aura now...
    if (GameObject* go = Map->GetGameObject(GetGuidData(DATA_HORDE_FLAG_IN_BASE)))
        player->AddAura(go->GetGOInfo()->newflag.pickupSpell, player);

    OnHordeFlagPickedUpFromGround(player);
}

void CaptureTheFlagBattlegroundScript::AllianceFlagPickedUpFromGround(Player* player)
{
    Scheduler.CancelGroup(SCHEDULE_ALLIANCE_FLAG_RESPAWN);

    battleground->SendBroadcastText(TEXT_ALLIANCE_FLAG_PICKED_UP, CHAT_MSG_BG_SYSTEM_HORDE, player);
    battleground->PlaySoundToAll(SOUND_ALLIANCE_FLAG_PICKED_UP);
    ToggleGameobjectSpawn(GetGuidData(DATA_ALLIANCE_FLAG_ON_GROUND), RESPAWN_ONE_DAY);
    SetHordieThatCarriesAllianceFlag(player->GetGUID());
    SetAllianceFlagState(ON_PLAYER);

    UpdateFlagWorldState(ALLIANCE);

    // They just add the aura now...
    if (GameObject* go = Map->GetGameObject(GetGuidData(DATA_ALLIANCE_FLAG_IN_BASE)))
        player->AddAura(go->GetGOInfo()->newflag.pickupSpell, player);

    OnAllianceFlagPickedUpFromGround(player);
}

void CaptureTheFlagBattlegroundScript::ReturnHordeFlag(Player* player)
{
    UpdateFlagWorldState(HORDE);
    battleground->SendBroadcastText(TEXT_HORDE_FLAG_RETURNED, CHAT_MSG_BG_SYSTEM_HORDE, player);
    battleground->PlaySoundToAll(SOUND_FLAG_RETURNED);
    RespawnFlag(HORDE);
    battleground->UpdatePlayerScore(player, SCORE_FLAG_RETURNS, 1);

    OnHordeFlagReturnedToBase(player);
}

void CaptureTheFlagBattlegroundScript::ReturnAllianceFlag(Player* player)
{
    UpdateFlagWorldState(ALLIANCE);
    battleground->SendBroadcastText(TEXT_ALLIANCE_FLAG_RETURNED, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);
    battleground->PlaySoundToAll(SOUND_FLAG_RETURNED);
    RespawnFlag(ALLIANCE);
    battleground->UpdatePlayerScore(player, SCORE_FLAG_RETURNS, 1);

    OnAllianceFlagReturnedToBase(player);
}

void CaptureTheFlagBattlegroundScript::CaptureAllianceFlag(Player* player)
{
    battleground->UpdatePlayerScore(player, SCORE_FLAG_CAPTURES, 1);

    battleground->PlaySoundToAll(SOUND_FLAG_CAPTURED_ALLIANCE);
    battleground->SendBroadcastText(TEXT_CAPTURED_ALLIANCE_FLAG, CHAT_MSG_BG_SYSTEM_HORDE, player);

    SetAllianceFlagState(WAIT_RESPAWN);
    SetHordieThatCarriesAllianceFlag(ObjectGuid::Empty);

    ToggleGameobjectSpawn(GetGuidData(DATA_HORDE_FLAG_IN_BASE), RESPAWN_ONE_DAY);

    SetHordeFlagState(WAIT_RESPAWN);
    SetAllianceFlagState(WAIT_RESPAWN);
    Scheduler.Schedule(10s, SCHEDULE_FLAGS_RESPAWN, [this](TaskContext)
    {
        RespawnFlagOnCapture();
    });

    battleground->AddPoints(HORDE, 1);
    UpdateFlagWorldState(ALLIANCE);
    UpdateWorldState(WORLD_STATE_FLAG_CAPTURES_HORDE, battleground->GetTeamScore(TEAM_HORDE));

    _lastFlagCapture = HORDE;

    player->RemoveAurasDueToSpell(SPELL_ALLIANCE_FLAG);

    OnAllianceFlagCaptured(player);

    if (battleground->GetTeamScore(TEAM_HORDE) == battleground->GetTemplate()->MaxPoints)
        battleground->EndBattleground(HORDE);
}

void CaptureTheFlagBattlegroundScript::CaptureHordeFlag(Player* player)
{
    battleground->UpdatePlayerScore(player, SCORE_FLAG_CAPTURES, 1);

    battleground->PlaySoundToAll(SOUND_FLAG_CAPTURED_HORDE);
    battleground->SendBroadcastText(TEXT_CAPTURED_HORDE_FLAG, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);

    SetAllianceFlagState(WAIT_RESPAWN);
    SetAllyThatCarriesHordeFlag(ObjectGuid::Empty);

    ToggleGameobjectSpawn(GetGuidData(DATA_ALLIANCE_FLAG_IN_BASE), RESPAWN_ONE_DAY);

    SetHordeFlagState(WAIT_RESPAWN);
    SetAllianceFlagState(WAIT_RESPAWN);
    Scheduler.Schedule(10s, SCHEDULE_FLAGS_RESPAWN, [this](TaskContext)
    {
        RespawnFlagOnCapture();
    });

    battleground->AddPoints(ALLIANCE, 1);
    UpdateFlagWorldState(HORDE);
    UpdateWorldState(WORLD_STATE_FLAG_CAPTURES_ALLIANCE, battleground->GetTeamScore(TEAM_ALLIANCE));

    _lastFlagCapture = ALLIANCE;

    player->RemoveAurasDueToSpell(SPELL_HORDE_FLAG);

    OnHordeFlagCaptured(player);

    if (battleground->GetTeamScore(TEAM_ALLIANCE) == battleground->GetTemplate()->MaxPoints)
        battleground->EndBattleground(ALLIANCE);
}

void CaptureTheFlagBattlegroundScript::UpdateFlagWorldState(Team team)
{
    switch (team)
    {
        case ALLIANCE:
            UpdateWorldState(WORLD_STATE_FLAG_STATE_ALLIANCE, GetAllianceFlagState());
            UpdateWorldState(WORLD_STATE_ALLIANCE_FLAG_PICKED_UP, IsAllianceFlagPickedUp() ? 2 : 1);
            break;
        case HORDE:
            UpdateWorldState(WORLD_STATE_FLAG_STATE_HORDE, GetHordeFlagState());
            UpdateWorldState(WORLD_STATE_HORDE_FLAG_PICKED_UP, IsHordeFlagPickedUp() ? 2 : 1);
            break;
        default:
            break;
    }
}

void CaptureTheFlagBattlegroundScript::OnGameObjectCreate(GameObject* gameObject)
{
    uint32 const entry = gameObject->GetEntry();

    if (entry == ALLIANCE_FLAG_ON_GROUND)
        SetGuidData(DATA_ALLIANCE_FLAG_ON_GROUND, gameObject->GetGUID());
    else if (entry == ALLIANCE_FLAG_IN_BASE)
        SetGuidData(DATA_ALLIANCE_FLAG_IN_BASE, gameObject->GetGUID());
    else if (entry == HORDE_FLAG_ON_GROUND)
        SetGuidData(DATA_HORDE_FLAG_ON_GROUND, gameObject->GetGUID());
    else if (entry == HORDE_FLAG_IN_BASE)
        SetGuidData(DATA_HORDE_FLAG_IN_BASE, gameObject->GetGUID());
}

void CaptureTheFlagBattlegroundScript::BuildPlayerPositionData(std::vector<WorldPackets::Battleground::BattlegroundPlayerPosition>* positions) const
{
    if (Player* player = ObjectAccessor::GetPlayer(Map, GetAllyThatCarriesHordeFlag()))
    {
        WorldPackets::Battleground::BattlegroundPlayerPosition position;
        position.Guid = player->GetGUID();
        position.Pos = player->GetPosition();
        position.IconID = PLAYER_POSITION_ICON_HORDE_FLAG;
        position.ArenaSlot = PLAYER_POSITION_ARENA_SLOT_1;
        positions->push_back(position);
    }

    if (Player* player = ObjectAccessor::GetPlayer(Map, GetHordieThatCarriesAllianceFlag()))
    {
        WorldPackets::Battleground::BattlegroundPlayerPosition position;
        position.Guid = player->GetGUID();
        position.Pos = player->GetPosition();
        position.IconID = PLAYER_POSITION_ICON_ALLIANCE_FLAG;
        position.ArenaSlot = PLAYER_POSITION_ARENA_SLOT_2;
        positions->push_back(position);
    }
}
