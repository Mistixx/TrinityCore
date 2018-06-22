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

#include "Battleground.h"
#include "BattlegroundScore.h"
#include "ControlPointBattlegroundScript.h"
#include "DB2Stores.h"
#include "GameObject.h"
#include "Log.h"
#include "PeriodicResourceGainBattlegroundScript.h"

struct battleground_arathi_basin : ControlPointBattlegroundScript, PeriodicResourceGainBattlegroundScript
{
    enum GameObjects : uint32
    {
        CAPTURE_POINT_FARM          = 227536,
        CAPTURE_POINT_LUMBER_MILL   = 227544,
        CAPTURE_POINT_BLACKSMITH    = 227522,
        CAPTURE_POINT_GOLD_MINE     = 227538,
        CAPTURE_POINT_STABLES       = 227420,

        // Farm Buffs
        FOOD_BUFF_FARM              = 180145,
        SPEED_BUFF_FARM             = 180381,
        BERSERK_BUFF_FARM           = 180382,

        // Lumber Mill Buffs
        BERSERK_BUFF_LUMBER_MILL    = 180378,
        SPEED_BUFF_LUMBER_MILL      = 180146,
        FOOD_BUFF_LUMBER_MILL       = 180377,

        // Blacksmith Buffs
        SPEED_BUFF_BLACKSMITH       = 180384,
        FOOD_BUFF_BLACKSMITH        = 180383,
        BERSERK_BUFF_BLACKSMITH     = 180378,

        // Gold Mine Buffs
        BERSERK_BUFF_GOLD_MINE      = 180376,
        SPEED_BUFF_GOLD_MINE        = 180147,
        FOOD_BUFF_GOLD_MINE         = 180362,

        // Stables Buffs
        FOOD_BUFF_STABLES           = 180144,
        SPEED_BUFF_STABLES          = 180379,
        BERSERK_BUFF_STABLES        = 180380
    };

    enum ScheduleGroups : uint32
    {
        SCHEDULE_GROUP_CAPTURE_FARM,
        SCHEDULE_GROUP_CAPTURE_BLACKSMITH,
        SCHEDULE_GROUP_CAPTURE_LUMBERMILL,
        SCHEDULE_GROUP_CAPTURE_GOLD_MINE,
        SCHEDULE_GROUP_CAPTURE_STABLES
    };

    enum WorldEvents
    {
        ALLIANCE_ASSAULT_FARM           = 39401,
        ALLIANCE_ASSAULT_LUMBER_MILL    = 39390,
        ALLIANCE_ASSAULT_BLACKSMITH     = 8874,
        ALLIANCE_ASSAULT_GOLD_MINE      = 39407,
        ALLIANCE_ASSAULT_STABLES        = 28522,

        ALLIANCE_DEFEND_FARM            = 39403,
        ALLIANCE_DEFEND_LUMBER_MILL     = 39392,
        ALLIANCE_DEFEND_BLACKSMITH      = 8774,
        ALLIANCE_DEFEND_GOLD_MINE       = 39409,
        ALLIANCE_DEFEND_STABLES         = 28524,

        ALLIANCE_CAPTURE_FARM           = 39402,
        ALLIANCE_CAPTURE_LUMBER_MILL    = 39391,
        ALLIANCE_CAPTURE_BLACKSMITH     = 8769,
        ALLIANCE_CAPTURE_GOLD_MINE      = 39408,
        ALLIANCE_CAPTURE_STABLES        = 28526,

        HORDE_ASSAULT_FARM              = 39398,
        HORDE_ASSAULT_LUMBER_MILL       = 39387,
        HORDE_ASSAULT_BLACKSMITH        = 8876,
        HORDE_ASSAULT_GOLD_MINE         = 39404,
        HORDE_ASSAULT_STABLES           = 28523,

        HORDE_DEFEND_FARM               = 39400,
        HORDE_DEFEND_LUMBER_MILL        = 39389,
        HORDE_DEFEND_BLACKSMITH         = 8770,
        HORDE_DEFEND_GOLD_MINE          = 39406,
        HORDE_DEFEND_STABLES            = 28525,

        HORDE_CAPTURE_FARM              = 39399,
        HORDE_CAPTURE_LUMBER_MILL       = 39388,
        HORDE_CAPTURE_BLACKSMITH        = 8773,
        HORDE_CAPTURE_GOLD_MINE         = 39405,
        HORDE_CAPTURE_STABLES           = 28527
    };

