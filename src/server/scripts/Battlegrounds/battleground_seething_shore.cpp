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
#include "BattlegroundMgr.h"
#include "ControlPointBattlegroundScript.h"
#include "DB2Stores.h"
#include "GameObject.h"
#include "GameObjectData.h"
#include "Log.h"
#include "Position.h"
#include "TransportMgr.h"

struct battleground_seething_shore : BattlegroundScript
{
    enum BroadcastTexts
    {
        TEXT_AZERITE_SPAWNING = 134097
    };

    enum Data
    {
        DATA_OCCUPIED_AZERITE_NODES,
        DATA_CREATURE_SLOT,
        DATA_CLEAR_NODE,
        DATA_CREATURE_SET_NODE_SPAWNED
    };

    enum AzeriteSlots
    {
        SLOT_TOWER,
        SLOT_RIDGE,
        SLOT_RUINS,
        SLOT_TIDE_POOLS,
        SLOT_CRASH_SITE,
        SLOT_BONFIRE,
        SLOT_WATERFALL,
        SLOT_TEMPLE,
        SLOT_OVERLOOK,
        SLOT_SHIPWREK,
        SLOT_TAR_PITS,
        SLOT_PLUNGS,
        MAX_SLOTS
    };

    enum Actions
    {
        ACTION_SET_AZERITE_SLOT_TOWER = SLOT_TOWER,
        ACTION_SET_AZERITE_SLOT_RIGDE = SLOT_RIDGE,
        ACTION_SET_AZERITE_SLOT_RUINS = SLOT_RUINS,
        ACTION_SET_AZERITE_SLOT_TIDE_POOLS = SLOT_TIDE_POOLS,
        ACTION_SET_AZERITE_SLOT_CRASH_SITE = SLOT_CRASH_SITE,
        ACTION_SET_AZERITE_SLOT_BONFIRE = SLOT_BONFIRE,
        ACTION_SET_AZERITE_SLOT_WATERFALL = SLOT_WATERFALL,
        ACTION_SET_AZERITE_SLOT_TEMPLE = SLOT_TEMPLE,
        ACTION_SET_AZERITE_SLOT_OVERLOOK = SLOT_OVERLOOK,
        ACTION_SET_AZERITE_SLOT_SHIPWREK = SLOT_SHIPWREK,
        ACTION_SET_AZERITE_SLOT_TAR_PITS = SLOT_TAR_PITS,
        ACTION_SET_AZERITE_SLOT_PLUNGS = SLOT_PLUNGS,
        ACTION_ADD_AZERITE_NODE
    };

    enum Sounds
    {
        SOUND_AZERITE_TAKEN = 8213
    };

    enum Spells
    {
        SPELL_ACTIVATE_AZERITE = 248688,
        SPELL_AZERITE_GEYSER = 248668,
        SPELL_SUMMON_AZERITE_CAPTURE_NODE_1 = 262749,
        SPELL_SUMMON_AZERITE_CAPTURE_NODE_2 = 248674,
        SPELL_AZERITE_KNOCK_BACK = 262385,
        SPELL_EARTHQUAKE_CAMERA_SHAKE = 248719,
        SPELL_AZERITE_CAPTURED_HORDE_COSMETIC_FX = 262512,
        SPELL_AZERITE_CAPTURED_ALLIANCE_COSMETIC_FX = 262508
    };

    enum Creatures
    {
        NPC_CONTROLLER = 125269, // casts Activate Azerite
        NPC_AZERITE_FISSURE = 125253
    };

    enum WorldStates
    {
        WORLD_STATE_ALLIANCE_AZERITE = 13845,
        WORLD_STATE_HORDE_AZERITE = 13844,
        WORLD_STATE_MAX_SCORE = 13846,
        WORLD_STATE_UNK_13856 = 13856
    };

    static uint32 constexpr SUMMON_AZERITE_SPELLS[] = { SPELL_SUMMON_AZERITE_CAPTURE_NODE_1, SPELL_SUMMON_AZERITE_CAPTURE_NODE_2 };

