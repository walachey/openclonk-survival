#appendto Sword

local MaxWear = 100;
local WeaponStrength = 9;

local QualitySettings = {
	Properties = {
		MaxWear = {Base = 100, FullBonus = 100, Name = "$Durability$"},
		WeaponStrength = {Base = 9, FullBonus = 5, Name = "$Damage$"},
	}
};

func SwordDamage(int shield)
{
	return 1000 * this.WeaponStrength;
}

private func WeaponDamage(a, int damage)
{
	DoWear(damage / 1000);
	return inherited(a, damage, ...);
}