    enum CriteriaObjectives
    {
        OBJECTIVE_ASSAULT_BASE  = 122,
        OBJECTIVE_DEFEND_BASE   = 123
    };

    enum Graveyards
    {
        GRAVEYARD_FARM          = 893,
        GRAVEYARD_BLACKSMITH    = 894,
        GRAVEYARD_STABLES       = 895,
        GRAVEYARD_GOLDMINE      = 896,
        GRAVEYARD_LUMBER_MILL   = 897,
        GRAVEYARD_ALLIANCE_BASE = 898,
        GRAVEYARD_HORDE_BASE    = 899
    };

    enum Creatures
    {
        NPC_ALLIANCE_SPIRIT_GUIDE = 13116,
        NPC_HORDE_SPIRIT_GUIDE = 13117
    };

    explicit battleground_arathi_basin(BattlegroundMap* map) : BattlegroundScript(map), ControlPointSpawnsBattlegroundScript(map), ControlPointBattlegroundScript(map), PeriodicResourceGainBattlegroundScript(map), _teamScoresDisadvantage{ false, false }
    {
        CapturePoints[CAPTURE_POINT_FARM] = { CAPTURE_POINT_STATUS_NEUTRAL, TEAM_OTHER, SCHEDULE_GROUP_CAPTURE_FARM, sWorldSafeLocsStore.AssertEntry(GRAVEYARD_FARM) };
        CapturePoints[CAPTURE_POINT_BLACKSMITH] = { CAPTURE_POINT_STATUS_NEUTRAL, TEAM_OTHER, SCHEDULE_GROUP_CAPTURE_BLACKSMITH, sWorldSafeLocsStore.AssertEntry(GRAVEYARD_BLACKSMITH) };
        CapturePoints[CAPTURE_POINT_GOLD_MINE] = { CAPTURE_POINT_STATUS_NEUTRAL, TEAM_OTHER, SCHEDULE_GROUP_CAPTURE_GOLD_MINE, sWorldSafeLocsStore.AssertEntry(GRAVEYARD_GOLDMINE) };
        CapturePoints[CAPTURE_POINT_LUMBER_MILL] = { CAPTURE_POINT_STATUS_NEUTRAL, TEAM_OTHER, SCHEDULE_GROUP_CAPTURE_LUMBERMILL, sWorldSafeLocsStore.AssertEntry(GRAVEYARD_LUMBER_MILL) };
        CapturePoints[CAPTURE_POINT_STABLES] = { CAPTURE_POINT_STATUS_NEUTRAL, TEAM_OTHER, SCHEDULE_GROUP_CAPTURE_STABLES, sWorldSafeLocsStore.AssertEntry(GRAVEYARD_STABLES) };
    }

    void OnUpdateInProgress(uint32 diff) override
    {
        PeriodicResourceGainBattlegroundScript::OnUpdateInProgress(diff);
        ControlPointBattlegroundScript::OnUpdateInProgress(diff);

        if (battleground->GetTeamScore(TEAM_ALLIANCE) > battleground->GetTeamScore(TEAM_HORDE) + DISADVANTAGE_POINT_COUNT)
            _teamScoresDisadvantage[TEAM_HORDE] = true;

        if (battleground->GetTeamScore(TEAM_HORDE) > battleground->GetTeamScore(TEAM_ALLIANCE) + DISADVANTAGE_POINT_COUNT)
            _teamScoresDisadvantage[TEAM_ALLIANCE] = true;
    }