    explicit battleground_seething_shore(BattlegroundMap* map) : BattlegroundScript(map), _nodesSpawnedOrSpawning(0)
    {
        Worldstates[WORLD_STATE_ALLIANCE_AZERITE] = 0;
        Worldstates[WORLD_STATE_HORDE_AZERITE] = 0;
        Worldstates[WORLD_STATE_MAX_SCORE] = battleground->GetTemplate()->MaxPoints;
    }

    void OnSetup() override
    {
        AddCreature(NPC_CONTROLLER, { 1399.684f, 2766.317f, 3.485144f, 0.0f });

        sTransportMgr->CreateInstanceTransports(Map);
        std::pair<AzeriteSlots, Position> azeritePositions[12]
        {
            { SLOT_TOWER, { 1461.358f, 2823.238f, 31.80409f, 0.0f } },
            { SLOT_RIDGE, { 1243.155f, 2721.465f, 11.96428f, 0.0f } },
            { SLOT_RUINS, { 1346.759f, 2920.885f, 33.31765f, 0.0f } },
            { SLOT_TIDE_POOLS, { 1390.245f, 2570.769f, 7.360876f, 0.0f } },
            { SLOT_CRASH_SITE, { 1113.924f, 2886.993f, 38.53647f, 0.0f } },
            { SLOT_BONFIRE, { 1361.733f, 2643.267f, 4.608177f, 0.0f } },
            { SLOT_WATERFALL, { 1257.406f, 2882.731f, 27.66519f, 0.0f } },
            { SLOT_TEMPLE, { 1454.085f, 2598.380f, 15.35741f, 0.0f } },
            { SLOT_OVERLOOK, { 1126.649f, 2781.239f, 31.00365f, 0.0f } },
            { SLOT_SHIPWREK, { 1259.161f, 2571.423f, 8.653152f, 0.0f } },
            { SLOT_TAR_PITS, { 1441.059f, 2700.151f, 9.876933f, 0.0f } },
            { SLOT_PLUNGS, { 1339.556f, 2785.681f, 2.559283f, 0.0f } }
        };

        for (auto const& posPair : azeritePositions)
        {
            if (Creature* creature = AddCreature(NPC_AZERITE_FISSURE, posPair.second))
            {
                _azeriteFissureSlots[posPair.first] = creature->GetGUID();
                // Set Slot
                creature->AI()->SetData(DATA_CREATURE_SLOT, posPair.first);
            }
        }
    }

    void OnBattlegroundStartingEvent3() override
    {
        Scheduler.Schedule(2s, [this](TaskContext context)
        {
            if (_nodesSpawnedOrSpawning < MAX_AZERITE_FISSURES)
            {
                if (Creature* controller = Map->GetCreature(_controllerGuid))
                    controller->CastSpell(nullptr, SPELL_ACTIVATE_AZERITE, true);

                battleground->SendBroadcastText(TEXT_AZERITE_SPAWNING, CHAT_MSG_BG_SYSTEM_NEUTRAL);
                battleground->CastSpellOnTeam(SPELL_EARTHQUAKE_CAMERA_SHAKE, ALLIANCE);
                battleground->CastSpellOnTeam(SPELL_EARTHQUAKE_CAMERA_SHAKE, HORDE);
                context.Repeat(100ms);
            }
            else
                context.Repeat(2s);
        });
    }

