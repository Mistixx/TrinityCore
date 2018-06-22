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

// This is where scripts' loading functions should be declared:
void AddSC_battleground_alterac_valley();
void AddSC_battleground_arathi_bassin();
void AddSC_battleground_battle_for_gilneas();
void AddSC_battleground_eye_of_the_storm();
void AddSC_battleground_isle_of_conquest();
void AddSC_battleground_seething_shore();
void AddSC_battleground_twin_peaks();
void AddSC_battleground_warsong_gulch();

// The name of this function should match:
// void Add${NameOfDirectory}Scripts()
void AddBattlegroundsScripts()
{
    AddSC_battleground_alterac_valley();
    AddSC_battleground_arathi_bassin();
    AddSC_battleground_battle_for_gilneas();
    AddSC_battleground_eye_of_the_storm();
    AddSC_battleground_isle_of_conquest();
    AddSC_battleground_seething_shore();
    AddSC_battleground_twin_peaks();
    AddSC_battleground_warsong_gulch();
}
