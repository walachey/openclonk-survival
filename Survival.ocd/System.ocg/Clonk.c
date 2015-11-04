#appendto Clonk

public func Construction()
{
	this.BleedDamage = 0;
	this.FractureDamage = 0;
	AddTimer("DegenerateDamage", 10);
	return _inherited(...);
}

private func DegenerateDamage()
{
	this.BleedDamage = Max(this.BleedDamage - 10000, 0);
	this.FractureDamage = Max(this.FractureDamage - 10000, 0);
}

public func OnDamage(object by, int exact_damage, int type, int by_player)
{
	var bleed_strength = 0;
	var fracture_strength = 0;
	if (type == DMG_Blunt)
	{
		fracture_strength = 100;
		bleed_strength = 25;
	}
	else if (type == DMG_Slash)
	{
		fracture_strength = 0;
		bleed_strength = 100;
	}
	else if (type == DMG_Pierce)
	{
		fracture_strength = 50;
		bleed_strength = 50;
	}
	
	if (bleed_strength > 0) CheckBleed(exact_damage * 100 / bleed_strength);
	if (fracture_strength > 0) CheckFracture(exact_damage * 100 / fracture_strength);
}

private func CheckBleed(int dmg)
{
	this.BleedDamage += dmg;
	// todo: draw random number and check if > dmg
}

private func CheckFracture(int dmg)
{
	this.FractureDamage += dmg;
}
