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

#include "AssaultStrategy.h"
#include "Battleground.h"
#include "BattlegroundScore.h"
#include "CaptureTheFlagBattlegroundScript.h"
#include "DB2Stores.h"
#include "Log.h"
#include "TimedBattlegroundScript.h"

struct battleground_twin_peaks : CaptureTheFlagBattlegroundScript, TimedBattlegroundScript, AssaultStrategy
{
    enum Spells
    {
        SPELL_WARSONG_FLAG_PICKED    = 61266,    // fake spell, does not exist but used as timer start event
        SPELL_SILVERWING_FLAG_PICKED = 61265    // fake spell, does not exist but used as timer start event
    };

    enum CriteriaObjectives
    {
        OBJECTIVE_CAPTURE_FLAG  = 290,
        OBJECTIVE_RETURN_FLAG   = 291
    };

    enum Graveyards
    {
        GRAVEYARD_HORDE_BASE        = 1728,
        GRAVEYARD_ALLIANCE_BASE     = 1729,
        GRAVEYARD_ALLIANCE_CENTER   = 1749,
        GRAVEYARD_HORDE_CENTER      = 1750
    };

    enum Areas
    {
        AREA_ALLIANCE_FLAG_ROOM = 5680,
        AREA_HORDE_FLAG_ROOM    = 5681
    };

    static uint32 const REPUTATION_GAIN = 35;
    static uint32 const HONOR_GAIN = 40; //todo move this to battleground ? first win of the day etc... quests ? Research.

    explicit battleground_twin_peaks(BattlegroundMap* map, uint32 resetMaxEndTimeTimer = 3000, uint32 maxDuration = 15 * 60) :
        BattlegroundScript(map), CaptureTheFlagBattlegroundScript(map), TimedBattlegroundScript(map, resetMaxEndTimeTimer, maxDuration), AssaultStrategy(map) { }

    void OnHordeFlagPickedUpFromBase(Player* player) override
    {
        player->StartCriteriaTimer(CRITERIA_TIMED_TYPE_SPELL_TARGET, SPELL_WARSONG_FLAG_PICKED);
        if (IsAllianceFlagInDanger())
            AddAndEnablePlayerToAssault(player);
    }

    void OnAllianceFlagPickedUpFromBase(Player* player) override
    {
        player->StartCriteriaTimer(CRITERIA_TIMED_TYPE_SPELL_TARGET, SPELL_SILVERWING_FLAG_PICKED);
        if (IsHordeFlagInDanger())
            AddAndEnablePlayerToAssault(player);
    }

    void OnGameObjectCreate(GameObject* gameObject) override
    {
        CaptureTheFlagBattlegroundScript::OnGameObjectCreate(gameObject);
        BattlegroundScript::OnGameObjectCreate(gameObject);
    }

    void OnStart() override
    {
        TimedBattlegroundScript::OnStart();
        BattlegroundScript::OnStart();
    }

    void OnUpdateInProgress(uint32 diff) override
    {
        TimedBattlegroundScript::OnUpdateInProgress(diff);
        AssaultStrategy::OnUpdateInProgress(diff);
        CaptureTheFlagBattlegroundScript::OnUpdateInProgress(diff);
        BattlegroundScript::OnUpdateInProgress(diff);
    }

    WorldSafeLocsEntry const* GetGraveYardForPlayer(Player* player) override
    {
        if (player->GetAreaId() == AREA_ALLIANCE_FLAG_ROOM || player->GetAreaId() == AREA_HORDE_FLAG_ROOM)
            return sWorldSafeLocsStore.LookupEntry(player->GetTeam() == ALLIANCE ? GRAVEYARD_ALLIANCE_BASE : GRAVEYARD_HORDE_BASE);

        return sWorldSafeLocsStore.LookupEntry(player->GetTeam() == ALLIANCE ? GRAVEYARD_ALLIANCE_CENTER : GRAVEYARD_HORDE_CENTER);
    }

    void OnAllianceFlagCaptured(Player* player) override
    {
        ResetAssault();
        //battleground->RewardReputationToTeam(REPUTATION_WARSONG_OUTRIDERS, REPUTATION_GAIN, HORDE);
        battleground->RewardHonorToTeam(HONOR_GAIN, HORDE);
    }

    void OnHordeFlagCaptured(Player*) override
    {
        ResetAssault();
        //battleground->RewardReputationToTeam(REPUTATION_SILVERWING_SENTINELS, REPUTATION_GAIN, ALLIANCE);
        battleground->RewardHonorToTeam(HONOR_GAIN, ALLIANCE);
    }

