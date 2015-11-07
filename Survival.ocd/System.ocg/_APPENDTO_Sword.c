#appendto Sword

local MaxWear = 100;

private func WeaponDamage(a, int damage)
{
	DoWear(damage / 1000);
	return inherited(a, damage, ...);
}
