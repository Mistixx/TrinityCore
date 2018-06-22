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
#include "SpellAuras.h"

void AssaultStrategy::OnUpdateInProgress(uint32 diff)
{
    if (!_enabled)
        return;

    if (_assaultTimer <= diff)
    {
        _assaultTimer = ASSAULT_TIMER * 1000;
        ApplyAssault(GetAffectedPlayers());
    }
    else
        _assaultTimer -= diff;
}

void AssaultStrategy::ApplyAssault(std::vector<Player*> const& players)
{
    if (_stackCount >= MAX_STACKS)
        return;

    ++_stackCount;

    for (Player* player : players)
    {
        if (_stackCount == STACK_COUNT_TO_BRUTAL_ASSAULT)
            player->RemoveAurasDueToSpell(SPELL_FOCUSED_ASSAULT);

        AddPlayerToAssault(player);
    }
}

void AssaultStrategy::ResetAssault()
{
    DisableAssault();
    _stackCount = 0;
    std::vector<Player*> const players = GetAffectedPlayers();
    for (Player* player : players)
        RemovePlayerFromAssault(player);
}

void AssaultStrategy::AddAndEnablePlayerToAssault(Player* player)
{
    EnableAssault();
    AddPlayerToAssault(player);
}

void AssaultStrategy::AddPlayerToAssault(Player* player) const
{
    if (GetAssaultState() == ASSAULT_STATE_FOCUSED)
    {
        player->CastSpell(player, SPELL_FOCUSED_ASSAULT, true);
        if (Aura* aura = player->GetAura(SPELL_FOCUSED_ASSAULT))
            aura->SetStackAmount(_stackCount);
    }
    else if (GetAssaultState() == ASSAULT_STATE_BRUTAL)
    {
        player->CastSpell(player, SPELL_BRUTAL_ASSAULT, true);
        if (Aura* aura = player->GetAura(SPELL_BRUTAL_ASSAULT))
            aura->SetStackAmount(_stackCount);
    }
}

void AssaultStrategy::RemovePlayerFromAssault(Player* player)
{
    player->RemoveAurasDueToSpell(SPELL_BRUTAL_ASSAULT);
    player->RemoveAurasDueToSpell(SPELL_FOCUSED_ASSAULT);
}
