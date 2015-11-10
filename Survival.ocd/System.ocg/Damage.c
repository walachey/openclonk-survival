
// "Old" damage types (just with the new prefix) are the same as predefined damage types.
static const DMG_Fire = 35;
static const DMG_Blast = 33;
static const DMG_Corrosion = 38;

// New damage types. 
static const DMG_Blunt = 100;
static const DMG_Slash = 101;
static const DMG_Pierce = 102;
static const DMG_True = 103;
static const DMG_Cold = 104;

global func GetDamageResistance(int type, int exact_damage)
{
	return 0;
}

global func DealDamage(object to, int exact_damage, int type, int by_player)
{
	if (!to) return;
	
	by_player = by_player ?? GetController();
	
	// Calculate armor.
	var resistance = to->GetDamageResistance(type, exact_damage);
	
	if (resistance != nil)
	{
		if (resistance > 0)
		{
			exact_damage = exact_damage * 100 / (100 + resistance);
		}
		else if (resistance < 0)
		{
			exact_damage = (2 * exact_damage) - (exact_damage * 100 / (100 - resistance)); 
		}
		if (exact_damage == 0) return false;
	}
	
	if (this)
	{
		to.LastDamagingObject = this->GetName();
	}
	
	to->~OnDamage(this, exact_damage, type, by_player);
	if (to) to->~CatchBlow(-exact_damage/1000, this);
	if (to) to->DoEnergy(-exact_damage, true, type, by_player, nil, nil, nil, true);
}

global func DoEnergy(a, b, c, d, e, f, g, overwrite)
{
	if (!overwrite) // Not called by DealDamage?
	{
		this.LastDamagingObject = nil;
	}
	return inherited(a, b, c, d, e, f, g, overwrite, ...);
}


global func WeaponDamage(object target, int damage, int damage_type, bool exact_damage)
{
	if (!exact_damage) damage *= 1000;

	return DealDamage(target, damage, damage_type);
}

global func MakeDestroyable_OnDamage(object by, int damage, int type, int by_player)
{
	this.Hitpoints = BoundBy(this.Hitpoints - damage, 0, this.MaxHitpoints);
	if (this.Hitpoints <= 0) this->~Death(by_player);
	if (this && this._OnDamage) this->_OnDamage(by, damage, type, by_player);
}

global func MakeDestroyable()
{
	this.Hitpoints = this.MaxHitpoints;
	this._OnDamage = this.OnDamage;
	this.OnDamage = Global.MakeDestroyable_OnDamage;
}