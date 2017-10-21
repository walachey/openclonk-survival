local Name = "$Name$";
local Description = "$Description$";
local ContactCalls = true;
local BorderBound = C4D_Border_Sides;

public func Construction()
{
	AddEffect("Activity", this, 1, 10, this);
	SetAction("Walk");
	if (GetOwner() == NO_OWNER)
		SetCreatureControlled();
	this.MeshTransformation = Trans_Rotate(Random(360), 0, 1, 0);
	return true;
}

public func Destruction()
{

}

public func Death()
{
	var particles = 
	{
		Prototype = Particles_Material(RGB(150, 20, 150)),
		Size = 3,
		OnCollision = nil,
		DampingX = 850, DampingY = 850,
		Alpha = PV_KeyFrames(0, 0, 255, 800, 255, 1000, 0)
	};
	CreateParticle("BlueSpark", PV_Random(-7, 7), PV_Random(-5, 5),
					PV_Random(-40, 40), PV_Random(-40, 40),
					PV_Random(30, 40), particles, 120);
	Sound("ChippieChirp*", false, 50, nil, nil, nil, -200);
	RemoveObject();
}


private func FxActivityTimer(target, effect, time)
{
	

	return 1;
}

private func FxActivityDamage(target, effect, dmg)
{
	if(dmg > 0) return dmg;
	
	return dmg;
}

local MaxEnergy = 60000;
local MaxBreath = 100000;
local NoBurnDecay = 1;
local ContactIncinerate = 0;
local CorrosionResist = 0;

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
