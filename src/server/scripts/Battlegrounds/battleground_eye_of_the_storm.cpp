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
#include "BattlegroundMgr.h"
#include "BattlegroundScore.h"
#include "CapturePointSpawnBattlegroundScript.h"
#include "DB2Stores.h"
#include "GameObject.h"
#include "Log.h"
#include "PeriodicResourceGainBattlegroundScript.h"

#include <memory>

enum EotSCreatures
{
    NPC_ALLIANCE_SPIRIT_GUIDE = 13116,
    NPC_HORDE_SPIRIT_GUIDE    = 13117
};

enum ProgressBarConsts
{
    POINT_MAX_CAPTURERS_COUNT = 5,
    POINT_RADIUS = 70,
    PROGRESS_BAR_DONT_SHOW = 0,
    PROGRESS_BAR_SHOW = 1,
    PROGRESS_BAR_PERCENT_GREY = 40,
    PROGRESS_BAR_STATE_MIDDLE = 50,
    PROGRESS_BAR_HORDE_CONTROLLED = 0,
    PROGRESS_BAR_NEUTRAL_LOW = 30,
    PROGRESS_BAR_NEUTRAL_HIGH = 70,
    PROGRESS_BAR_ALI_CONTROLLED = 100
};

struct EotSCapturePointInfoTemplate
{
    uint32 WorldStates[3] = { };
    uint32 LostTextId[BG_TEAMS_COUNT] = { };
    uint32 CapturedTextId[BG_TEAMS_COUNT] = { };
    WorldSafeLocsEntry const* Graveyard = nullptr;
};

struct EotSCapturePointInfo
{
    EotSCapturePointInfo() = default;
    EotSCapturePointInfo(std::shared_ptr<EotSCapturePointInfoTemplate> eotsTemplate) : Template(std::move(eotsTemplate)) { }

    TeamId Owner = TEAM_NEUTRAL;
    GuidVector GameObjects;
    ObjectGuid SpiritGuide;
    ObjectGuid CapturePointObject;
    ObjectGuid HonorableDefender;
    GuidVector PlayersInPoint[BG_TEAMS_COUNT];
    uint8 StatusBarValue = PROGRESS_BAR_STATE_MIDDLE;

    std::shared_ptr<EotSCapturePointInfoTemplate> const Template = nullptr;
};

enum EotSCapturePoint : uint8
{
    CAPTURE_POINT_FEL_REAVER_RUINS = 1,
    CAPTURE_POINT_BLOOD_ELF_TOWER,
    CAPTURE_POINT_DRAENEI_RUINS,
    CAPTURE_POINT_MAGE_TOWER
};

struct battleground_eye_of_the_storm : AssaultStrategy, PeriodicResourceGainBattlegroundScript, ControlPointSpawnsBattlegroundScript
{
    enum GameObjects
    {
        GO_VISUAL_BANNER_NEUTRAL    = 184382,
        GO_VISUAL_BANNER_HORDE      = 184380,
        GO_VISUAL_BANNER_ALLIANCE   = 184381,

        // Capture points... not sure how/when they are spawned. But they have a GUID and is targeted by texts like capturing/losing a point
        GO_BLOOD_ELF_TOWER_CAP  = 184080,
        GO_FEL_REAVER_RUINS_CAP = 184081,
        GO_MAGE_TOWER_CAP       = 184082,
        GO_DRAENEI_RUINS_CAP    = 184083,

        GO_EYE_OF_THE_STORM_FLAG         = 208977,
        GO_DROPPED_EYE_OF_THE_STORM_FLAG = 228508,

        // Buffs
        GO_SPEED_BUFF_BLOOD_ELF      = 184964,
        GO_RESTORATION_BLOOD_ELF     = 184965,
        GO_BERSERK_BLOOD_ELF         = 184966,
        GO_SPEED_BUFF_FEL_REAVER     = 184970,
        GO_RESTORATION_FEL_REAVER    = 184971,
        GO_BERSERK_FEL_REAVER        = 184972,
        GO_SPEED_BUFF_MAGE_TOWER     = 184973,
        GO_RESTORATION_MAGE_TOWER    = 184974,
        GO_BERSERK_MAGE_TOWER        = 184975,
        GO_SPEED_BUFF_DRAENEI_RUINS  = 184976,
        GO_RESTORATION_DRAENEI_RUINS = 184977,
        GO_BERSERK_DRAENEI_RUINS     = 184978
    };

    enum Graveyards
    {
        GRAVEYARD_ALLIANCE_START = 1103,
        GRAVEYARD_HORDE_START = 1104,
        GRAVEYARD_FEL_REAVER_RUINS = 1105,
        GRAVEYARD_BLOOD_ELF_TOWER = 1106,
        GRAVEYARD_DRAENEI_TOWER = 1107,
        GRAVEYARD_MAGE_TOWER = 1108
    };

    enum WorldStates
    {
        WORLD_STATE_ALLIANCE_BASE   = 2752,
        WORLD_STATE_HORDE_BASE      = 2753,

        WORLD_STATE_HORDE_BASE_2    = 1778,
        WORLD_STATE_ALLIANCE_BASE_2 = 1779,

        // Draenei
        WORLD_STATE_DRAENEI_RUINS_HORDE_CONTROL     = 2733,
        WORLD_STATE_DRAENEI_RUINS_ALLIANCE_CONTROL  = 2732,
        WORLD_STATE_DRAENEI_RUINS_UNCONTROL         = 2731,

