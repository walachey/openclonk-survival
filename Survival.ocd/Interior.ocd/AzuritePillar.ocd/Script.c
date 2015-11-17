local Name = "$Name$";
local Description = "$Description$";

local Plane = 300;

public func Construction()
{
	this.Hits = 0;
	this.MeshTransformation = Trans_Mul(Trans_Scale(RandomX(900, 1000), RandomX(900, 1000), RandomX(900, 1000)), Trans_Rotate(RandomX(-90, 90), 0, 1, 0));
	SetClrModulation(RGB(RandomX(200, 255), RandomX(200, 255), RandomX(200, 255)));
	return true;
}

public func Death()
{
	var particles = 
	{
		Prototype = Particles_WoodChip(),
		R = PV_Random(0, 20), G = PV_Random(0, 20), B = PV_Random(0, 20),
		Attach = 0
	};
	
	CreateParticle("WoodChip", PV_Random(-5, 5), PV_Random(-8, 8), PV_Random(-10, 10), PV_Random(-20, 5), PV_Random(30, 120), particles, 200);
	particles.B = 200;
	CreateParticle("WoodChip", PV_Random(-3, 3), PV_Random(-6, 6), PV_Random(-5, 5), PV_Random(-10, 0), PV_Random(30, 120), particles, 50);
	Sound("ProjectileHitLiving?", nil, nil, nil, nil, nil, -75);
	RemoveObject();
}

public func OnHitByPickaxe(object pickaxe)
{
	Sound("GlassHit*", nil, 25, nil, nil, nil, -50 - Random(20));
	
	if (Random(2)) return;
	this.Hits += 1;
	if (this.Hits == 6)
		Sound("GlassBreak", nil, nil, nil, nil, nil, -90);
	else if (this.Hits > 6)
	{
		return Death();
	}
}

public func CanBeHitByPickaxe()
{
	return true;
}
