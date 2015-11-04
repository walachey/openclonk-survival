global func Normal(int mid, int deviation)
{
	mid = mid ?? 500;
	deviation /= 2;
	for (var i = 0; i < 20; ++i)
	{
		mid += RandomX(-deviation, deviation);
	}
	return mid;
}

global func SteepRandom(int mid, int deviation)
{
	mid = mid ?? 500;
	var val = 0;
	for (var i = 0; i < 10; ++i)
		val += Random(deviation + 1);
	return (val - 5 * deviation) + mid;
}