        // Mage Tower
        WORLD_STATE_MAGE_TOWER_ALLIANCE_CONTROL = 2730,
        WORLD_STATE_MAGE_TOWER_HORDE_CONTROL    = 2729,
        WORLD_STATE_MAGE_TOWER_UNCONTROL        = 2728,

        // Fel Reaver
        WORLD_STATE_FEL_REAVER_HORDE_CONTROL    = 2727,
        WORLD_STATE_FEL_REAVER_ALLIANCE_CONTROL = 2726,
        WORLD_STATE_FEL_REAVER_UNCONTROL        = 2725,

        // Blood elf
        WORLD_STATE_BLOOD_ELF_HORDE_CONTROL     = 2724,
        WORLD_STATE_BLOOD_ELF_ALLIANCE_CONTROL  = 2723,
        WORLD_STATE_BLOOD_ELF_UNCONTROL         = 2722,

        // Progress bar
        WORLD_STATE_PROGRESS_BAR_PERCENT_GREY   = 2720, //100 = empty (only grey), 0 = blue|red (no grey)
        WORLD_STATE_PROGRESS_BAR_STATUS         = 2719, //50 init!, 48 ... hordak bere .. 33 .. 0 = full 100% hordacky, 100 = full alliance
        WORLD_STATE_PROGRESS_BAR_SHOW           = 2718, //1 init, 0 druhy send - bez messagu, 1 = controlled aliance

        // Flag
        WORLD_STATE_NETHERSTORM_FLAG                = 8863, // 1 - reset (not picked up). 2 - Taken. 4 - captured.
        WORLD_STATE_NETHERSTORM_FLAG_STATE_ALLIANCE = 2769, //set to 2 when flag is picked up, and to 1 if it is dropped
        WORLD_STATE_NETHERSTORM_FLAG_STATE_HORDE    = 2770,

        // Bases, resources, flag bar
        WORLD_STATE_ALLIANCE_INFO_BAR   = 9808, // 2 - picked up flag, 1 - show bar
        WORLD_STATE_HORDE_INFO_BAR      = 9809
    };

    enum Sounds
    {
        SOUND_FLAG_PICKED_UP_ALLIANCE = 8212,
        SOUND_FLAG_CAPTURED_HORDE = 8213,
        SOUND_FLAG_PICKED_UP_HORDE = 8174,
        SOUND_FLAG_CAPTURED_ALLIANCE = 8173,
        SOUND_FLAG_RESET = 8192
    };

    enum FlagState : uint8
    {
        FLAG_STATE_NOT_PICKED_UP    = 1,
        FLAG_STATE_PICKED_UP        = 2,
        FLAG_STATE_DROPPED          = 3,
        FLAG_STATE_CAPTURED         = 4
    };

    enum Texts
    {
        TEXT_ALLIANCE_TAKEN_FEL_REAVER_RUINS    = 17828,
        TEXT_HORDE_TAKEN_FEL_REAVER_RUINS       = 17829,
        TEXT_ALLIANCE_LOST_FEL_REAVER_RUINS     = 91961,
        TEXT_HORDE_LOST_FEL_REAVER_RUINS        = 91962,

        TEXT_ALLIANCE_TAKEN_BLOOD_ELF_TOWER     = 17819,
        TEXT_HORDE_TAKEN_BLOOD_ELF_TOWER        = 17823,
        TEXT_ALLIANCE_LOST_BLOOD_ELF_TOWER      = 91957,
        TEXT_HORDE_LOST_BLOOD_ELF_TOWER         = 91958,

        TEXT_ALLIANCE_TAKEN_DRAENEI_RUINS       = 17827,
        TEXT_HORDE_TAKEN_DRAENEI_RUINS          = 91917,
        TEXT_ALLIANCE_LOST_DRAENEI_RUINS        = 91959,
        TEXT_HORDE_LOST_DRAENEI_RUINS           = 91960,

        TEXT_ALLIANCE_TAKEN_MAGE_TOWER          = 17824,
        TEXT_HORDE_TAKEN_MAGE_TOWER             = 17825,
        TEXT_ALLIANCE_LOST_MAGE_TOWER           = 91963,
        TEXT_HORDE_LOST_MAGE_TOWER              = 91964,

        TEXT_TAKEN_FLAG             = 18359,
        TEXT_FLAG_DROPPED           = 18361,
        TEXT_FLAG_RESET             = 18364,
        TEXT_ALLIANCE_CAPTURED_FLAG = 18375,
        TEXT_HORDE_CAPTURED_FLAG    = 18384,
    };

    enum Creatures
    {
        WORLD_TRIGGER_HORDE_FRIENDLY = 20212, // Honorable Defender
        WORLD_TRIGGER_ALLIANCE_FRIENDLY = 20213
    };

    enum Spells
    {
        SPELL_NETHERSTORM_FLAG = 34976,
        SPELL_RECENTLY_DROPPED_FLAG = 50327
    };

    enum Criterias
    {
        OBJECTIVE_CAPTURE_FLAG = 183
    };

    enum ScheduleGroups
    {
        SCHEDULE_DROPPED_FLAG_RESPAWN
    };

