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

#include "PeriodicResourceGainBattlegroundScript.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"

PeriodicResourceGainBattlegroundScript::PeriodicResourceGainBattlegroundScript(BattlegroundMap* map, int32 tickUpdate, int32 scoreHonor, uint32 initialPointsToAdd) :
    BattlegroundScript(map), INIT_TICK_UPDATE(tickUpdate), _pointsToAdd{ initialPointsToAdd, initialPointsToAdd }, _scoreUpdateTimer(tickUpdate), _scoreHonor(scoreHonor),
    _lastHonorScore{0, 0}
{
    Worldstates[WORLD_STATE_SCORE_ALLIANCE] = 0;
    Worldstates[WORLD_STATE_SCORE_HORDE] = 0;
    Worldstates[WORLD_STATE_MAX_SCORE] = battleground->GetTemplate()->MaxPoints;
}

void PeriodicResourceGainBattlegroundScript::OnUpdateInProgress(uint32 diff)
{
    if (battleground->GetTeamScore(TEAM_ALLIANCE) == battleground->GetTemplate()->MaxPoints || battleground->GetTeamScore(TEAM_HORDE) == battleground->GetTemplate()->MaxPoints)
    {
        if (battleground->GetTeamScore(TEAM_ALLIANCE) == battleground->GetTeamScore(TEAM_HORDE))
            battleground->EndBattleground(TEAM_OTHER);
        else if (battleground->GetTeamScore(TEAM_ALLIANCE) > battleground->GetTeamScore(TEAM_HORDE))
            battleground->EndBattleground(ALLIANCE);
        else
            battleground->EndBattleground(HORDE);

        return;
    }

    if (battleground->GetTeamScore(TEAM_HORDE) >= battleground->GetTemplate()->MaxPoints)
    {
        battleground->EndBattleground(HORDE);
        return;
    }

    if (_scoreUpdateTimer <= static_cast<int32>(diff))
    {
        if (_pointsToAdd[TEAM_ALLIANCE] > 0)
        {
            battleground->AddPoints(ALLIANCE, std::min(_pointsToAdd[TEAM_ALLIANCE], battleground->GetTemplate()->MaxPoints - battleground->GetTeamScore(TEAM_ALLIANCE)));
            UpdateWorldState(WORLD_STATE_SCORE_ALLIANCE, battleground->GetTeamScore(TEAM_ALLIANCE));
        }

        if (_pointsToAdd[TEAM_HORDE] > 0)
        {
            battleground->AddPoints(HORDE, std::min(_pointsToAdd[TEAM_HORDE], battleground->GetTemplate()->MaxPoints - battleground->GetTeamScore(TEAM_HORDE)));
            UpdateWorldState(WORLD_STATE_SCORE_HORDE, battleground->GetTeamScore(TEAM_HORDE));
        }

        // take floor, check if it is last honor thing, if not, add 10 honor and make it the last one
        if ((battleground->GetTeamScore(TEAM_ALLIANCE) / _scoreHonor) != static_cast<uint32>(_lastHonorScore[TEAM_ALLIANCE]))
        {
            _lastHonorScore[TEAM_ALLIANCE] = battleground->GetTeamScore(TEAM_ALLIANCE) / _scoreHonor;
            for (auto const& bgPlayer : battleground->GetPlayers())
                if (bgPlayer.second.Team == ALLIANCE)
                    if (Player* player = ObjectAccessor::FindConnectedPlayer(bgPlayer.first))
                        player->RewardHonor(nullptr, 0, 10);
        }

        if ((battleground->GetTeamScore(TEAM_HORDE) / _scoreHonor) != static_cast<uint32>(_lastHonorScore[TEAM_HORDE]))
        {
            _lastHonorScore[TEAM_HORDE] = battleground->GetTeamScore(TEAM_HORDE) / _scoreHonor;
            for (auto const& bgPlayer : battleground->GetPlayers())
                if (bgPlayer.second.Team == HORDE)
                    if (Player* player = ObjectAccessor::FindConnectedPlayer(bgPlayer.first))
                        player->RewardHonor(nullptr, 0, 10);
        }

        _scoreUpdateTimer = INIT_TICK_UPDATE;
    }
    else
        _scoreUpdateTimer -= diff;
}

void PeriodicResourceGainBattlegroundScript::SetResourcesForTeam(Team team, int32 points) const
{
    battleground->SetPoints(team, points);
}

