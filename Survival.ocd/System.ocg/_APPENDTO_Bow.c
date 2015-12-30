#appendto Bow

local shooting_strength = 50;
local MaxWear = 100;

local QualitySettings = {
	Properties = {
		MaxWear = {Base = 100, FullBonus = 100, Name = "$Durability$"},
		shooting_strength = {Base = 50, FullBonus = 100, Name = "$Range$"},
	}
};

public func FinishedAiming(...)
{
	DoWear(5);
	return inherited(...);
}