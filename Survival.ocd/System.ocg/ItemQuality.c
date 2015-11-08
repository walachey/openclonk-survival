
/**
	Example:
	local QualitySettings =
	{
		Properties = 
		{
			WeaponDamage = 
			{
				Name = "$TxtWpnDmg$,
				Base = 50,
				FullBonus = 30, 
			}
		}
	};
*/

global func ApplyBaseQuality()
{
	if (!this.QualitySettings) return false;
	var properties = GetProperties(this.QualitySettings.Properties);
	var len = GetLength(properties);
	for (var i = 0; i < len; ++i)
	{
		this[properties[i]] = this.QualitySettings.Properties[properties[i]].Base;
	}
	return true;
}

/*
	The base quality is always 100%. So SetQualityBonus(100) would set the quality to 200%.
*/
global func SetQualityBonus(int bonus)
{
	if (!this.QualitySettings) return false;
	if (!bonus) return false;
	
	// Visuals!
	
	var quality_name = "$QualityFine$";
	var quality_color = RGB(200, 255, 200);
	if (bonus >= 200)
	{
		quality_name = "$QualityMaster$";
		quality_color = RGB(255, 200, 255);
	}
	else if (bonus >= 100)
	{
		quality_name = "$QualitySuperior$";
		quality_color = RGB(255, 255, 200);
	}
	
	var name = this->GetName();
	if (this.QualityBonus)
		name = this.Prototype->GetName();
	this.Name = Format("<c %x>%s %s</c>", quality_color, quality_name, name);
	this.QualityBonus = bonus;
	
	// Actual effect of quality!
	var properties = GetProperties(this.QualitySettings.Properties);
	var property_count = GetLength(properties);
	var pools = CreateArray(property_count);
	var remaining_bonus = bonus;
	var step = 10;
	while (remaining_bonus > 0)
	{
		pools[Random(property_count)] += Min(remaining_bonus, step);
		remaining_bonus -= step;
	}
	
	var quality_description_label = "";
	var quality_description_number= "|";
	var quality_hash = 0;
	
	for (var i = 0; i < property_count; i += 1)
	{
		var pooled_bonus = pools[i];
		
		var property = this.QualitySettings.Properties[properties[i]]; 
		var base = property.Base;
		var full_bonus = property.FullBonus;
		
		var actual_bonus = pooled_bonus * full_bonus / 100;
		var breaks = "||";
		if (i == property_count - 1) breaks = "";
		
		quality_description_label = Format("%s%s:%s", quality_description_label, property.Name, breaks);
		quality_description_number= Format("%s%2d + %2d%s", quality_description_number, base, actual_bonus, breaks);
		this[properties[i]] = base + actual_bonus;
		
		quality_hash = 2 * quality_hash + actual_bonus;
	}
	
	this.QualityHash    = this.QualityBonus + 100 * quality_hash;
	this.QualityLabels  = quality_description_label;
	this.QualityNumbers = quality_description_number;
	
	return true;
}

global func CanBeStackedWith(object other)
{
	if (this.QualityHash != other.QualityHash) return false;
	return inherited(other, ...);
}