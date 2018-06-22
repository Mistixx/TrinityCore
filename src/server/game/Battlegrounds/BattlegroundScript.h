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
#include "ZoneScript.h"
#include "SharedDefines.h"
#include "TaskScheduler.h"

class Battleground;
class BattlegroundMap;
enum BattlegroundStatus;
enum ScoreType : uint32;

namespace WorldPackets
{
    namespace Battleground
    {
        struct BattlegroundPlayerPosition;
    }
}

class CaptureTheFlagBattlegroundScript;

class TC_GAME_API BattlegroundScript : public ZoneScript
{
public:
    explicit BattlegroundScript(BattlegroundMap* map);
    virtual ~BattlegroundScript() = default;

    void UpdateWorldState(uint32 variable, uint32 value, bool hidden = false);

    void ToggleGameobjectSpawn(ObjectGuid guid, uint32 respawntime) const;

    GameObject* AddObject(uint32 entry, Position const& pos, QuaternionData const& rot, uint32 respawnTime = 0, GOState goState = GO_STATE_READY) const;
    Creature* AddCreature(uint32 entry, Position const& pos, uint32 respawntime = 0, Transport* transport = nullptr) const;
    Creature* AddSpiritGuide(Position const& pos, TeamId teamId = TEAM_NEUTRAL) const;

    virtual void BuildPlayerPositionData(std::vector<WorldPackets::Battleground::BattlegroundPlayerPosition>* /*positions*/) const { }

    void OnUpdate(uint32 diff);

    virtual void OnUpdateInProgress(uint32 /*diff*/) { }
    virtual void OnUpdateWaitJoin(uint32 /*diff*/) { }
    virtual void OnUpdateWaitLeave(uint32 /*diff*/) { }

    virtual void OnStart();
    virtual void OnEnd(Team /*winner*/) { Scheduler.CancelAll(); }

    virtual void OnSetup() { }

    // todo ? bool login/logout
    virtual void OnPlayerEnter(Player* /*player*/) { }
    virtual void OnPlayerLeave(Player* /*player*/) { }

    virtual void OnPlayerKillsUnit(Player* /*killer*/, Unit* /*victim*/) { }

    virtual void DoAction(uint32 /*action*/, ObjectGuid /*var*/) { }

    virtual WorldSafeLocsEntry const* GetGraveYardForPlayer(Player* /*player*/) { return nullptr; }

    virtual Team CalculatePrematureWinner();
    // Achievement criteria additional requirements check
    // NOTE: not use this if same can be checked existed requirement types from AchievementCriteriaRequirementType
    virtual bool CheckAchievementCriteriaMeet(uint32 /*criteria_id*/, Player const* /*source*/, Unit const* /*target*/ = nullptr, uint32 /*miscvalue1*/ = 0);

    bool OnGameObjectClicked(Player* player, GameObject* gameObject) override;
    virtual void OnPlayerDroppedFlag(Player* /*player*/) { }

    void OnGameObjectCreate(GameObject* /*gameObject*/) override { }
    void OnGameObjectRemove(GameObject* /*gameObject*/) override { }

    ObjectGuid GetGuidData(uint32 /*data*/) const override { return ObjectGuid::Empty; }
    void SetGuidData(uint32 /*data*/, ObjectGuid /*guid*/) override { }

    void OnQueryCountdownTimer(uint32 timerType) override;

    virtual void HandleAreaTrigger(Player* player, uint32 triggerId, bool entered);
    virtual void OnUpdatePlayerScore(ScoreType /*scoreType*/, Player* /*player*/) { }

    // After 1 minute
    virtual void OnBattlegroundStartingEvent2() { }
    // After 30 seconds
    virtual void OnBattlegroundStartingEvent3() { }

    virtual bool InitStartMessageIds(uint32& /*first*/, uint32& /*second*/, uint32& /*third*/, uint32& /*fourth*/) { return false; }

    virtual bool HandlePlayerUnderMap(Player* /*player*/) { return false; }

protected:
    TaskScheduler Scheduler;
    BattlegroundMap* Map;
    Battleground* battleground; // lowercase here, using Battleground in scheduler lambdas gives an error

private:
    static uint32 const EVENT_START_BATTLE = 35912;

#ifdef TRINITY_API_USE_DYNAMIC_LINKING
    // Strong reference to the associated script module
    std::shared_ptr<ModuleReference> module_reference;
#endif // #ifndef TRINITY_API_USE_DYNAMIC_LINKING
};

class TC_GAME_API ArenaScript : public BattlegroundScript
{
public:
    explicit ArenaScript(BattlegroundMap* map) : BattlegroundScript(map) { }

    void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates&) override;
};
