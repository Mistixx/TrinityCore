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

#ifndef __BATTLEGROUND_H
#define __BATTLEGROUND_H

#include "DBCEnums.h"
#include "ObjectGuid.h"
#include "SharedDefines.h"
#include <map>

enum ScoreType : unsigned;
class Arena;
class Battleground;
class BattlegroundMap;
class BattlegroundScript;
class Creature;
class GameObject;
class Group;
class Player;
class Transport;
class Unit;
class WorldObject;
class WorldPacket;
struct BattlegroundScore;
struct BattlegroundTemplate;
struct PVPDifficultyEntry;
struct WorldSafeLocsEntry;

namespace WorldPackets
{
    namespace Battleground
    {
        class PVPLogData;
        struct BattlegroundPlayerPosition;
    }

    namespace WorldState
    {
        class InitWorldStates;
    }
}

enum BattlegroundCriteriaId
{
    BG_CRITERIA_CHECK_RESILIENT_VICTORY,
    BG_CRITERIA_CHECK_SAVE_THE_DAY,
    BG_CRITERIA_CHECK_EVERYTHING_COUNTS,
    BG_CRITERIA_CHECK_AV_PERFECTION,
    BG_CRITERIA_CHECK_DEFENSE_OF_THE_ANCIENTS,
    BG_CRITERIA_CHECK_NOT_EVEN_A_SCRATCH,
};

enum BattlegroundBroadcastTexts
{
    BG_TEXT_ALLIANCE_WINS       = 10633,
    BG_TEXT_HORDE_WINS          = 10634,

    BG_TEXT_START_TWO_MINUTES   = 18193,
    BG_TEXT_START_ONE_MINUTE    = 18194,
    BG_TEXT_START_HALF_MINUTE   = 18195,
    BG_TEXT_BATTLE_HAS_BEGUN    = 18196,
};

enum BattlegroundSounds
{
    SOUND_HORDE_WINS                = 8454,
    SOUND_ALLIANCE_WINS             = 8455,
    SOUND_BG_START                  = 3439,
    SOUND_BG_START_L70ETC           = 11803
};

enum BattlegroundQuests
{
    SPELL_WS_QUEST_REWARD           = 43483,
    SPELL_AB_QUEST_REWARD           = 43484,
    SPELL_AV_QUEST_REWARD           = 43475,
    SPELL_AV_QUEST_KILLED_BOSS      = 23658,
    SPELL_EY_QUEST_REWARD           = 43477,
    SPELL_SA_QUEST_REWARD           = 61213,
    SPELL_AB_QUEST_REWARD_4_BASES   = 24061,
    SPELL_AB_QUEST_REWARD_5_BASES   = 24064
};

enum BattlegroundMarks
{
    SPELL_WS_MARK_LOSER             = 24950,
    SPELL_WS_MARK_WINNER            = 24951,
    SPELL_AB_MARK_LOSER             = 24952,
    SPELL_AB_MARK_WINNER            = 24953,
    SPELL_AV_MARK_LOSER             = 24954,
    SPELL_AV_MARK_WINNER            = 24955,
    SPELL_SA_MARK_WINNER            = 61160,
    SPELL_SA_MARK_LOSER             = 61159,
    ITEM_AV_MARK_OF_HONOR           = 20560,
    ITEM_WS_MARK_OF_HONOR           = 20558,
    ITEM_AB_MARK_OF_HONOR           = 20559,
    ITEM_EY_MARK_OF_HONOR           = 29024,
    ITEM_SA_MARK_OF_HONOR           = 42425
};

enum BattlegroundMarksCount
{
    ITEM_WINNER_COUNT               = 3,
    ITEM_LOSER_COUNT                = 1
};

enum BattlegroundCreatures
{
    BG_CREATURE_ENTRY_A_SPIRITGUIDE      = 13116,           // alliance
    BG_CREATURE_ENTRY_H_SPIRITGUIDE      = 13117            // horde
};

