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

class TC_GAME_API PeriodicResourceGainBattlegroundScript : public virtual BattlegroundScript
{
public:
    explicit PeriodicResourceGainBattlegroundScript(BattlegroundMap* map, int32 tickUpdate = 2000, int32 scoreHonor = 100, uint32 initialPoints = 0);

    enum ScoreWorldStates
    {
        WORLD_STATE_SCORE_ALLIANCE  = 1776,
        WORLD_STATE_SCORE_HORDE     = 1777,
        WORLD_STATE_MAX_SCORE       = 1780
    };

    void OnUpdateInProgress(uint32 diff) override;

    void SetPointsToAddForTeam(TeamId teamId, int32 points)
    {
        _pointsToAdd[teamId] = points;
    }

    void SetResourcesForTeam(Team team, int32 points) const;

    int32 const INIT_TICK_UPDATE;
private:
    uint32 _pointsToAdd[BG_TEAMS_COUNT];
    int32 _scoreUpdateTimer;
    int32 _scoreHonor;
    int32 _lastHonorScore[BG_TEAMS_COUNT];
};