    explicit battleground_eye_of_the_storm(BattlegroundMap* map) : BattlegroundScript(map), AssaultStrategy(map), PeriodicResourceGainBattlegroundScript(map), ControlPointSpawnsBattlegroundScript(map),
        _flagState(FLAG_STATE_NOT_PICKED_UP), _towerCapTimer(2000)
    {
        EotSCapturePointInfoTemplate bloodElfTowerTemplate;
        bloodElfTowerTemplate.Graveyard = sWorldSafeLocsStore.AssertEntry(GRAVEYARD_BLOOD_ELF_TOWER);
        bloodElfTowerTemplate.WorldStates[TEAM_ALLIANCE] = WORLD_STATE_BLOOD_ELF_ALLIANCE_CONTROL;
        bloodElfTowerTemplate.WorldStates[TEAM_HORDE] = WORLD_STATE_BLOOD_ELF_HORDE_CONTROL;
        bloodElfTowerTemplate.WorldStates[TEAM_NEUTRAL] = WORLD_STATE_BLOOD_ELF_UNCONTROL;
        bloodElfTowerTemplate.CapturedTextId[TEAM_HORDE] = TEXT_HORDE_TAKEN_BLOOD_ELF_TOWER;
        bloodElfTowerTemplate.CapturedTextId[TEAM_ALLIANCE] = TEXT_ALLIANCE_TAKEN_BLOOD_ELF_TOWER;
        bloodElfTowerTemplate.LostTextId[TEAM_HORDE] = TEXT_HORDE_LOST_BLOOD_ELF_TOWER;
        bloodElfTowerTemplate.LostTextId[TEAM_ALLIANCE] = TEXT_ALLIANCE_LOST_BLOOD_ELF_TOWER;

        EotSCapturePointInfoTemplate mageTowerTemplate;
        mageTowerTemplate.Graveyard = sWorldSafeLocsStore.AssertEntry(GRAVEYARD_MAGE_TOWER);
        mageTowerTemplate.WorldStates[TEAM_ALLIANCE] = WORLD_STATE_MAGE_TOWER_ALLIANCE_CONTROL;
        mageTowerTemplate.WorldStates[TEAM_HORDE] = WORLD_STATE_MAGE_TOWER_HORDE_CONTROL;
        mageTowerTemplate.WorldStates[TEAM_NEUTRAL] = WORLD_STATE_MAGE_TOWER_UNCONTROL;
        mageTowerTemplate.CapturedTextId[TEAM_HORDE] = TEXT_HORDE_TAKEN_MAGE_TOWER;
        mageTowerTemplate.CapturedTextId[TEAM_ALLIANCE] = TEXT_ALLIANCE_TAKEN_MAGE_TOWER;
        mageTowerTemplate.LostTextId[TEAM_HORDE] = TEXT_HORDE_LOST_MAGE_TOWER;
        mageTowerTemplate.LostTextId[TEAM_ALLIANCE] = TEXT_ALLIANCE_LOST_MAGE_TOWER;

        EotSCapturePointInfoTemplate draeneiRuinsTemplate;
        draeneiRuinsTemplate.Graveyard = sWorldSafeLocsStore.AssertEntry(GRAVEYARD_DRAENEI_TOWER);
        draeneiRuinsTemplate.WorldStates[TEAM_ALLIANCE] = WORLD_STATE_DRAENEI_RUINS_ALLIANCE_CONTROL;
        draeneiRuinsTemplate.WorldStates[TEAM_HORDE] = WORLD_STATE_DRAENEI_RUINS_HORDE_CONTROL;
        draeneiRuinsTemplate.WorldStates[TEAM_NEUTRAL] = WORLD_STATE_DRAENEI_RUINS_UNCONTROL;
        draeneiRuinsTemplate.CapturedTextId[TEAM_HORDE] = TEXT_HORDE_TAKEN_DRAENEI_RUINS;
        draeneiRuinsTemplate.CapturedTextId[TEAM_ALLIANCE] = TEXT_ALLIANCE_TAKEN_DRAENEI_RUINS;
        draeneiRuinsTemplate.LostTextId[TEAM_HORDE] = TEXT_HORDE_LOST_DRAENEI_RUINS;
        draeneiRuinsTemplate.LostTextId[TEAM_ALLIANCE] = TEXT_ALLIANCE_LOST_DRAENEI_RUINS;

        EotSCapturePointInfoTemplate felReaverRuinsTemplate;
        felReaverRuinsTemplate.Graveyard = sWorldSafeLocsStore.AssertEntry(GRAVEYARD_FEL_REAVER_RUINS);
        felReaverRuinsTemplate.WorldStates[TEAM_ALLIANCE] = WORLD_STATE_FEL_REAVER_ALLIANCE_CONTROL;
        felReaverRuinsTemplate.WorldStates[TEAM_HORDE] = WORLD_STATE_FEL_REAVER_HORDE_CONTROL;
        felReaverRuinsTemplate.WorldStates[TEAM_NEUTRAL] = WORLD_STATE_FEL_REAVER_UNCONTROL;
        felReaverRuinsTemplate.CapturedTextId[TEAM_HORDE] = TEXT_HORDE_TAKEN_FEL_REAVER_RUINS;
        felReaverRuinsTemplate.CapturedTextId[TEAM_ALLIANCE] = TEXT_ALLIANCE_TAKEN_FEL_REAVER_RUINS;
        felReaverRuinsTemplate.LostTextId[TEAM_HORDE] = TEXT_HORDE_LOST_FEL_REAVER_RUINS;
        felReaverRuinsTemplate.LostTextId[TEAM_ALLIANCE] = TEXT_ALLIANCE_LOST_FEL_REAVER_RUINS;

        _capturePoints.emplace(CAPTURE_POINT_BLOOD_ELF_TOWER, EotSCapturePointInfo(std::make_shared<EotSCapturePointInfoTemplate>(bloodElfTowerTemplate)));
        _capturePoints.emplace(CAPTURE_POINT_MAGE_TOWER, EotSCapturePointInfo(std::make_shared<EotSCapturePointInfoTemplate>(mageTowerTemplate)));
        _capturePoints.emplace(CAPTURE_POINT_DRAENEI_RUINS, EotSCapturePointInfo(std::make_shared<EotSCapturePointInfoTemplate>(draeneiRuinsTemplate)));
        _capturePoints.emplace(CAPTURE_POINT_FEL_REAVER_RUINS, EotSCapturePointInfo(std::make_shared<EotSCapturePointInfoTemplate>(felReaverRuinsTemplate)));

        Worldstates[WORLD_STATE_HORDE_BASE] = 0;
        Worldstates[WORLD_STATE_ALLIANCE_BASE] = 0;
        Worldstates[WORLD_STATE_HORDE_BASE_2] = 0;
        Worldstates[WORLD_STATE_ALLIANCE_BASE_2] = 0;
        Worldstates[WORLD_STATE_DRAENEI_RUINS_HORDE_CONTROL] = 0;
        Worldstates[WORLD_STATE_DRAENEI_RUINS_ALLIANCE_CONTROL] = 0;
        Worldstates[WORLD_STATE_DRAENEI_RUINS_UNCONTROL] = 1;
        Worldstates[WORLD_STATE_MAGE_TOWER_ALLIANCE_CONTROL] = 0;
        Worldstates[WORLD_STATE_MAGE_TOWER_HORDE_CONTROL] = 0;
        Worldstates[WORLD_STATE_MAGE_TOWER_UNCONTROL] = 1;
        Worldstates[WORLD_STATE_FEL_REAVER_HORDE_CONTROL] = 0;
        Worldstates[WORLD_STATE_FEL_REAVER_ALLIANCE_CONTROL] = 0;
        Worldstates[WORLD_STATE_FEL_REAVER_UNCONTROL] = 1;
        Worldstates[WORLD_STATE_BLOOD_ELF_HORDE_CONTROL] = 0;
        Worldstates[WORLD_STATE_BLOOD_ELF_ALLIANCE_CONTROL] = 0;
        Worldstates[WORLD_STATE_BLOOD_ELF_UNCONTROL] = 1;
        Worldstates[WORLD_STATE_ALLIANCE_INFO_BAR] = GetFlagStateForTeamId(TEAM_ALLIANCE);
        Worldstates[WORLD_STATE_HORDE_INFO_BAR] = GetFlagStateForTeamId(TEAM_ALLIANCE);
        Worldstates[WORLD_STATE_NETHERSTORM_FLAG] = _flagState;
    }