    void ProcessEvent(WorldObject* target, uint32 eventId, WorldObject* invoker) override
    {
        ControlPointBattlegroundScript::ProcessEvent(target, eventId, invoker);

        if (!target)
            return;

        GameObject* gameObject = target->ToGameObject();
        if (!gameObject || gameObject->GetGoType() != GAMEOBJECT_TYPE_CAPTURE_POINT)
            return;

        switch (eventId)
        {
            case HORDE_ASSAULT_FARM:
            case HORDE_ASSAULT_GOLD_MINE:
            case HORDE_ASSAULT_LUMBER_MILL:
            case HORDE_ASSAULT_STABLES:
            case HORDE_ASSAULT_BLACKSMITH:
                HordeAssaultCapturePoint(gameObject, invoker);
                break;
            case HORDE_CAPTURE_GOLD_MINE:
            case HORDE_CAPTURE_FARM:
            case HORDE_CAPTURE_STABLES:
            case HORDE_CAPTURE_LUMBER_MILL:
            case HORDE_CAPTURE_BLACKSMITH:
                HordeCaptureCapturePoint(gameObject, invoker);
                break;
            case ALLIANCE_CAPTURE_FARM:
            case ALLIANCE_CAPTURE_GOLD_MINE:
            case ALLIANCE_CAPTURE_STABLES:
            case ALLIANCE_CAPTURE_LUMBER_MILL:
            case ALLIANCE_CAPTURE_BLACKSMITH:
                AllianceCaptureCapturePoint(gameObject, invoker);
                break;
            case ALLIANCE_ASSAULT_FARM:
            case ALLIANCE_ASSAULT_GOLD_MINE:
            case ALLIANCE_ASSAULT_LUMBER_MILL:
            case ALLIANCE_ASSAULT_STABLES:
            case ALLIANCE_ASSAULT_BLACKSMITH:
                AllianceAssaultCapturePoint(gameObject, invoker);
                break;
            case ALLIANCE_DEFEND_BLACKSMITH:
            case ALLIANCE_DEFEND_FARM:
            case ALLIANCE_DEFEND_GOLD_MINE:
            case ALLIANCE_DEFEND_LUMBER_MILL:
            case ALLIANCE_DEFEND_STABLES:
                AllianceDefendCapturePoint(gameObject, invoker);
                break;
            case HORDE_DEFEND_BLACKSMITH:
            case HORDE_DEFEND_FARM:
            case HORDE_DEFEND_GOLD_MINE:
            case HORDE_DEFEND_LUMBER_MILL:
            case HORDE_DEFEND_STABLES:
                HordeDefendCapturePoint(gameObject, invoker);
                break;
            default:
                break;
        }
    }

    void OnSetup() override
    {
        BattlegroundScript::OnSetup();
        ControlPointBattlegroundScript::OnSetup();
        uint32 const farmBuffs[] = { FOOD_BUFF_FARM, SPEED_BUFF_FARM, BERSERK_BUFF_FARM };
        AddObject(Trinity::Containers::SelectRandomContainerElement(farmBuffs), { 818.0089f, 842.3543f, -56.54062f, 3.176533f }, { 0.0f, 0.0f, -0.9998474f, 0.01746928f });
        uint32 const lumberMillBuffs[] = { FOOD_BUFF_LUMBER_MILL, SPEED_BUFF_LUMBER_MILL, BERSERK_BUFF_LUMBER_MILL }; // 3868
        AddObject(Trinity::Containers::SelectRandomContainerElement(lumberMillBuffs), { 808.8463f, 1185.417f, 11.92161f, 5.619962f }, { 0.0f, 0.0f, -0.3255672f, 0.9455189f });
        uint32 const blacksmithBuffs[] = { SPEED_BUFF_BLACKSMITH, FOOD_BUFF_BLACKSMITH, BERSERK_BUFF_BLACKSMITH }; // 3870
        AddObject(Trinity::Containers::SelectRandomContainerElement(blacksmithBuffs), { 989.9393f, 1008.75f, -42.60327f, 0.8203033f }, { 0.0f, 0.0f, 0.3987484f, 0.9170604f });
        uint32 const goldMineBuffs[] = { BERSERK_BUFF_GOLD_MINE, SPEED_BUFF_GOLD_MINE, FOOD_BUFF_GOLD_MINE }; // 3869
        AddObject(Trinity::Containers::SelectRandomContainerElement(goldMineBuffs), { 1147.091f, 816.8362f, -98.39896f, 6.056293f }, { 0.0f, 0.0f, -0.113203f, 0.9935719f });
        uint32 const stableBuffs[] = { FOOD_BUFF_STABLES, SPEED_BUFF_STABLES, BERSERK_BUFF_STABLES }; // 3866
        AddObject(Trinity::Containers::SelectRandomContainerElement(stableBuffs), { 1185.566f, 1184.629f, -56.36329f, 2.303831f }, { 0.0f, 0.0f, 0.9135447f, 0.4067384f });
    }

    void OnUpdatePlayerScore(ScoreType scoreType, Player* player) override
    {
        switch (scoreType)
        {
            case SCORE_BASES_ASSAULTED:
                player->UpdateCriteria(CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, OBJECTIVE_ASSAULT_BASE);
                break;
            case SCORE_BASES_DEFENDED:
                player->UpdateCriteria(CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, OBJECTIVE_DEFEND_BASE);
                break;
            default:
                break;
        }
    }

