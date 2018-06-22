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

class TC_GAME_API CaptureTheFlagBattlegroundScript : public virtual BattlegroundScript
{
public:
    enum Data : uint32
    {
        DATA_ALLIANCE_FLAG_IN_BASE,
        DATA_HORDE_FLAG_IN_BASE,
        DATA_ALLIANCE_FLAG_ON_GROUND,
        DATA_HORDE_FLAG_ON_GROUND
    };

    enum FlagState : uint8
    {
        IN_BASE,
        ON_PLAYER,
        ON_GROUND,
        WAIT_RESPAWN // aka captured
    };

    enum FlagText
    {
        TEXT_CAPTURED_HORDE_FLAG        = 9801,
        TEXT_CAPTURED_ALLIANCE_FLAG     = 9802,
        TEXT_FLAGS_PLACED               = 9803,
        TEXT_ALLIANCE_FLAG_PICKED_UP    = 9804,
        TEXT_ALLIANCE_FLAG_DROPPED      = 9805,
        TEXT_HORDE_FLAG_PICKED_UP       = 9807,
        TEXT_HORDE_FLAG_DROPPED         = 9806,
        TEXT_ALLIANCE_FLAG_RETURNED     = 9808,
        TEXT_HORDE_FLAG_RETURNED        = 9809
    };

    enum FlagSound
    {
        SOUND_FLAG_CAPTURED_ALLIANCE    = 8173,
        SOUND_FLAG_CAPTURED_HORDE       = 8213,
        SOUND_FLAG_PLACED               = 8232,
        SOUND_FLAG_RETURNED             = 8192,
        SOUND_HORDE_FLAG_PICKED_UP      = 8212,
        SOUND_ALLIANCE_FLAG_PICKED_UP   = 8174,
        SOUND_FLAGS_RESPAWNED           = 8232
    };

    enum Spells
    {
        SPELL_FOCUSED_ASSAULT                   = 46392,
        SPELL_BRUTAL_ASSAULT                    = 46393,
        SPELL_RECENTLY_DROPPED_HORDE_FLAG       = 50326,
        SPELL_RECENTLY_DROPPED_ALLIANCE_FLAG    = 42792,
        SPELL_HORDE_FLAG                        = 156618,
        SPELL_ALLIANCE_FLAG                     = 156621
    };

    enum ScheduleGroup
    {
        SCHEDULE_FLAGS_RESPAWN,
        SCHEDULE_HORDE_FLAG_RESPAWN,
        SCHEDULE_ALLIANCE_FLAG_RESPAWN
    };

    enum GameObjects
    {
        ALLIANCE_FLAG_IN_BASE   = 227741,
        HORDE_FLAG_IN_BASE      = 227740,
        ALLIANCE_FLAG_ON_GROUND = 227745,
        HORDE_FLAG_ON_GROUND    = 227744
    };

    explicit CaptureTheFlagBattlegroundScript(BattlegroundMap* map);
    virtual ~CaptureTheFlagBattlegroundScript() = default;

    enum WorldStatesCtf
    {
        /*
        * 1 - in base (returned)
        * 2 - picked up
        * 3 - dropped
        * 4 - captured
        */
        WORLD_STATE_FLAG_STATE_ALLIANCE = 1545,
        /*
        * 1 - in base (returned)
        * 2 - picked up
        * 3 - dropped
        * 4 - captured
        */
        WORLD_STATE_FLAG_STATE_HORDE = 1546,
        WORLD_STATE_FLAG_CAPTURES_ALLIANCE = 1581,
        WORLD_STATE_FLAG_CAPTURES_HORDE = 1582,
        WORLD_STATE_FLAG_CAPTURES_MAX = 1601,
        /*
        * 1 - $n captured the Alliance Flag! || The Alliance Flag was dropped by $n!
        * 2 - The Alliance Flag was picked up by $n!
        */
        WORLD_STATE_ALLIANCE_FLAG_PICKED_UP = 2338,
        /*
        * 1 - The Horde Flag was dropped by $n! || The Horde Flag was returned to its base by $n!
        * 2 - The Horde Flag was picked up by $n!
        */
        WORLD_STATE_HORDE_FLAG_PICKED_UP = 2339
    };

    void OnUpdateInProgress(uint32 diff) override;

    void RespawnFlagOnCapture();
    void RespawnFlag(Team team);

    ObjectGuid GetAllyThatCarriesHordeFlag() const { return _flagCarriers[TEAM_HORDE]; }
    ObjectGuid GetHordieThatCarriesAllianceFlag() const { return _flagCarriers[TEAM_ALLIANCE]; }

    void SetAllyThatCarriesHordeFlag(ObjectGuid objectGuid) { _flagCarriers[TEAM_HORDE] = objectGuid; }
    void SetHordieThatCarriesAllianceFlag(ObjectGuid objectGuid) { _flagCarriers[TEAM_ALLIANCE] = objectGuid; }

    bool IsHordeFlagInDanger() const
    {
        return IsHordeFlagPickedUp() || _flagStates[TEAM_HORDE] == ON_GROUND;
    }
    bool IsHordeFlagPickedUp() const
    {
        return _flagStates[TEAM_HORDE] == ON_PLAYER;
    }
    bool IsHordeFlagInBase() const
    {
        return _flagStates[TEAM_HORDE] == IN_BASE;
    }
    bool IsHordeFlagOnGround() const
    {
        return _flagStates[TEAM_HORDE] == ON_GROUND;
    }
    bool IsHordeFlagWaitingForRespawn() const
    {
        return _flagStates[TEAM_HORDE] == WAIT_RESPAWN;
    }

