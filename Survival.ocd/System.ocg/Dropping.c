
func GetDroppableItems()
{
	return
	{
		common = 
		{
			rarity = 100,
			objects = [nil]
		},
		medium = 
		{
			rarity = 500,
			objects = [nil]
		},
		rare =
		{
			rarity = 900,
			objects = [nil]
		}
	};
}

func CalculateLoot(object to)
{
	var overall_drop = Random(1000);
	overall_drop = BoundBy((35 - Sqrt(1000 - overall_drop)) * 3, 0, 100);
	
	// overall drop is now between 0 and 100
	var possible_loot = GetDroppableItems();
	var categories = GetProperties(possible_loot);
	
	while (overall_drop > 0)
	{
		// degenerate drop? :<
		if (!Random(2)) --overall_drop;
		
		// check random category
		var random_category = categories[Random(GetLength(categories))];
		var rarity = possible_loot[random_category].rarity;
		
		// check rollan'
		if (rarity < Random(1000)) continue;
		
		// now get random object form category
		var objects = possible_loot[random_category].objects;
		var ID = objects[Random(GetLength(objects))];
		
		to->CreateContents(ID);
	}
}