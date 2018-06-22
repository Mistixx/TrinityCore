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

#include "ControlPointBattlegroundScript.h"
#include "BattlegroundMgr.h"
#include "BattlegroundScore.h"
#include "ScriptedCreature.h"

bool ControlPointBattlegroundScript::OnGameObjectClicked(Player* player, GameObject* gameobject)
{
    if (gameobject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT)
    {
        CapturePointClicked(player, gameobject);
        return true;
    }

    return false;
}

void ControlPointBattlegroundScript::CapturePointClicked(Player* player, GameObject* gameObject)
{
    if (CapturePoints.find(gameObject->GetEntry()) == CapturePoints.end())
        return;

    ASSERT(gameObject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT);

    switch (player->GetTeam())
    {
        case ALLIANCE:
        {
            switch (CapturePoints[gameObject->GetEntry()].Status)
            {
                case CAPTURE_POINT_STATUS_HORDE_CONTESTED:
                    if (CapturePoints[gameObject->GetEntry()].CurrentOwner == ALLIANCE)
                    {
                        ProcessEvent(gameObject, gameObject->GetGOInfo()->capturePoint.DefendedEventAlliance, player);
                        break;
                    }
                    // no break
                case CAPTURE_POINT_STATUS_HORDE_TAKEN:
                case CAPTURE_POINT_STATUS_NEUTRAL:
                    ProcessEvent(gameObject, gameObject->GetGOInfo()->capturePoint.ContestedEventAlliance, player);
                    break;
                default:
                    break;
            }
            break;
        }
        case HORDE:
        {
            switch (CapturePoints[gameObject->GetEntry()].Status)
            {
                case CAPTURE_POINT_STATUS_ALLIANCE_CONTESTED:
                    if (CapturePoints[gameObject->GetEntry()].CurrentOwner == HORDE)
                    {
                        ProcessEvent(gameObject, gameObject->GetGOInfo()->capturePoint.DefendedEventHorde, player);
                        break;
                    }
                    // no break
                case CAPTURE_POINT_STATUS_ALLIANCE_TAKEN:
                case CAPTURE_POINT_STATUS_NEUTRAL:
                    ProcessEvent(gameObject, gameObject->GetGOInfo()->capturePoint.ContestedEventHorde, player);
                    break;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

void ControlPointBattlegroundScript::ScheduleCapture(WorldObject* player, Team team, GameObject* gameObject, uint32 scheduleGroup)
{
    Scheduler.Schedule(Milliseconds(gameObject->GetGOInfo()->capturePoint.CaptureTime), scheduleGroup, [this, player, team, gameObject] (TaskContext)
    {
        ProcessEvent(gameObject, team == ALLIANCE ? gameObject->GetGOInfo()->capturePoint.CaptureEventAlliance : gameObject->GetGOInfo()->capturePoint.CaptureEventHorde, player);
    });
}

void ControlPointBattlegroundScript::HordeAssaultCapturePoint(GameObject* gameObject, WorldObject* player)
{
    ASSERT(gameObject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT);

    gameObject->SendCustomAnim(1);
    gameObject->SetSpellVisual(gameObject->GetGOInfo()->capturePoint.SpellVisual2);

    battleground->SendBroadcastText(gameObject->GetGOInfo()->capturePoint.AssaultBroadcastHorde, CHAT_MSG_BG_SYSTEM_HORDE, player);
    battleground->PlaySoundToAll(PVP_FLAG_TAKEN_HORDE, gameObject->GetGUID());
    UpdateCapturePointStatus(gameObject, CAPTURE_POINT_STATUS_HORDE_CONTESTED);

    if (std::vector<ObjectGuid::LowType> const* creaturesSpawnIds = GetSpawnedCreatures(gameObject->GetEntry()))
    {
        for (ObjectGuid::LowType const spawnId : *creaturesSpawnIds)
        {
            auto const bounds = Map->GetCreatureBySpawnIdStore().equal_range(spawnId);
            if (bounds.first != bounds.second)
                for (auto itr = bounds.first; itr != bounds.second; ++itr)
                    if (itr->second->GetEntry() == NPC_HORDE_SPIRIT_GUIDE || itr->second->GetEntry() == NPC_ALLIANCE_SPIRIT_GUIDE)
                        battleground->RelocateDeadPlayers(itr->second->GetGUID());
        }
    }

    DespawnCapturePointEntities(gameObject->GetEntry());
    Scheduler.CancelGroup(CapturePoints[gameObject->GetEntry()].ScheduleGroup);
    ScheduleCapture(player, HORDE, gameObject, CapturePoints[gameObject->GetEntry()].ScheduleGroup);
    if (player && player->IsPlayer())
        battleground->UpdatePlayerScore(player->ToPlayer(), SCORE_BASES_ASSAULTED, 1);

    if (CapturePoints[gameObject->GetEntry()].CurrentOwner == ALLIANCE)
    {
        UpdateWorldState(WORLD_STATE_ALLIANCE_BASE_COUNT, GetTeamCapturedCapturePointsCount(ALLIANCE));
        if (Creature* trigger = GetClosestCreatureWithEntry(gameObject, WORLD_TRIGGER_ALLIANCE_FRIENDLY, 10.0f))
            trigger->RemoveFromWorld();
    }

    OnHordeAssaultCapturePoint(gameObject, player);
}

void ControlPointBattlegroundScript::AllianceAssaultCapturePoint(GameObject* gameObject, WorldObject* player)
{
    ASSERT(gameObject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT);

    gameObject->SendCustomAnim(2);
    gameObject->SetSpellVisual(gameObject->GetGOInfo()->capturePoint.SpellVisual3);

    battleground->SendBroadcastText(gameObject->GetGOInfo()->capturePoint.AssaultBroadcastAlliance, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);
    battleground->PlaySoundToAll(PVP_FLAG_TAKEN_ALLIANCE, gameObject->GetGUID());
    UpdateCapturePointStatus(gameObject, CAPTURE_POINT_STATUS_ALLIANCE_CONTESTED);

    if (std::vector<ObjectGuid::LowType> const* creaturesSpawnIds = GetSpawnedCreatures(gameObject->GetEntry()))
    {
        for (ObjectGuid::LowType const spawnId : *creaturesSpawnIds)
        {
            auto const bounds = Map->GetCreatureBySpawnIdStore().equal_range(spawnId);
            if (bounds.first != bounds.second)
                for (auto itr = bounds.first; itr != bounds.second; ++itr)
                    if (itr->second->GetEntry() == NPC_HORDE_SPIRIT_GUIDE || itr->second->GetEntry() == NPC_ALLIANCE_SPIRIT_GUIDE)
                        battleground->RelocateDeadPlayers(itr->second->GetGUID());
        }
    }

    DespawnCapturePointEntities(gameObject->GetEntry());
    Scheduler.CancelGroup(CapturePoints[gameObject->GetEntry()].ScheduleGroup);
    ScheduleCapture(player, ALLIANCE, gameObject, CapturePoints[gameObject->GetEntry()].ScheduleGroup);
    if (player && player->IsPlayer())
        battleground->UpdatePlayerScore(player->ToPlayer(), SCORE_BASES_ASSAULTED, 1);

    if (CapturePoints[gameObject->GetEntry()].CurrentOwner == HORDE)
    {
        UpdateWorldState(WORLD_STATE_HORDE_BASE_COUNT, GetTeamCapturedCapturePointsCount(HORDE));
        if (Creature* trigger = GetClosestCreatureWithEntry(gameObject, WORLD_TRIGGER_HORDE_FRIENDLY, 10.0f))
            trigger->RemoveFromWorld();
    }

    OnAllianceAssaultCapturePoint(gameObject, player);
}

void ControlPointBattlegroundScript::HordeCaptureCapturePoint(GameObject* gameObject, WorldObject* player)
{
    ASSERT(gameObject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT);

    gameObject->SendCustomAnim(3);
    if (gameObject->GetGOInfo()->capturePoint.SpellVisual4)
        gameObject->SetSpellVisual(gameObject->GetGOInfo()->capturePoint.SpellVisual4);
    if (gameObject->GetGOInfo()->capturePoint.CaptureBroadcastHorde)
        battleground->SendBroadcastText(gameObject->GetGOInfo()->capturePoint.CaptureBroadcastHorde, CHAT_MSG_BG_SYSTEM_HORDE, player);
    battleground->PlaySoundToAll(PVP_FLAG_CAPTURED_HORDE, gameObject->GetGUID());
    UpdateCapturePointStatus(gameObject, CAPTURE_POINT_STATUS_HORDE_TAKEN);

    SpawnCapturePointEntities(gameObject->GetEntry(), TEAM_HORDE);
    CapturePoints[gameObject->GetEntry()].CurrentOwner = HORDE;

    UpdateWorldState(WORLD_STATE_HORDE_BASE_COUNT, GetTeamCapturedCapturePointsCount(HORDE));

    AddCreature(WORLD_TRIGGER_HORDE_FRIENDLY, gameObject->GetPosition());

    OnHordeCaptureCapturePoint(gameObject, player);
}

void ControlPointBattlegroundScript::AllianceCaptureCapturePoint(GameObject* gameObject, WorldObject* player)
{
    ASSERT(gameObject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT);

    gameObject->SendCustomAnim(4);
    gameObject->SetSpellVisual(gameObject->GetGOInfo()->capturePoint.SpellVisual5);
    battleground->SendBroadcastText(gameObject->GetGOInfo()->capturePoint.CaptureBroadcastAlliance, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);
    battleground->PlaySoundToAll(PVP_FLAG_CAPTURED_ALLIANCE, gameObject->GetGUID());
    UpdateCapturePointStatus(gameObject, CAPTURE_POINT_STATUS_ALLIANCE_TAKEN);

    SpawnCapturePointEntities(gameObject->GetEntry(), TEAM_ALLIANCE);
    CapturePoints[gameObject->GetEntry()].CurrentOwner = ALLIANCE;

    UpdateWorldState(WORLD_STATE_ALLIANCE_BASE_COUNT, GetTeamCapturedCapturePointsCount(ALLIANCE));

    AddCreature(WORLD_TRIGGER_ALLIANCE_FRIENDLY, gameObject->GetPosition());

    OnAllianceCaptureCapturePoint(gameObject, player);
}

void ControlPointBattlegroundScript::HordeDefendCapturePoint(GameObject* gameObject, WorldObject* player)
{
    ASSERT(gameObject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT);

    Scheduler.CancelGroup(CapturePoints[gameObject->GetEntry()].ScheduleGroup);
    gameObject->SendCustomAnim(3);
    gameObject->SetSpellVisual(gameObject->GetGOInfo()->capturePoint.SpellVisual4);

    battleground->SendBroadcastText(gameObject->GetGOInfo()->capturePoint.DefendedBroadcastHorde, CHAT_MSG_BG_SYSTEM_HORDE, player);
    battleground->PlaySoundToAll(PVP_FLAG_CAPTURED_HORDE, gameObject->GetGUID());
    UpdateCapturePointStatus(gameObject, CAPTURE_POINT_STATUS_HORDE_TAKEN);

    SpawnCapturePointEntities(gameObject->GetEntry(), TEAM_HORDE);
    if (player && player->IsPlayer())
        battleground->UpdatePlayerScore(player->ToPlayer(), SCORE_BASES_DEFENDED, 1);

    UpdateWorldState(WORLD_STATE_HORDE_BASE_COUNT, GetTeamCapturedCapturePointsCount(HORDE));

    AddCreature(WORLD_TRIGGER_HORDE_FRIENDLY, gameObject->GetPosition());

    OnHordeDefendCapturePoint(gameObject, player);
}

void ControlPointBattlegroundScript::AllianceDefendCapturePoint(GameObject* gameObject, WorldObject* player)
{
    ASSERT(gameObject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT);

    Scheduler.CancelGroup(CapturePoints[gameObject->GetEntry()].ScheduleGroup);
    gameObject->SendCustomAnim(4);
    gameObject->SetSpellVisual(gameObject->GetGOInfo()->capturePoint.SpellVisual5);
    battleground->SendBroadcastText(gameObject->GetGOInfo()->capturePoint.DefendedBroadcastAlliance, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);
    battleground->PlaySoundToAll(PVP_FLAG_CAPTURED_ALLIANCE, gameObject->GetGUID());
    UpdateCapturePointStatus(gameObject, CAPTURE_POINT_STATUS_ALLIANCE_TAKEN);

    SpawnCapturePointEntities(gameObject->GetEntry(), TEAM_ALLIANCE);
    if (player && player->IsPlayer())
        battleground->UpdatePlayerScore(player->ToPlayer() , SCORE_BASES_DEFENDED, 1);

    UpdateWorldState(WORLD_STATE_ALLIANCE_BASE_COUNT, GetTeamCapturedCapturePointsCount(ALLIANCE));

    AddCreature(WORLD_TRIGGER_ALLIANCE_FRIENDLY, gameObject->GetPosition());

    OnAllianceDefendCapturePoint(gameObject, player);
}

void ControlPointBattlegroundScript::UpdateCapturePointStatus(GameObject* gameObject, CapturePointStatus newStatus)
{
    ASSERT(gameObject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT);

    CapturePoints[gameObject->GetEntry()].Status = newStatus;
    if (gameObject->GetGOInfo()->capturePoint.worldState1)
        UpdateWorldState(gameObject->GetGOInfo()->capturePoint.worldState1, newStatus);
}
