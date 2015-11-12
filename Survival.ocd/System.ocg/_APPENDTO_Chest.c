#appendto Chest

local key_quality = "wooden";
local skin;
local is_locked;

local opening_callbacks;

public func IsContainer() { return is_open; }
public func RejectCollect() { return is_open == false; }


public func AddOpeningCallback(callback)
{
	if (!opening_callbacks) opening_callbacks = [];
	PushBack(opening_callbacks, callback);
}

public func SetLocked(bool lock)
{
	is_locked = lock ?? true;
}

public func SetSkin(string new_skin)
{
	skin = new_skin;
}

public func GetInteractionMetaInfo(object clonk)
{
	return { Description = Format("$Open$", GetName()) };
}

public func IsInteractable(object clonk)
{
	return !is_open;
}

public func Interact(object clonk)
{
	if (is_locked)
	{
		var key = FindObject(Find_Container(clonk), Find_Func("IsKey"), Find_Func("CanUnlock", key_quality));
		if (key)
		{
			is_locked = false;
			Sound("Click");
			Schedule(clonk, "Sound(\"Confirm*\")", 20);
			key->RemoveObject();
		}
		else
		{
			key = FindObject(Find_Container(clonk), Find_Func("IsKey"));
			var msg = "$Locked$";
			if (key)
				msg = "$WrongKey$";
			PlayerMessage(clonk->GetOwner(), msg);
			Sound("DullMetalHit*", nil, nil, nil, nil, nil, -600);
			return true;
		}
	}
	
	if (!is_open) return Open(clonk);
}

private func Open(object by)
{
	if (is_open) return;
	if (opening_callbacks)
	{
		for (var callback in opening_callbacks)
		{
			this->Call(callback, by);
		}
	}
	return inherited(by, ...);
}

/*-- Interaction --*/

// Always show an interaction menu with at least the damage entry.
public func HasInteractionMenu() { if (is_locked) return true; return _inherited(...); }

// Show damage and allow a player to repair the building when damaged.
public func GetInteractionMenus(object clonk)
{
	var menus = _inherited() ?? [];		
	if (is_locked || !is_open)
	{
		var menu =
		{
			title = "",
			entries_callback = this.GetInfoMenuEntries,
			BackgroundColor = RGB(0, 50, 50),
			Priority = 1
		};
		PushBack(menus, menu);
	}
	return menus;
}

// Returns the contents of the "damage" section in the interaction menu.
public func GetInfoMenuEntries()
{	
	var menu = 
	{
		Bottom = "8em",
		Text = Format("$LockedInfoTxt$", this->GetName()),
		Style = GUI_TextVCenter | GUI_TextHCenter,
		symbol = 
		{
			Right = "2em",
			Bottom = "2em",
			Symbol = this
		}
	};
	
	if (!is_locked)
		menu.Text = Format("$ClosedInfoTxt$", this->GetName());
	
	return [{symbol = this, custom = menu}];
}