    void OnHordeAssaultCapturePoint(GameObject* gameObject, WorldObject* /*player*/) override
    {
        if (CapturePoints[gameObject->GetEntry()].CurrentOwner == ALLIANCE)
            SetPointsToAddForTeam(TEAM_ALLIANCE, POINTS_FOR_BASE[GetTeamCapturedCapturePointsCount(ALLIANCE)]);
    }

    void OnAllianceAssaultCapturePoint(GameObject* gameObject, WorldObject* /*player*/) override
    {
        if (CapturePoints[gameObject->GetEntry()].CurrentOwner == HORDE)
            SetPointsToAddForTeam(TEAM_HORDE, POINTS_FOR_BASE[GetTeamCapturedCapturePointsCount(HORDE)]);
    }

    void OnHordeDefendCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) override
    {
        SetPointsToAddForTeam(TEAM_HORDE, POINTS_FOR_BASE[GetTeamCapturedCapturePointsCount(HORDE)]);
    }

    void OnAllianceDefendCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) override
    {
        SetPointsToAddForTeam(TEAM_ALLIANCE, POINTS_FOR_BASE[GetTeamCapturedCapturePointsCount(ALLIANCE)]);
    }

    void OnHordeCaptureCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) override
    {
        SetPointsToAddForTeam(TEAM_HORDE, POINTS_FOR_BASE[GetTeamCapturedCapturePointsCount(HORDE)]);
    }

    void OnAllianceCaptureCapturePoint(GameObject* /*gameObject*/, WorldObject* /*player*/) override
    {
        SetPointsToAddForTeam(TEAM_ALLIANCE, POINTS_FOR_BASE[GetTeamCapturedCapturePointsCount(ALLIANCE)]);
    }

    WorldSafeLocsEntry const* GetGraveYardForPlayer(Player* player) override
    {
        WorldSafeLocsEntry const* gy = player->GetTeam() == ALLIANCE ? sWorldSafeLocsStore.AssertEntry(GRAVEYARD_ALLIANCE_BASE) : sWorldSafeLocsStore.AssertEntry(GRAVEYARD_HORDE_BASE);
        float dist = player->GetPosition().GetExactDist2d(gy->Loc.X, gy->Loc.Y);

        for (auto const& capturePoint : CapturePoints)
        {
            if (!capturePoint.second.IsTakenBy(player->GetTeam()) || !capturePoint.second.GraveYard)
                continue;

            float const thisDistance = player->GetPosition().GetExactDist2d(capturePoint.second.GraveYard->Loc.X, capturePoint.second.GraveYard->Loc.Y);
            if (dist > thisDistance)
            {
                dist = thisDistance;
                gy = capturePoint.second.GraveYard;
            }
        }

        return gy;
    }

    bool CheckAchievementCriteriaMeet(uint32 criteriaId, Player const* player, Unit const* target, uint32 miscvalue) override
    {
        switch (criteriaId)
        {
            case BG_CRITERIA_CHECK_RESILIENT_VICTORY:
                return _teamScoresDisadvantage[player->GetTeamId()];
            default:
                return BattlegroundScript::CheckAchievementCriteriaMeet(criteriaId, player, target, miscvalue);
        }
    }

    Team CalculatePrematureWinner() override
    {
        uint8 const allyBases = GetTeamCapturedCapturePointsCount(ALLIANCE);
        uint8 const hordeBases = GetTeamCapturedCapturePointsCount(HORDE);

        if (allyBases > hordeBases)
            return ALLIANCE;

        if (hordeBases > allyBases)
            return HORDE;

        return BattlegroundScript::CalculatePrematureWinner();
    }

    void OnStart() override
    {
        ControlPointBattlegroundScript::OnStart();
    }

    bool OnGameObjectClicked(Player* player, GameObject* gameobject) override
    {
        if (!BattlegroundScript::OnGameObjectClicked(player, gameobject))
            return false;

        return ControlPointBattlegroundScript::OnGameObjectClicked(player, gameobject);
    }

private:
    bool _teamScoresDisadvantage[BG_TEAMS_COUNT];
    static uint16 const DISADVANTAGE_POINT_COUNT = 500;
    uint8 const POINTS_FOR_BASE[6] = { 0, 2, 3, 4, 5, 30 }; // 5 & 30 guessed
};

void AddSC_battleground_arathi_bassin()
{
    RegisterBattlegroundMapScript(battleground_arathi_basin, 529);
}
