local Name = "$Name$";
local Description = "$Description$";

local fuel;

public func IsContainer()
{
	return true;
}

public func RejectCollect(object obj)
{
	
}

public func Collection2(object obj)
{
	var fuel_amount = obj->~GetFuelAmount();
	
	if (fuel_amount)
	{
		this.fuel += fuel_amount;
		CheckFuel();
		obj->RemoveObject();
		return;
	}
	
	CheckFuel();
}

public func CheckFuel()
{
	var full = ContentsCount() > 0;
	var burning = GetEffect("IntBurning", this);
	var fueled = fuel > 5;
	
	if (fueled && !burning)
	{
		Sound("Inflame");
		AddEffect("IntBurning", this, 1, 2, this);
	}
	else if ((!fueled || !full) && burning)
	{
		RemoveEffect("IntBurning", this);
	}
}

public func RoastFood()
{

}

public func Definition()
{
	this.PictureTransformation=Trans_Mul(Trans_Scale(1500, 1500, 1500), Trans_Rotate(-45, 1, 1, 0));
}

public func Construction()
{
	fuel = 0;
	this.MeshTransformation = Trans_Rotate(RandomX(-90, 90), 0, 1, 0);
	return true;
}

/*-- Burning Effect --*/

private func FxIntBurningStart(object target, effect fx, int temporary)
{
	if (temporary)
		return 1;
	// Ensure the interval is always one frame.
	fx.Interval = 1;
	// Fire particle
	fx.flame = 
	{
		R = PV_KeyFrames(0, 0, 0, 200, 255, 800, 255, 1000, 255),
		G = PV_KeyFrames(0, 0, 0, 200, 210, 800, 70, 1000, 70),
		B = PV_KeyFrames(0, 0, 255, 200, 100, 800, 0, 1000, 0),
		
		Alpha = PV_KeyFrames(1000, 0, 0, 10, 255, 500, 255, 1000, 0),
		Size = PV_Linear(PV_Random(2, 3), PV_Random(4, 5)),
		Stretch = 1000,
		Phase = PV_Random(0, 4),
		Rotation = PV_Random(0, 359),
		DampingX = 900,
		DampingY = 1000,
		BlitMode = GFX_BLIT_Additive,
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		Attach = ATTACH_Front
	};
	fx.smoke = 
	{
		Prototype = Particles_Smoke(),
		Alpha = PV_KeyFrames(0, 0, 0, 300, 255, 1000, 0),
		Size = PV_Linear(PV_Random(2, 3), PV_Random(3, 5))
	};
	// Set the light range for this torch.
	SetLightRange(80, 60);
	SetLightColor(FIRE_LIGHT_COLOR);
	return 1;
}

private func FxIntBurningTimer (object target, effect fx, int time)
{
	if (time % 4 == 0)
	{
		fuel -= 1;
		if (time % 20 == 0) CheckFuel();
		if (time % (32 * 3) == 0) RoastFood();
	}
	// If the torched is attached or fixed it should emit some fire and smoke particles.
	CreateParticle("FireSharp", PV_Random(-1, 2), PV_Random(0, -3), PV_Random(-2, 2), PV_Random(-10, -5), 10 + Random(3), fx.flame, 8);
	// Smoke effects.
	CreateParticle("Smoke", PV_Random(-1, 2), PV_Random(-7, -9), PV_Random(-2, 2), PV_Random(-2, 2), 54 + Random(12), fx.smoke, 2);
	return 1;
}

protected func FxIntBurningStop(object target, proplist effect, int reason, bool temporary)
{
	if (temporary)
		return 1;
	// Remove the light from this torch.	
	SetLightRange(0);
	return 1;
}