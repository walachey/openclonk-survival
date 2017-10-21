/**
	Stats
	Handling of attributes, stats and skills.

	@author Zapper
*/

local Plane = 1;

public func Init()
{
	var character_sheet = 
	{
		// Stats are the base of the attributes.
		stats =
		{
		
		},
		// Attributes are calculated values based on the stats
		attributes =
		{

		},
		// Skills are on-or-off things that can depend on each other.
		skills =
		{
			learned = {},
			available = {},
			Learn = this.Skills_Learn,
		},
		CalculateAttributes = this.CalculateAttributes,
		GetAttributeValue = this.GetAttributeValue,
		skillpoints = 0,
	};
	
	for (var available_skill in AllPassiveSkills)
		character_sheet.skills.available[available_skill->GetName()] = available_skill;
	
	return character_sheet;
}

public func GetAttributeValue(string attribute_name)
{
	if (!this.attributes[attribute_name]) return 0;
	return this.attributes[attribute_name].value;
}

public func Attribute(string name, string description)
{
	return
	{
		Name = name,
		Description = description,
		value = 0,
	};
}

public func CalculateAttributes()
{
	// Reset attributes.
	for (var attr_name in GetProperties(this.attributes))
	{
		var attr = this.attributes[attr_name];
		attr.value = 0;
	}
	
	// Apply base stats.
	for (var stat in this.stats)
	{
		if (!stat.is_base_value) continue;
		stat->Calculate(this.attributes);
	}
	
	// Apply stacked stats.
	for (var stat in this.stats)
	{
		if (stat.is_base_value) continue;
		stat->Calculate(this.attributes);
	}
}

local Stats_Effect = 
{
	GetName = Global.GetName,
	affects = nil,
	is_base_value = false,
	factor = 1,
	exponent = 0,
	Calculate = func(int stat_value, int attr_value)
	{
		var value = stat_value;
		if (!this.is_base_value) value = attr_value;
		return this.factor * value ** this.exponent / 10000;
	}
};

private func MakeStat(int start)
{
	return new Stats_Stat { value = start };
}

local Stats_Stat = {
	GetName = Global.GetName,
	value = nil,
	effects =
	{
		
	},
	Calculate = func (proplist attributes)
	{
		for (var fx in this.effects)
		{
			var attr = attributes[fx.affects];
			if (!attr) continue;
			var calculated = fx->Calculate(this.value, attr.value);
			attr.value += calculated;
		}
	}
};

public func Skills_Learn(string name)
{
	var skill = this.available[name];
	if (!skill) return false;
	this.learned[name] = skill;
	this.available[name] = nil;
	
	for (var skillname in GetProperties(this.available))
	{
		var skill = this.available[skillname];
		if (!skill) continue;
		skill->FulfillDependency(name);
	}	
	return true;
}

public func MakeSkill(symbol, string name, string description, activate)
{
	return new Stats_Skill
	{
		symbol = symbol,
		Name = name,
		Description = description,
		Activate = activate,
	};
}

local Stats_Skill = {
	GetName = Global.GetName,
	symbol = nil,
	Name = nil,
	Description = nil,
	Activate = nil,
	depends_on = [],
	SetDependency = func(string name)
	{
		PushBack(this.dependencies,
		{
			Name = name,
			ok = false
		});
	},
	FulfillDependency = func(string name)
	{
		for (var dependency in this.depends_on)
		{
			if (dependency->GetName() == name)
			{
				dependency.ok = true;
			}
		}
	},
	CanBeLearned = func()
	{
		for (var dependency in this.depends_on)
		{
			if (!dependency.ok) return false;
		}
		return true;
	},
};