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

#include "ScriptMgr.h"
#include "Battleground.h"
#include "GameObject.h"
#include "Map.h"
#include "MotionMaster.h"
#include "ObjectAccessor.h"
#include "PassiveAI.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "SpellInfo.h"
#include "SpellScript.h"
#include "Vehicle.h"

enum MiscValues
{
    NPC_HIGH_COMMANDER_HALFORD_WYRMBANE = 34924, // Alliance Boss
    NPC_OVERLORD_AGMAR = 34922, // Horde Boss
    NPC_KEEP_CANNON = 34944,
    NPC_DEMOLISHER = 34775,
    NPC_SIEGE_ENGINE_H = 35069,
    NPC_SIEGE_ENGINE_A = 34776,
    NPC_GLAIVE_THROWER_A = 34802,
    NPC_GLAIVE_THROWER_H = 35273,
    NPC_CATAPULT = 34793,
    NPC_HORDE_GUNSHIP_CAPTAIN = 35003,
    NPC_ALLIANCE_GUNSHIP_CAPTAIN = 34960,

    SPELL_OIL_REFINERY = 68719,
    SPELL_QUARRY = 68720,
    SPELL_PARACHUTE = 66656,
    SPELL_SLOW_FALL = 12438,
    SPELL_DESTROYED_VEHICLE_ACHIEVEMENT = 68357,
    SPELL_BACK_DOOR_JOB_ACHIEVEMENT = 68502,
    SPELL_DRIVING_CREDIT_DEMOLISHER = 68365,
    SPELL_DRIVING_CREDIT_GLAIVE = 68363,
    SPELL_DRIVING_CREDIT_SIEGE = 68364,
    SPELL_DRIVING_CREDIT_CATAPULT = 68362,
    SPELL_SIMPLE_TELEPORT = 12980,
    SPELL_TELEPORT_VISUAL_ONLY = 51347,
    SPELL_PARACHUTE_IC = 66657,
    SPELL_LAUNCH_NO_FALLING_DAMAGE = 66251,

    ACTION_GUNSHIP_READY = 1,
    BG_IC_NPC_GUNSHIP_CAPTAIN_1 = 1 // TODO
};

// TO-DO: This should be done with SmartAI, but yet it does not correctly support vehicles's AIs.
//        Even adding ReactState Passive we still have issues using SmartAI.

class npc_four_car_garage : public CreatureScript
{
    public:
        npc_four_car_garage() : CreatureScript("npc_four_car_garage") { }

        struct npc_four_car_garageAI : public NullCreatureAI
        {
            npc_four_car_garageAI(Creature* creature) : NullCreatureAI(creature) { }

            void PassengerBoarded(Unit* who, int8 /*seatId*/, bool apply) override
            {
                if (apply)
                {
                    uint32 spellId = 0;

                    switch (me->GetEntry())
                    {
                        case NPC_DEMOLISHER:
                            spellId = SPELL_DRIVING_CREDIT_DEMOLISHER;
                            break;
                        case NPC_GLAIVE_THROWER_A:
                        case NPC_GLAIVE_THROWER_H:
                            spellId = SPELL_DRIVING_CREDIT_GLAIVE;
                            break;
                        case NPC_SIEGE_ENGINE_H:
                        case NPC_SIEGE_ENGINE_A:
                            spellId = SPELL_DRIVING_CREDIT_SIEGE;
                            break;
                        case NPC_CATAPULT:
                            spellId = SPELL_DRIVING_CREDIT_CATAPULT;
                            break;
                        default:
                            return;
                    }

                    me->CastSpell(who, spellId, true);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_four_car_garageAI(creature);
        }
};

enum Events
{
    EVENT_TALK  = 1,
    EVENT_DESPAWN
};

enum Texts
{
    SAY_ONBOARD = 0
};

class npc_ioc_gunship_captain : public CreatureScript
{
    public:
        npc_ioc_gunship_captain() : CreatureScript("npc_ioc_gunship_captain") { }

        struct npc_ioc_gunship_captainAI : public ScriptedAI
        {
            npc_ioc_gunship_captainAI(Creature* creature) : ScriptedAI(creature) { }

            void DoAction(int32 action) override
            {
                if (action == ACTION_GUNSHIP_READY)
                {
                    DoCast(me, SPELL_SIMPLE_TELEPORT);
                    _events.ScheduleEvent(EVENT_TALK, 3000);
                }
            }

            void UpdateAI(uint32 diff) override
            {
                _events.Update(diff);
                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_TALK:
                            _events.ScheduleEvent(EVENT_DESPAWN, 1000);
                            Talk(SAY_ONBOARD);
                            DoCast(me, SPELL_TELEPORT_VISUAL_ONLY);
                            break;
                        case EVENT_DESPAWN:
                            me->RemoveFromWorld();
                            break;
                        default:
                            break;
                    }
                }
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_ioc_gunship_captainAI(creature);
        }
};

class spell_ioc_gunship_portal : public SpellScriptLoader
{
    public:
        spell_ioc_gunship_portal() : SpellScriptLoader("spell_ioc_gunship_portal") { }

