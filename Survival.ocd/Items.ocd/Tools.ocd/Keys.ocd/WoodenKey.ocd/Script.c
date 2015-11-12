public func Hit()
{
	Sound("WoodHit?", nil, 50);
}

public func IsKey()
{
	return true;
}

public func CanUnlock(string quality)
{
	if (quality == "wooden") return true;
	return false;
}

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