enum BattlegroundSpells
{
    SPELL_WAITING_FOR_RESURRECT     = 2584,                 // Waiting to Resurrect
    SPELL_SPIRIT_HEAL_CHANNEL       = 22011,                // Spirit Heal Channel
    VISUAL_SPIRIT_HEAL_CHANNEL      = 3060,
    SPELL_SPIRIT_HEAL               = 22012,                // Spirit Heal
    SPELL_RESURRECTION_VISUAL       = 24171,                // Resurrection Impact Visual
    SPELL_ARENA_PREPARATION         = 32727,                // use this one, 32728 not correct
    SPELL_PREPARATION               = 44521,                // Preparation
    SPELL_SPIRIT_HEAL_MANA          = 44535,                // Spirit Heal
    SPELL_RECENTLY_DROPPED_FLAG     = 42792,                // Recently Dropped Flag
    SPELL_AURA_PLAYER_INACTIVE      = 43681,                // Inactive
    SPELL_HONORABLE_DEFENDER_25Y    = 68652,                // +50% honor when standing at a capture point that you control, 25yards radius (added in 3.2)
    SPELL_HONORABLE_DEFENDER_60Y    = 66157                 // +50% honor when standing at a capture point that you control, 60yards radius (added in 3.2), probably for 40+ player battlegrounds
};

enum BattlegroundTimeIntervals
{
    CHECK_PLAYER_POSITION_INVERVAL  = 1000,                 // ms
    RESURRECTION_INTERVAL           = 30000,                // ms
    //REMIND_INTERVAL                 = 10000,                // ms
    INVITATION_REMIND_TIME          = 20000,                // ms
    INVITE_ACCEPT_WAIT_TIME         = 90000,                // ms
    TIME_AUTOCLOSE_BATTLEGROUND     = 120000,               // ms
    MAX_OFFLINE_TIME                = 300,                  // secs
    RESPAWN_ONE_DAY                 = 86400,                // secs
    RESPAWN_IMMEDIATELY             = 0,                    // secs
    BUFF_RESPAWN_TIME               = 90,                   // secs
    BATTLEGROUND_COUNTDOWN_MAX      = 120,                  // secs
    ARENA_COUNTDOWN_MAX             = 60,                   // secs
    PLAYER_POSITION_UPDATE_INTERVAL = 5                     // secs
};

enum BattlegroundStartTimeIntervals
{
    BG_START_DELAY_2M               = 120000,               // ms (2 minutes)
    BG_START_DELAY_1M               = 60000,                // ms (1 minute)
    BG_START_DELAY_30S              = 30000,                // ms (30 seconds)
    BG_START_DELAY_15S              = 15000,                // ms (15 seconds) Used only in arena
    BG_START_DELAY_NONE             = 0                     // ms
};

enum BattlegroundStatus
{
    STATUS_NONE         = 0,                                // first status, should mean bg is not instance
    STATUS_WAIT_QUEUE   = 1,                                // means bg is empty and waiting for queue
    STATUS_WAIT_JOIN    = 2,                                // this means, that BG has already started and it is waiting for more players
    STATUS_IN_PROGRESS  = 3,                                // means bg is running
    STATUS_WAIT_LEAVE   = 4                                 // means some faction has won BG and it is ending
};

struct BattlegroundPlayer
{
    time_t  OfflineRemoveTime;                              // for tracking and removing offline players from queue after 5 minutes
    Team    Team;                                           // Player's team
    int32   ActiveSpec;                                     // Player's active spec
};

struct BattlegroundObjectInfo
{
    BattlegroundObjectInfo() : object(nullptr), timer(0), spellid(0) { }

    GameObject  *object;
    int32       timer;
    uint32      spellid;
};

enum ArenaType
{
    ARENA_TYPE_NONE = 0,
    ARENA_TYPE_2v2  = 2,
    ARENA_TYPE_3v3  = 3,
    ARENA_TYPE_5v5  = 5
};