    void OnSetup() override
    {
        BattlegroundScript::OnSetup();
        for (auto& itr : _capturePoints)
            SpawnObjectsForCapturePointForTeamId(itr.first, TEAM_NEUTRAL);

        if (GameObject* go = AddObject(GO_BLOOD_ELF_TOWER_CAP, { 2050.493164f, 1372.235962f, 1194.563477f,  1.710423f }, { 0.000000f,  0.000000f,  0.7547100f,  0.656059f }))
            _capturePoints[CAPTURE_POINT_BLOOD_ELF_TOWER].CapturePointObject = go->GetGUID();
        if (GameObject* go = AddObject(GO_DRAENEI_RUINS_CAP, { 2301.010498f, 1386.931641f, 1197.183472f,  1.570796f }, { 0.000000f,  0.000000f,  0.7071070f,  0.707107f }))
            _capturePoints[CAPTURE_POINT_DRAENEI_RUINS].CapturePointObject = go->GetGUID();
        if (GameObject* go = AddObject(GO_FEL_REAVER_RUINS_CAP, { 2024.600708f, 1742.819580f, 1195.157715f,  2.443461f }, { 0.000000f,  0.000000f,  0.9396930f,  0.342020f }))
            _capturePoints[CAPTURE_POINT_FEL_REAVER_RUINS].CapturePointObject = go->GetGUID();
        if (GameObject* go = AddObject(GO_MAGE_TOWER_CAP, { 2282.121582f, 1760.006958f, 1189.707153f,  1.919862f }, { 0.000000f,  0.000000f,  0.8191520f,  0.573576f }))
            _capturePoints[CAPTURE_POINT_MAGE_TOWER].CapturePointObject = go->GetGUID();

        uint32 const bloodElfTowerBuffs[] = { GO_BERSERK_BLOOD_ELF, GO_RESTORATION_BLOOD_ELF, GO_SPEED_BUFF_BLOOD_ELF };
        AddObject(Trinity::Containers::SelectRandomContainerElement(bloodElfTowerBuffs), { 2050.468f, 1372.202f, 1194.563f, 1.675514f }, { 0.0f, 0.0f, 0.743144f, 0.6691315f });
        uint32 const felReaverRuinsBuffs[] = { GO_BERSERK_FEL_REAVER, GO_RESTORATION_FEL_REAVER, GO_SPEED_BUFF_FEL_REAVER };
        AddObject(Trinity::Containers::SelectRandomContainerElement(felReaverRuinsBuffs), { 2046.463f, 1749.167f, 1190.01f, 5.410522f }, { 0.0f, 0.0f, -0.4226179f, 0.9063079f });
        uint32 const mageTowerBuffs[] = { GO_BERSERK_MAGE_TOWER, GO_RESTORATION_MAGE_TOWER, GO_SPEED_BUFF_MAGE_TOWER };
        AddObject(Trinity::Containers::SelectRandomContainerElement(mageTowerBuffs), { 2283.71f, 1748.87f, 1189.707f, 4.782203f }, { 0.0f, 0.0f, -0.6819983f, 0.7313538f });
        uint32 const draeneiRuinsBuffs[] = { GO_BERSERK_DRAENEI_RUINS, GO_RESTORATION_DRAENEI_RUINS, GO_SPEED_BUFF_DRAENEI_RUINS };
        AddObject(Trinity::Containers::SelectRandomContainerElement(draeneiRuinsBuffs), { 2302.477f, 1391.245f, 1197.736f, 1.762782f }, { 0.0f, 0.0f, 0.7716246f, 0.6360782f });
    }

