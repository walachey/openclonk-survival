
#include Library_Stackable

public func MaxStackCount() { return 20; }
public func ProjectileStrength() { return 5; }
public func TumbleStrength() { return 10; }


local aiming;

local animation_set;

func Initialize()
{
	animation_set = {
		AimMode         = AIM_Position, // The aiming animation is done by adjusting the animation position to fit the angle
		AnimationAim    = "SpearAimArms",
		AnimationShoot  = "SpearThrowArms",
		AnimationShoot2 = "SpearThrow2Arms",
		AnimationShoot3 = "SpearThrow3Arms",
		ShootTime       = 16,
		ShootTime2      =  8,
		WalkBack        =  0,
	};
}

public func GetAnimationSet() { return animation_set; }

public func RejectUse(object clonk)
{
	return !clonk->HasHandAction();
}

public func ControlUseStart(object clonk, int x, int y)
{
	aiming = 1;

	clonk->StartAim(this);

	ControlUseHolding(clonk, x, y);
	
	Sound("DrawJavelin", nil, 50, nil, nil, nil, 100);
	return 1;
}

public func HoldingEnabled() { return true; }

func ControlUseHolding(object clonk, ix, iy)
{
	var angle = Angle(0,0,ix,iy);
	angle = Normalize(angle,-180);

	clonk->SetAimPosition(angle);

	return true;
}

protected func ControlUseStop(object clonk, ix, iy)
{
	if(aiming)
		clonk->StopAim();
	return true;
}

// Callback from the clonk, when he actually has stopped aiming
public func FinishedAiming(object clonk, int angle)
{
	clonk->StartShoot(this);
	return true;
}

public func ControlUseCancel(object clonk, int x, int y)
{
	clonk->CancelAiming(this);
	return true;
}

public func Reset(clonk)
{
	aiming = 0;
}

// Called in the half of the shoot animation (when ShootTime2 is over)
public func DuringShoot(object clonk, int angle)
{
	DoThrow(clonk, angle);
}

public func DoThrow(object clonk, int angle)
{
	var javelin=TakeObject();
	
	var div = 60; // 40% is converted to the direction of the throwing angle.
	var xdir = clonk->GetXDir(1000);
	var ydir = clonk->GetYDir(1000);
	var speed = clonk.ThrowSpeed * 10 + (100 - div) * Sqrt(xdir**2 + ydir**2) / 100;
	var jav_x = div * xdir / 100 + Sin(angle, speed);
	var jav_y = div * ydir / 100 - Cos(angle, speed);
		
	javelin->SetXDir(jav_x, 1000);
	javelin->SetYDir(jav_y, 1000);
	javelin->SetPosition(javelin->GetX(),javelin->GetY());
	
	SetController(clonk->GetController());
	javelin->AddEffect("Flight",javelin,1,1,javelin,nil);
	javelin->AddEffect("HitCheck",javelin,1,1,nil,nil,clonk);
	
	Sound("ThrowJavelin?", nil, 50, nil, nil, nil, 100);
	
	aiming = -1;
	clonk->UpdateAttach();
}

public func HitObject(object obj)
{
	var dmg = ProjectileStrength() * 1000;

	if (WeaponCanHit(obj))
	{
		if (obj->GetAlive())
			Sound("ProjectileHitLiving?", nil, 50, nil, nil, nil, 100);
		else
		{
			SoundAt("JavelinHitGround", nil, nil, 50, nil, nil, 100);
		}
		
		obj->~OnProjectileHit(this);
		DealDamage(obj, dmg, DMG_Blunt);
		if (!Random(10))
			WeaponTumble(obj, this->TumbleStrength());
		if (!this) return;
	}
	
	RemoveObject();
	return;
}

protected func Hit()
{
	if (GetEffect("Flight",this))
	{
		SoundAt("JavelinHitGround", nil, nil, 50, nil, nil, 100);
		RemoveObject();
	}
	else
		Sound("RockHit?", nil, 50);
}


private func FxFlightStart(object target, effect fx, temp)
{
	if (temp) return;
	target.Collectible = 0;
}

private func FxFlightTimer()
{
	return FX_OK;
}

private func FxFlightStop(object target, effect fx, int reason, temp)
{
	if (temp) return;
	if (target)
		target->RemoveObject();
}

public func IsWeapon() { return true; }
public func IsArmoryProduct() { return true; }


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
local Components = [[Stone, 2]];
