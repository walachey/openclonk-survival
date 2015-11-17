
func Initialize()
{
	var chest = CreateObjectAbove(Chest, 50, 200, -1);
	chest->CreateContents(Flintstones, 5);
	chest->CreateContents(PrimitiveDarts, 5);
	chest->CreateContents(PrimitiveSpikes, 5);
	chest->CreateContents(Sword, 5);
	chest->CreateContents(Sword)->SetQualityBonus(50);
	chest->CreateContents(Sword)->SetQualityBonus(100);
	chest->CreateContents(Sword)->SetQualityBonus(200);
	chest->CreateContents(StoneKnife, 5);
	chest->CreateContents(StoneKnife)->SetQualityBonus(200);
	chest->CreateContents(MushroomPowder, 5);
	chest->CreateContents(Fiber, 5);
	chest->CreateContents(Driftwood, 10);
	
	chest->CreateContents(Bread, 10);
	chest->CreateContents(Sproutberry, 10);
	chest->CreateContents(CookedMushroom, 10);
	chest->CreateContents(Mushroom, 10);
	chest->CreateContents(WoodenKey, 10);
	chest->CreateContents(SilverKey, 10);
	chest->CreateContents(GoldenKey, 10);
	chest->CreateContents(Pickaxe, 5);
	chest->CreateContents(Shovel, 5);
	
	chest->AddOpeningCallback(Scenario.OnChestOpen);
	
	var locked_chest = CreateObjectAbove(Chest, 70, 200, -1);
	locked_chest->SetLocked();
	locked_chest->CreateContents(Nugget);
	
	chest = CreateObjectAbove(Chest, 15, 200, -1);
	chest->AssignChestLootRarity(200);
	chest = CreateObjectAbove(Chest, 25, 200, -1);
	chest->AssignChestLootRarity(600);
	chest = CreateObjectAbove(Chest, 35, 200, -1);
	chest->AssignChestLootRarity(900);
	
	CreateObjectAbove(StrawMan, 80, 200, -1);
	CreateObjectAbove(StrawMan, 110, 200, -1);
	CreateObjectAbove(StrawMan, 130, 200, -1);
	
	CastObjects(Amphora, 5, 10, 200, 150);
	CreateObject(Campfire, 150, 190)->CreateContents(Wood, 10);
	
	PricklyPear->Place(3, Shape->Rectangle(LandscapeWidth()/2, 0, LandscapeWidth()/2, LandscapeHeight()));
	
	var c = CreateObjectAbove(Ruin_ChemicalLab, 350, 200, -1);
	AddEffect("Toxic", c, 1, 30);
	
	var cave = Landscape_Cave->Place(1, Shape->Rectangle(0, LandscapeHeight()/2 - 50, LandscapeWidth(), LandscapeHeight()/2 + 50), {width = 200 + Random(50), height = 100 + Random(50), borderheight = 0, bordermat = "Earth", bordertex = "earth" });
	if (GetLength(cave))
	{
		cave = cave[0];
		CastObjects(Urchin, 5, 30, cave->GetX(), cave->GetY());
	}
	
	DigFreeRect(50, 220, 400, 50, true);
	for (var i = 0; i < 5; ++i)
	{
		CreateObject(AzuritePillar, 75 + Random(75), 230, -1);
		CreateObject(MalachitePillar, 120 + Random(75), 230, -1);
	}
}

func OnChestOpen(object clonk)
{
	AddEffect("CastSparks", this, 1, 1, nil, nil);
}

global func FxCastSparksStart(object target, effect fx)
{
	fx.particles = 
	{
		Prototype = Particles_Glimmer(),
		Size = 1,
		DampingY = PV_Linear(PV_Random(1000, 900), PV_Random(800, 900)),
		ForceX = PV_Random(-10, 10, 5)
	};
}

global func FxCastSparksTimer(object target, effect fx, int time)
{
	if (time > 36) return -1;
	target->CreateParticle("StarSpark", PV_Random(-6, 6), PV_Random(2, 5), PV_Random(-10, 10), PV_Random(-time * 6, -20), PV_Random(100, 600), fx.particles, 50);
}

func InitializePlayer(int plr)
{
	GetCrew(plr)->SetPosition(20,100);	
}

global func FxToxicTimer(object target)
{
	if (Random(3)) return;
	target->CreateToxicSmoke(0, 0, 50, 10 * 1000);
}