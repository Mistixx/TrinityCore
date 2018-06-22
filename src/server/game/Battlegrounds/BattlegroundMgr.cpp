/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "BattlegroundMgr.h"
#include "Arena.h"
#include "BattlegroundPackets.h"
#include "Common.h"
#include "Containers.h"
#include "DatabaseEnv.h"
#include "DB2Stores.h"
#include "DisableMgr.h"
#include "GameEventMgr.h"
#include "Language.h"
#include "Log.h"
#include "Map.h"
#include "MapInstanced.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "SharedDefines.h"
#include "World.h"

bool BattlegroundTemplate::IsArena() const
{
    return BattlemasterEntry->InstanceType == MAP_ARENA;
}

/*********************************************************/
/***            BATTLEGROUND MANAGER                   ***/
/*********************************************************/

BattlegroundMgr::BattlegroundMgr() :
    m_NextRatedArenaUpdate(sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER)),
    m_UpdateTimer(0), m_ArenaTesting(false), m_Testing(false)
{ }

BattlegroundMgr::~BattlegroundMgr()
{
    DeleteAllBattlegrounds();
}

void BattlegroundMgr::DeleteAllBattlegrounds()
{
    for (BattlegroundDataContainer::iterator itr1 = bgDataStore.begin(); itr1 != bgDataStore.end(); ++itr1)
    {
        BattlegroundData& data = itr1->second;

        while (!data.m_Battlegrounds.empty())
            delete data.m_Battlegrounds.begin()->second;
        data.m_Battlegrounds.clear();

        while (!data.BGFreeSlotQueue.empty())
            delete data.BGFreeSlotQueue.front();
    }

    bgDataStore.clear();
}

BattlegroundMgr* BattlegroundMgr::instance()
{
    static BattlegroundMgr instance;
    return &instance;
}

// used to update running battlegrounds, and delete finished ones
void BattlegroundMgr::Update(uint32 diff)
{
    m_UpdateTimer += diff;
    if (m_UpdateTimer > BATTLEGROUND_OBJECTIVE_UPDATE_INTERVAL)
    {
        for (BattlegroundDataContainer::iterator itr1 = bgDataStore.begin(); itr1 != bgDataStore.end(); ++itr1)
        {
            BattlegroundContainer& bgs = itr1->second.m_Battlegrounds;
            BattlegroundContainer::iterator itrDelete = bgs.begin();
            // first one is template and should not be deleted
            for (BattlegroundContainer::iterator itr = ++itrDelete; itr != bgs.end();)
            {
                itrDelete = itr++;
                Battleground* bg = itrDelete->second;

                bg->Update(m_UpdateTimer);
                if (bg->ToBeDeleted())
                {
                    itrDelete->second = nullptr;
                    bgs.erase(itrDelete);
                    BattlegroundClientIdsContainer& clients = itr1->second.m_ClientBattlegroundIds[bg->GetBracketId()];
                    if (!clients.empty())
                        clients.erase(bg->GetClientInstanceId());

                    delete bg;
                }
            }
        }

        m_UpdateTimer = 0;
    }

    // update events timer
    for (uint32 qtype = BATTLEGROUND_QUEUE_NONE; qtype < MAX_BATTLEGROUND_QUEUE_TYPES; ++qtype)
        m_BattlegroundQueues[qtype].UpdateEvents(diff);

    // update scheduled queues
    if (!m_QueueUpdateScheduler.empty())
    {
        std::vector<uint64> scheduled;
        std::swap(scheduled, m_QueueUpdateScheduler);

        for (uint32 i = 0; i < scheduled.size(); ++i)
        {
            uint32 arenaMMRating = scheduled[i] >> 40;
            ArenaType arenaType = ArenaType(scheduled[i] >> 32 & 0xFF);
            BattlegroundQueueTypeId bgQueueTypeId = BattlegroundQueueTypeId(scheduled[i] >> 24 & 0xFF);
            BattlegroundTypeId bgTypeId = BattlegroundTypeId((scheduled[i] >> 8) & 0xFFFF);
            BattlegroundBracketId bracket_id = BattlegroundBracketId(scheduled[i] & 0xFF);
            m_BattlegroundQueues[bgQueueTypeId].BattlegroundQueueUpdate(diff, bgTypeId, bracket_id, arenaType, arenaMMRating > 0, arenaMMRating);
        }
    }

    // if rating difference counts, maybe force-update queues
    if (sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE) && sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER))
    {
        // it's time to force update
        if (m_NextRatedArenaUpdate < diff)
        {
            // forced update for rated arenas (scan all, but skipped non rated)
            TC_LOG_TRACE("bg.arena", "BattlegroundMgr: UPDATING ARENA QUEUES");
            for (int qtype = BATTLEGROUND_QUEUE_2v2; qtype <= BATTLEGROUND_QUEUE_5v5; ++qtype)
                for (int bracket = BG_BRACKET_ID_FIRST; bracket < MAX_BATTLEGROUND_BRACKETS; ++bracket)
                    m_BattlegroundQueues[qtype].BattlegroundQueueUpdate(diff,
                        BATTLEGROUND_AA, BattlegroundBracketId(bracket),
                        BattlegroundMgr::BGArenaType(BattlegroundQueueTypeId(qtype)), true, 0);

            m_NextRatedArenaUpdate = sWorld->getIntConfig(CONFIG_ARENA_RATED_UPDATE_TIMER);
        }
        else
            m_NextRatedArenaUpdate -= diff;
    }
}

