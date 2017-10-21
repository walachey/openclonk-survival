local Name = "$Name$";
local Description = "$Description$";

local DamageEffect = new Effect
{
	Damage = func (int dmg)
	{
		if(dmg > 0) return dmg;
		if (dmg < -5 || Random(2))
			this.Target->Wobble();
		this.Target->Splatter(Abs(dmg));
		return dmg;
	}
};

public func Construction()
{
	CreateEffect(DamageEffect, 1, 0);
	AddTimer(this.Activity, 10);
	
	SetAction("Walk");
	this.MeshTransformation = Trans_Rotate(Random(360), 0, 1, 0);
	
	SetObjectBlitMode(GFX_BLIT_Mod2 | GFX_BLIT_Additive);
	SetClrModulation(RGB(0, 0, 255));
	
	this.particles = new Particles_Material(GetClrModulation())
	{
		Hit = PC_Bounce(),
		Size = PV_Linear(5, 0),
		BlitMode = GFX_BLIT_Additive
	};
	this.particles_bright = new this.particles
	{
		R = 255, G = 255, B = 255,
		Size = PV_Linear(2, 0),
		Stretch = PV_Speed(4000, 2000),
	};
	return true;
}

public func Destruction()
{

}

public func Death()
{
	this.particles.Stretch = PV_Speed(1000);
	this.particles_bright.Stretch = PV_Speed(1000);
	this.particles.ForceY = PV_Gravity(500);
	this.particles.DampingX = 950;
	this.particles.DampingY = 950;
	CreateParticle("BlueSpark", PV_Random(-7, 7), PV_Random(-5, 5),
					PV_Random(-40, 40), PV_Random(-40, 40),
					PV_Random(10, 30), this.particles, 220);
	CreateParticle("BlueSpark", PV_Random(-7, 7), PV_Random(-5, 5),
					PV_Random(-30, 30), PV_Random(-30, 30),
					PV_Random(10, 30), this.particles_bright, 220);
	Sound("ChippieChirp*", false, 50, nil, nil, nil, -200);
	RemoveObject();
}


private func Activity()
{

}

public func Wobble()
{
	if (this.wobble_animation) return;
	var len = GetAnimationLength("Hit");
	var duration = 30;
	this.wobble_animation = PlayAnimation("Hit", 5, Anim_Linear(0, 0, len, duration, ANIM_Remove), Anim_Const(500));
	ScheduleCall(this, this.StopWobble, duration, 0);
}

public func StopWobble()
{
	StopAnimation(this.wobble_animation);
	this.wobble_animation = nil;
}

local SplatterParticleEffect = new Effect
{
	Timer = func(int time)
	{
		if (time > 100) return FX_Execute_Kill;
		this.Target->CreateParticle("SphereSpark", 0, 0, PV_Random(-5, 5), PV_Random(-5, 5), PV_Random(5, 15), this.particles, 3);
		this.Target->CreateParticle("SphereSpark", 0, 0, PV_Random(-3, 3), PV_Random(-3, 3), PV_Random(3, 10), this.particles_bright, 3);
		return FX_OK;
	},
	
	Destruction = func()
	{
		this.Target->CreateParticle("SphereSpark", 0, 0, PV_Random(-20, 20), PV_Random(-20, 20), PV_Random(1, 5), this.particles, 30);
		this.Target->CreateParticle("SphereSpark", 0, 0, PV_Random(-15, 15), PV_Random(-15, 15), PV_Random(1, 3), this.particles_bright, 30);
	}
};

public func Splatter(int amount)
{
	
	while (amount > 0)
	{
		var dummy = CreateObject(Dummy, 0, 0, NO_OWNER);
		dummy->SetCategory(C4D_None);
		dummy.Visibility = VIS_All;
		dummy->SetSpeed(RandomX(-40, 40), RandomX(-40, -5));
		dummy->AddVertex(0, 0);
		dummy.Hit = this.Remove;
		var fx = dummy->CreateEffect(SplatterParticleEffect, 1, 1);
		fx.particles = this.particles;
		fx.particles_bright = this.particles_bright;
		amount -= 5000;
	}
}

public func Remove()
{
	RemoveObject();
}

local MaxEnergy = 60000;
local MaxBreath = 100000;
local NoBurnDecay = 1;
local ContactIncinerate = 0;
local CorrosionResist = 0;
local BorderBound = C4D_Border_Sides;
local ContactCalls = true;

local ActMap = {
Walk = {
	Prototype = Action,
	Name = "Walk",
	Procedure = DFA_WALK,
	Accel = 16,
	Decel = 22,
	Speed = 100,
	Directions = 2,
	FlipDir = 1,
	Length = 20,
	Delay = 1,
},
Swim = {
	Prototype = Action,
	Name = "Swim",
	Procedure = DFA_SWIM,
	Accel = 16,
	Decel = 22,
	Speed = 100,
	Directions = 2,
	FlipDir = 1,
	Length = 20,
	Delay = 1,
},
Scale = {
	Prototype = Action,
	Name = "Scale",
	Procedure = DFA_SCALE,
	Accel = 16,
	Decel = 22,
	Speed = 50,
	Directions = 2,
	FlipDir = 1,
	Length = 20,
	Delay = 2,
},
Hangle = {
	Prototype = Action,
	Name = "Hangle",
	Procedure = DFA_HANGLE,
	Accel = 16,
	Decel = 22,
	Speed = 50,
	Directions = 2,
	FlipDir = 1,
	Length = 20,
	Delay = 2,
},
Kneel = {
	Prototype = Action,
	Name = "Kneel",
	Procedure = DFA_KNEEL,
	Directions = 2,
	FlipDir = 1,
	Length = 20,
	Delay = 2,
},
Jump = {
	Prototype = Action,
	Name = "Jump",
	Procedure = DFA_FLIGHT,
	Speed = 200,
	Accel = 14,
	Directions = 2,
	FlipDir = 1,
	Length = 20,
	Delay = 2,
},
Dead = {
	Prototype = Action,
	Name = "Dead",
	Directions = 2,
	FlipDir = 1,
	Length = 10,
	Delay = 3,
	NoOtherAction = 1,
	ObjectDisabled = 1,
},
Claw = {
	Prototype = Action,
	Name = "Claw",
	Procedure = DFA_ATTACH,
	Directions = 2,
	FlipDir = 1,
	Length = 1,
	Delay = 0,
},
};