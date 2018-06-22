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

#include "TimedBattlegroundScript.h"
#include "Battleground.h"
#include "WorldStatePackets.h"

TimedBattlegroundScript::TimedBattlegroundScript(BattlegroundMap* map, uint32 resetMaxEndTimeTimer, uint32 maxDuration) :
    BattlegroundScript(map), _resetMaxEndTimer(resetMaxEndTimeTimer), MAX_DURATION(maxDuration), _maxEndTime(time(nullptr) + maxDuration)
{
    Worldstates[WORLD_STATE_BATTLEGROUND_MAX_END_TIME] = _maxEndTime;
    Worldstates[WORLD_STATE_TIMER_ACTIVE] = 0;
}

void TimedBattlegroundScript::OnUpdateWaitJoin(uint32 diff)
{
    if (_resetMaxEndTimer <= diff)
    {
        _resetMaxEndTimer = 3000;
        ResetMaxEndTime();
        UpdateWorldState(WORLD_STATE_BATTLEGROUND_MAX_END_TIME, _maxEndTime);
    }
    else
        _resetMaxEndTimer -= diff;
}

void TimedBattlegroundScript::OnUpdateInProgress(uint32 /*diff*/)
{
    if (time(nullptr) >= _maxEndTime)
        battleground->EndBattleground(GetWinnerWhenTimeElapsed());
}

void TimedBattlegroundScript::OnStart()
{
    UpdateWorldState(WORLD_STATE_TIMER_ACTIVE, 1);
}
