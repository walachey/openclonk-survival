
func Initialize()
{
	var chest = CreateObjectAbove(Chest, 50, 200, -1);
	chest->CreateContents(Flintstones, 5);
	chest->CreateContents(PrimitiveDarts, 5);
	chest->CreateContents(PrimitiveSpikes, 5);
	
	CreateObjectAbove(StrawMan, 80, 200, -1);
	CreateObjectAbove(StrawMan, 110, 200, -1);
	CreateObjectAbove(StrawMan, 130, 200, -1);
}

func InitializePlayer(int plr)
{
	GetCrew(plr)->SetPosition(20,100);	
}

