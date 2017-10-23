public func Hit()
{
	Sound("LightMetalHit?", nil, 50, nil, nil, nil, 100);
}

public func IsKey()
{
	return true;
}

public func CanUnlock(string quality)
{
	if (quality == "wooden") return true;
	if (quality == "silver") return true;
	return false;
}

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Components = [[Metal, 2]];
