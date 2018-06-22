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

#include "CapturePointSpawnBattlegroundScript.h"
#include "Battleground.h"
#include "BattlegroundMgr.h"

void ControlPointSpawnsBattlegroundScript::SpawnCapturePointEntities(uint32 capturePointId, TeamId teamId)
{
    auto const itr = battleground->GetTemplate()->CreatureCapturePointSpawns.find(capturePointId);
    if (itr != battleground->GetTemplate()->CreatureCapturePointSpawns.end())
    {
        for (ObjectGuid::LowType const spawnId : itr->second[teamId])
        {
            if (CreatureData const* data = sObjectMgr->GetCreatureData(spawnId))
            {
                sObjectMgr->AddCreatureToGrid(spawnId, data);
                Creature::CreateCreatureFromDB(spawnId, Map);
                _capturePointCreatureSpawns[capturePointId].push_back(spawnId);
            }
        }
    }

    auto const itr2 = battleground->GetTemplate()->GameObjectCapturePointSpawns.find(capturePointId);
    if (itr2 != battleground->GetTemplate()->GameObjectCapturePointSpawns.end())
    {
        for (ObjectGuid::LowType const spawnId : itr2->second[teamId])
        {
            if (GameObjectData const* data = sObjectMgr->GetGOData(spawnId))
            {
                sObjectMgr->AddGameobjectToGrid(spawnId, data);
                GameObject::CreateGameObjectFromDB(spawnId, Map);
                _capturePointGameObjectSpawns[capturePointId].push_back(spawnId);
            }
        }
    }
}

void ControlPointSpawnsBattlegroundScript::DespawnCapturePointEntities(uint32 capturePointId)
{
    auto const itr = _capturePointCreatureSpawns.find(capturePointId);
    if (itr != _capturePointCreatureSpawns.end())
    {
        for (auto const spawnId : _capturePointCreatureSpawns[capturePointId])
        {
            if (CreatureData const* data = sObjectMgr->GetCreatureData(spawnId))
            {
                sObjectMgr->RemoveCreatureFromGrid(spawnId, data);
                auto const bounds = Map->GetCreatureBySpawnIdStore().equal_range(spawnId);
                for (auto itr2 = bounds.first; itr2 != bounds.second;)
                {
                    Creature* creature = itr2->second;
                    ++itr2;
                    creature->AddObjectToRemoveList();
                }
            }
        }

        _capturePointCreatureSpawns[capturePointId].clear();
    }

    auto const itr2 = _capturePointGameObjectSpawns.find(capturePointId);
    if (itr2 != _capturePointGameObjectSpawns.end())
    {
        for (auto const spawnId : _capturePointGameObjectSpawns[capturePointId])
        {
            if (GameObjectData const* data = sObjectMgr->GetGOData(spawnId))
            {
                sObjectMgr->RemoveGameobjectFromGrid(spawnId, data);
                auto const bounds = Map->GetGameObjectBySpawnIdStore().equal_range(spawnId);
                for (auto itr3 = bounds.first; itr3 != bounds.second;)
                {
                    GameObject* gameObject = itr3->second;
                    ++itr3;
                    gameObject->AddObjectToRemoveList();
                }
            }
        }

        _capturePointGameObjectSpawns[capturePointId].clear();
    }
}
