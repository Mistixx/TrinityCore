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

struct battleground_warsong_gulch : CaptureTheFlagBattlegroundScript, TimedBattlegroundScript, AssaultStrategy
{
    enum Spells
    {
        SPELL_WARSONG_FLAG_PICKED    = 61266,    // fake spell, does not exist but used as timer start event
        SPELL_SILVERWING_FLAG_PICKED = 61265    // fake spell, does not exist but used as timer start event
    };

    enum Graveyards
    {
        GRAVEYARD_FLAGROOM_ALLIANCE     = 769,
        GRAVEYARD_FLAGROOM_HORDE        = 770,
        GRAVEYARD_MAIN_ALLIANCE         = 771,
        GRAVEYARD_MAIN_HORDE            = 772
    };

    enum Reputation
    {
        REPUTATION_WARSONG_OUTRIDERS    = 889,
        REPUTATION_SILVERWING_SENTINELS = 890
    };

    enum CriteriaObjectives
    {
        OBJECTIVE_CAPTURE_FLAG  = 42,
        OBJECTIVE_RETURN_FLAG   = 44
    };

    static uint32 const REPUTATION_GAIN = 35;
    static uint32 const HONOR_GAIN = 40; //todo move this to battleground ? first win of the day etc... quests ? Research.

    explicit battleground_warsong_gulch(BattlegroundMap* map, uint32 resetMaxEndTimeTimer = 3000, uint32 maxDuration = 15 * 60) :
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
        BattlegroundScript::OnGameObjectCreate(gameObject);
        CaptureTheFlagBattlegroundScript::OnGameObjectCreate(gameObject);
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

    bool CheckAchievementCriteriaMeet(uint32 criteriaId, Player const*player, Unit const* target, uint32 miscValue) override
    {
        switch (criteriaId)
        {
            case BG_CRITERIA_CHECK_SAVE_THE_DAY:
                if (target)
                    if (Player const* playerTarget = target->ToPlayer())
                        return playerTarget->GetTeam() == ALLIANCE ? IsAllianceFlagInBase() : IsHordeFlagInBase();

                return false;
            default:
                return CaptureTheFlagBattlegroundScript::CheckAchievementCriteriaMeet(criteriaId, player, target, miscValue);
        }
    }

    WorldSafeLocsEntry const* GetGraveYardForPlayer(Player* player) override
    {
        if (player->GetTeam() == ALLIANCE)
        {
            if (battleground->GetStatus() == STATUS_IN_PROGRESS)
                return sWorldSafeLocsStore.LookupEntry(GRAVEYARD_MAIN_ALLIANCE);

            return sWorldSafeLocsStore.LookupEntry(GRAVEYARD_FLAGROOM_ALLIANCE);
        }

        if (battleground->GetStatus() == STATUS_IN_PROGRESS)
            return sWorldSafeLocsStore.LookupEntry(GRAVEYARD_MAIN_HORDE);

        return sWorldSafeLocsStore.LookupEntry(GRAVEYARD_FLAGROOM_HORDE);
    }

    void OnAllianceFlagCaptured(Player* player) override
    {
        ResetAssault();
        battleground->RewardReputationToTeam(REPUTATION_WARSONG_OUTRIDERS, REPUTATION_GAIN, HORDE);
        battleground->RewardHonorToTeam(HONOR_GAIN, HORDE);
    }

    void OnHordeFlagCaptured(Player*) override
    {
        ResetAssault();
        battleground->RewardReputationToTeam(REPUTATION_SILVERWING_SENTINELS, REPUTATION_GAIN, ALLIANCE);
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

void AddSC_battleground_warsong_gulch()
{
    RegisterBattlegroundMapScript(battleground_warsong_gulch, 489);
}
