#include Pickaxe

local Name = "$Name$";
local Description = "$Description$";

local MaxWear = 50;
local PickaxeStrength = 20;

local QualitySettings = {
	Properties = {
		MaxWear = {Base = 50, FullBonus = 100, Name = "$Durability$"},
		PickaxeStrength = {Base = 20, FullBonus = 25, Name = "$PickaxeStrength$"},
	}
};

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(90, 1, 0, 0), Trans_Rotate(90, 0, 0, 1));
}
