#include Library_MeleeWeapon

local KnifeStrength = 30;
local WeaponStrength = 5;
local MaxWear = 100;

local QualitySettings = {
	Properties = {
		KnifeStrength = {Base = 30, FullBonus = 20, Name = "$KnifeStrength$"},
		WeaponStrength = {Base = 5, FullBonus = 5, Name = "$WeaponStrength$"},
		MaxWear = {Base = 100, FullBonus = 300, Name = "$Durability$"},
	}
};

local magic_number;

public func RejectUse(object clonk)
{
	return !clonk->IsWalking() && !clonk->IsJumping();
}

// used by this object
func ReadyToBeUsed(proplist data)
{
	var clonk = data.clonk;
	return !RejectUse(clonk) && CanStrikeWithWeapon(clonk) && clonk->HasHandAction();
}

public func ControlUse(object clonk, int angle)
{
	if (!CanStrikeWithWeapon(clonk) || !clonk->HasHandAction())
	{
		clonk->PauseUse(this, "ReadyToBeUsed", {clonk = clonk});
		return true;
	}
	
	var rand = 2;
	var arm = "R";
	var animation = Format("SwordSlash%d.%s", rand, arm);
	var length = 15;
	var carry_bone = "pos_hand2";
	
	if(clonk->GetHandPosByItemPos(clonk->GetItemPos(this)) == 1)
	{
		arm = "L";
		carry_bone = "pos_hand1";
		animation  = Format("SwordSlash%d.%s", rand, arm);
	}
	if(clonk->IsJumping())
	{
		rand = 1;
		if(clonk->GetYDir() < -5) rand = 2;
		animation = Format("SwordJump%d.%s",rand,arm);
	}
	
	PlayWeaponAnimation(clonk, animation, 10, Anim_Linear(0, 0, clonk->GetAnimationLength(animation), length, ANIM_Remove), Anim_Const(1000));
	clonk->UpdateAttach();
	
	magic_number=((magic_number+1)%10) + (ObjectNumber()*10);
	StartWeaponHitCheckEffect(clonk, length, 1);
}

private func Hit(int x, int y)
{
	StonyObjectHit(x,y);
	return 1;
}

/* Combat */

func CheckStrike(iTime)
{
	var  offset_x=7;
	var offset_y=0;
	if(Contained()->GetDir() == DIR_Left) offset_x*=-1;

	if(!(Contained()->GetContact(-1) & CNAT_Bottom))
		offset_y=10;

	var width=10;
	var height=20;

	for(var obj in FindObjects(Find_AtRect(offset_x - width/2, offset_y - height/2, width, height),
							   Find_NoContainer(),
							   Find_Exclude(Contained()),
							   Find_Layer(GetObjectLayer())))
	{
		if (obj->~IsProjectileTarget(this, Contained()) || obj->GetOCF() & OCF_Alive)
		{
			var effect_name=Format("HasBeenHitByWeaponEffect%d", magic_number);
			var axe_name=Format("HasBeenHitByWeapon%d", this->ObjectNumber());
			var first=true;
			// don't hit objects twice
			if(!GetEffect(effect_name, obj))
			{
				AddEffect(effect_name, obj, 1, 60 /* arbitrary */, nil, 0);

				if(GetEffect(axe_name, obj))
				{
					first=false;
				}
				else
				{
					AddEffect(axe_name, obj, 1, 40, nil, 0);
				}
				DoWear(2 * this.WeaponStrength);
				DealDamage(obj, this.WeaponStrength * 1000, DMG_Slash);
				
				// sound and done. We can only hit one target
				Sound("WeaponHit?", false);
				break;
			}
		}
	}
}


local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
local Components = [[Wood, 2], [Flintstones, 2]];
