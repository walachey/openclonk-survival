#include Library_Stackable

public func MaxStackCount() { return 50; }

protected func Hit()
{
	Sound("GeneralHit?");
}

public func IsMillIngredient() { return true; }

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
