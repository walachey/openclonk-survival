static Biomes;
static Biomes_SearchTree;

private func InitBiomes()
{
	if (Biomes) FatalError("Calling InitBiomes twice is not allowed");
	Biomes = {
		Shore = {
			ID = 1 << 0,
		}
	};
	
	// Initialize some basic properties.
	for (var biome in GetBiomeDefinitions())
	{
		biome.Locations = [];
		biome.Construction = Global.Biome_Construction;
	}
}

global func GetBiomeDefinitions()
{
	var props = GetProperties(Biomes);
	var results = CreateArray(GetLength(props));
	for (var cnt = 0; cnt < GetLength(props); ++cnt)
	{
		var biome = Biomes[props[cnt]];
		PushBack(results, biome);
	}
	return results;
}

global func Biome_Construction(int x, int y, int wdt, int hgt)
{
	if (!this.Prototype) return FatalError("Must be called on child instance.");
	PushBack(this.Prototype.Locations, this);
	this.Shape = Shape->Rectangle(x, y, wdt, hgt);
}

global func GetBiomes(int x, int y, int ID)
{
	ID = ID ?? 0xffffffff;
	var point = {X = x, Y = y};
	var current_node = Biomes_SearchTree;
	
	// Descend the tree!
	while (true)
	{
		if (current_node.Biomes & ID == 0) break;
		if (!current_node.Left || !current_node.Right) break;
		
		if (point[current_node.Split] <= current_node.Threshold)
			current_node = current_node.Left;
		else current_node = current_node.Right;
	}
	if (!current_node) return [];
	
	var biomes = [];
	for (var location in current_node.Locations)
	{
		if (location.ID & ID == 0) continue;
		PushBack(biomes, location);
	}
	return biomes;
}

private func Biomes_BuildKDTree()
{
	var all_locations = [];
	for (var def in GetBiomeDefinitions())
	{
		for (var location in def.Locations)
			PushBack(all_locations, location);
	}
	
	SortArrayByProperty(all_locations, "X", false);
	// Add root node.
	Biomes_SearchTree = 
	{
		Locations = all_locations,
		Split = "X",
		Biomes = Biomes_BuildKDTreeGetBiomesTag(all_locations)
	};
	Biomes_BuildKDTreeContinueNode(Biomes_SearchTree);
}

private func Biomes_BuildKDTreeGetBiomesTag(array locations)
{
	var value = 0;
	for (var loc in locations)
		value |= loc.ID;
	return value;
}

private func Biomes_BuildKDTreeContinueNode(proplist node)
{
	var location_length = GetLength(node.Locations);
	if (location_length <= 1) return;
	
	var next_split = "X";
	var wdthgt = "Wdt";
	if (node.Split == "X")
	{
		next_split = "Y";
		wdthgt = "Hgt";
	}
	
	// Split at median of (pre-sorted) list.
	var median = location_length / 2;
	if (median == 0) return;
	
	node.Threshold = node.Locations[median][node.Split];
	var left = [];
	var right = [];
	
	for (var location in node.Locations)
	{
		if (location[node.Split] <= node.Threshold)
		{
			PushBack(left, location);
		}
		
		if (location[node.Split] + location[wdthgt] > node.Threshold)
		{
			PushBack(right, location);
		}
	}
	
	if (GetLength(left) == 0 || GetLength(right) == 0) return;
	SortArrayByProperty(left, next_split, false);
	SortArrayByProperty(right, next_split, false);
	
	node.Left = 
	{
		Location = left,
		Split = next_split,
		Biomes = Biomes_BuildKDTreeGetBiomesTag(left)
	};
	node.Right = 
	{
		Location = right,
		Split = next_split,
		Biomes = Biomes_BuildKDTreeGetBiomesTag(right)
	};
	Biomes_BuildKDTreeContinueNode(node.Left);
	Biomes_BuildKDTreeContinueNode(node.Right);
}