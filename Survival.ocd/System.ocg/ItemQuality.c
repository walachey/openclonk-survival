
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
	var had_quality = false;
	if (this.QualityBonus)
	{
		name = this.Prototype->GetName();
		had_quality = true;
	}
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
	
	var description = "";
	var quality_hash = 0;
	
	for (var i = 0; i < property_count; i += 1)
	{
		var pooled_bonus = pools[i];
		
		var property = this.QualitySettings.Properties[properties[i]]; 
		var base = property.Base;
		var full_bonus = property.FullBonus;
		
		var actual_bonus = pooled_bonus * full_bonus / 100;

		var first_comma = ", ";
		if (i == 0) first_comma = "";
		description = Format("%s%s<c ff999900>%s</c>: %d+%d", description, first_comma, property.Name, base, actual_bonus);
		
		this[properties[i]] = base + actual_bonus;
		
		quality_hash = 2 * quality_hash + actual_bonus;
	}
	
	this.Description    = this.Description;
	if (had_quality) this.Description = this.Prototype.Description;
	this.Description = Format("%s|%s", description, this.Description);
	this.QualityHash    = this.QualityBonus + 100 * quality_hash;
	
	return true;
}

global func CanBeStackedWith(object other)
{
	if (this.QualityHash != other.QualityHash) return false;
	return inherited(other, ...);
}