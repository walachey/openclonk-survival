protected func CatchBlow(dmg)
{
	if (GetAction() == "Dead") return;
	if ((dmg == nil || Abs(dmg) >= 10) && !Random(5)) this->Hurt();
}