    bool OnCapturePointClick(Player* player, GameObject* gameObject)
    {
        gameObject->SendCustomAnim(1);
        battleground->AddPoints(player->GetTeam(), 100);
        if (player->GetTeam() == ALLIANCE)
        {
            battleground->SendBroadcastText(gameObject->GetGOInfo()->capturePoint.AssaultBroadcastAlliance, CHAT_MSG_BG_SYSTEM_ALLIANCE, player);
            battleground->PlaySoundToAll(SOUND_AZERITE_TAKEN, gameObject->GetGUID());
            UpdateWorldState(WORLD_STATE_ALLIANCE_AZERITE, battleground->GetTeamScore(TEAM_ALLIANCE));
            player->CastSpell(player, SPELL_AZERITE_CAPTURED_ALLIANCE_COSMETIC_FX, true);
        }
        else
        {
            battleground->SendBroadcastText(gameObject->GetGOInfo()->capturePoint.AssaultBroadcastHorde, CHAT_MSG_BG_SYSTEM_HORDE, player);
            battleground->PlaySoundToAll(SOUND_AZERITE_TAKEN, gameObject->GetGUID());
            UpdateWorldState(WORLD_STATE_HORDE_AZERITE, battleground->GetTeamScore(TEAM_HORDE));
            player->CastSpell(player, SPELL_AZERITE_CAPTURED_HORDE_COSMETIC_FX, true);
        }

        //ServerToClient: SMSG_UPDATE_WORLD_STATE(0x278C) Length : 9 ConnIdx : 0
        //VariableID : 13856 - Value : 100
        //Hidden : False

        gameObject->SetAnimKitId(2560, false);
        for (uint8 i = 0; i < MAX_SLOTS; ++i)
        {
            if (_occupiedSlots[i] == gameObject->GetGUID())
            {
                if (Creature* creature = Map->GetCreature(_azeriteFissureSlots[i]))
                    creature->AI()->SetData(DATA_CREATURE_SET_NODE_SPAWNED, false);
                _occupiedSlots[i] = ObjectGuid::Empty;
            }
        }

        --_nodesSpawnedOrSpawning;
        Scheduler.Schedule(2s, [gameObject](TaskContext)
        {
            gameObject->DespawnAndRemoveFromWorld(true);
        });

        player->RewardHonor(nullptr, 0, 10);

        if (battleground->GetTeamScore(player->GetTeamId()) == battleground->GetTemplate()->MaxPoints)
            battleground->EndBattleground(player->GetTeam());

        return true;
    }

    bool OnGameObjectClicked(Player* player, GameObject* gameobject) override
    {
        if (!BattlegroundScript::OnGameObjectClicked(player, gameobject))
            return false;

        if (gameobject->GetGoType() == GAMEOBJECT_TYPE_CAPTURE_POINT)
            return OnCapturePointClick(player, gameobject);

        return false;
    }

    void OnCreatureCreate(Creature* creature) override
    {
        BattlegroundScript::OnCreatureCreate(creature);
        if (creature->GetEntry() == NPC_CONTROLLER)
            _controllerGuid = creature->GetGUID();
    }

    uint32 GetData(uint32 data) const override
    {
        switch (data)
        {
            case DATA_OCCUPIED_AZERITE_NODES:
                return _nodesSpawnedOrSpawning;
            default:
                return 0;
        }
    }

    void DoAction(uint32 action, ObjectGuid guid) override
    {
        switch (action)
        {
            case ACTION_ADD_AZERITE_NODE:
                ++_nodesSpawnedOrSpawning;
                break;
            case ACTION_SET_AZERITE_SLOT_TOWER:
            case ACTION_SET_AZERITE_SLOT_RIGDE:
            case ACTION_SET_AZERITE_SLOT_RUINS:
            case ACTION_SET_AZERITE_SLOT_TIDE_POOLS:
            case ACTION_SET_AZERITE_SLOT_CRASH_SITE:
            case ACTION_SET_AZERITE_SLOT_BONFIRE:
            case ACTION_SET_AZERITE_SLOT_WATERFALL:
            case ACTION_SET_AZERITE_SLOT_TEMPLE:
            case ACTION_SET_AZERITE_SLOT_OVERLOOK:
            case ACTION_SET_AZERITE_SLOT_SHIPWREK:
            case ACTION_SET_AZERITE_SLOT_TAR_PITS:
            case ACTION_SET_AZERITE_SLOT_PLUNGS:
                _occupiedSlots[AzeriteSlots(action)] = guid;
                break;
            default:
                break;
        }
    }

    static uint8 const MAX_AZERITE_FISSURES = 3;

private:
    ObjectGuid _controllerGuid;
    ObjectGuid _occupiedSlots[MAX_SLOTS];
    ObjectGuid _azeriteFissureSlots[MAX_SLOTS];
    uint8 _nodesSpawnedOrSpawning;
};

struct npc_bg_seething_shore_azerite_fissure : ScriptedAI
{
    explicit npc_bg_seething_shore_azerite_fissure(Creature* creature) : ScriptedAI(creature),
        _slot(battleground_seething_shore::AzeriteSlots::MAX_SLOTS), _nodeSpawned(false) { }