    void OnUpdatePlayerScore(ScoreType scoreType, Player* player) override
    {
        switch (scoreType)
        {
            case SCORE_FLAG_CAPTURES:
                player->UpdateCriteria(CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, OBJECTIVE_CAPTURE_FLAG);
                break;
            case SCORE_FLAG_RETURNS:
                player->UpdateCriteria(CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, OBJECTIVE_RETURN_FLAG);
                break;
            default:
                break;
        }
    }

    std::vector<Player*> GetAffectedPlayers() const override
    {
        std::vector<Player*> result;
        if (Player* player = ObjectAccessor::FindPlayer(GetAllyThatCarriesHordeFlag()))
            result.push_back(player);

        if (Player* player = ObjectAccessor::FindPlayer(GetHordieThatCarriesAllianceFlag()))
            result.push_back(player);

        return result;
    }

    Team GetWinnerWhenTimeElapsed() const override
    {
        if (battleground->GetTeamScore(TEAM_ALLIANCE) == 0)
        {
            if (battleground->GetTeamScore(TEAM_HORDE) == 0)        // No one scored - result is tie
                return TEAM_OTHER;

            return HORDE;
        }

        if (battleground->GetTeamScore(TEAM_HORDE) == 0)
            return ALLIANCE;           // Alliance has > 0, Horde has 0, alliance wins
        if (battleground->GetTeamScore(TEAM_HORDE) == battleground->GetTeamScore(TEAM_ALLIANCE)) // Team score equal, winner is team that scored the last flag
            return GetLastFlagCaptureTeam();
        if (battleground->GetTeamScore(TEAM_HORDE) > battleground->GetTeamScore(TEAM_ALLIANCE))  // Last but not least, check who has the higher score
            return HORDE;

        return ALLIANCE;
    }

    void OnAllianceFlagReturnedToBase(Player*) override
    {
        if (!IsHordeFlagInDanger())
            ResetAssault();
    }

    void OnHordeFlagReturnedToBase(Player*) override
    {
        if (!IsAllianceFlagInDanger())
            ResetAssault();
    }

    void OnAllianceFlagPickedUpFromGround(Player* player) override
    {
        if (IsHordeFlagInDanger())
            AddAndEnablePlayerToAssault(player);
    }

    void OnHordeFlagPickedUpFromGround(Player* player) override
    {
        if (IsAllianceFlagInDanger())
            AddAndEnablePlayerToAssault(player);
    }

    void OnHordeFlagDropped(Player* player) override
    {
        RemovePlayerFromAssault(player);
    }

    void OnAllianceFlagDropped(Player* player) override
    {
        RemovePlayerFromAssault(player);
    }

    void OnRespawnFlag(Team team) override
    {
        switch (team)
        {
            case ALLIANCE:
                if (IsHordeFlagInBase())
                    ResetAssault();
                break;
            case HORDE:
                if (IsAllianceFlagInBase())
                    ResetAssault();
                break;
            default:
                break;
        }
    }

    void BuildPlayerPositionData(std::vector<WorldPackets::Battleground::BattlegroundPlayerPosition>* positions) const override
    {
        CaptureTheFlagBattlegroundScript::BuildPlayerPositionData(positions);
    }

    Team CalculatePrematureWinner() override
    {
        return CaptureTheFlagBattlegroundScript::CalculatePrematureWinner();
    }

    ObjectGuid GetGuidData(uint32 data) const override
    {
        return CaptureTheFlagBattlegroundScript::GetGuidData(data);
    }

    void OnPlayerDroppedFlag(Player* player) override
    {
        CaptureTheFlagBattlegroundScript::OnPlayerDroppedFlag(player);
    }

    void SetGuidData(uint32 data, ObjectGuid guid) override
    {
        CaptureTheFlagBattlegroundScript::SetGuidData(data, guid);
    }

    void OnUpdateWaitJoin(uint32 diff) override
    {
        TimedBattlegroundScript::OnUpdateWaitJoin(diff);
    }

    bool OnGameObjectClicked(Player* player, GameObject* gameobject) override
    {
        if (!BattlegroundScript::OnGameObjectClicked(player, gameobject))
            return false;

        return CaptureTheFlagBattlegroundScript::OnGameObjectClicked(player, gameobject);
    }
};

void AddSC_battleground_twin_peaks()
{
    RegisterBattlegroundMapScript(battleground_twin_peaks, 726);
}
