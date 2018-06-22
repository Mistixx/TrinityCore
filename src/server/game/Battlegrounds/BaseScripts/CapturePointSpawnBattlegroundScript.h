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

#include <unordered_map>

class TC_GAME_API ControlPointSpawnsBattlegroundScript : public virtual BattlegroundScript
{
public:
    explicit ControlPointSpawnsBattlegroundScript(BattlegroundMap* map) : BattlegroundScript(map) { }

    void SpawnCapturePointEntities(uint32 capturePointId, TeamId teamId);
    void DespawnCapturePointEntities(uint32 capturePointId);

    std::vector<ObjectGuid::LowType> const* GetSpawnedCreatures(uint32 capturePoint) const
    {
        auto const itr = _capturePointCreatureSpawns.find(capturePoint);
        if (itr != _capturePointCreatureSpawns.end())
            return &itr->second;

        return nullptr;
    }
    std::vector<ObjectGuid::LowType> const* GetSpawnedGameObjects(uint32 capturePoint) const
    {
        auto const itr = _capturePointGameObjectSpawns.find(capturePoint);
        if (itr != _capturePointGameObjectSpawns.end())
            return &itr->second;

        return nullptr;
    }
private:
    std::unordered_map<uint32, std::vector<ObjectGuid::LowType>> _capturePointCreatureSpawns;
    std::unordered_map<uint32, std::vector<ObjectGuid::LowType>> _capturePointGameObjectSpawns;
};
