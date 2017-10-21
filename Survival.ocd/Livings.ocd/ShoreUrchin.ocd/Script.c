local Name = "$Name$";
local Description = "$Description$";

local rotation;

public func Construction()
{
	AddEffect("Activity", this, 1, 10, this);
	SetAction("Walk");
	SetMeshRotation(Random(360));
	return true;
}

public func Destruction()
{

}

public func Death()
{
	var particles = 
	{
		Prototype = Particles_Material(RGB(20, 20, 150)),
		Size = 2,
		OnCollision = nil,
		DampingX = 850, DampingY = 850,
		Alpha = PV_KeyFrames(0, 0, 255, 800, 255, 1000, 0)
	};
	CreateParticle("BlueSpark", PV_Random(-7, 7), PV_Random(-5, 5),
					PV_Random(-40, 40), PV_Random(-40, 40),
					PV_Random(30, 40), particles, 120);
	Sound("BallonPop", false, 50, nil, nil, nil, 100);
	RemoveObject();
}

public func SetMeshRotation(int r)
{
	rotation = r;
	this.MeshTransformation = Trans_Rotate(r, 0, 1, 0);
}

private func FxActivityTimer(object target, effect fx, int time)
{
	if (!Random(8) && (GetContact(-1) & CNAT_Bottom))
		Jump();
	if (!Random(15))
	{
		var anim_len = GetAnimationLength("Idle");
		PlayAnimation("Idle", 1, Anim_Linear(0, 0, anim_len, 60 + Random(30), ANIM_Remove), Anim_Const(1000));
	}
	var targets = FindObjects(Find_Distance(15), Find_OCF(OCF_Alive), Find_Hostile(GetOwner()), Find_Exclude(this));
	if (GetLength(targets) > 0)
	{
		Sound("OrganicHit?", nil, nil, nil, nil, nil, 100);
		PlaySpikeAnimation();
		for (var t in targets)
		{
			DealDamage(t, this.HittingStrength + RandomX(-2000, 5000), DMG_Pierce);
		}
	}
	return 1;
}

private func FxActivityDamage(target, effect, dmg)
{
	if(dmg > 0) return dmg;
	
	return dmg;
}

public func PlaySpikeAnimation()
{
	var anim_len = GetAnimationLength("Spike");
	PlayAnimation("Spike", 2, Anim_Linear(0, 0, anim_len, 20, ANIM_Remove), Anim_Linear(0, 0, 1000, 5, ANIM_Hold));
}

public func Jump()
{
	Bubble(10);
	SetAction("Jump");
	SetSpeed(0, -RandomX(30, 50));
	AddTimer("Spin", 1);
}

public func Spin()
{
	SetMeshRotation(rotation + 3);
	if (GetContact(-1) & CNAT_Bottom) RemoveTimer("Spin");
}

local HittingStrength = 5000;

local MaxEnergy = 30000;
local BreatheWater = 1;
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
/*Swim = {
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
},*/
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
};