    void JustSummonedGameobject(GameObject* gameobject) override
    {
        if (BattlegroundScript* bgScript = me->GetBattlegroundScript())
            bgScript->DoAction(_slot, gameobject->GetGUID());
    }

    void SetData(uint32 data, uint32 value) override
    {
        if (data == battleground_seething_shore::Data::DATA_CREATURE_SLOT)
            _slot = battleground_seething_shore::AzeriteSlots(value);
        else if (data == battleground_seething_shore::DATA_CREATURE_SET_NODE_SPAWNED)
            _nodeSpawned = value;
    }

    uint32 GetData(uint32 data) const override
    {
        if (data == battleground_seething_shore::DATA_CREATURE_SET_NODE_SPAWNED)
            return _nodeSpawned;

        return 0;
    }

private:
    battleground_seething_shore::AzeriteSlots _slot;
    bool _nodeSpawned;
};

class spell_bg_seething_shore_activate_azerite : public SpellScript
{
    PrepareSpellScript(spell_bg_seething_shore_activate_azerite);

    bool Validate(SpellInfo const* /*spellInfo*/) override
    {
        return ValidateSpellInfo(
        {
            battleground_seething_shore::Spells::SPELL_AZERITE_GEYSER,
            battleground_seething_shore::Spells::SPELL_ACTIVATE_AZERITE
        });
    }

    void FilterTargets(std::list<WorldObject*>& targets)
    {
        if (BattlegroundScript* bgScript = GetCaster()->GetBattlegroundScript())
        {
            auto itr = targets.begin();
            while (itr != targets.end())
            {
                Creature* creature = (*itr)->ToCreature();
                if (!creature || !creature->GetBattlegroundScript() || !creature->GetAI() || creature->AI()->GetData(battleground_seething_shore::Data::DATA_CREATURE_SET_NODE_SPAWNED))
                {
                    targets.erase(itr++);
                    continue;
                }

                ++itr;
            }

            if (!bgScript->GetData(battleground_seething_shore::Data::DATA_OCCUPIED_AZERITE_NODES) && targets.size() > battleground_seething_shore::MAX_AZERITE_FISSURES)
                Trinity::Containers::RandomResize(targets, battleground_seething_shore::MAX_AZERITE_FISSURES);
            else if (targets.size() > 1)
                Trinity::Containers::RandomResize(targets, 1);
        }
        else
            targets.clear();
    }

    void HandleDummy(SpellEffIndex /*effIndex*/)
    {
        GetHitCreature()->CastSpell(GetHitUnit(), battleground_seething_shore::Spells::SPELL_AZERITE_GEYSER, true);
        GetHitCreature()->GetBattlegroundScript()->DoAction(battleground_seething_shore::Actions::ACTION_ADD_AZERITE_NODE, GetHitCreature()->GetGUID());
        GetHitCreature()->AI()->SetData(battleground_seething_shore::Data::DATA_CREATURE_SET_NODE_SPAWNED, true);
    }

    void Register() override
    {
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_bg_seething_shore_activate_azerite::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
        OnEffectLaunchTarget += SpellEffectFn(spell_bg_seething_shore_activate_azerite::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
    }
};

class aura_bg_seething_shore_azerite_geyser : public AuraScript
{
    PrepareAuraScript(aura_bg_seething_shore_azerite_geyser);

    void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
    {
        if (Unit* unitOwner = GetUnitOwner())
        {
            unitOwner->CastSpell(unitOwner, Trinity::Containers::SelectRandomContainerElement(battleground_seething_shore::SUMMON_AZERITE_SPELLS), true);
            unitOwner->CastSpell(unitOwner, battleground_seething_shore::Spells::SPELL_AZERITE_KNOCK_BACK, true);
            unitOwner->GetMotionMaster()->MoveTargetedHome();
        }
    }

    void Register() override
    {
        OnEffectRemove += AuraEffectRemoveFn(aura_bg_seething_shore_azerite_geyser::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
    }
};

void AddSC_battleground_seething_shore()
{
    RegisterBattlegroundMapScript(battleground_seething_shore, 1803);
    RegisterCreatureAI(npc_bg_seething_shore_azerite_fissure);
    RegisterSpellScript(spell_bg_seething_shore_activate_azerite);
    RegisterAuraScript(aura_bg_seething_shore_azerite_geyser);
}
