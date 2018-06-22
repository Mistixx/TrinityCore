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

struct battleground_battle_for_gilneas : ControlPointBattlegroundScript, PeriodicResourceGainBattlegroundScript
{
    enum GameObjects : uint32
    {
        CAPTURE_POINT_MINES         = 228053,
        CAPTURE_POINT_WATERWORKS    = 228052,
        CAPTURE_POINT_LIGHTHOUSE    = 228050,

        // Buffs

        // Lighthouse
        GO_SPEED_BUFF_LIGHTHOUSE   = 180379,
        GO_BERSERK_BUFF_LIGHTHOUSE = 180380,
        //GO_FOOD_BUFF_LIGHTHOUSE  = ,

        // Waterworks
        GO_SPEED_BUFF_WATERWORKS_1 = 180146, // Pretty sure it's always a speedbuff
        GO_SPEED_BUFF_WATERWORKS_2 = 180147, // Pretty sure it's always a speedbuff

        GO_BERSERK_BUFF_WATERWORKS_3  = 180148,
        GO_FOOD_BUFF_WATERWORKS_3     = 180383,
        //GO_SPEED_BUFF_WATERWORKS_3 = , //?


        // Mines
        GO_SPEED_BUFF_MINES = 180381,
        GO_FOOD_BUFF_MINES  = 180145,
        GO_BERSERK_BUFF_MINES   = 180382
    };

    enum ScheduleGroups : uint32
    {
        SCHEDULE_GROUP_CAPTURE_MINES,
        SCHEDULE_GROUP_CAPTURE_WATERWORKS,
        SCHEDULE_GROUP_CAPTURE_LIGHTHOUSE
    };

    enum WorldEvents
    {
        HORDE_ASSAULT_MINES = 39612,
        HORDE_CAPTURE_MINES = 39613,
        HORDE_DEFEND_MINES  = 39614,

        ALLIANCE_ASSAULT_MINES  = 39615,
        ALLIANCE_CAPUTRE_MINES  = 39616,
        ALLIANCE_DEFEND_MINES   = 39617,

        HORDE_ASSAULT_WATERWORKS = 39607,
        HORDE_CAPTURE_WATERWORKS = 39608,
        HORDE_DEFEND_WATERWORKS  = 39609,

        ALLIANCE_ASSAULT_WATERWORKS = 39610,
        ALLIANCE_CAPTURE_WATERWORKS = 39611,
        ALLIANCE_DEFEND_WATERWORKS  = 39605,

        HORDE_ASSAULT_LIGHTHOUSE = 39601,
        HORDE_CAPTURE_LIGHTHOUSE = 39602,
        HORDE_DEFEND_LIGHTHOUSE  = 39603,

        ALLIANCE_ASSAULT_LIGHTHOUSE = 39604,
        ALLIANCE_CAPTURE_LIGHTHOUSE = 39606,
        ALLIANCE_DEFEND_LIGHTHOUSE  = 39618
    };

    enum CriteriaObjectives
    {
        OBJECTIVE_ASSAULT_BASE  = 370,
        OBJECTIVE_DEFEND_BASE   = 371
    };

    enum Graveyards
    {
        GRAVEYARD_MINES          = 1735,
        GRAVEYARD_LIGHTHOUSE     = 1736,
        GRAVEYARD_WATERWORKS     = 1738,
        GRAVEYARD_HORDE_START    = 1739,
        GRAVEYARD_ALLIANCE_START = 1740
    };

    enum Creatures
    {
        NPC_ALLIANCE_SPIRIT_GUIDE   = 13116,
        NPC_HORDE_SPIRIT_GUIDE      = 13117
    };

    enum Areas
    {
        AREA_HORDE_LANDING      = 5453,
        AREA_MINES              = 5452,
        AREA_WATERWORKS         = 5451,
        AREA_GILNEAN_COAST      = 5468,
        AREA_LIGHTHOUSE         = 5450,
        AREA_GILNEAN_STRONGHOLD = 5454,
        AREA_THE_OVERLOOK       = 5457,
        AREA_BATTLE_FOR_GILNEAS = 5449
    };

    enum Points
    {
        POINT_MINES,
        POINT_WATERWORKS,
        POINT_LIGHTHOUSE
    };

