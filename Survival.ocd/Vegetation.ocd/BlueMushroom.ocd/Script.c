/*-- Initialization --*/

local is_glowing;

public func Construction()
{
	StartGrowth(3);
	RootSurface();
	this.MeshTransformation = Trans_Rotate(RandomX(0, 359), 0, 1, 0);
	AddTimer("CheckBlow", 200 + Random(200));
	return _inherited(...);
}

private func CheckBlow()
{
	if (Contained()) return;
	
	if (GetCon() >= 75 && !is_glowing)
	{
		is_glowing = true;
		var glow = 
		{
			R = 50, G = 50, B = 100,
			Prototype = Particles_Flash(),
			Size = PV_Random(20, 30),
			Alpha = PV_Step(1, 0, nil, 200),
			Attach = ATTACH_Front | ATTACH_MoveRelative
		};
		CreateParticle("Flash", 0, 0, 0, 0, 0, glow, 1);
	}
	if (GetCon() < 100) return;
	if (Random(5)) return;
	Blow();
}

private func Blow()
{
	var fx = AddEffect("BlowUpEffect", nil, 1, 1, nil, GetID());
	fx.x = GetX();
	fx.y = GetY();
	fx.particles = 
	{
		Prototype = Particles_Glimmer(),
		R = PV_Linear(255, 0), B = 255, G = PV_Linear(255, 0),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-10, 10), PV_Random(-20, 0), PV_Random(20, 60), fx.particles, 50);
	
	// And disintegrate the mushroom itself.
	var particles = 
	{
		Prototype = Particles_WoodChip(),
		Size = PV_Random(0, 1),
		Stretch = PV_Speed(1000, 1000),
		R = PV_Random(150, 170), G = PV_Random(150, 170), B = 255,
		Attach = 0,
	};
	CreateParticle("SmokeDirty", PV_Random(-2, 2), PV_Random(-3, 3), PV_Random(-5, 5), PV_Random(-10, 0), PV_Random(30, 90), particles, 150);
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
}

// Nutritional value depends on the completion of the mushroom.
public func NutritionalValue() { return GetCon() / 10; }
public func WaterValue() { return GetCon() / 20; }

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;
local Placement = 4;
