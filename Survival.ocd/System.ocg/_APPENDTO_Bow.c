#appendto Bow

local shooting_strength = 50;
local MaxWear = 100;

local QualitySettings = {
	Properties = {
		MaxWear = {Base = 100, FullBonus = 100, Name = "$Durability$"},
		shooting_strength = {Base = 50, FullBonus = 100, Name = "$Range$"},
	}
};

// Callback from the clonk, when he actually has stopped aiming
public func FinishedAiming(object clonk, int angle)
{
	clonk->DetachMesh(iArrowMesh);
	iArrowMesh = nil;

	// shoot
	if(Contents(0))
	{
		if(Contents(0)->~IsArrow())
		{
			var arrow = Contents(0)->TakeObject();
			arrow->Launch(angle,shooting_strength,clonk);
			Sound("Objects::Weapons::Bow::Shoot?");
			OnArrowFired(clonk, arrow, angle, shooting_strength);
		}
	}

	// Open the hand to let the string go and play the fire animation
	PlayAnimation("Fire", 6, Anim_Linear(0, 0, GetAnimationLength("Fire"), animation_set["ShootTime"], ANIM_Hold));
	clonk->PlayAnimation("Close1Hand", 11, Anim_Const(0), Anim_Const(1000));
	clonk->StartShoot(this);
	return true;
}

public func OnArrowFired(object clonk, object arrow, int angle, int shooting_strength, ...)
{Log("OnaArrowFired");
	DoWear(5);
	var fx;
	for (var i = 0; fx = GetEffect("*Bow*", clonk, i); ++i)
		fx->~OnArrowFired(arrow, this, angle, shooting_strength);
	return _inherited(clonk, arrow, ...);
}

public func ControlUseStop(object clonk, ...)
{
	OnStopAiming(clonk);
	return inherited(clonk, ...);
}

public func ControlUseCancel(object clonk, ...)
{
	OnCancelAiming(clonk);
	return inherited(clonk, ...);
}

public func AddArrow(object clonk, ...)
{
	OnLoadingFinished(clonk);
	return inherited(clonk, ...);
}

private func OnStopAiming(object clonk)
{
	var fx;
	for (var i = 0; fx = GetEffect("*Bow*", clonk, i); ++i)
		fx->~OnStoppedAiming(this);
	return _inherited(clonk, ...);
}

private func OnCancelAiming(object clonk)
{
	var fx;
	for (var i = 0; fx = GetEffect("*Bow*", clonk, i); ++i)
		fx->~OnCanceledAiming(this);
	return _inherited(clonk, ...);
}

private func OnLoadingFinished(object clonk)
{Log("OnLoadingFinished");
	var fx;
	for (var i = 0; fx = GetEffect("*Bow*", clonk, i); ++i)
		fx->~OnLoadingFinished(this);
	return _inherited(clonk, ...);
}