    FlagState GetHordeFlagState() { return _flagStates[TEAM_HORDE]; }

    bool IsAllianceFlagInDanger() const
    {
        return IsAllianceFlagPickedUp() || _flagStates[TEAM_ALLIANCE] == ON_GROUND;
    }
    bool IsAllianceFlagPickedUp() const
    {
        return _flagStates[TEAM_ALLIANCE] == ON_PLAYER;
    }
    bool IsAllianceFlagInBase() const
    {
        return _flagStates[TEAM_ALLIANCE] == IN_BASE;
    }
    bool IsAllianceFlagOnGround() const
    {
        return _flagStates[TEAM_ALLIANCE] == ON_GROUND;
    }
    bool IsAllianceFlagWaitingForRespawn() const
    {
        return _flagStates[TEAM_ALLIANCE] == WAIT_RESPAWN;
    }

    FlagState GetAllianceFlagState() const { return _flagStates[TEAM_ALLIANCE]; }

    void SetHordeFlagState(FlagState state) { _flagStates[TEAM_HORDE] = state; }
    void SetAllianceFlagState(FlagState state) { _flagStates[TEAM_ALLIANCE] = state; }

    Team CalculatePrematureWinner() override;

    bool OnGameObjectClicked(Player* player, GameObject* gameobject) override;

    void PlayerClickedOnFlag(Player* player, GameObject* gameObject);
    void PlayerClickedOnDroppedFlag(Player* player, GameObject* gameObject);
    void OnPlayerDroppedFlag(Player* player) override;

    void HordeFlagPickedUpFromBase(Player* ally);
    void AllianceFlagPickedUpFromBase(Player* hordie);
    void HordeFlagPickedUpFromGround(Player* player);
    void AllianceFlagPickedUpFromGround(Player* player);
    void ReturnHordeFlag(Player* player);
    void ReturnAllianceFlag(Player* player);
    void CaptureAllianceFlag(Player* player);
    void CaptureHordeFlag(Player* player);

    void UpdateFlagWorldState(Team team);

    void OnGameObjectCreate(GameObject* gameObject) override;

    ObjectGuid GetGuidData(uint32 data) const override
    {
        switch (data)
        {
            case DATA_ALLIANCE_FLAG_IN_BASE:
                return _flagGuids[TEAM_ALLIANCE];
            case DATA_ALLIANCE_FLAG_ON_GROUND:
                return _droppedFlagGuids[TEAM_ALLIANCE];
            case DATA_HORDE_FLAG_IN_BASE:
                return _flagGuids[TEAM_HORDE];
            case DATA_HORDE_FLAG_ON_GROUND:
                return _droppedFlagGuids[TEAM_HORDE];
            default:
                return ObjectGuid::Empty;
        }
    }

    void SetGuidData(uint32 data, ObjectGuid guid) override
    {
        switch (data)
        {
            case DATA_ALLIANCE_FLAG_IN_BASE:
                _flagGuids[TEAM_ALLIANCE] = guid;
                break;
            case DATA_ALLIANCE_FLAG_ON_GROUND:
                _droppedFlagGuids[TEAM_ALLIANCE] = guid;
                break;
            case DATA_HORDE_FLAG_IN_BASE:
                _flagGuids[TEAM_HORDE] = guid;
                break;
            case DATA_HORDE_FLAG_ON_GROUND:
                _droppedFlagGuids[TEAM_HORDE] = guid;
                break;
            default:
                break;
        }
    }

    virtual void OnHordeFlagPickedUpFromBase(Player* /*ally*/) { }
    virtual void OnAllianceFlagPickedUpFromBase(Player* /*hordie*/) { }
    virtual void OnHordeFlagPickedUpFromGround(Player* /*player*/) { }
    virtual void OnAllianceFlagPickedUpFromGround(Player* /*player*/) { }
    virtual void OnHordeFlagReturnedToBase(Player* /*player*/) { }
    virtual void OnAllianceFlagReturnedToBase(Player* /*player*/) { }
    virtual void OnHordeFlagCaptured(Player* /*capturer*/) { }
    virtual void OnAllianceFlagCaptured(Player* /*capturer*/) { }
    virtual void OnHordeFlagDropped(Player* /*player*/) { }
    virtual void OnAllianceFlagDropped(Player* /*player*/) { }
    virtual void OnRespawnFlag(Team /*team*/) { }

    void BuildPlayerPositionData(std::vector<WorldPackets::Battleground::BattlegroundPlayerPosition>* positions) const override;

    Team GetLastFlagCaptureTeam() const { return _lastFlagCapture; }

private:
    ObjectGuid _flagCarriers[BG_TEAMS_COUNT] = { };
    ObjectGuid _flagGuids[BG_TEAMS_COUNT] = { };
    ObjectGuid _droppedFlagGuids[BG_TEAMS_COUNT] = { };
    FlagState _flagStates[BG_TEAMS_COUNT];
    Team _lastFlagCapture; // Latest team who captured the enemy flag
};
