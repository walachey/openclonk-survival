global func GetWear(bool percent)
{
	if (!percent) return this.Wear;
	return 100 * this.Wear / this.MaxWear;
}

global func DoWear(int change)
{
	if (!this.MaxWear) return false;
	if (this.Wear == nil) this.Wear = 0;
	
	this.Wear = Max(this.Wear + change, 0);
	
	if (this.Wear >= this.MaxWear)
	{
		this->OnMaxWearReached();
	}
	else if (Contained())
	{
		Contained()->~OnInventoryChange();
	}
}

global func OnMaxWearReached()
{
	Sound("TreeCrack", nil, 50, nil, nil, nil, 100);
	AddEffect("IntScheduleRemove", this, 1, 1, this);
}

global func FxIntScheduleRemoveTimer()
{
	RemoveObject();
}

global func CanBeStackedWith(object other)
{
	if (this.Wear != other.Wear) return false;
	return inherited(other, ...);
}

global func GetInventoryIconOverlay()
{
	if (!this.Wear) return nil;

	var percentage = 100 - GetWear(true);
	var color = RGB(0, 255, 0);
	if (percentage < 25) color = RGB(255, 0, 0);
	else if (percentage < 50) color = RGB(255, 255, 0);
	
	// Overlay a usage bar.
	var overlay = 
	{
		Bottom = "0.75em", Margin = ["0.1em", "0.25em"],
		BackgroundColor = RGB(0, 0, 0),
		inner = 
		{
			Margin = "0.05em",
			BackgroundColor = color,
			Right = Format("%d%%", percentage),
		}
	};
	
	return overlay;
}