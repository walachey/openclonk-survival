
static LootTable;
global func LootTable_Init()
{
 	LootTable = {
		Initialized = false,
		Items = [
			{Rarity =   10, Item = Fiber},
			{Rarity =   10, Item = Flintstones},
			{Rarity =   50, Item = MushroomPowder},
			{Rarity =   75, Item = Mushroom},
			{Rarity =   75, Item = Sproutberry},
			{Rarity =  100, Item = StoneKnife},
			{Rarity =  100, Item = PrimitiveSpikes},
			{Rarity =  200, Item = CookedMushroom},
			{Rarity =  200, Item = WoodenKey},
			{Rarity =  250, Item = Balloon},
			{Rarity =  250, Item = Loam},
			{Rarity =  300, Item = Dynamite},
			{Rarity =  300, Item = Arrow},
			{Rarity =  300, Item = PrimitiveDarts},
			{Rarity =  350, Item = Sickle},
			{Rarity =  400, Item = Bow},
			{Rarity =  500, Item = SilverKey},
			{Rarity =  550, Item = Lantern},
			{Rarity =  600, Item = Bread},
			{Rarity =  650, Item = PowderKeg},
			{Rarity =  650, Item = Boompack},
			{Rarity =  700, Item = GoldenKey},
		],
		Events = [
			//{Frequency = 1000, LootBonus = 0, Callback = }
		]
	};

	LootTable.Initialized = true;
	var sum = 0;
	for (var event in LootTable.Events)
		sum += event.Frequency;
	LootTable.TotalEventFrequency = sum;
}

global func AssignChestLootRarity(int base_rarity)
{
	if (!LootTable) LootTable_Init();
	
	AssignLootRarity(base_rarity);
	
	var key_required = !Random(3);
	
	if (this.LootRarity > 700)
	{
		this->SetMeshMaterial("GoldChest");
		if (key_required) this->LockWithKeyQuality("golden");
	}
	else if (this.LootRarity > 400)
	{
		this->SetMeshMaterial("SilverChest");
		if (key_required) this->LockWithKeyQuality("silver");
	}
	else
	{
		if (key_required) this->LockWithKeyQuality("wooden");
	}
	
	if (key_required)
	{
		var bonus = Random(3);
		if (bonus == 0)
			this.LootCount += Random(5);
		else if (bonus == 1)
			this.LootBonus += 200 + Abs(NormalRandom(0, 100));
		else if (bonus == 3)
		{
			this.LootMinIdx += Random(5);
			this.LootMaxIdx = Min(this.LootMaxIdx + Random(5), GetLength(LootTable.Items));
		}
	}
	
	// Now possibly draw a random event.
	if (!Random(5))
	{
		var random_event = Random(LootTable.TotalEventFrequency);
		var event = nil, event_count = GetLength(LootTable.Events);
		for (var i = 0; i < event_count; ++i)
		{
			var candidate = LootTable.Events[i];
			if (candidate.Frequency < random_event) continue;
			event = candidate;
			break;
		}
		if (event)
		{
			this->~AddOpeningCallback(event.Callback);
		
			this.LootBonus += RandomX(event.LootBonus / 2, event.LootBonus);
		}
	}
}

global func AssignLootRarity(int base_rarity)
{
	this.LootRarity = BoundBy(NormalRandom(base_rarity, 100), 0, 1000);
	this.LootCount  = BoundBy(NormalRandom(1000, 500) / 100, 5, 15);
	this.LootBonus  = BoundBy(NormalRandom(1000, 1000) / 10 - 100, 0, 500);
	
	var loot_minimum = this.LootRarity - 100;
	var loot_maximum = this.LootRarity + 100;
	for (var i = 0; i < GetLength(LootTable.Items); ++i)
	{
		var item = LootTable.Items[i];
		if (!item) continue;
		if (item.Rarity < loot_minimum) continue;
		if (this.LootMinIdx == nil) this.LootMinIdx = i;
		if (item.Rarity < loot_maximum) continue;
		this.LootMaxIdx = i;
		break;
	}
}

global func CreateLootContents()
{
	var items = GetLootItems();
	for (var item in items)
		item->Enter(this);
	return items;
}

global func GetLootItems()
{
	var items = [];
	for (var i = 0; i < this.LootCount; ++i)
		PushBack(items, GetLootItem());
	return items;
}

global func GetLootItem()
{
	var range = this.LootMaxIdx - this.LootMinIdx;
	var random_index = this.LootMinIdx + Random(range);
	var item = LootTable.Items[random_index];
	var obj = CreateObject(item.Item, nil, nil, NO_OWNER);
	
	// Can we still upgrade the quality?
	if (this.LootBonus > 0 && obj.QualitySettings)
	{
		var upgrade = Min(this.LootBonus, RandomX(200, 500));
		this.LootBonus -= upgrade;
		obj->SetQualityBonus(upgrade);
	}
	
	return obj;
}