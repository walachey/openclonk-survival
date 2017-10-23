#include Flintstones

public func MaxStackCount() { return 10; }
public func ProjectileStrength() { return 5; }
public func TumbleStrength() { return 10; }

public func DoThrow(object clonk, int angle)
{
	var javelin=this->TakeObject();
	
	var div = 60; // 40% is converted to the direction of the throwing angle.
	var xdir = clonk->GetXDir(1000);
	var ydir = clonk->GetYDir(1000);
	var speed = clonk.ThrowSpeed * 15 + (100 - div) * Sqrt(xdir**2 + ydir**2) / 100;
	var jav_x = div * xdir / 100 + Sin(angle, speed);
	var jav_y = div * ydir / 100 - Cos(angle, speed);
		
	javelin->SetXDir(jav_x, 1000);
	javelin->SetYDir(jav_y, 1000);
	javelin->SetPosition(javelin->GetX(),javelin->GetY());
	
	SetController(clonk->GetController());
	javelin->AddEffect("Flight",javelin,1,1,javelin,nil);
	javelin->AddEffect("HitCheck",javelin,1,1,nil,nil,clonk);
	
	Sound("ThrowJavelin?", nil, 50, nil, nil, nil, 100);
	
	this.aiming = -1;
	clonk->UpdateAttach();
	
	javelin->SetGraphics(nil, Javelin);
	javelin->SetR(angle);
	javelin.MeshTransformation = Trans_Scale(500, 500, 500);
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
		DealDamage(obj, dmg, DMG_Pierce);
		if (!Random(10))
			WeaponTumble(obj, this->TumbleStrength());
		if (!this) return;
	}
	
	RemoveObject();
	return;
}

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
local Components = [[Flintstones, 5], [Wood, 2]];
