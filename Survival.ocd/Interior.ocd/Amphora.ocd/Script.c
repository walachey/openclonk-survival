local Name = "$Name$";
local Description = "$Description$";

local Plane = 300;
local MaxHitpoints = 20000;

public func Construction()
{
	MakeDestroyable();
	this.MeshTransformation = Trans_Mul(Trans_Scale(RandomX(700, 1000), RandomX(700, 1000), RandomX(700, 1000)), Trans_Rotate(RandomX(-5, 5), 0, 1, 0));
	SetClrModulation(RGB(RandomX(200, 255), RandomX(200, 255), RandomX(200, 255)));
	return true;
}

public func IsProjectileTarget() { return true; }

public func Death()
{
	SoundAt("GlassBreak");
	
	var particles = 
	{
		Prototype = Particles_WoodChip(),
		R = 100, G = 50, B = 0,
		Attach = 0
	};
	
	CreateParticle("WoodChip", PV_Random(-3, 3), PV_Random(-5, 5), PV_Random(-7, 7), PV_Random(-20, 5), PV_Random(30, 120), particles, 200);
	
	RemoveObject();
}
