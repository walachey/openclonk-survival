global func CreateToxicSmoke(int x, int y, int range, int strength)
{
	var size = strength / 1000;
	var particles = 
	{
		Prototype = Particles_Smoke(true),
		R = PV_Random(50, 100), G = 255, B = PV_Random(50, 100),
		ForceX = PV_Random(-10, 10, 10),
		ForceY = PV_Random(-10, 10, 10),
		Size = PV_Linear(PV_Random(4, 10), PV_Random(size, 2 * size)),
	};
	
	CreateParticle("Smoke", x, y, PV_Random(-range, range), PV_Random(-range, range), PV_Random(10, 90), particles, 25 * size);
	for (var obj in FindObjects(Find_Distance(range), Find_OCF(OCF_Alive), Find_Layer()))
	{
		obj->~DoDaze(strength);
	}
	return true;
}

