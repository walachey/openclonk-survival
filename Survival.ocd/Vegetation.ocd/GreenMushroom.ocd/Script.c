/*-- Initialization --*/

local is_smoking;
local smoke_particles;

public func Construction()
{
	StartGrowth(3);
	RootSurface();
	this.MeshTransformation = Trans_Rotate(RandomX(-90, 90), 0, 1, 0);
	AddTimer("CheckBlow", 200 + Random(200));
	return _inherited(...);
}

private func CheckBlow()
{
	if (Contained()) return;
	
	if (GetCon() >= 75 && !is_smoking)
	{
		is_smoking = true;
		smoke_particles = 
		{
			Prototype = Particles_Smoke(),
			Size = PV_Linear(PV_Random(2, 5), PV_Random(20, 30, 10)),
			R = 100, G = 255, B = 100,
			Alpha = PV_KeyFrames(0, 0, 0, 300, 128, 1000, 0)
		};
		AddTimer("RandomSmoking", 30 + Random(30));
	}
	if (GetCon() < 100) return;
	if (Random(5)) return;
	Blow();
}

private func RandomSmoking()
{
	if (Random(4)) return;
	CreateParticle("Smoke", 0, 0, PV_Random(-10, 10), PV_Random(-10, 10), PV_Random(30, 90), smoke_particles, 15);
}

private func Blow()
{
	// KO close Clonks.
	for (var obj in FindObjects(Find_Distance(60), Find_OCF(OCF_Alive), Find_Layer()))
	{
		obj->~DoDaze(100 + Random(300));
	}
	
	// Smoke a bit.
	var particles = 
	{
		Size = PV_Linear(PV_Random(2, 5), PV_Random(10, 20)),
		Prototype = Particles_Smoke(),
		Stretch = PV_Speed(1000, 1000),
		Rotation = PV_Direction(),
		DampingX = PV_Random(700, 900), DampingY = PV_Random(700, 900),
		R = 0, G = 255, B = 0,
		Alpha = PV_KeyFrames(0, 0, 0, 100, 128, 1000, 0)
	};
	CreateParticle("Smoke", 0, 0, PV_Random(-80, 80), PV_Random(-70, 10), PV_Random(20, 60), particles, 300);
	
	// And disintegrate the mushroom itself.
	var particles = 
	{
		Prototype = Particles_WoodChip(),
		Size = PV_Random(1, 3),
		Stretch = PV_Speed(1000, 1000),
		R = PV_Random(50, 100), B = PV_Random(50, 100), G = 150,
		Attach = 0,
		Phase = PV_Random(0, 4)
	};
	CreateParticle("WoodChip", PV_Random(-2, 2), PV_Random(-3, 3), PV_Random(-15, 15), PV_Random(-20, 0), PV_Random(30, 90), particles, 250);
	RemoveObject();
}

private func FxBlowUpEffectTimer(object target, effect fx, int time)
{
	if (fx.blow_up)
	{
		var livetime = time - 200;
		if (livetime > Random(100)) return FX_Execute_Kill;
		if (Random(BoundBy(livetime/2, 1, 10))) return FX_OK;
		CreateParticle("StarSpark", fx.x, fx.y, PV_Random(-1, 1), PV_Random(-1, 1), PV_Random(5, 20), fx.particles, 2);
		return FX_OK;
	}
	
	fx.blow_up = time > 20 + Random(20) || GBackSemiSolid(fx.x, fx.y);
	if (fx.blow_up)
	{
		fx.R = 0;
		fx.G = 0;
		CreateParticle("StarSpark", fx.x, fx.y, PV_Random(-30, 30), PV_Random(-30, 30), PV_Random(10, 20), fx.particles, 30);
		fx.particles.ForceY = PV_Gravity(100);
		fx.particles.DampingX = 500;
		fx.particles.DampingY = 500;
		fx.particles.Stretch = PV_KeyFrames(0, 0, 0, 100, 4, 1000, 0);
		fx.x = PV_Random(fx.x - 30, fx.x + 30);
		fx.y = PV_Random(fx.y - 30, fx.y + 30);
		return FX_OK;
	}
	CreateParticle("StarSpark", fx.x, fx.y, PV_Random(-10, 10), PV_Random(-5, 10), PV_Random(10, 20), fx.particles, 10);
	
	if (Random(2)) fx.x += RandomX(-1, 1);
	fx.y -= 1;
}

/*-- Eating --*/

public func ControlUse(object clonk)
{
	clonk->Eat(this);
	if (clonk)
		clonk->~DoDaze(300);
}

// Nutritional value depends on the completion of the mushroom.
public func NutritionalValue() { return GetCon() / 5; }
public func WaterValue() { return -GetCon() / 5; }

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;
local Placement = 4;
