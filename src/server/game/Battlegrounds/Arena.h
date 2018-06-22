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

#ifndef TRINITY_ARENA_H
#define TRINITY_ARENA_H

#include "ArenaScore.h"
#include "Battleground.h"

enum ArenaBroadcastTexts
{
    ARENA_TEXT_START_ONE_MINUTE             = 15740,
    ARENA_TEXT_START_THIRTY_SECONDS         = 15741,
    ARENA_TEXT_START_FIFTEEN_SECONDS        = 15739,
    ARENA_TEXT_START_BATTLE_HAS_BEGUN       = 15742,
};

enum ArenaSpellIds
{
    SPELL_ALLIANCE_GOLD_FLAG                = 32724,
    SPELL_ALLIANCE_GREEN_FLAG               = 32725,
    SPELL_HORDE_GOLD_FLAG                   = 35774,
    SPELL_HORDE_GREEN_FLAG                  = 35775,

    SPELL_LAST_MAN_STANDING                 = 26549  // Achievement Credit
};

enum ArenaWorldStates
{
    ARENA_WORLD_STATE_ALIVE_PLAYERS_GREEN   = 3600,
    ARENA_WORLD_STATE_ALIVE_PLAYERS_GOLD    = 3601
};

class TC_GAME_API Arena : public Battleground
{
    public:
        explicit Arena(BattlegroundTemplate const* battlegroundTemplate);
        // used for rated arena battles
        void SetArenaTeamIdForTeam(Team team, uint32 arenaTeamId) { _arenaTeamIds[GetTeamIndexByTeamId(team)] = arenaTeamId; }
        uint32 GetArenaTeamIdForTeam(Team team) const { return _arenaTeamIds[GetTeamIndexByTeamId(team)]; }
        uint32 GetArenaTeamIdByIndex(uint32 index) const { return _arenaTeamIds[index]; }
        void SetArenaMatchmakerRating(Team team, uint32 mmr) { _arenaTeamMMR[GetTeamIndexByTeamId(team)] = mmr; }
        uint32 GetArenaMatchmakerRating(Team team) const { return _arenaTeamMMR[GetTeamIndexByTeamId(team)]; }

        bool CanAwardArenaPoints() const { return GetMinLevel() >= BG_AWARD_ARENA_POINTS_MIN_LEVEL; }

        void StartBattleground() override;
        ArenaType GetArenaType() const override { return _arenaType; }
        void SetArenaType(ArenaType type) { _arenaType = type; } // TODO kill this

        uint8 GetMaxPlayersPerTeam() const override
        {
            switch (_arenaType)
            {
                case ARENA_TYPE_2v2:
                    return 2;
                case ARENA_TYPE_3v3:
                    return 3;
                case ARENA_TYPE_5v5:
                    return 5;
                default:
                    return Battleground::GetMaxPlayersPerTeam();
            }
        }
    protected:
        void AddPlayer(Player* player) override;
        void RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 /*team*/) override;

        void UpdateArenaWorldState();

        void HandleKillPlayer(Player* player, Player* killer) override;

        void BuildPvPLogDataPacket(WorldPackets::Battleground::PVPLogData& pvpLogData) const override;

    private:
        void RemovePlayerAtLeave(ObjectGuid guid, bool transport, bool sendPacket) override;
        void CheckWinConditions() override;
        void EndBattleground(Team winner) override;

        ArenaTeamScore _arenaTeamScores[BG_TEAMS_COUNT];

        uint32 _arenaTeamIds[BG_TEAMS_COUNT];
        uint32 _arenaTeamMMR[BG_TEAMS_COUNT];

        ArenaType _arenaType;
};

#endif // TRINITY_ARENA_H