enum BattlegroundStartingEvents
{
    BG_STARTING_EVENT_NONE  = 0x00,
    BG_STARTING_EVENT_1     = 0x01,
    BG_STARTING_EVENT_2     = 0x02,
    BG_STARTING_EVENT_3     = 0x04,
    BG_STARTING_EVENT_4     = 0x08
};

enum BattlegroundStartingEventsIds
{
    BG_STARTING_EVENT_FIRST     = 0,
    BG_STARTING_EVENT_SECOND    = 1,
    BG_STARTING_EVENT_THIRD     = 2,
    BG_STARTING_EVENT_FOURTH    = 3
};

enum BGHonorMode
{
    BG_NORMAL = 0,
    BG_HOLIDAY,
    BG_HONOR_MODE_NUM
};

enum BattlegroudMisc
{
    BG_MAX_BUFFS                    =   3,

    BG_STARTING_EVENT_COUNT         =   4,

    BG_AWARD_ARENA_POINTS_MIN_LEVEL =  71,

    ARENA_TIMELIMIT_POINTS_LOSS     = -16
};

enum BattlegroundPlayerPositionConstants
{
    PLAYER_POSITION_ICON_NONE           = 0,
    PLAYER_POSITION_ICON_HORDE_FLAG     = 1,
    PLAYER_POSITION_ICON_ALLIANCE_FLAG  = 2,

    PLAYER_POSITION_ARENA_SLOT_NONE     = 1,
    PLAYER_POSITION_ARENA_SLOT_1        = 2,
    PLAYER_POSITION_ARENA_SLOT_2        = 3,
    PLAYER_POSITION_ARENA_SLOT_3        = 4,
    PLAYER_POSITION_ARENA_SLOT_4        = 5,
    PLAYER_POSITION_ARENA_SLOT_5        = 6
};

/**
 * This class is used to:
 * 1. Add player to battleground
 * 2. Remove player from battleground
 * 3. some certain cases, same for all battlegrounds
 * 4. It has properties same for all battlegrounds
 */
class TC_GAME_API Battleground
{
public:
    explicit Battleground(BattlegroundTemplate const* battlegroundTemplate, PVPDifficultyEntry const* bracket, uint32 instanceId, uint32 clientInstanceId, BattlegroundTypeId originalTypeId, bool rated);
    explicit Battleground(BattlegroundTemplate const* battlegroundTemplate);
    virtual ~Battleground();

    void Update(uint32 diff);

    bool SetupBattleground();

    /* achievement req. */
    // Move to script ?
    void StartCriteriaTimer(CriteriaTimedTypes type, uint32 entry) const;

    /* Battleground */
    // Get methods:
    char const* GetName() const;
    uint64 GetQueueId() const { return _queueId; }
    BattlegroundTypeId GetTypeId(bool random = false) const { return random ? _randomTypeId : _typeId; }
    BattlegroundBracketId GetBracketId() const;
    uint32 GetInstanceId() const        { return _instanceId; }
    BattlegroundStatus GetStatus() const { return _status; }
    uint32 GetClientInstanceId() const  { return _clientInstanceId; }
    uint32 GetElapsedTime() const       { return _startTime; }
    uint32 GetRemainingTime() const     { return _endTime; }
    uint32 GetLastResurrectTime() const { return _lastResurrectTime; }
    uint32 GetMaxPlayers() const { return GetMaxPlayersPerTeam() * BG_TEAMS_COUNT; }
    uint32 GetMinPlayers() const { return GetMinPlayersPerTeam() * BG_TEAMS_COUNT; }

    uint8 GetMinLevel() const;
    uint8 GetMaxLevel() const;

    virtual uint8 GetMaxPlayersPerTeam() const;
    uint8 GetMinPlayersPerTeam() const;