    void OnPlayerEnter(Player* player) override
    {
        _playerOutOfPoints.push_back(player->GetGUID());
    }

    void OnPlayerLeave(Player* player) override
    {
        auto itr = std::find(_playerOutOfPoints.begin(), _playerOutOfPoints.end(), player->GetGUID());
        if (itr != _playerOutOfPoints.end())
            _playerOutOfPoints.erase(itr);
        else
        {
            bool found = false;
            for (auto& point : _capturePoints)
            {
                for (GuidVector& guidVector : point.second.PlayersInPoint)
                {
                    itr = std::find(guidVector.begin(), guidVector.end(), player->GetGUID());
                    if (itr != guidVector.end())
                    {
                        guidVector.erase(itr);
                        found = true;
                        break;
                    }
                }

                if (found)
                    break;
            }
        }
    }

    void SpawnObjectsForCapturePointForTeamId(EotSCapturePoint const capturePoint, TeamId const teamId)
    {
        DespawnCapturePointEntities(capturePoint);
        SpawnCapturePointEntities(capturePoint, teamId);
    }

    WorldSafeLocsEntry const* GetGraveYardForPlayer(Player* player) override
    {
        WorldSafeLocsEntry const* gy = player->GetTeam() == ALLIANCE ? sWorldSafeLocsStore.AssertEntry(GRAVEYARD_ALLIANCE_START) : sWorldSafeLocsStore.AssertEntry(GRAVEYARD_HORDE_START);
        float dist = player->GetPosition().GetExactDist2d(gy->Loc.X, gy->Loc.Y);

        for (auto const& capturePoint : _capturePoints)
        {
            if (capturePoint.second.Owner != player->GetTeamId())
                continue;

            float const thisDistance = player->GetPosition().GetExactDist2d(capturePoint.second.Template->Graveyard->Loc.X, capturePoint.second.Template->Graveyard->Loc.Y);
            if (dist > thisDistance)
            {
                dist = thisDistance;
                gy = capturePoint.second.Template->Graveyard;
            }
        }

        return gy;
    }

    void OnGameObjectCreate(GameObject* gameObject) override
    {
        BattlegroundScript::OnGameObjectCreate(gameObject);
        switch (gameObject->GetEntry())
        {
            case GO_EYE_OF_THE_STORM_FLAG:
                _flag = gameObject->GetGUID();
                break;
            default:
                break;
        }
    }

