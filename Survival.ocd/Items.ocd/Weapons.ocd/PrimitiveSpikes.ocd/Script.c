#include Library_Stackable

public func MaxStackCount() { return 5; }
public func TrapStrength() { return 15; }

public func RejectUse(object clonk)
{
	return !clonk->IsWalking();
}

public func ControlUse(object clonk, int angle)
{
	clonk->DoKneel(true);
	
	var item = this->TakeObject();
	
	item->SetPosition(clonk->GetX(),clonk->GetY() + 7);
	item->SetController(clonk->GetController());
	item->AddTimer("HitCheck", 5);
	item->SetR(-45);
	item.Collectible = 0;
	
	Sound("GeneralHit?", nil, 20, nil, nil, nil, -100);
}

public func HitCheck()
{
	for (var obj in FindObjects(Find_AtRect(-5, -5, 10, 10), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsProjectileTarget")), Find_Layer(), Find_Not(Find_Owner(GetController()))))
	{
		DealDamage(obj, TrapStrength(), DMG_Pierce);
		if (obj)
			obj->~OnProjectileHit(this);
		SoundAt("OrganicHit*");
		RemoveObject();
		return;
	}
}

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
local Components = [[Wood, 1]];
