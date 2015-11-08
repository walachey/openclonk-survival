#appendto GUI_ObjectInteractionMenu

func OnMenuEntryHover(proplist menu_info, int entry_index, int player)
{
	var info = GetEntryInformation(menu_info, entry_index);
	if (!info.entry) return;
	if (!info.entry.symbol) return;
	var symbol_update = {Symbol = info.entry.symbol, left={Text=""}, right={Text=""}};
	
	// and update description itself
	// clean up existing description window in case it has been cluttered by sub-windows
	GuiClose(current_main_menu_id, 1, current_description_box.desc_target);
	// now add new subwindow to replace the recently removed one
	GuiUpdate({new_subwindow = {Target = current_description_box.desc_target, ID = 1}}, current_main_menu_id, 1, current_description_box.target);
	// default to description of object
	if (!info.menu.callback_target || !info.menu.callback_hover)
	{
		var text = Format("%s:|%s", info.entry.symbol->GetName(), info.entry.symbol.Description);
		var obj = nil;
		if (info.entry.extra_data && info.entry.extra_data.objects)
		{
			for (var possible in info.entry.extra_data.objects)
			{
				if (possible == nil) continue;
				obj = possible;
				break;
			}
		}
		// For contents menus, we can sometimes present additional information about objects.
		if (info.menu.flag == InteractionMenu_Contents && obj)
		{
			if (obj.QualityBonus)
			{
				symbol_update.left = {Text = obj.QualityLabels, Style=GUI_TextLeft | GUI_TextTop};
				symbol_update.right = {Text = obj.QualityNumbers, Style=GUI_TextRight | GUI_TextTop};
			}
			
			var additional = nil;
			if (obj->Contents())
			{
				additional = "$Contains$ ";
				var i = 0, count = obj->ContentsCount();
				// This currently justs lists contents one after the other.
				// Items are not stacked, which should be enough for everything we have ingame right now. If this is filed into the bugtracker at some point, fix here.
				for (;i < count;++i)
				{
					if (i > 0)
						additional = Format("%s, ", additional);
					additional = Format("%s%s", additional, obj->Contents(i)->GetName());
				}
			}
			if (additional != nil)
				text = Format("%s||%s", text, additional);
		}
		
		GuiUpdateText(text, current_main_menu_id, 1, current_description_box.desc_target);
	}
	else
	{
		info.menu.callback_target->Call(info.menu.callback_hover, info.entry.symbol, info.entry.extra_data, current_description_box.desc_target, current_main_menu_id);
	}
	
	// update symbol of description box
	GuiUpdate(symbol_update, current_main_menu_id, 1, current_description_box.symbol_target);
}