void BattlegroundMgr::BuildBattlegroundStatusHeader(WorldPackets::Battleground::BattlefieldStatusHeader* header, Battleground* bg, Player* player, uint32 ticketId, uint32 joinTime, uint32 arenaType)
{
    header->Ticket.RequesterGuid = player->GetGUID();
    header->Ticket.Id = ticketId;
    header->Ticket.Type = WorldPackets::LFG::RideType::Battlegrounds;
    header->Ticket.Time = joinTime;
    header->QueueID = bg->GetQueueId();
    header->RangeMin = bg->GetMinLevel();
    header->RangeMax = bg->GetMaxLevel();
    header->TeamSize = bg->IsArena() ? arenaType : 0;
    header->InstanceID = bg->GetClientInstanceId();
    header->RegisteredMatch = bg->IsRated();
    header->TournamentRules = false;
}

void BattlegroundMgr::BuildBattlegroundStatusNone(WorldPackets::Battleground::BattlefieldStatusNone* battlefieldStatus, Player* player, uint32 ticketId, uint32 joinTime)
{
    battlefieldStatus->Ticket.RequesterGuid = player->GetGUID();
    battlefieldStatus->Ticket.Id = ticketId;
    battlefieldStatus->Ticket.Type = WorldPackets::LFG::RideType::Battlegrounds;
    battlefieldStatus->Ticket.Time = joinTime;
}

void BattlegroundMgr::BuildBattlegroundStatusNeedConfirmation(WorldPackets::Battleground::BattlefieldStatusNeedConfirmation* battlefieldStatus, Battleground* bg, Player* player, uint32 ticketId, uint32 joinTime, uint32 timeout, uint32 arenaType)
{
    BuildBattlegroundStatusHeader(&battlefieldStatus->Hdr, bg, player, ticketId, joinTime, arenaType);
    battlefieldStatus->Mapid = bg->GetMapId();
    battlefieldStatus->Timeout = timeout;
    battlefieldStatus->Role = 0;
}

void BattlegroundMgr::BuildBattlegroundStatusActive(WorldPackets::Battleground::BattlefieldStatusActive* battlefieldStatus, Battleground* bg, Player* player, uint32 ticketId, uint32 joinTime, uint32 arenaType)
{
    BuildBattlegroundStatusHeader(&battlefieldStatus->Hdr, bg, player, ticketId, joinTime, arenaType);
    battlefieldStatus->ShutdownTimer = bg->GetRemainingTime();
    battlefieldStatus->ArenaFaction = player->GetBGTeam() == HORDE ? BG_TEAM_HORDE : BG_TEAM_ALLIANCE;
    battlefieldStatus->LeftEarly = false;
    battlefieldStatus->StartTimer = bg->GetElapsedTime();
    battlefieldStatus->Mapid = bg->GetMapId();
}

void BattlegroundMgr::BuildBattlegroundStatusQueued(WorldPackets::Battleground::BattlefieldStatusQueued* battlefieldStatus, Battleground* bg, Player* player, uint32 ticketId, uint32 joinTime, uint32 avgWaitTime, uint32 arenaType, bool asGroup)
{
    BuildBattlegroundStatusHeader(&battlefieldStatus->Hdr, bg, player, ticketId, joinTime, arenaType);
    battlefieldStatus->AverageWaitTime = avgWaitTime;
    battlefieldStatus->AsGroup = asGroup;
    battlefieldStatus->SuspendedQueue = false;
    battlefieldStatus->EligibleForMatchmaking = true;
    battlefieldStatus->WaitTime = GetMSTimeDiffToNow(joinTime);
}

void BattlegroundMgr::BuildBattlegroundStatusFailed(WorldPackets::Battleground::BattlefieldStatusFailed* battlefieldStatus, Battleground* bg, Player* pPlayer, uint32 ticketId, uint32 arenaType, GroupJoinBattlegroundResult result, ObjectGuid const* errorGuid /*= nullptr*/)
{
    battlefieldStatus->Ticket.RequesterGuid = pPlayer->GetGUID();
    battlefieldStatus->Ticket.Id = ticketId;
    battlefieldStatus->Ticket.Type = WorldPackets::LFG::RideType::Battlegrounds;
    battlefieldStatus->Ticket.Time = pPlayer->GetBattlegroundQueueJoinTime(BGQueueTypeId(bg->GetTypeId(), arenaType));
    battlefieldStatus->QueueID = bg->GetQueueId();
    battlefieldStatus->Reason = result;
    if (errorGuid && (result == ERR_BATTLEGROUND_NOT_IN_BATTLEGROUND || result == ERR_BATTLEGROUND_JOIN_TIMED_OUT))
        battlefieldStatus->ClientID = *errorGuid;
}

Battleground* BattlegroundMgr::GetBattleground(uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    if (!instanceId)
        return nullptr;

    BattlegroundDataContainer::const_iterator begin, end;

    if (bgTypeId == BATTLEGROUND_TYPE_NONE)
    {
        begin = bgDataStore.begin();
        end = bgDataStore.end();
    }
    else
    {
        end = bgDataStore.find(bgTypeId);
        if (end == bgDataStore.end())
            return nullptr;
        begin = end++;
    }

    for (BattlegroundDataContainer::const_iterator it = begin; it != end; ++it)
    {
        BattlegroundContainer const& bgs = it->second.m_Battlegrounds;
        BattlegroundContainer::const_iterator itr = bgs.find(instanceId);
        if (itr != bgs.end())
           return itr->second;
    }

    return nullptr;
}

