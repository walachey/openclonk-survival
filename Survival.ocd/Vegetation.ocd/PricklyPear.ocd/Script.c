local parent;
local children;
local children_mask;
local child_count = 0;
local depth = 1;

local MaxHitpoints = 20000;

/* Placement */

/** Places the given amount of plants inside the area. If no area is given, the whole landscape is used.
	@param amount The amount of plants to be created (not necessarily every plant is created).
	@param rectangle The area where to put the plants.
	@param settings A proplist defining further setttings: { growth = 100000, keep_area = false }. Growth will get passed over to PlaceVegetation, keep_area will confine the plants and their offspring to rectangle.
	@return Returns an array of all objects created.
*/
public func Place(int amount, proplist area, proplist settings)
{
	// No calls to objects, only definitions
	if (GetType(this) == C4V_C4Object) return;
	// Default parameters
	if (!settings) settings = { growth = 100000, keep_area = false };
	if (!settings.growth) settings.growth = 100000;
	var rectangle;
	if (area) rectangle = area->GetBoundingRectangle(); else rectangle = Shape->LandscapeRectangle();

	var plants = CreateArray(), plant;
	for (var i = 0 ; i < amount ; i++)
	{
		plant = PlaceVegetation(this, rectangle.x, rectangle.y, rectangle.w, rectangle.h, settings.growth, area);
		if (plant)
		{
			plants[GetLength(plants)] = plant;
		}
		plant = nil;
	}
	return plants;
}

public func Construction()
{
	this.Hitpoints = this.MaxHitpoints;
	StartGrowth(1);
	AddTimer("CheckChild", 20 + Random(30));
	children = [nil, nil, nil];
	children_mask = [true, true, true];
	_inherited(...);
}


public func Incineration()
{
	CreateParticle("Grass", 0, 0, PV_Random(-20, 20), PV_Random(-20, 10), PV_Random(30, 100), Particles_Straw(), 200);
	RemoveObject();
}

public func Destruction()
{
	if (parent) parent->OnLoseChild();
	for (var child in children)
	{
		if (!child) continue;
		child->Incinerate();
		child.parent = nil;
	}
}

public func OnDamage(object from, int damage, int damage_type, int by_player)
{
	this.Hitpoints -= damage;
	if (this.Hitpoints <= 0) Incinerate();
}

private func OnLoseChild()
{
	child_count -= 1;
	if (child_count == 0)
	{
		this.ContactIncinerate = this.Prototype.ContactIncinerate;
		this.BlastIncinerate = this.Prototype.BlastIncinerate;
		this.IsProjectileTarget = this.Prototype.IsProjectileTarget;
	}
}

private func CheckChild()
{
	if (GetCon() < 100) return;
	if (!Random(5 + 5 * depth))
		CreateChild();
}

private func CreateChild()
{
	if (child_count >= 3) return;
	
	var possible = [];
	for (var i = 0; i < 3; ++i)
	{
		if (!children[i] && children_mask[i]) PushBack(possible, i);
	}

	var random_index = possible[Random(GetLength(possible))];
	
	var x, y, r;
	if (random_index == 0)
	{
		x = 0;
		y = -15;
		r = 0;
	}
	else if (random_index == 1)
	{
		x = -5;
		y = -10;
		r = -45;
	}
	else if (random_index == 2)
	{
		x = 5;
		y = -10;
		r = 45;
	}
	
	if (GBackSemiSolid(2 * x, 2 * y)) return;
	if (FindObject(Find_AtPoint(x * 2, y * 2), Find_ID(GetID()), Find_Exclude(this))) return;
	
	var child = CreateObject(GetID());
	child->SetCon(5);
	child->SetAction("Attach", this);
	child->SetActionData(1 + random_index);
	child->SetR(r + RandomX(-10, 10));
	child.parent = this;
	child.depth = this.depth + 1;
	child.children_mask[random_index] = false;
	
	children[random_index] = child;
	child_count += 1;
	if (child_count == 1)
	{
		this.ContactIncinerate = 0;
		this.BlastIncinerate = 0;
		this.IsProjectileTarget = nil;
	}
}

local ActMap = 
{
	Attach = 
	{
		Prototype = Action,
		Procedure = DFA_ATTACH,
		FacetBase = 1,
		NextAction = "Hold"
	}
};

public func IsProjectileTarget() { return true; }

local Name = "$Name$";
local BlastIncinerate = 1;
local ContactIncinerate = 3;
local Placement = 4;