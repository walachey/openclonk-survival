#appendto Clonk

public func GetInteractionMenus(object clonk)
{
	var menus = _inherited(clonk, ...) ?? [];
	var menu =
	{
		title = "$StatsAndSkills$",
		entries_callback = this.GetStatsMenuEntries,
		callback = nil, // The callback is handled internally. See GetProductionMenuEntries.
		callback_hover = this.OnStatsEntryHover,
		callback_target = this,
		BackgroundColor = RGB(0, 0, 50),
		Priority = 20
	};
	PushBack(menus, menu);
	
	return menus;
}

public func OnStatsEntryHover()
{

}

public func GetStatsMenuEntries()
{
	var menu_entries = [];
	var default_container =
	{
		Style = GUI_FitChildren,
		header = 
		{
			Style = GUI_TextHCenter | GUI_TextVCenter,
			Bottom = "1em",
			BackgroundColor = RGB(50, 50, 50),
			Text = "Heading"
		},
		contents = 
		{
			Top = "1em",
			Style = GUI_FitChildren | GUI_VerticalLayout,
		}
	};
	
	// default design of an item
	var custom_entry = 
	{
		Right = "100%", Bottom = "1em",
		BackgroundColor = {Std = 0, Hover = 0x50ff0000},
		image = {Right = "2em"},
		text = {Left = "2em"},
		desc = {Style = GUI_TextRight},
		OnMouseIn = GuiAction_SetTag("Hover"),
		OnMouseOut = GuiAction_SetTag("Std"),
	};
	
	var data = this.character;
	
	var header = 
	{
		Priority = -1,
		Bottom = "1em",
		Text = "<c ffffff00>$AvailablePoints$:</c>",
		Style = GUI_TextLeft | GUI_TextVCenter,
		sub = { Text = Format("<c ffeeee00>%d</c>", data.skillpoints), Style = GUI_TextRight | GUI_TextVCenter } 
	};
	PushBack(menu_entries, {symbol = Clonk, custom = header});
	
	var skill_container = new default_container {header = new default_container.header {}, contents = new default_container.contents {}};
	skill_container.header.Text = "$Skills$";
	PushBack(menu_entries, {symbol = Stats, custom = skill_container});
	var skill_idx = 0;
	for (var skillset in [data.skills.learned, data.skills.available])
	{
		var learned = skillset == data.skills.learned;
		
		for (var skillname in GetProperties(skillset))
		{
			var skill = skillset[skillname];
			skill_idx += 1;
			
			var skill_entry = new custom_entry {Priority = skill_idx,
												text = new custom_entry.text {},
												desc = new custom_entry.desc {},
												image = new custom_entry.image {}};
			skill_entry.text.Text = skill->GetName();
			skill_entry.desc.Text = Format("<c 99ffffff>%s</c>", skill.Description);
			if (learned)
			{
				skill_entry.image.Symbol = Icon_Ok;
				skill_entry.text.Text = Format("<c ffffff99>%s</c>", skill_entry.text.Text);
				skill_entry.BackgroundColor = {Std = 0, OnHover = 0x20ff0000};
			}
			else if (skill->CanBeLearned())
			{
				skill_entry.image.Symbol = Icon_Lightbulb;
				skill_entry.image.GraphicsName = "Green";
				skill_entry.OnClick = GuiAction_Call(this, "TryLearnSkill", skill);
			}
			else
			{
				skill_entry.image.Symbol = Icon_Lightbulb;
				skill_entry.image.GraphicsName = "Red";
				skill_entry.text.Text = Format("<c ff999999>%s</c>", skill_entry.text.Text);
				skill_entry.BackgroundColor = 0;
			}
			skill_container.contents[skillname] = skill_entry;
		}
	}
	
	return menu_entries;			
}

public func TryLearnSkill(proplist skill)
{
	Sound("UI::Click*");
	UpdateInteractionMenus(this.GetStatsMenuEntries);
}