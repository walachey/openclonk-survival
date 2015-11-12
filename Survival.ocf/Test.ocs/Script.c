
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
	
	chest->CreateContents(Bread, 10);
	chest->CreateContents(Sproutberry, 10);
	chest->CreateContents(CookedMushroom, 10);
	chest->CreateContents(Mushroom, 10);
	
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