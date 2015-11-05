#appendto Clonk

public func Construction()
{
	this.BleedDamage = 0;
	this.FractureDamage = 0;
	this.Daze = 0;
	AddTimer("DegenerateDamage", 10);
	
	// Make ActMap writeable.
	if (ActMap == this.Prototype.ActMap)
		ActMap = { Prototype = this.Prototype.ActMap };
	
	// Add "unconscious" action.
	ActMap.Unconscious = {
		Prototype = ActMap.Dead,
		StartCall = "StartUnconscious",
		StopCall = "StopUnconscious",
		AbortCall = "StopUnconscious"
	};
	
	return _inherited(...);
}

private func StartUnconscious()
{
	this.UnconsciousAnimation = PlayAnimation("Dead", CLONK_ANIM_SLOT_Death, Anim_Linear(0, 0, GetAnimationLength("Dead"), 20, ANIM_Hold), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
	// Update carried items
	this->UpdateAttach();
	// Set proper turn type
	this->SetTurnType(1);
}

private func StopUnconscious()
{
	if (this.UnconsciousAnimation)
		StopAnimation(this.UnconsciousAnimation);
	this.UnconsciousAnimation = nil;
}

private func DegenerateDamage()
{
	this.BleedDamage = Max(this.BleedDamage - 10000, 0);
	this.FractureDamage = Max(this.FractureDamage - 10000, 0);
	this.Daze = Max(this.Daze - 1000, 0);
}

public func DoDaze(int amount)
{
	this.Daze = Max(this.Daze + amount, 0);
	
	if (amount > 0)
	{
		if (CheckStatusTriggered(this.Daze / 100, 500, 1000))
			TryApplyStatusEffect("StatusUnconscious");
	}
}

public func OnDamage(object by, int exact_damage, int type, int by_player)
{
	if (exact_damage <= 0) return;
	
	var bleed_strength = 0;
	var fracture_strength = 0;
	if (type == DMG_Blunt)
	{
		fracture_strength = 100;
		bleed_strength = 25;
	}
	else if (type == DMG_Slash)
	{
		fracture_strength = 0;
		bleed_strength = 100;
	}
	else if (type == DMG_Pierce)
	{
		fracture_strength = 50;
		bleed_strength = 50;
	}
	
	if (bleed_strength > 0) CheckBleed(exact_damage * 100 / bleed_strength);
	if (fracture_strength > 0) CheckFracture(exact_damage * 100 / fracture_strength);
}

private func CheckStatusTriggered(int value, int stddev, int target)
{
	var random = Abs(NormalRandom(0, stddev));
	if (value + random >= target) return true;
	return false;
}

private func CheckBleed(int dmg)
{
	this.BleedDamage += dmg;
	if (CheckStatusTriggered(this.BleedDamage / 100, 500, 1000))
		TryApplyStatusEffect("StatusBleeding");
}

private func CheckFracture(int dmg)
{
	this.FractureDamage += dmg;
	if (CheckStatusTriggered(this.FractureDamage / 100, 500, 1000))
		TryApplyStatusEffect("StatusFracture");
}

private func TryApplyStatusEffect(string name)
{
	if (GetEffect(name, this)) return;
	AddEffect(name, this, 1, 5, this);
}

private func FxStatusBleedingStart(object target, effect fx, temp)
{
	if (temp) return;
	fx.particles =
	{
		Prototype = Particles_Material(RGB(255, 0, 0)),
		ForceX = PV_Random(-5, 5),
		DampingX = 950,
		OnCollision = PC_Stop(),
		Alpha = PV_Linear(255, 0),
	};
}

private func FxStatusBleedingTimer(object target, effect fx, int time)
{
	if (!Random(5))
		target->CreateParticleAtBone("SphereSpark", "skeleton_head", [0, 0, 0], [0, 0, RandomX(200, 300)], PV_Random(10, 30), fx.particles, 20); 
	DealDamage(target, 250, DMG_True);
}

private func FxStatusFractureStart(object target, effect fx, temp)
{
	if (temp) return;
	target->PushActionSpeed("Walk", 100);
	target->PushActionSpeed("Jump", 200);
}

private func FxStatusFractureStop(object target, effect fx, int reason, temp)
{
	if (temp) return;
	target->PopActionSpeed("Walk");
	target->PopActionSpeed("Jump");
}

private func FxStatusUnconsciousStart(object target, effect fx, temp)
{
	if (temp) return;
	target->SetAction("Unconscious");
}

private func FxStatusUnconsciousTimer(object target, effect fx)
{
	if (target.Daze <= 0) return FX_Execute_Kill;
	return FX_OK;
}

private func FxStatusUnconsciousStop(object target, effect fx, int reason, temp)
{
	if (temp) return;
	target->SetAction("KneelUp");
}