    int32 GetStartDelayTime() const         { return _startDelayTime; }
    virtual ArenaType GetArenaType() const  { return ARENA_TYPE_NONE; }
    BattlegroundTeamId GetWinner() const    { return _winnerTeamId; }
    uint32 GetScriptId() const;
    uint32 GetBonusHonorFromKill(uint32 kills) const;

    bool IsRandom() const { return _typeId == BATTLEGROUND_RB; }
    bool IsArena() const;
    bool IsBattleground() const { return !IsArena(); }
    bool IsRated() const { return _isRated; }

    void AddToBGFreeSlotQueue();                        //this queue will be useful when more battlegrounds instances will be available
    void RemoveFromBGFreeSlotQueue();                   //this method could delete whole BG instance, if another free is available

    void DecreaseInvitedCount(Team team)      { (team == ALLIANCE) ? --_invitedAlliance : --_invitedHorde; }
    void IncreaseInvitedCount(Team team)      { (team == ALLIANCE) ? ++_invitedAlliance : ++_invitedHorde; }

    uint32 GetInvitedCount(Team team) const   { return (team == ALLIANCE) ? _invitedAlliance : _invitedHorde; }
    bool HasFreeSlots() const;
    uint32 GetFreeSlotsForTeam(Team team) const;

    BattlegroundTemplate const* GetTemplate() const { return _battlegroundTemplate; }

    typedef std::map<ObjectGuid, BattlegroundPlayer> BattlegroundPlayerMap;
    BattlegroundPlayerMap const& GetPlayers() const { return Players; }
    uint32 GetPlayersSize() const { return uint32(Players.size()); }

    typedef std::map<ObjectGuid, BattlegroundScore*> BattlegroundScoreMap;
    uint32 GetPlayerScoresSize() const { return uint32(PlayerScores.size()); }

    uint32 GetReviveQueueSize() const { return uint32(ReviveQueue.size()); }

    void AddPlayerToResurrectQueue(ObjectGuid creatureGuid, ObjectGuid playerGuid);
    void RemovePlayerFromResurrectQueue(ObjectGuid playerGuid);

    /// Relocate all players in ReviveQueue to the closest graveyard
    void RelocateDeadPlayers(ObjectGuid spiritGuideGuid);

    virtual void StartBattleground();

    // Map pointers
    void SetBgMap(BattlegroundMap* map) { _map = map; }
    BattlegroundMap* GetBgMap() const;
    BattlegroundMap* FindBgMap() const { return _map; }
    inline uint32 GetMapId() const;

    // Packet Transfer
    void SendPacketToTeam(Team team, WorldPacket const* packet, Player* except = nullptr) const;
    void SendPacketToAll(WorldPacket const* packet) const;

    void SendChatMessage(Creature* source, uint8 textId, WorldObject* target = nullptr);
    void SendBroadcastText(uint32 id, ChatMsg msgType, WorldObject const* target = nullptr);

    template<class Do>
    void BroadcastWorker(Do& _do);

    void PlaySoundToTeam(uint32 soundId, Team team) const;
    void PlaySoundToAll(uint32 soundId, ObjectGuid sourceGuid = ObjectGuid::Empty) const;
    void CastSpellOnTeam(uint32 spellId, Team team);
    void RemoveAuraOnTeam(uint32 spellId, Team team);
    void RewardHonorToTeam(uint32 honor, Team team);
    void RewardReputationToTeam(uint32 factionId, uint32 reputation, Team team);
    void UpdateWorldState(uint32 variable, uint32 value, bool hidden = false) const;

    void SendMessageToAll(uint32 entry, ChatMsg msgType, Player const* source = nullptr);
    void PSendMessageToAll(uint32 entry, ChatMsg msgType, Player const* source, ...);

    virtual void BuildPvPLogDataPacket(WorldPackets::Battleground::PVPLogData& pvpLogData) const;