    void OnUpdateInProgress(uint32 diff) override
    {
        PeriodicResourceGainBattlegroundScript::OnUpdateInProgress(diff);
        AssaultStrategy::OnUpdateInProgress(diff);

        if (_flagState == FLAG_STATE_PICKED_UP)
        {
            if (Player* flagCarrier = ObjectAccessor::GetPlayer(Map, _flagCarrier))
            {
                if (flagCarrier->IsInRange2d(2043.91f, 1729.68f, 0.0f, 2.0f))
                {
                    if (flagCarrier->GetTeamId() == _capturePoints[CAPTURE_POINT_FEL_REAVER_RUINS].Owner)
                        CaptureFlag(flagCarrier);
                }
                else if (flagCarrier->IsInRange2d(2048.38f, 1393.64f, 0.0f, 2.0f))
                {
                    if (flagCarrier->GetTeamId() == _capturePoints[CAPTURE_POINT_BLOOD_ELF_TOWER].Owner)
                        CaptureFlag(flagCarrier);
                }
                else if (flagCarrier->IsInRange2d(2286.55f, 1402.39f, 0.0f, 2.0f))
                {
                    if (flagCarrier->GetTeamId() == _capturePoints[CAPTURE_POINT_DRAENEI_RUINS].Owner)
                        CaptureFlag(flagCarrier);
                }
                else if (flagCarrier->IsInRange2d(2284.69f, 1730.92f, 0.0f, 2.0f))
                {
                    if (flagCarrier->GetTeamId() == _capturePoints[CAPTURE_POINT_MAGE_TOWER].Owner)
                        CaptureFlag(flagCarrier);
                }
            }
        }

        // Only each 2 seconds
        _towerCapTimer -= diff;
        if (_towerCapTimer <= 0)
        {
            auto itr = _playerOutOfPoints.begin();
            while (itr != _playerOutOfPoints.end())
            {
                bool removed = false;
                if (Player* player = ObjectAccessor::GetPlayer(Map, *itr))
                {
                    for (auto& point : _capturePoints)
                    {
                        if (GameObject* go = Map->GetGameObject(point.second.CapturePointObject))
                        {
                            if (player->CanCaptureTowerPoint() && player->IsWithinDistInMap(go, POINT_RADIUS))
                            {
                                player->SendUpdateWorldState(WORLD_STATE_PROGRESS_BAR_PERCENT_GREY, PROGRESS_BAR_PERCENT_GREY);
                                player->SendUpdateWorldState(WORLD_STATE_PROGRESS_BAR_STATUS, point.second.StatusBarValue);
                                player->SendUpdateWorldState(WORLD_STATE_PROGRESS_BAR_SHOW, PROGRESS_BAR_SHOW);

                                point.second.PlayersInPoint[player->GetTeamId()].push_back(*itr);
                                itr = _playerOutOfPoints.erase(itr);
                                removed = true;
                                break;
                            }
                        }
                    }
                }

                if (!removed)
                    ++itr;
            }

            for (auto& point : _capturePoints)
            {
                bool removed = false;
                if (GameObject* go = Map->GetGameObject(point.second.CapturePointObject))
                {
                    for (auto& guidVector : point.second.PlayersInPoint)
                    {
                        itr = guidVector.begin();
                        while (itr != guidVector.end())
                        {
                            if (Player* player = ObjectAccessor::GetPlayer(Map, *itr))
                            {
                                if (!player->CanCaptureTowerPoint() || !player->IsWithinDistInMap(go, POINT_RADIUS))
                                {
                                    player->SendUpdateWorldState(WORLD_STATE_PROGRESS_BAR_SHOW, PROGRESS_BAR_DONT_SHOW);
                                    _playerOutOfPoints.push_back(*itr);
                                    itr = guidVector.erase(itr);
                                    removed = true;
                                    break;
                                }
                            }

                            if (!removed)
                                ++itr;
                        }
                    }
                }

                // minimum a 5 point "increase"
                int32 const allianceCount = point.second.PlayersInPoint[TEAM_ALLIANCE].size();
                int32 const hordeCount = point.second.PlayersInPoint[TEAM_HORDE].size();
                int32 countDiff = allianceCount - hordeCount;
                if (countDiff == 0)
                    continue;

                if (countDiff > 0)
                    point.second.StatusBarValue = std::min<int32>(PROGRESS_BAR_ALI_CONTROLLED, point.second.StatusBarValue + std::min<int32>(countDiff, POINT_MAX_CAPTURERS_COUNT));
                else if (countDiff < 0)
                    point.second.StatusBarValue = std::max<int32>(PROGRESS_BAR_HORDE_CONTROLLED, point.second.StatusBarValue - std::min<int32>(-countDiff, POINT_MAX_CAPTURERS_COUNT));

                if (point.second.StatusBarValue <= PROGRESS_BAR_NEUTRAL_LOW && point.second.Owner != TEAM_HORDE)
                    PointChangesOwner(point.first, TEAM_HORDE);
                else if (point.second.StatusBarValue >= PROGRESS_BAR_NEUTRAL_HIGH && point.second.Owner != TEAM_ALLIANCE)
                    PointChangesOwner(point.first, TEAM_ALLIANCE);
                else if (point.second.StatusBarValue < PROGRESS_BAR_NEUTRAL_HIGH && point.second.StatusBarValue > PROGRESS_BAR_NEUTRAL_LOW && point.second.Owner != TEAM_NEUTRAL)
                    PointChangesOwner(point.first, TEAM_NEUTRAL);

                for (GuidVector const& guidVector : point.second.PlayersInPoint)
                    for (ObjectGuid const guid : guidVector)
                        if (Player* player = ObjectAccessor::GetPlayer(Map, guid))
                            player->SendUpdateWorldState(WORLD_STATE_PROGRESS_BAR_STATUS, point.second.StatusBarValue);
            }

            _towerCapTimer = 2000;
        }
    }

    std::vector<Player*> GetAffectedPlayers() const override
    {
        std::vector<Player*> players;
        players.reserve(1);
        if (Player* player = ObjectAccessor::GetPlayer(Map, _flagCarrier))
            players.push_back(player);

        return players;
    }

    uint8 GetBasesForTeamId(TeamId const teamId) const
    {
        return std::count_if(_capturePoints.begin(), _capturePoints.end(), [teamId](std::pair<EotSCapturePoint, EotSCapturePointInfo> const& capturePointInfo)
        {
            return capturePointInfo.second.Owner == teamId;
        });
    }

    uint8 GetFlagStateForTeamId(TeamId const teamId) const
    {
        switch (_flagState)
        {
            case FLAG_STATE_PICKED_UP:
                if (Player* player = ObjectAccessor::FindPlayer(_flagCarrier))
                    return player->GetTeamId() == teamId ? FLAG_STATE_PICKED_UP : FLAG_STATE_NOT_PICKED_UP;
            case FLAG_STATE_NOT_PICKED_UP:
            default:
                return FLAG_STATE_NOT_PICKED_UP;
        }
    }

