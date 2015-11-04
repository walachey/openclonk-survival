/**
	Stats
	Handling of attributes, stats and skills.

	@author Zapper
*/

public func Init()
{
	return
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
			Learn = Skills_Learn,
		},
		CalculateAttributes = CalculateAttributes,
		GetAttributeValue = GetAttributeValue,
	};
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

private func Effect_Calculate(int stat_value, int attr_value)
{
	var value = stat_value;
	if (!this.is_base_value) value = attr_value;
	return this.factor * value ** this.exponent / 10000;
}

public func Effect()
{
	return
	{
		affects = nil,
		is_base_value = false,
		factor = 1,
		exponent = 0,
		Calculate = Effect_Calculate,
	};
}

public func Stat_Calculate(proplist attributes)
{
	for (var fx in this.effects)
	{
		var attr = attributes[fx.affects];
		if (!attr) continue;
		var calculated = fx->Calculate(this.value, attr.value);
		attr.value += calculated;
	}
}

public func Stat(int start)
{
	return
	{
		value = start,
		effects =
		{
			
		},
		Calculate = Stat_Calculate,
	};
}

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

public func Skill_FulfillDependency(string name)
{
	for (var dependency in this.depends_on)
	{
		if (dependency->GetName() == name)
		{
			dependency.ok = true;
		}
	}
}

public func Skill_SetDependency(string name)
{
	PushBack(this.dependencies,
	{
		Name = name,
		ok = false
	});
}

public func Skill_CanBeLearned()
{
	for (var dependency in this.depends_on)
	{
		if (!dependency.ok) return false;
	}
	return true;
}

public func Skill(symbol, string name, string description, activate)
{
	return
	{
		symbol = symbol,
		Name = name,
		Description = description,
		Activate = activate,
		depends_on = [],
		SetDependency = Skill_SetDependency,
		FulfillDependency = Skill_FulfillDependency,
		CanBeLearned = Skill_CanBeLearned,
	};
}