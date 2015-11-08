#appendto StrawMan

func CatchBlow(dmg)
{
	var msg = CreateObject(FloatingMessage);
	msg->SetMessage(Format("%d", dmg));
}