    void OnFlagClicked(Player* player, GameObject* flag)
    {
        if (!flag || flag->GetEntry() != GO_EYE_OF_THE_STORM_FLAG || flag->GetGoType() != GAMEOBJECT_TYPE_NEW_FLAG)
            return;

        _flagCarrier = player->GetGUID();
        _flagState = FLAG_STATE_PICKED_UP;

        UpdateWorldState(flag->GetGOInfo()->newflag.worldState1, _flagState);
        battleground->SendBroadcastText(TEXT_TAKEN_FLAG, player->GetTeam() == ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE, player);

        if (player->GetTeam() == ALLIANCE)
            battleground->PlaySoundToAll(SOUND_FLAG_PICKED_UP_ALLIANCE, flag->GetGUID());
        else
            battleground->PlaySoundToAll(SOUND_FLAG_PICKED_UP_HORDE, flag->GetGUID());

        // Gameobject should cast the spell...
        player->CastSpell(player, flag->GetGOInfo()->newflag.pickupSpell, true);
        ToggleGameobjectSpawn(flag->GetGUID(), RESPAWN_ONE_DAY);
        UpdateWorldState(player->GetTeam() == ALLIANCE ? WORLD_STATE_ALLIANCE_INFO_BAR : WORLD_STATE_HORDE_INFO_BAR, GetFlagStateForTeamId(player->GetTeamId()));
        AddAndEnablePlayerToAssault(player);
    }

    void OnDroppedFlagClicked(Player* player, GameObject* droppedFlag)
    {
        Scheduler.CancelGroup(SCHEDULE_DROPPED_FLAG_RESPAWN);
        OnFlagClicked(player, Map->GetGameObject(_flag));
    }

    void CaptureFlag(Player* player)
    {
        ResetAssault();
        _flagCarrier = ObjectGuid::Empty;
        _flagState = FLAG_STATE_CAPTURED;

        UpdateWorldState(player->GetTeam() == ALLIANCE ? WORLD_STATE_ALLIANCE_INFO_BAR : WORLD_STATE_HORDE_INFO_BAR, GetFlagStateForTeamId(player->GetTeamId()));
        UpdateWorldState(WORLD_STATE_NETHERSTORM_FLAG, _flagState);
        player->RemoveAurasDueToSpell(SPELL_NETHERSTORM_FLAG);

        player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT);

        if (player->GetTeam() == ALLIANCE)
        {
            battleground->SendBroadcastText(TEXT_ALLIANCE_CAPTURED_FLAG, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);
            battleground->PlaySoundToAll(SOUND_FLAG_CAPTURED_ALLIANCE);
        }
        else
        {
            battleground->SendBroadcastText(TEXT_HORDE_CAPTURED_FLAG, CHAT_MSG_BG_SYSTEM_HORDE, player);
            battleground->PlaySoundToAll(SOUND_FLAG_CAPTURED_HORDE);
        }

        battleground->UpdatePlayerScore(player, SCORE_FLAG_CAPTURES, 1);
        battleground->AddPoints(player->GetTeam(), std::min<uint32>(battleground->GetTemplate()->MaxPoints - battleground->GetTeamScore(player->GetTeamId()), SCORE_FLAG_CAPTURE_BY_BASE_COUNT[GetBasesForTeamId(player->GetTeamId())]));
        UpdateWorldState(player->GetTeam() == ALLIANCE ? WORLD_STATE_SCORE_ALLIANCE : WORLD_STATE_SCORE_HORDE, battleground->GetTeamScore(player->GetTeamId()));