Battleground* BattlegroundMgr::GetBattlegroundTemplate(BattlegroundTypeId bgTypeId)
{
    auto const itr = bgDataStore.find(bgTypeId);
    if (itr == bgDataStore.end())
        return nullptr;

    BattlegroundContainer const& bgs = itr->second.m_Battlegrounds;
    // map is sorted and we can be sure that lowest instance id has only BG template
    return bgs.empty() ? nullptr : bgs.begin()->second;
}

uint32 BattlegroundMgr::CreateClientVisibleInstanceId(BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id)
{
    if (IsArenaType(bgTypeId))
        return 0;                                           //arenas don't have client-instanceids

    // we create here an instanceid, which is just for
    // displaying this to the client and without any other use..
    // the client-instanceIds are unique for each battleground-type
    // the instance-id just needs to be as low as possible, beginning with 1
    // the following works, because std::set is default ordered with "<"
    // the optimalization would be to use as bitmask std::vector<uint32> - but that would only make code unreadable

    BattlegroundClientIdsContainer& clientIds = bgDataStore[bgTypeId].m_ClientBattlegroundIds[bracket_id];
    uint32 lastId = 0;
    for (BattlegroundClientIdsContainer::const_iterator itr = clientIds.begin(); itr != clientIds.end();)
    {
        if ((++lastId) != *itr)                             //if there is a gap between the ids, we will break..
            break;
        lastId = *itr;
    }

    clientIds.insert(++lastId);
    return lastId;
}

// create a new battleground that will really be used to play
Battleground* BattlegroundMgr::CreateNewBattleground(BattlegroundTypeId originalBgTypeId, PVPDifficultyEntry const* bracketEntry, ArenaType arenaType, bool isRated)
{
    BattlegroundTypeId bgTypeId = GetRandomBG(originalBgTypeId);

    // get the template BG
    BattlegroundTemplate const* battlegroundTemplate = GetBattlegroundTemplateByTypeId(bgTypeId);

    if (!battlegroundTemplate)
    {
        TC_LOG_ERROR("bg.battleground", "Battleground: CreateNewBattleground - bg template not found for %u", bgTypeId);
        return nullptr;
    }

    // create a copy of the BG template

    Battleground* bg = new Battleground(battlegroundTemplate, bracketEntry, sMapMgr->GenerateInstanceId(),
        CreateClientVisibleInstanceId(originalBgTypeId, bracketEntry->GetBracketId()), originalBgTypeId, isRated);

    bg->SetStatus(STATUS_WAIT_JOIN); // start the joining of the bg

    if (Arena* arena = bg->ToArena())
        arena->SetArenaType(arenaType);

    return bg;
}

// used to create the BG templates
bool BattlegroundMgr::CreateBattleground(BattlegroundTemplate const* bgTemplate)
{
    Battleground* bg = new Battleground(bgTemplate);

    AddBattleground(bg);

    return true;
}

