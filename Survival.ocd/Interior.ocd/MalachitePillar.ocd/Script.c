#include AzuritePillar

local Name = "$Name$";
local Description = "$Description$";

public func Death()
{
	var particles = 
	{
		Prototype = Particles_WoodChip(),
		R = PV_Random(0, 20), G = PV_Random(0, 20), B = PV_Random(0, 20),
		Attach = 0,
		Phase = PV_Random(0, 4)
	};
	
	CreateParticle("MagicFire", PV_Random(-8, 8), PV_Random(-8, 8), PV_Random(-15, 15), PV_Random(-20, 5), PV_Random(30, 120), particles, 200);
	particles.G = 200;
	CreateParticle("SmokeDirty", PV_Random(-4, 4), PV_Random(-6, 6), PV_Random(-5, 5), PV_Random(-10, 0), PV_Random(30, 120), particles, 50);
	Sound("ProjectileHitLiving?", nil, nil, nil, nil, nil, -75);
	RemoveObject();
}

public func GetSpawnID()
{
	return GreenMushroom;
}