        class spell_ioc_gunship_portal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ioc_gunship_portal_SpellScript);

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleScript(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();
                /*
                 * HACK: GetWorldLocation() returns real position and not transportposition.
                 * ServertoClient: SMSG_MOVE_TELEPORT (0x0B39)
                 * counter: 45
                 * Tranpsort Guid: Full: xxxx Type: MOTransport Low: xxx
                 * Transport Position X: 0 Y: 0 Z: 0 O: 0
                 * Position: X: 7.305609 Y: -0.095246 Z: 34.51022 O: 0
                 */
                caster->TeleportTo(GetHitCreature()->GetWorldLocation(), TELE_TO_NOT_LEAVE_TRANSPORT);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_ioc_gunship_portal_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ioc_gunship_portal_SpellScript();
        }
};

class spell_ioc_parachute_ic : public SpellScriptLoader
{
    public:
        spell_ioc_parachute_ic() : SpellScriptLoader("spell_ioc_parachute_ic") { }

        class spell_ioc_parachute_ic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ioc_parachute_ic_AuraScript);

            void HandleTriggerSpell(AuraEffect const* /*aurEff*/)
            {
                if (Player* target = GetTarget()->ToPlayer())
                    if (target->m_movementInfo.GetFallTime() > 2000 && !target->GetTransport())
                        target->CastSpell(target, SPELL_PARACHUTE_IC, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_ioc_parachute_ic_AuraScript::HandleTriggerSpell, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_ioc_parachute_ic_AuraScript();
        }
};

class StartLaunchEvent : public BasicEvent
{
    public:
        StartLaunchEvent(Position const& pos, ObjectGuid const& guid) : _pos(pos), _guid(guid)
        {
        }

        bool Execute(uint64 /*time*/, uint32 /*diff*/) override
        {
            Player* player = ObjectAccessor::FindPlayer(_guid);
            if (!player || !player->GetVehicle())
                return true;

            player->AddAura(SPELL_LAUNCH_NO_FALLING_DAMAGE, player); // prevents falling damage
            float speedZ = 10.0f;
            float dist = player->GetExactDist2d(&_pos);

            player->ExitVehicle();
            player->GetMotionMaster()->MoveJump(_pos, dist, speedZ, EVENT_JUMP, true);
            return true;
        }

    private:
        Position _pos;
        ObjectGuid _guid;
};

class spell_ioc_launch : public SpellScriptLoader
{
    public:
        spell_ioc_launch() : SpellScriptLoader("spell_ioc_launch") { }

        class spell_ioc_launch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ioc_launch_SpellScript);

            void Launch()
            {
                if (!GetCaster()->ToCreature() || !GetExplTargetDest())
                    return;

                GetCaster()->ToCreature()->m_Events.AddEvent(new StartLaunchEvent(*GetExplTargetDest(), GetHitPlayer()->GetGUID()), GetCaster()->ToCreature()->m_Events.CalculateTime(2500));
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_ioc_launch_SpellScript::Launch);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_ioc_launch_SpellScript();
        }
};

enum SeaforiumBombSpells
{
    SPELL_SEAFORIUM_BLAST       = 66676,
    SPELL_HUGE_SEAFORIUM_BLAST  = 66672,
    SPELL_A_BOMB_INABLE_CREDIT  = 68366,
    SPELL_A_BOMB_INATION_CREDIT = 68367
};

class spell_ioc_seaforium_blast_credit : public SpellScriptLoader
{
    public:
        spell_ioc_seaforium_blast_credit() : SpellScriptLoader("spell_ioc_seaforium_blast_credit") { }

        class spell_ioc_seaforium_blast_credit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ioc_seaforium_blast_credit_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                return ValidateSpellInfo({ SPELL_A_BOMB_INABLE_CREDIT, SPELL_A_BOMB_INATION_CREDIT });
            }

            void HandleAchievementCredit(SpellEffIndex /*effIndex*/)
            {
                uint32 _creditSpell = 0;
                Unit* caster = GetOriginalCaster();
                if (!caster)
                    return;

                if (GetSpellInfo()->Id == SPELL_SEAFORIUM_BLAST)
                    _creditSpell = SPELL_A_BOMB_INABLE_CREDIT;
                else if (GetSpellInfo()->Id == SPELL_HUGE_SEAFORIUM_BLAST)
                    _creditSpell = SPELL_A_BOMB_INATION_CREDIT;

                if (GetHitGObj() && GetHitGObj()->IsDestructibleBuilding())
                    caster->CastSpell(caster, _creditSpell, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_ioc_seaforium_blast_credit_SpellScript::HandleAchievementCredit, EFFECT_1, SPELL_EFFECT_GAMEOBJECT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ioc_seaforium_blast_credit_SpellScript();
        }
};

void AddSC_isle_of_conquest()
{
    new npc_four_car_garage();
    new npc_ioc_gunship_captain();
    new spell_ioc_gunship_portal();
    new spell_ioc_parachute_ic();
    new spell_ioc_launch();
    new spell_ioc_seaforium_blast_credit();
}
