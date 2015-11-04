
func InitializePlayer(int plr)
{
	GetCrew(plr)->SetPosition(20,100);
	GetCrew(plr)->CreateContents(Flintstones);
	
	CreateObjectAbove(StrawMan, 60, 200, -1);
	CreateObjectAbove(StrawMan, 20, 200, -1);
	CreateObjectAbove(StrawMan, 80, 200, -1);
}