    explicit battleground_battle_for_gilneas(BattlegroundMap* map) : BattlegroundScript(map), ControlPointSpawnsBattlegroundScript(map), ControlPointBattlegroundScript(map), PeriodicResourceGainBattlegroundScript(map, 1000),
        _teamScoresDisadvantage{ false, false }
    {
        CapturePoints[CAPTURE_POINT_MINES] = { CAPTURE_POINT_STATUS_NEUTRAL, TEAM_OTHER, SCHEDULE_GROUP_CAPTURE_MINES, sWorldSafeLocsStore.AssertEntry(GRAVEYARD_MINES) };
        CapturePoints[CAPTURE_POINT_WATERWORKS] = { CAPTURE_POINT_STATUS_NEUTRAL, TEAM_OTHER, SCHEDULE_GROUP_CAPTURE_WATERWORKS, sWorldSafeLocsStore.AssertEntry(GRAVEYARD_WATERWORKS) };
        CapturePoints[CAPTURE_POINT_LIGHTHOUSE] = { CAPTURE_POINT_STATUS_NEUTRAL, TEAM_OTHER, SCHEDULE_GROUP_CAPTURE_LIGHTHOUSE, sWorldSafeLocsStore.AssertEntry(GRAVEYARD_LIGHTHOUSE) };

        _areaGraveyards[AREA_MINES] = GRAVEYARD_MINES;
        _areaGraveyards[AREA_WATERWORKS] = GRAVEYARD_WATERWORKS;
        _areaGraveyards[AREA_LIGHTHOUSE] = GRAVEYARD_LIGHTHOUSE;
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
            case HORDE_ASSAULT_LIGHTHOUSE:
            case HORDE_ASSAULT_MINES:
            case HORDE_ASSAULT_WATERWORKS:
                HordeAssaultCapturePoint(gameObject, invoker);
                break;
            case HORDE_CAPTURE_LIGHTHOUSE:
            case HORDE_CAPTURE_MINES:
            case HORDE_CAPTURE_WATERWORKS:
                HordeCaptureCapturePoint(gameObject, invoker);
                break;
            case ALLIANCE_CAPTURE_LIGHTHOUSE:
            case ALLIANCE_CAPUTRE_MINES:
            case ALLIANCE_CAPTURE_WATERWORKS:
                AllianceCaptureCapturePoint(gameObject, invoker);
                break;
            case ALLIANCE_ASSAULT_LIGHTHOUSE:
            case ALLIANCE_ASSAULT_MINES:
            case ALLIANCE_ASSAULT_WATERWORKS:
                AllianceAssaultCapturePoint(gameObject, invoker);
                break;
            case ALLIANCE_DEFEND_LIGHTHOUSE:
            case ALLIANCE_DEFEND_MINES:
            case ALLIANCE_DEFEND_WATERWORKS:
                AllianceDefendCapturePoint(gameObject, invoker);
                break;
            case HORDE_DEFEND_LIGHTHOUSE:
            case HORDE_DEFEND_MINES:
            case HORDE_DEFEND_WATERWORKS:
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

        uint32 const lighthouseBuffs[] = { GO_SPEED_BUFF_LIGHTHOUSE, GO_BERSERK_BUFF_LIGHTHOUSE };
        uint32 const minesBuffs[] = { GO_SPEED_BUFF_MINES, GO_BERSERK_BUFF_MINES, GO_FOOD_BUFF_MINES };
        uint32 const waterworksBuffs1[] = { GO_SPEED_BUFF_WATERWORKS_1 };
        uint32 const waterworksBuffs2[] = { GO_SPEED_BUFF_WATERWORKS_2 };
        uint32 const waterworksBuffs3[] = { GO_BERSERK_BUFF_WATERWORKS_3, GO_FOOD_BUFF_WATERWORKS_3 };

        AddObject(Trinity::Containers::SelectRandomContainerElement(lighthouseBuffs), { 1063.564f, 1308.979f, 4.911692f, 4.031712f }, { 0.0f, 0.0f, -0.902585f, 0.4305117f });
        AddObject(Trinity::Containers::SelectRandomContainerElement(minesBuffs), { 1195.73f, 1020.3f, 7.97874f, 5.777041f }, { 0.0f, 0.0f, -0.2503796f, 0.9681478f });
        AddObject(Trinity::Containers::SelectRandomContainerElement(waterworksBuffs1), { 966.458f, 1043.57f, 13.1467f, 0.9075702f }, { 0.0f, 0.0f, 0.4383707f, 0.8987942f });
        AddObject(Trinity::Containers::SelectRandomContainerElement(waterworksBuffs2), { 1111.431f, 921.6163f, 30.07139f, 5.567601f }, { 0.0f, 0.0f, -0.3502073f, 0.9366722f });
        AddObject(Trinity::Containers::SelectRandomContainerElement(waterworksBuffs3), { 990.297f, 983.373f, 12.9826f, 4.555311f }, { 0.0f, 0.0f, -0.7604055f, 0.6494485f });
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
        WorldSafeLocsEntry const* gy = player->GetTeam() == ALLIANCE ? sWorldSafeLocsStore.AssertEntry(GRAVEYARD_ALLIANCE_START) : sWorldSafeLocsStore.AssertEntry(GRAVEYARD_HORDE_START);
        float dist = player->GetPosition().GetExactDist2d(gy->Loc.X, gy->Loc.Y);

        for (auto const& capturePoint : CapturePoints)
        {
            if (!capturePoint.second.IsTakenBy(player->GetTeam()) || !capturePoint.second.GraveYard)
                continue;

            auto const& itr = _areaGraveyards.find(player->GetAreaId());
            if (itr != _areaGraveyards.end())
                if (capturePoint.second.GraveYard->ID == itr->second)
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

    // todo ?
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

    bool OnGameObjectClicked(Player* player, GameObject* gameobject) override
    {
        if (!BattlegroundScript::OnGameObjectClicked(player, gameobject))
            return false;

        return ControlPointBattlegroundScript::OnGameObjectClicked(player, gameobject);
    }

private:
    uint8 const POINTS_FOR_BASE[4] = { 0, 1, 2, 4 }; // TODO all bases
    std::unordered_map<uint32, uint32> _areaGraveyards;
    bool _teamScoresDisadvantage[BG_TEAMS_COUNT];
    static uint16 const DISADVANTAGE_POINT_COUNT = 500;
};

void AddSC_battleground_battle_for_gilneas()
{
    RegisterBattlegroundMapScript(battleground_battle_for_gilneas, 761);
}
