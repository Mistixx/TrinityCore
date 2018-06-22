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

#pragma once

#include "BattlegroundScript.h"
#include "CapturePointSpawnBattlegroundScript.h"

#include <unordered_map>

enum CapturePointStatus : uint8
{
    CAPTURE_POINT_STATUS_NEUTRAL             = 1,
    CAPTURE_POINT_STATUS_HORDE_CONTESTED     = 2,
    CAPTURE_POINT_STATUS_ALLIANCE_CONTESTED  = 3,
    CAPTURE_POINT_STATUS_HORDE_TAKEN         = 4,
    CAPTURE_POINT_STATUS_ALLIANCE_TAKEN      = 5
};

struct CapturePointInfo
{
    CapturePointStatus Status = CAPTURE_POINT_STATUS_NEUTRAL;
    Team CurrentOwner = TEAM_OTHER;
    uint32 ScheduleGroup = 0;
    WorldSafeLocsEntry const* GraveYard = nullptr;

    bool IsTaken() const
    {
        return Status == CAPTURE_POINT_STATUS_ALLIANCE_TAKEN || Status == CAPTURE_POINT_STATUS_HORDE_TAKEN;
    }

    bool IsTakenBy(Team team) const
    {
        return team == ALLIANCE ? Status == CAPTURE_POINT_STATUS_ALLIANCE_TAKEN : Status == CAPTURE_POINT_STATUS_HORDE_TAKEN;
    }
};

class TC_GAME_API ControlPointBattlegroundScript : public virtual BattlegroundScript, public virtual ControlPointSpawnsBattlegroundScript
{
public:
    explicit ControlPointBattlegroundScript(BattlegroundMap* map) : BattlegroundScript(map), ControlPointSpawnsBattlegroundScript(map) { }

    enum Texts
    {
        TEXT_ALLIANCE_NEAR_VICOTRY  = 10598,
        TEXT_HORDE_NEAR_VICTORY     = 10599
    };

    enum Sounds
    {
        PVP_FLAG_CAPTURED_ALLIANCE  = 8173,
        PVP_FLAG_TAKEN_ALLIANCE     = 8174,
        PVP_FLAG_TAKEN_HORDE        = 8212,
        PVP_FLAG_CAPTURED_HORDE     = 8213
    };

    enum WorldStates
    {
        WORLD_STATE_ALLIANCE_BASE_COUNT = 1779,
        WORLD_STATE_HORDE_BASE_COUNT    = 1778
    };

    enum Creatures
    {
        WORLD_TRIGGER_HORDE_FRIENDLY    = 20212, // Honorable Defender
        WORLD_TRIGGER_ALLIANCE_FRIENDLY = 20213,
        NPC_ALLIANCE_SPIRIT_GUIDE       = 13116,
        NPC_HORDE_SPIRIT_GUIDE          = 13117
    };

    bool OnGameObjectClicked(Player* player, GameObject* gameobject) override;
    void CapturePointClicked(Player* player, GameObject* gameObject);

    void ScheduleCapture(WorldObject* player, Team team, GameObject* gameObject, uint32 scheduleGroup);

    void HordeAssaultCapturePoint(GameObject* gameObject, WorldObject* player);
    void AllianceAssaultCapturePoint(GameObject* gameObject, WorldObject* player);
    void HordeCaptureCapturePoint(GameObject* gameObject, WorldObject* player);
    void AllianceCaptureCapturePoint(GameObject* gameObject, WorldObject* player);
    void HordeDefendCapturePoint(GameObject* gameObject, WorldObject* player);
    void AllianceDefendCapturePoint(GameObject* gameObject, WorldObject* player);

    virtual void OnHordeAssaultCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) { }
    virtual void OnAllianceAssaultCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) { }
    virtual void OnHordeCaptureCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) { }
    virtual void OnAllianceCaptureCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) { }
    virtual void OnHordeDefendCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) { }
    virtual void OnAllianceDefendCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) { }

    // This function will update the worldstate & the status of the given capture point
    void UpdateCapturePointStatus(GameObject* gameObject, CapturePointStatus newStatus);

    uint8 GetTeamCapturedCapturePointsCount(Team team)
    {
        return std::count_if(CapturePoints.begin(), CapturePoints.end(), [team](std::pair<uint32, CapturePointInfo> const& capturePointInfo)
        {
            return capturePointInfo.second.CurrentOwner == team && (capturePointInfo.second.Status == CAPTURE_POINT_STATUS_ALLIANCE_TAKEN || capturePointInfo.second.Status == CAPTURE_POINT_STATUS_HORDE_TAKEN);
        });
    }

protected:
    std::unordered_map<uint32, CapturePointInfo> CapturePoints;
};
