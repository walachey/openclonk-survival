/**
	Player HUD
	HUD, indicating the current status of a Clonk.
	
	@author Zapper
*/

local Plane = 1;

static all_player_huds;

local menu_id;
local Visibility = VIS_Owner;

local UpdateEffect = new Effect
{
	Timer = func() { this.hud->Update(this.Target); }
};

public func Update(object target)
{
	if (this == PlayerHUD)
	{
		var plr = target->GetOwner();
		if (all_player_huds == nil) all_player_huds = [];
		if (all_player_huds[plr] == nil)
		{
			all_player_huds[plr] = CreateObject(PlayerHUD, AbsX(1), AbsY(1), plr);
			var fx = target->CreateEffect(PlayerHUD.UpdateEffect, 1, 2);
			fx.hud = all_player_huds[plr];
		}
		return all_player_huds[plr]->Update(target);
	}
	
	if (menu_id != 0)
		GuiClose(menu_id);
	GenerateMenu(target);
}

public func GenerateMenu(object target)
{
	var bleeding = target->GetEffect("StatusBleeding", target) != nil;
	var fracture = target->GetEffect("StatusFracture", target) != nil;
	var unconscious = target->GetEffect("StatusUnconscious", target) != nil;
	
	var menu =
	{
		Decoration = GUI_MenuDeco,
		BackgroundColor = RGB(0, 0, 0),
		Target = this,
		Style = GUI_VerticalLayout | GUI_FitChildren | GUI_Multiple,
		Left = "100% - 10em",
		Top = "5em",
	};
	
	AddBar(menu, 1,  target->GetEnergy() * 1000, target.MaxEnergy, "$Hitpoints$", RGB(200, 10, 10));
	AddBar(menu, 2, target.Hunger, target.MaxHunger, "$Hunger$", RGB(100, 100, 0));
	AddBar(menu, 3, target.Thirst, target.MaxThirst, "$Thirst$", RGB(0, 0, 255));
	
	var status = 
	{
		Bottom = "1em", Style = GUI_TextHCenter
	};
	
	if (bleeding) GuiAddSubwindow(new status { Text = "<c ffee00ee>$Bleeding$</c>", Priority = 4 }, menu);
	if (fracture) GuiAddSubwindow(new status { Text = "<c ffee00ee>$Fracture$</c>", Priority = 5 }, menu);
	if (unconscious) GuiAddSubwindow(new status { Text = "<c ffee00ee>$Unconscious$</c>", Priority = 6 }, menu);
	menu_id = GuiOpen(menu);
}

private func AddBar(proplist menu, int priority, int value, int max_value, string text, int color)
{
	var promille =  1000 * value / max_value;
	var bar = 
	{
		Priority = priority,
		Bottom = "1.2em",
		bar =
		{
			Top = "0.9em",
			Margin = "0.05em",
			Right = ToPercentString(promille),
			BackgroundColor = color
		},
		Text = Format("<c %x>%s</c>", color, text)
	};
	GuiAddSubwindow(bar, menu);
}