void BattlegroundMgr::LoadBattlegroundTemplates()
{
    uint32 oldMSTime = getMSTime();

    _battlegroundMapTemplates.clear();
    _battlegroundTemplates.clear();

    std::unordered_map<int16 /*mapId*/, std::vector<std::pair<uint32 /*areaTriggerId*/, uint32 /*entry*/>>> buffSpawnTriggers;
    //                                                0     1
    QueryResult bgBuffs = WorldDatabase.Query("SELECT Entry, AreaTriggerEntry FROM gameobject_areatrigger");
    if (bgBuffs)
    {
        uint32 count = 0;
        do
        {
            Field* fields = bgBuffs->Fetch();
            uint32 areaTriggerId = fields[1].GetUInt32();
            AreaTriggerEntry const* at = sAreaTriggerStore.LookupEntry(areaTriggerId);
            if (!at)
            {
                TC_LOG_ERROR("sql.sql", "Table `gameobject_areatrigger` contains non-existing AreaTriggerEntry %u", areaTriggerId);
                continue;
            }

            buffSpawnTriggers[at->ContinentID].push_back(std::make_pair(fields[1].GetUInt32(), fields[0].GetUInt64()));
            ++count;
        }
        while (bgBuffs->NextRow());

        TC_LOG_INFO("server.loading", ">> Loaded %u gameobject_areatriggers in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
    }

    // Holds creatures that should not be spawned in the given battlegrounds, by map
    std::unordered_map<std::pair<uint32 /*mapId*/, ObjectGuid::LowType>, std::unordered_set<uint32>> removedCreatures;
    // Holds gameobjects that should not be spawned in the given battlegrounds, by map
    std::unordered_map<std::pair<uint32 /*mapId*/, ObjectGuid::LowType>, std::unordered_set<uint32>> removedGameObjects;

    QueryResult removedCreaturesQuery = WorldDatabase.Query("SELECT battlegroundEntry, spawnId, map FROM battleground_creature bc INNER JOIN creature c ON bc.spawnId=c.guid WHERE remove=1");
    if (removedCreaturesQuery)
    {
        do
        {
            Field* fields = removedCreaturesQuery->Fetch();
            removedCreatures[std::make_pair<>(fields[2].GetUInt32(), fields[1].GetUInt64())].emplace(fields[0].GetUInt32());
        } while (removedCreaturesQuery->NextRow());
    }

    QueryResult removedGameObjectsQuery = WorldDatabase.Query("SELECT battlegroundEntry, spawnId, map FROM battleground_gameobject bg INNER JOIN gameobject g ON bg.spawnId=g.guid WHERE remove=1");
    if (removedGameObjectsQuery)
    {
        do
        {
            Field* fields = removedGameObjectsQuery->Fetch();
            removedGameObjects[std::make_pair<>(fields[2].GetUInt32(), fields[1].GetUInt64())].emplace(fields[0].GetUInt32());
        } while (removedGameObjectsQuery->NextRow());
    }

    //                                               0   1                 2              3             4       5           6                   7                8
    QueryResult result = WorldDatabase.Query("SELECT ID, AllianceStartLoc, HordeStartLoc, StartMaxDist, Weight, ScriptName, AllianceExploitLoc, HordeExploitLoc, MaxPoints FROM battleground_template");
    if (!result)
    {
        TC_LOG_ERROR("server.loading", ">> Loaded 0 battlegrounds. DB table `battleground_template` is empty.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        BattlegroundTypeId bgTypeId = BattlegroundTypeId(fields[0].GetUInt32());

        if (DisableMgr::IsDisabledFor(DISABLE_TYPE_BATTLEGROUND, bgTypeId, nullptr))
            continue;

        // can be overwrite by values from DB
        BattlemasterListEntry const* bl = sBattlemasterListStore.LookupEntry(bgTypeId);
        if (!bl)
        {
            TC_LOG_ERROR("bg.battleground", "Battleground ID %u could not be found in BattlemasterList.db2. The battleground was not created.", bgTypeId);
            continue;
        }

        uint32 const allianceStartId = fields[1].GetUInt32();
        uint32 const hordeStartId = fields[2].GetUInt32();
        uint32 allianceExploitId = fields[6].GetUInt32();
        uint32 hordeExploitId = fields[7].GetUInt32();

        if (!allianceExploitId)
            allianceExploitId = allianceStartId;
        if (!hordeExploitId)
            hordeExploitId = hordeStartId;

        WorldSafeLocsEntry const* allianceStart = sWorldSafeLocsStore.LookupEntry(allianceStartId);
        WorldSafeLocsEntry const* hordeStart = sWorldSafeLocsStore.LookupEntry(hordeStartId);
        WorldSafeLocsEntry const* allianceExploit = sWorldSafeLocsStore.LookupEntry(allianceExploitId);
        WorldSafeLocsEntry const* hordeExploit = sWorldSafeLocsStore.LookupEntry(hordeExploitId);

        if (bgTypeId != BATTLEGROUND_AA && bgTypeId != BATTLEGROUND_RB)
        {
            if (!allianceStart)
            {
                TC_LOG_ERROR("sql.sql", "Table `battleground_template` for id %u contains a non-existing WorldSafeLocs.dbc id %u in field `AllianceStartLoc`. BG not created.", bgTypeId, allianceStartId);
                continue;
            }

            if (!hordeStart)
            {
                TC_LOG_ERROR("sql.sql", "Table `battleground_template` for id %u contains a non-existing WorldSafeLocs.dbc id %u in field `HordeStartLoc`. BG not created.", bgTypeId, hordeStartId);
                continue;
            }

            if (!allianceExploit)
            {
                TC_LOG_ERROR("sql.sql", "Table `battleground_template` for id %u contains a non-existing WorldSafeLocs.dbc id %u in field `AllianceExploiLoc`. Using startLocation instead.", bgTypeId, allianceExploitId);
                allianceExploit = allianceStart;
            }

            if (!hordeExploit)
            {
                TC_LOG_ERROR("sql.sql", "Table `battleground_template` for id %u contains a non-existing WorldSafeLocs.dbc id %u in field `HordeExploitLoc`. Using startLocation instead.", bgTypeId, hordeExploitId);
                hordeExploit = hordeStart;
            }
        }

        BattlegroundTemplate* bgTemplate = &_battlegroundTemplates[bgTypeId];
        bgTemplate->Id = bgTypeId;
        float const dist = fields[3].GetFloat();
        bgTemplate->MaxStartDistSq = dist * dist;
        bgTemplate->Weight = fields[4].GetUInt8();
        bgTemplate->ScriptId = sObjectMgr->GetScriptId(fields[5].GetString());
        bgTemplate->BattlemasterEntry = bl;
        bgTemplate->StartLocation[TEAM_ALLIANCE] = allianceStart;
        bgTemplate->StartLocation[TEAM_HORDE] = hordeStart;
        bgTemplate->ExploitLocation[TEAM_ALLIANCE] = allianceExploit;
        bgTemplate->ExploitLocation[TEAM_HORDE] = hordeExploit;
        bgTemplate->MaxPoints = fields[8].GetUInt16();

        if (bgTemplate->BattlemasterEntry->MapID[1] == -1)
            for (auto const& buffSpawnByMap : buffSpawnTriggers)
                if (buffSpawnByMap.first == bl->MapID[0])
                    for (auto const& buffSpawn : buffSpawnByMap.second)
                        bgTemplate->BuffSpawnTriggers[buffSpawn.first].push_back(buffSpawn.second);

        QueryResult scoreTypesQuery = WorldDatabase.PQuery("SELECT ScoreType FROM battleground_template_score WHERE BattlegroundEntry=%u ORDER BY `OrderIndex`", bgTemplate->Id);
        if (scoreTypesQuery)
        {
            do
            {
                Field* scoreFields = scoreTypesQuery->Fetch();
                bgTemplate->Scores.push_back(ScoreType(scoreFields[0].GetUInt32()));
            } while (scoreTypesQuery->NextRow());
        }

        //                                                                         0                1        2
        QueryResult capturePointCreatureSpawnsQuery = WorldDatabase.PQuery("SELECT CapturePointKey, SpawnId, TeamId FROM battleground_capture_point_creature WHERE BattlegroundEntry=%u", bgTemplate->Id);
        if (capturePointCreatureSpawnsQuery)
        {
            do
            {
                Field* capturePointCreatureSpawn = capturePointCreatureSpawnsQuery->Fetch();

                uint32 capturePointId = capturePointCreatureSpawn[0].GetUInt32();
                ObjectGuid::LowType spawnId = capturePointCreatureSpawn[1].GetUInt64();
                TeamId const teamId = TeamId(capturePointCreatureSpawn[2].GetUInt8());

                CreatureData const* data = sObjectMgr->GetCreatureData(spawnId);
                if (!data)
                {
                    TC_LOG_ERROR("sql.sql", "`battleground_creature_capture_point` contains creature (GUID: " UI64FMTD ") not found in `creature` table.", spawnId);
                    continue;
                }

                bgTemplate->CreatureCapturePointSpawns[capturePointId][teamId].push_back(spawnId);
            } while (capturePointCreatureSpawnsQuery->NextRow());
        }

        //                                                                           0                1        2
        QueryResult capturePointGameObjectSpawnsQuery = WorldDatabase.PQuery("SELECT CapturePointKey, SpawnId, TeamId FROM battleground_capture_point_gameobject WHERE BattlegroundEntry=%u", bgTemplate->Id);
        if (capturePointGameObjectSpawnsQuery)
        {
            do
            {
                Field* capturePointGameObjectSpawn = capturePointGameObjectSpawnsQuery->Fetch();

                uint32 capturePointId = capturePointGameObjectSpawn[0].GetUInt32();
                ObjectGuid::LowType spawnId = capturePointGameObjectSpawn[1].GetUInt64();
                TeamId const teamId = TeamId(capturePointGameObjectSpawn[2].GetUInt8());

                GameObjectData const* data = sObjectMgr->GetGOData(spawnId);
                if (!data)
                {
                    TC_LOG_ERROR("sql.sql", "`battleground_gameobject_capture_point` contains gameobject (GUID: " UI64FMTD ") not found in `gameobject` table.", spawnId);
                    continue;
                }

                bgTemplate->GameObjectCapturePointSpawns[capturePointId][teamId].push_back(spawnId);
            } while (capturePointGameObjectSpawnsQuery->NextRow());
        }

        QueryResult doorsQuery = WorldDatabase.PQuery("SELECT GameObjectSpawnId FROM battleground_door WHERE BattlegroundEntry=%u", bgTemplate->Id);
        if (doorsQuery)
        {
            do
            {
                Field* doorFields = doorsQuery->Fetch();
                bgTemplate->Doors.push_back(doorFields[0].GetUInt64());
            } while (doorsQuery->NextRow());
        }

        QueryResult creaturesQuery = WorldDatabase.PQuery("SELECT spawnId FROM battleground_creature WHERE battlegroundEntry=%u AND remove=0", bgTemplate->Id);
        if (creaturesQuery)
        {
            do
            {
                Field* creatureFields = creaturesQuery->Fetch();
                ObjectGuid::LowType spawnId = creatureFields[0].GetUInt64();
                CreatureData const* data = sObjectMgr->GetCreatureData(spawnId);
                if (!data)
                {
                    TC_LOG_ERROR("sql.sql", "`battleground_creature` contains creature (GUID: " UI64FMTD ") not found in `creature` table.", spawnId);
                    continue;
                }

               bgTemplate->CreaturesToSpawn.push_back(spawnId);

            } while (creaturesQuery->NextRow());
        }

        QueryResult gameobjectsQuery = WorldDatabase.PQuery("SELECT spawnId, remove FROM battleground_gameobject WHERE battlegroundEntry=%u AND remove=0", bgTemplate->Id);
        if (gameobjectsQuery)
        {
            do
            {
                Field* gameobjectsFields = gameobjectsQuery->Fetch();
                ObjectGuid::LowType spawnId = gameobjectsFields[0].GetUInt64();
                GameObjectData const* data = sObjectMgr->GetGOData(spawnId);
                if (!data)
                {
                    TC_LOG_ERROR("sql.sql", "`battleground_gameobject` contains gameobject (GUID: " UI64FMTD ") not found in `gameobject` table.", spawnId);
                    continue;
                }

                bgTemplate->GameObjectsToSpawn.push_back(spawnId);

            } while (gameobjectsQuery->NextRow());
        }

        for (auto const& pair : removedCreatures)
        {
            if (bgTemplate->BattlemasterEntry->MapID[0] == static_cast<int16>(pair.first.first) && bgTemplate->BattlemasterEntry->MapID[1] == -1)
            {
                auto const itr = pair.second.find(bgTemplate->Id);
                if (itr == pair.second.end())
                    bgTemplate->CreaturesToSpawn.push_back(pair.first.second);
            }
        }

        for (auto const& pair : removedGameObjects)
        {
            if (bgTemplate->BattlemasterEntry->MapID[0] == static_cast<int16>(pair.first.first) && bgTemplate->BattlemasterEntry->MapID[1] == -1)
            {
                auto const itr = pair.second.find(bgTemplate->Id);
                if (itr == pair.second.end())
                    bgTemplate->GameObjectsToSpawn.push_back(pair.first.second);
            }
        }

        CreateBattleground(bgTemplate);

        if (bgTemplate->BattlemasterEntry->MapID[1] == -1) // in this case we have only one mapId, also check for brawl ?
            _battlegroundMapTemplates[bgTemplate->BattlemasterEntry->MapID[0]] = bgTemplate;

        ++count;
    }
    while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded %u battlegrounds in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BattlegroundMgr::SendBattlegroundList(Player* player, ObjectGuid const& guid, BattlegroundTypeId bgTypeId)
{
    BattlegroundTemplate const* bgTemplate = GetBattlegroundTemplateByTypeId(bgTypeId);
    if (!bgTemplate)
        return;

    WorldPackets::Battleground::BattlefieldList battlefieldList;
    battlefieldList.BattlemasterGuid = guid;
    battlefieldList.BattlemasterListID = bgTypeId;
    battlefieldList.MinLevel = bgTemplate->BattlemasterEntry->MinLevel;
    battlefieldList.MaxLevel = bgTemplate->BattlemasterEntry->MaxLevel;
    battlefieldList.PvpAnywhere = guid.IsEmpty();
    battlefieldList.HasRandomWinToday = player->GetRandomWinner();
    player->SendDirectMessage(battlefieldList.Write());
}

void BattlegroundMgr::SendToBattleground(Player* player, uint32 instanceId, BattlegroundTypeId bgTypeId)
{
    if (Battleground* bg = GetBattleground(instanceId, bgTypeId))
    {
        Team const team = player->GetBGTeam();

        WorldSafeLocsEntry const* loc = bg->GetTemplate()->StartLocation[Battleground::GetTeamIndexByTeamId(team)];
        ASSERT(loc, "Non-existing start location for bgType %u", bgTypeId);
        TC_LOG_DEBUG("bg.battleground", "BattlegroundMgr::SendToBattleground: Sending %s to map %u, X: %f Y: %f Z: %f O: %f (bgType %u)", player->GetName().c_str(), loc->MapID, loc->Loc.X, loc->Loc.Y, loc->Loc.Z, static_cast<float>((loc->Facing * M_PI) / 180), bgTypeId);
        player->TeleportTo(loc);
    }
    else
        TC_LOG_ERROR("bg.battleground", "BattlegroundMgr::SendToBattleground: Instance %u (bgType %u) not found while trying to teleport player %s", instanceId, bgTypeId, player->GetName().c_str());
}

void BattlegroundMgr::SendAreaSpiritHealerQueryOpcode(Player* player, Battleground* bg, ObjectGuid const& guid)
{
    uint32 time_ = 30000 - bg->GetLastResurrectTime();      // resurrect every 30 seconds
    if (time_ == uint32(-1))
        time_ = 0;

    WorldPackets::Battleground::AreaSpiritHealerTime areaSpiritHealerTime;
    areaSpiritHealerTime.HealerGuid = guid;
    areaSpiritHealerTime.TimeLeft = time_;
    player->SendDirectMessage(areaSpiritHealerTime.Write());
}

bool BattlegroundMgr::IsArenaType(BattlegroundTypeId bgTypeId)
{
    return bgTypeId == BATTLEGROUND_AA
            || bgTypeId == BATTLEGROUND_BE
            || bgTypeId == BATTLEGROUND_NA
            || bgTypeId == BATTLEGROUND_DS
            || bgTypeId == BATTLEGROUND_RV
            || bgTypeId == BATTLEGROUND_RL;
}

BattlegroundQueueTypeId BattlegroundMgr::BGQueueTypeId(BattlegroundTypeId bgTypeId, uint8 arenaType)
{
    switch (bgTypeId)
    {
        case BATTLEGROUND_AB:
            return BATTLEGROUND_QUEUE_AB;
        case BATTLEGROUND_AV:
            return BATTLEGROUND_QUEUE_AV;
        case BATTLEGROUND_EY:
            return BATTLEGROUND_QUEUE_EY;
        case BATTLEGROUND_IC:
            return BATTLEGROUND_QUEUE_IC;
        case BATTLEGROUND_TP:
            return BATTLEGROUND_QUEUE_TP;
        case BATTLEGROUND_BFG:
            return BATTLEGROUND_QUEUE_BFG;
        case BATTLEGROUND_RB:
            return BATTLEGROUND_QUEUE_RB;
        case BATTLEGROUND_SA:
            return BATTLEGROUND_QUEUE_SA;
        case BATTLEGROUND_WS:
            return BATTLEGROUND_QUEUE_WS;
        case BATTLEGROUND_SEETHING_SHORE:
            return BATTLEGROUND_QUEUE_SEETHING_SHORE;
        case BATTLEGROUND_AA:
        case BATTLEGROUND_BE:
        case BATTLEGROUND_DS:
        case BATTLEGROUND_NA:
        case BATTLEGROUND_RL:
        case BATTLEGROUND_RV:
            switch (arenaType)
            {
                case ARENA_TYPE_2v2:
                    return BATTLEGROUND_QUEUE_2v2;
                case ARENA_TYPE_3v3:
                    return BATTLEGROUND_QUEUE_3v3;
                case ARENA_TYPE_5v5:
                    return BATTLEGROUND_QUEUE_5v5;
                default:
                    return BATTLEGROUND_QUEUE_NONE;
            }
        default:
            return BATTLEGROUND_QUEUE_NONE;
    }
}

BattlegroundTypeId BattlegroundMgr::BGTemplateId(BattlegroundQueueTypeId bgQueueTypeId)
{
    switch (bgQueueTypeId)
    {
        case BATTLEGROUND_QUEUE_WS:
            return BATTLEGROUND_WS;
        case BATTLEGROUND_QUEUE_AB:
            return BATTLEGROUND_AB;
        case BATTLEGROUND_QUEUE_AV:
            return BATTLEGROUND_AV;
        case BATTLEGROUND_QUEUE_EY:
            return BATTLEGROUND_EY;
        case BATTLEGROUND_QUEUE_SA:
            return BATTLEGROUND_SA;
        case BATTLEGROUND_QUEUE_IC:
            return BATTLEGROUND_IC;
        case BATTLEGROUND_QUEUE_TP:
            return BATTLEGROUND_TP;
        case BATTLEGROUND_QUEUE_BFG:
            return BATTLEGROUND_BFG;
        case BATTLEGROUND_QUEUE_RB:
            return BATTLEGROUND_RB;
        case BATTLEGROUND_QUEUE_SEETHING_SHORE:
            return BATTLEGROUND_SEETHING_SHORE;
        case BATTLEGROUND_QUEUE_2v2:
        case BATTLEGROUND_QUEUE_3v3:
        case BATTLEGROUND_QUEUE_5v5:
            return BATTLEGROUND_AA;
        default:
            return BattlegroundTypeId(0);                   // used for unknown template (it exists and does nothing)
    }
}

ArenaType BattlegroundMgr::BGArenaType(BattlegroundQueueTypeId bgQueueTypeId)
{
    switch (bgQueueTypeId)
    {
        case BATTLEGROUND_QUEUE_2v2:
            return ARENA_TYPE_2v2;
        case BATTLEGROUND_QUEUE_3v3:
            return ARENA_TYPE_3v3;
        case BATTLEGROUND_QUEUE_5v5:
            return ARENA_TYPE_5v5;
        default:
            return ARENA_TYPE_NONE;
    }
}

void BattlegroundMgr::ToggleTesting()
{
    m_Testing = !m_Testing;
    sWorld->SendWorldText(m_Testing ? LANG_DEBUG_BG_ON : LANG_DEBUG_BG_OFF);
}

void BattlegroundMgr::ToggleArenaTesting()
{
    m_ArenaTesting = !m_ArenaTesting;
    sWorld->SendWorldText(m_ArenaTesting ? LANG_DEBUG_ARENA_ON : LANG_DEBUG_ARENA_OFF);
}

void BattlegroundMgr::SetHolidayWeekends(uint32 mask)
{
    // The current code supports battlegrounds up to BattlegroundTypeId(31)
    for (uint32 bgtype = 1; bgtype < MAX_BATTLEGROUND_TYPE_ID && bgtype < 32; ++bgtype)
        if (Battleground* bg = GetBattlegroundTemplate(BattlegroundTypeId(bgtype)))
            bg->SetHoliday((mask & (1 << bgtype)) != 0);
}

void BattlegroundMgr::ScheduleQueueUpdate(uint32 arenaMatchmakerRating, uint8 arenaType, BattlegroundQueueTypeId bgQueueTypeId, BattlegroundTypeId bgTypeId, BattlegroundBracketId bracket_id)
{
    //This method must be atomic, @todo add mutex
    //we will use only 1 number created of bgTypeId and bracket_id
    uint64 const scheduleId = ((uint64)arenaMatchmakerRating << 40) | ((uint64)arenaType << 32) | ((uint64)bgQueueTypeId << 24) | ((uint64)bgTypeId << 8) | (uint64)bracket_id;
    if (std::find(m_QueueUpdateScheduler.begin(), m_QueueUpdateScheduler.end(), scheduleId) == m_QueueUpdateScheduler.end())
        m_QueueUpdateScheduler.push_back(scheduleId);
}

uint32 BattlegroundMgr::GetMaxRatingDifference() const
{
    // this is for stupid people who can't use brain and set max rating difference to 0
    uint32 diff = sWorld->getIntConfig(CONFIG_ARENA_MAX_RATING_DIFFERENCE);
    if (diff == 0)
        diff = 5000;
    return diff;
}

uint32 BattlegroundMgr::GetRatingDiscardTimer() const
{
    return sWorld->getIntConfig(CONFIG_ARENA_RATING_DISCARD_TIMER);
}

uint32 BattlegroundMgr::GetPrematureFinishTime() const
{
    return sWorld->getIntConfig(CONFIG_BATTLEGROUND_PREMATURE_FINISH_TIMER);
}

void BattlegroundMgr::LoadBattleMastersEntry()
{
    uint32 oldMSTime = getMSTime();

    mBattleMastersMap.clear();                                  // need for reload case

    QueryResult result = WorldDatabase.Query("SELECT entry, bg_template FROM battlemaster_entry");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 battlemaster entries. DB table `battlemaster_entry` is empty!");
        return;
    }

    uint32 count = 0;

    do
    {
        ++count;

        Field* fields = result->Fetch();

        uint32 entry = fields[0].GetUInt32();
        if (CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(entry))
        {
            if ((cInfo->npcflag & UNIT_NPC_FLAG_BATTLEMASTER) == 0)
                TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) listed in `battlemaster_entry` is not a battlemaster.", entry);
        }
        else
        {
            TC_LOG_ERROR("sql.sql", "Creature (Entry: %u) listed in `battlemaster_entry` does not exist.", entry);
            continue;
        }

        uint32 bgTypeId  = fields[1].GetUInt32();
        if (!sBattlemasterListStore.LookupEntry(bgTypeId))
        {
            TC_LOG_ERROR("sql.sql", "Table `battlemaster_entry` contains entry %u for a non-existing battleground type %u, ignored.", entry, bgTypeId);
            continue;
        }

        mBattleMastersMap[entry] = BattlegroundTypeId(bgTypeId);
    }
    while (result->NextRow());

    CheckBattleMasters();

    TC_LOG_INFO("server.loading", ">> Loaded %u battlemaster entries in %u ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void BattlegroundMgr::CheckBattleMasters()
{
    CreatureTemplateContainer const* ctc = sObjectMgr->GetCreatureTemplates();
    for (CreatureTemplateContainer::const_iterator itr = ctc->begin(); itr != ctc->end(); ++itr)
    {
        if ((itr->second.npcflag & UNIT_NPC_FLAG_BATTLEMASTER) && mBattleMastersMap.find(itr->second.Entry) == mBattleMastersMap.end())
        {
            TC_LOG_ERROR("sql.sql", "Creature_Template Entry: %u has UNIT_NPC_FLAG_BATTLEMASTER, but no data in the `battlemaster_entry` table. Removing flag.", itr->second.Entry);
            const_cast<CreatureTemplate*>(&itr->second)->npcflag &= ~UNIT_NPC_FLAG_BATTLEMASTER;
        }
    }
}

HolidayIds BattlegroundMgr::BGTypeToWeekendHolidayId(BattlegroundTypeId bgTypeId)
{
    switch (bgTypeId)
    {
        case BATTLEGROUND_AV: return HOLIDAY_CALL_TO_ARMS_AV;
        case BATTLEGROUND_EY: return HOLIDAY_CALL_TO_ARMS_EY;
        case BATTLEGROUND_WS: return HOLIDAY_CALL_TO_ARMS_WS;
        case BATTLEGROUND_SA: return HOLIDAY_CALL_TO_ARMS_SA;
        case BATTLEGROUND_AB: return HOLIDAY_CALL_TO_ARMS_AB;
        case BATTLEGROUND_IC: return HOLIDAY_CALL_TO_ARMS_IC;
        case BATTLEGROUND_TP: return HOLIDAY_CALL_TO_ARMS_TP;
        case BATTLEGROUND_BFG: return HOLIDAY_CALL_TO_ARMS_BFG;
        default: return HOLIDAY_NONE;
    }
}

BattlegroundTypeId BattlegroundMgr::WeekendHolidayIdToBGType(HolidayIds holiday)
{
    switch (holiday)
    {
        case HOLIDAY_CALL_TO_ARMS_AV: return BATTLEGROUND_AV;
        case HOLIDAY_CALL_TO_ARMS_EY: return BATTLEGROUND_EY;
        case HOLIDAY_CALL_TO_ARMS_WS: return BATTLEGROUND_WS;
        case HOLIDAY_CALL_TO_ARMS_SA: return BATTLEGROUND_SA;
        case HOLIDAY_CALL_TO_ARMS_AB: return BATTLEGROUND_AB;
        case HOLIDAY_CALL_TO_ARMS_IC: return BATTLEGROUND_IC;
        case HOLIDAY_CALL_TO_ARMS_TP: return BATTLEGROUND_TP;
        case HOLIDAY_CALL_TO_ARMS_BFG: return BATTLEGROUND_BFG;
        default: return BATTLEGROUND_TYPE_NONE;
    }
}

bool BattlegroundMgr::IsBGWeekend(BattlegroundTypeId bgTypeId)
{
    return IsHolidayActive(BGTypeToWeekendHolidayId(bgTypeId));
}

BattlegroundTypeId BattlegroundMgr::GetRandomBG(BattlegroundTypeId bgTypeId)
{
    if (BattlegroundTemplate const* bgTemplate = GetBattlegroundTemplateByTypeId(bgTypeId))
    {
        std::vector<BattlegroundTypeId> ids;
        ids.reserve(16);
        std::vector<double> weights;
        weights.reserve(16);
        for (int32 mapId : bgTemplate->BattlemasterEntry->MapID)
        {
            if (mapId == -1)
                break;

            if (BattlegroundTemplate const* bg = GetBattlegroundTemplateByMapId(mapId))
            {
                ids.push_back(bg->Id);
                weights.push_back(bg->Weight);
            }
        }

        return *Trinity::Containers::SelectRandomWeightedContainerElement(ids, weights);
    }

    return BATTLEGROUND_TYPE_NONE;
}

BGFreeSlotQueueContainer& BattlegroundMgr::GetBGFreeSlotQueueStore(BattlegroundTypeId bgTypeId)
{
    return bgDataStore[bgTypeId].BGFreeSlotQueue;
}

void BattlegroundMgr::AddToBGFreeSlotQueue(BattlegroundTypeId bgTypeId, Battleground* bg)
{
    bgDataStore[bgTypeId].BGFreeSlotQueue.push_front(bg);
}

void BattlegroundMgr::RemoveFromBGFreeSlotQueue(BattlegroundTypeId bgTypeId, uint32 instanceId)
{
    BGFreeSlotQueueContainer& queues = bgDataStore[bgTypeId].BGFreeSlotQueue;
    for (BGFreeSlotQueueContainer::iterator itr = queues.begin(); itr != queues.end(); ++itr)
        if ((*itr)->GetInstanceId() == instanceId)
        {
            queues.erase(itr);
            return;
        }
}

void BattlegroundMgr::AddBattleground(Battleground* bg)
{
    if (bg)
        bgDataStore[bg->GetTypeId()].m_Battlegrounds[bg->GetInstanceId()] = bg;
}

void BattlegroundMgr::RemoveBattleground(BattlegroundTypeId bgTypeId, uint32 instanceId)
{
    bgDataStore[bgTypeId].m_Battlegrounds.erase(instanceId);
}
