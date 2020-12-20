/*
* This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Spells/Scripts/SpellScript.h"
#include "Spells/SpellAuras.h"

enum
{
    SPELL_SHAMAN_ELEMENTAL_MASTERY = 16166,
};

struct ItemShamanT10Elemental2PBonus : public AuraScript
{
    SpellAuraProcResult OnProc(Aura* aura, ProcExecutionData& procData) const override
    {
        if (Unit* target = aura->GetTarget())
            if (target->IsPlayer())
                static_cast<Player*>(target)->ModifyCooldown(SPELL_SHAMAN_ELEMENTAL_MASTERY, -aura->GetAmount());
        return SPELL_AURA_PROC_OK;
    }
};

void LoadShamanScripts()
{
    RegisterAuraScript<ItemShamanT10Elemental2PBonus>("spell_item_shaman_t10_elemental_2p_bonus");
}