    // End Battleground
    void BlockMovement(Player* player) const;
    virtual void EndBattleground(Team winner);

    // Raid Group
    Group* GetBgRaid(Team team) const { return team == ALLIANCE ? _bgRaids[TEAM_ALLIANCE] : _bgRaids[TEAM_HORDE]; }
    void SetBgRaid(Team team, Group* bgRaid);

    bool UpdatePlayerScore(Player* player, ScoreType type, uint32 value, bool doAddHonor = true);

    static TeamId GetTeamIndexByTeamId(Team team) { return team == ALLIANCE ? TEAM_ALLIANCE : TEAM_HORDE; }
    uint32 GetPlayersCountByTeam(Team team) const { return _playersCount[GetTeamIndexByTeamId(team)]; }
    uint32 GetAlivePlayersCountByTeam(Team team) const;   // used in arenas to correctly handle death in spirit of redemption / last stand etc. (killer = killed) cases
    void UpdatePlayersCountByTeam(Team team, bool remove)
    {
        if (remove)
            --_playersCount[GetTeamIndexByTeamId(team)];
        else
            ++_playersCount[GetTeamIndexByTeamId(team)];
    }

    virtual void CheckWinConditions() { }

    // must be implemented in BG subclass if need AND call base class generic code
    virtual void HandleKillPlayer(Player* victim, Player* killer);
    virtual void HandleKillUnit(Creature* victim, Player* killer);

    // Battleground events
    void EventPlayerLoggedIn(Player* player);
    void EventPlayerLoggedOut(Player* player);

    // Death related
    WorldSafeLocsEntry const* GetExploitTeleportLocation(Team team) const;
    void TeleportPlayerToExploitLocation(Player* player) const;

    virtual void AddPlayer(Player* player);

    void AddOrSetPlayerToCorrectBgGroup(Player* player, Team team);

    virtual void RemovePlayerAtLeave(ObjectGuid guid, bool transport, bool sendPacket); // can be extended in in BG subclass

    void HandleTriggerBuff(Player* player, uint32 areaTriggerId) const;
    void SetHoliday(bool is_holiday);

    bool ToBeDeleted() const { return _setDeleteThis; }
    void SetDeleteThis() { _setDeleteThis = true; }

    // because BattleGrounds with different types and same level range has different m_BracketId
    uint8 GetUniqueBracketId() const;
    static void RewardXPAtKill(Player* killer, Player* victim);

    // Teams
    // since arenas can be AvA or Hvh, we have to get the "temporary" team of a player
    Team GetPlayerTeam(ObjectGuid guid) const;
    static Team GetOtherTeam(Team team);
    bool IsPlayerInBattleground(ObjectGuid guid) const;

    // Scores
    // team
    void AddPoints(Team team, uint32 points = 1) { TeamScores[GetTeamIndexByTeamId(team)] += points; SendBattlegroundPoints(); }
    void SetPoints(Team team, uint32 points = 0) { TeamScores[GetTeamIndexByTeamId(team)] = points; SendBattlegroundPoints(); }
    void RemovePoints(Team team, uint32 points = 1) { TeamScores[GetTeamIndexByTeamId(team)] -= points; SendBattlegroundPoints(); }
    uint32 GetTeamScore(TeamId teamId) const;
    // player
    BattlegroundScoreMap PlayerScores; // Player scores

    // Arena
    Arena* ToArena()
    {
        if (IsArena())
            return reinterpret_cast<Arena*>(this);

        return nullptr;
    }
    Arena const* ToArena() const
    {
        if (IsArena())
            return reinterpret_cast<Arena const*>(this);

        return nullptr;
    }

    // Status
    void SetStatus(BattlegroundStatus status) { _status = status; }

    inline BattlegroundScript* GetBattlegroundScript() const;

    void SendBattlegroundPoints() const;

protected:
    // this method is called, when BG cannot spawn its own spirit guide, or something is wrong, It correctly ends Battleground
    void EndNow();
    void PlayerAddedToBGCheckIfBGIsRunning(Player* player);

