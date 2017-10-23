#appendto Pickaxe

local MaxWear = 100;
local PickaxeStrength = 100;

local QualitySettings = {
	Properties = {
		MaxWear = {Base = 100, FullBonus = 200, Name = "$Durability$"},
		PickaxeStrength = {Base = 100, FullBonus = 100, Name = "$PickaxeStrength$"},
	}
};

protected func DoSwing(object clonk, int ix, int iy)
{
	var angle = Angle(0,0,ix,iy);

	//Creates an imaginary line which runs for 'MaxReach' distance (units in pixels)
	//or until it hits a solid wall.
	var iDist=0;
	while(!GBackSolid(Sin(180-angle,iDist),Cos(180-angle,iDist)) && iDist < MaxReach)
	{
		++iDist;
	}
	
	//Point of contact, where the pick strikes the landscape
	var x2 = Sin(180-angle,iDist);
	var y2 = Cos(180-angle,iDist);
	var is_solid = GBackSolid(x2,y2);
	
	// alternatively hit certain objects
	var target_obj = FindObject(Find_AtPoint(x2, y2), Find_Func("CanBeHitByPickaxe"));
	
	// notify the object that it has been hit
	if(target_obj)
	{
		target_obj->~OnHitByPickaxe(this, clonk);
		DoWear(5);
	}
		
	// special effects only ifhit something
	if(is_solid || target_obj)
	{	
		var mat = GetMaterial(x2,y2);
		var tex = GetTexture(x2,y2);

		//Is the material struck made of a diggable material?
		if(is_solid && GetMaterialVal("DigFree","Material",mat))
		{
			var clr = GetAverageTextureColor(tex);
			var particles =
			{
				Prototype = Particles_Dust(),
				R = (clr >> 16) & 0xff,
				G = (clr >> 8) & 0xff,
				B = clr & 0xff,
				Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(2, 50), 1000, 0),
			};
			CreateParticle("Dust", x2, y2, PV_Random(-3, 3), PV_Random(-3, -3), PV_Random(18, 1 * 36), particles, 3);
			Sound("Dig?");
			DoWear(1);
		}
		//It's solid, but not diggable. So it is a hard mineral.
		else
		{
			var spark = Particles_Glimmer();
			var pitch = nil;
			if (GetMaterialVal("Density","Material",mat) > MaxPickDensity)
			{
				pitch = 60;
				spark.B = 255;
				spark.R = PV_Random(0, 128, 2);
				spark.OnCollision = PC_Bounce();
			}
			CreateParticle("StarSpark", x2*9/10,y2*9/10, PV_Random(-20, 20), PV_Random(-20, 20), PV_Random(10, 20), spark, 10);
			Sound("Clang?", nil, nil, nil, nil, nil, pitch);
			DoWear(3);
		}
		
		// Do blastfree after landscape checks are made. Otherwise, mat always returns as "tunnel"
		BlastFree(GetX()+x2,GetY()+y2,5,GetController(),MaxPickDensity);
		
		// Make sure that new loose objects do not directly hit the Clonk and tumble it.
		for (var obj in FindObjects(Find_Distance(10, x2, y2), Find_Category(C4D_Object), Find_Layer(), Find_NoContainer()))
		{
			if (obj->Stuck()) continue;
			AddEffect("IntNoHitAllowed", obj, 1, 30, nil, GetID());
		}
	}

}
