
func Initialize()
{
	
}

func InitializePlayer(int plr)
{
	GetCrew(plr)->SetPosition(20,100);
	SetPlayerZoomByViewRange(plr, LandscapeWidth(), 0, PLRZOOM_Direct);
}