    Player* GetPlayer(ObjectGuid guid, bool offlineRemove, const char* context) const;
    Player* GetPlayer(BattlegroundPlayerMap::iterator itr, const char* context) const { return GetPlayer(itr->first, itr->second.OfflineRemoveTime != 0, context); }
    Player* GetPlayer(BattlegroundPlayerMap::const_iterator itr, const char* context) const { return GetPlayer(itr->first, itr->second.OfflineRemoveTime != 0, context); }
    Player* GetPlayerForTeam(Team team, BattlegroundPlayerMap::const_iterator itr, const char* context) const;

    /* Pre- and post-update hooks */

    void ProcessOfflineQueue();
    void ProcessResurrect(uint32 diff);
    void ProcessProgress(uint32 diff);
    void ProcessLeave(uint32 diff);
    void ProcessJoin(uint32 diff);
    void CheckSafePositions(uint32 diff);
    void ProcessPlayerPositionBroadcast(uint32 diff);

    virtual void RemovePlayer(Player* /*player*/, ObjectGuid /*guid*/, uint32 /*team*/) { }

    // Player lists, those need to be accessible by inherited classes
    BattlegroundPlayerMap Players;
    // Spirit Guide guid + Player list GUIDS
    std::map<ObjectGuid, GuidVector> ReviveQueue;

    // these are important variables used for starting messages
    uint8 Events;
    BattlegroundStartTimeIntervals  StartDelayTimes[BG_STARTING_EVENT_COUNT];
    // this must be filled in constructors!
    uint32 StartMessageIds[BG_STARTING_EVENT_COUNT];

    BGHonorMode HonorMode;
    int32 TeamScores[BG_TEAMS_COUNT];

private:
    // Battleground
    BattlegroundTypeId const _typeId;
    BattlegroundTypeId const _randomTypeId; // If we queued for Random Battleground, this is the battlegroundTypeID we got in
    uint32 const _instanceId; // Battleground Instance's GUID!
    uint32 const _clientInstanceId; // the instance-id which is sent to the client and without any other internal use
    BattlegroundStatus _status;

    /// TIMERS
    uint32 _startTime;
    uint32 _countdownTimer;
    uint32 _resetStatTimer;
    uint32 _validStartPositionTimer;
    int32 _endTime;                                    // it is set to 120000 when bg is ending and it decreases itself
    uint32 _lastResurrectTime;
    bool   _prematureCountDown;
    uint32 _prematureCountDownTimer;
    uint32 _lastPlayerPositionBroadcast;
    int32  _startDelayTime;
    ///

    bool   _inBGFreeSlotQueue;                         // used to make sure that BG is only once inserted into the BattlegroundMgr.BGFreeSlotQueue[bgTypeId] deque
    bool   _setDeleteThis;                             // used for safe deletion of the bg after end / all players leave

    BattlegroundTeamId _winnerTeamId;

    bool const _isRated; // is this battle rated?
    uint64 const _queueId;

    // Player containers
    GuidVector _resurrectQueue;                        // Player GUID
    GuidDeque _offlineQueue;                           // Player GUID

    // Invited counters are useful for player invitation to BG - do not allow, if BG is started to one faction to have 2 more players than another faction
    // Invited counters will be changed only when removing already invited player from queue, removing player from battleground and inviting player to BG
    // Invited players counters
    uint32 _invitedAlliance;
    uint32 _invitedHorde;

    // Raid Group
    Group* _bgRaids[BG_TEAMS_COUNT];                   // 0 - alliance, 1 - horde

    // Players count by team
    uint32 _playersCount[BG_TEAMS_COUNT];

    BattlegroundTemplate const* _battlegroundTemplate;
    PVPDifficultyEntry const* _pvpDifficultyEntry;
    BattlegroundMap* _map;
};
#endif
