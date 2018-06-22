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

class TC_GAME_API AssaultStrategy : public virtual BattlegroundScript
{
public:
    AssaultStrategy(BattlegroundMap* map) : BattlegroundScript(map), _enabled(false), _stackCount(0), _assaultTimer(ASSAULT_TIMER * 1000) { }

    static uint32 const ASSAULT_TIMER = 30;
    static uint32 const ASSAULT_SCHEDULE_GROUP = 50;
    static uint32 const STACK_COUNT_TO_BRUTAL_ASSAULT = 5;
    static uint32 const MAX_STACKS = 15;

    enum Spells
    {
        SPELL_FOCUSED_ASSAULT = 46392,
        SPELL_BRUTAL_ASSAULT = 46393
    };

    enum State
    {
        ASSAULT_STATE_NONE,
        ASSAULT_STATE_FOCUSED,
        ASSAULT_STATE_BRUTAL
    };

    void OnUpdateInProgress(uint32 diff) override;

    void ApplyAssault(std::vector<Player*> const& players);

    void ResetAssault();

    void AddAndEnablePlayerToAssault(Player* player);
    void AddPlayerToAssault(Player* player) const;
    static void RemovePlayerFromAssault(Player* player);

    State GetAssaultState() const
    {
        if (!_stackCount)
            return ASSAULT_STATE_NONE;

        if (_stackCount < STACK_COUNT_TO_BRUTAL_ASSAULT)
            return ASSAULT_STATE_FOCUSED;

        return ASSAULT_STATE_BRUTAL;
    }

    void EnableAssault() { _enabled = true; }
    void DisableAssault() { _enabled = false; }

    virtual std::vector<Player*> GetAffectedPlayers() const = 0;

private:
    bool _enabled;
    uint8 _stackCount;
    uint32 _assaultTimer;
};
