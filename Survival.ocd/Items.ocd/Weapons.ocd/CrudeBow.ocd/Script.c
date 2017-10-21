#include Bow

local Name = "$Name$";
local Description = "$Description$";

local shooting_strength = 35;
local MaxWear = 100;

local QualitySettings = {
	Properties = {
		MaxWear = {Base = 100, FullBonus = 200, Name = "$Durability$"},
		shooting_strength = {Base = 35, FullBonus = 50, Name = "$Range$"},
	}
};

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(90,1,0,0), Trans_Rotate(90,0,0,1));
}

func Definition(def)
{
	//def.PictureTransformation = Trans_Mul(Trans_Translate(-4000,-2000,4000),Trans_Rotate(180,0,1,0),Trans_Rotate(-45,0,0,1));
}