        Scheduler.Schedule(10s, [this](TaskContext context)
        {
            ToggleGameobjectSpawn(_flag, RESPAWN_IMMEDIATELY);
            _flagState = FLAG_STATE_NOT_PICKED_UP;
            battleground->SendBroadcastText(TEXT_FLAG_RESET, CHAT_MSG_BG_SYSTEM_NEUTRAL);
            battleground->PlaySoundToAll(SOUND_FLAG_RESET);             // flags respawned sound...
            UpdateWorldState(WORLD_STATE_ALLIANCE_INFO_BAR, GetFlagStateForTeamId(TEAM_ALLIANCE));
            UpdateWorldState(WORLD_STATE_HORDE_INFO_BAR, GetFlagStateForTeamId(TEAM_HORDE));
            UpdateWorldState(WORLD_STATE_NETHERSTORM_FLAG, _flagState);
        });
    }

    void PointChangesOwner(EotSCapturePoint const point, TeamId const teamId)
    {
        TeamId const oldTeam = _capturePoints[point].Owner;
        ASSERT(oldTeam != teamId);

        _capturePoints[point].Owner = teamId;

        SetPointsToAddForTeam(teamId, SCORE_BY_BASE_COUNT[GetBasesForTeamId(teamId)]);

        UpdateWorldState(_capturePoints[point].Template->WorldStates[TEAM_ALLIANCE], teamId == TEAM_ALLIANCE);
        UpdateWorldState(_capturePoints[point].Template->WorldStates[TEAM_HORDE], teamId == TEAM_HORDE);
        UpdateWorldState(_capturePoints[point].Template->WorldStates[TEAM_NEUTRAL], teamId == TEAM_NEUTRAL);
        UpdateWorldState(WORLD_STATE_ALLIANCE_BASE, GetBasesForTeamId(TEAM_ALLIANCE));
        UpdateWorldState(WORLD_STATE_HORDE_BASE, GetBasesForTeamId(TEAM_HORDE));

        if (oldTeam == TEAM_NEUTRAL)
        {
            battleground->SendBroadcastText(_capturePoints[point].Template->CapturedTextId[teamId], teamId == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE);
            battleground->PlaySoundToAll(teamId == TEAM_ALLIANCE ? SOUND_FLAG_CAPTURED_ALLIANCE : SOUND_FLAG_CAPTURED_HORDE);
        }
        else // lost point
        {
            battleground->SendBroadcastText(_capturePoints[point].Template->LostTextId[oldTeam], oldTeam == TEAM_ALLIANCE ? CHAT_MSG_BG_SYSTEM_ALLIANCE : CHAT_MSG_BG_SYSTEM_HORDE);
            battleground->PlaySoundToAll(oldTeam == TEAM_ALLIANCE ? SOUND_FLAG_CAPTURED_HORDE : SOUND_FLAG_CAPTURED_ALLIANCE);
            battleground->RelocateDeadPlayers(_capturePoints[point].SpiritGuide);
        }

        SpawnObjectsForCapturePointForTeamId(point, teamId);

        UpdateWorldState(WORLD_STATE_ALLIANCE_BASE_2, GetBasesForTeamId(TEAM_ALLIANCE));
        UpdateWorldState(WORLD_STATE_HORDE_BASE_2, GetBasesForTeamId(TEAM_HORDE));
    }

    void BuildPlayerPositionData(std::vector<WorldPackets::Battleground::BattlegroundPlayerPosition>* positions) const override
    {
        if (Player* player = ObjectAccessor::GetPlayer(Map, _flagCarrier))
        {
            WorldPackets::Battleground::BattlegroundPlayerPosition position;
            position.Guid = player->GetGUID();
            position.Pos = player->GetPosition();
            position.IconID = player->GetTeam() == ALLIANCE ? PLAYER_POSITION_ICON_ALLIANCE_FLAG : PLAYER_POSITION_ICON_HORDE_FLAG;
            position.ArenaSlot = PLAYER_POSITION_ARENA_SLOT_NONE;
            positions->push_back(position);
        }
    }

    void OnUpdatePlayerScore(ScoreType scoreType, Player* player) override
    {
        switch (scoreType)
        {
            case SCORE_FLAG_CAPTURES:
                player->UpdateCriteria(CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE, OBJECTIVE_CAPTURE_FLAG);
                break;
            default:
                break;
        }
    }

    void OnPlayerDroppedFlag(Player* player) override
    {
        if (GameObject* go = Map->GetGameObject(_flag))
        {
            player->CastSpell(player, SPELL_RECENTLY_DROPPED_FLAG, true);
            GameObject* droppedFlag = GameObject::CreateGameObject(go->GetGOInfo()->newflag.FlagDrop, Map, player->GetPosition(), QuaternionData::fromEulerAnglesZYX(player->GetOrientation(), 0.f, 0.f), 255, GO_STATE_READY);
            if (droppedFlag)
                Map->AddToMap(droppedFlag);

            _flagState = FLAG_STATE_DROPPED;
            _flagCarrier = ObjectGuid::Empty;
            UpdateWorldState(WORLD_STATE_ALLIANCE_INFO_BAR, GetFlagStateForTeamId(TEAM_ALLIANCE));
            UpdateWorldState(WORLD_STATE_HORDE_INFO_BAR, GetFlagStateForTeamId(TEAM_HORDE));
            UpdateWorldState(WORLD_STATE_NETHERSTORM_FLAG, _flagState);

            Scheduler.Schedule(Milliseconds(go->GetGOInfo()->newflag.RespawnTime), SCHEDULE_DROPPED_FLAG_RESPAWN, [this, droppedFlagGuid = droppedFlag->GetGUID()](TaskContext)
            {
                if (GameObject* droppedFlag = Map->GetGameObject(droppedFlagGuid))
                    droppedFlag->RemoveFromWorld();

                ToggleGameobjectSpawn(_flag, RESPAWN_IMMEDIATELY);
                _flagState = FLAG_STATE_NOT_PICKED_UP;
                battleground->SendBroadcastText(TEXT_FLAG_RESET, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                battleground->PlaySoundToAll(SOUND_FLAG_RESET); // flags respawned sound...
                UpdateWorldState(WORLD_STATE_ALLIANCE_INFO_BAR, GetFlagStateForTeamId(TEAM_ALLIANCE));
                UpdateWorldState(WORLD_STATE_HORDE_INFO_BAR, GetFlagStateForTeamId(TEAM_HORDE));
                UpdateWorldState(WORLD_STATE_NETHERSTORM_FLAG, _flagState);
            });
        }
    }

    bool OnGameObjectClicked(Player* player, GameObject* gameobject) override
    {
        if (!BattlegroundScript::OnGameObjectClicked(player, gameobject))
            return false;

        switch (gameobject->GetGoType())
        {
            case GAMEOBJECT_TYPE_NEW_FLAG:
                OnFlagClicked(player, gameobject);
                return true;
            case GAMEOBJECT_TYPE_NEW_FLAG_DROP:
                OnDroppedFlagClicked(player, gameobject);
                return true;
            default:
                return false;
        }
    }

private:
    std::unordered_map<EotSCapturePoint, EotSCapturePointInfo> _capturePoints;
    FlagState _flagState;
    ObjectGuid _flagCarrier;
    ObjectGuid _flag;
    ObjectGuid _droppedFlag;
    GuidVector _playerOutOfPoints;

    int32 _towerCapTimer;

    uint16 const SCORE_FLAG_CAPTURE_BY_BASE_COUNT[5] = { 0, 75, 85, 100, 500 }; // todo confirm
    uint8 const SCORE_BY_BASE_COUNT[5] = { 0, 2, 3, 4, 8 }; // todo confirm 8
};

void AddSC_battleground_eye_of_the_storm()
{
    RegisterBattlegroundMapScript(battleground_eye_of_the_storm, 566);
}
