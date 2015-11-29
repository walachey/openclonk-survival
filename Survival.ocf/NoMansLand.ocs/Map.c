/**
	Vast lands.
	
	@author Zapper
*/

#include Library_Map

static const MAP_W = 1000;
static const MAP_H = 500;
static const GROUND_Y_PROMILLE = 300;

private func P2W(int promille) { return promille * MAP_W / 1000; }
private func P2H(int promille) { return promille * MAP_H / 1000; }
private func A2W(array a) { for (var i = GetLength(a) - 1; i >= 0; --i) a[i] = P2W(a[i]); return a; }
private func A2H(array a) { for (var i = GetLength(a) - 1; i >= 0; --i) a[i] = P2H(a[i]); return a; }

private func MirrorX(array a, offset)
{
	offset = offset ?? 0;
	for (var i = GetLength(a) - 1; i >= 0; --i)
		a[i] = offset - a[i];
	return a;
}

// Called be the engine: draw the complete map here.
protected func InitializeMap(proplist map)
{
	var map_size = [MAP_W, MAP_H];
	map->Resize(map_size[0], map_size[1]);
	Log("Mapsize: (%dx%d)x8", map.Wdt, map.Hgt);
	
	DrawRegularGround(nil, P2H(GROUND_Y_PROMILLE), 10);

	DrawOcean(DIR_Left);
	DrawOcean(DIR_Right);
	
	var desert_layer = CreateLayer(nil, P2W(250), MAP_H);
	DrawDesert(desert_layer, 250);
	Blit({Algo = MAPALGO_Offset, Op=desert_layer, OffX = map.Wdt/2 - desert_layer.Wdt/2, OffY = 0});
	
	var mountain_x = 350;
	var skylands_x = 150;
	var skylands_y = 2 * GROUND_Y_PROMILLE / 3;
	
	for (var mirror = 0; mirror <= 1; ++mirror)
	{
		for (var i = 0; i < 4; ++i)
		{
			var x = mountain_x + RandomX(-50, 50);
			if (mirror) x = 1000 - x;
			DrawMountain(x);
		}
		
		for (var i = 0; i < 10; ++i)
		{
			var x = skylands_x + RandomX(-100, 100);
			if (mirror) x = 1000 - x;
			var y = skylands_y + RandomX(-70, 70);
			DrawSkyland(x, y);
		}
	}
	
	DrawCaves();
	return true;
}

public func DrawCave(int x, int y, int wdt, int hgt)
{
	var cave_layer = CreateLayer("Gold", wdt, hgt);
	var dug_out = cave_layer->DigOutCave();
	
	cave_layer = {Algo = MAPALGO_Filter, Op = cave_layer, Filter="~Gold"};
	cave_layer = {Algo = MAPALGO_Scale, X = 200, Y = 150, Op = cave_layer, OffX = cave_layer.Wdt/2, OffY = cave_layer.Hgt/2};
	cave_layer = {Algo = MAPALGO_Offset, Op=cave_layer, OffX = x, OffY = y};
	
	Blit(cave_layer);
	//cave_layer = {Algo = MAPALGO_Border, Op = cave_layer, Left=2, Right=2, Top=2, Bottom=2};
	//Draw("Tunnel", cave_layer);
	return dug_out;
}

public func DrawCaves()
{
	Log("Generating caves...");
	
	// Predefine some core regions to reduce variance in them.
	var regions = [[250, GROUND_Y_PROMILLE + 30], [400, GROUND_Y_PROMILLE + 200], [300, GROUND_Y_PROMILLE + 300]]; 
	
	for (var mirror = 0; mirror <= 1; ++mirror)
	for (var region in regions)
	{
		var dug_out = 0;
		while (dug_out < (400 * 300) / 20)
		//for (var cave = 0; cave < 10; ++cave)
		{
			var x = P2W(region[0] + RandomX(-150, 150));
			var y = P2H(region[1] + RandomX(-0, 200));
			if (mirror) x = P2W(1000) - x;
			
			x -= P2W(125);
			dug_out += DrawCave(x, y, P2W(RandomX(100, 150)), P2H(RandomX(30, 70)));
		}
	}
	Log("Caves have been carved out!");
	return true;
}

public func DigOutCave()
{
	var center_x = this.Wdt / 2;
	var center_y = this.Hgt / 2;
	var runners = [[center_x, center_y]];
	var runner_count = 1;
	
	var dug_out = 0;
	var possible_x_dirs = [-1, 1, -1, 1, 0, 0];
	var possible_y_dirs = [0, 0, 0, 0, -1, 1];
	var possible_dir_count = 6;
	var tunnel = GetMaterialTextureIndex("Tunnel");
	var iterations = 5 * Max(this.Wdt, this.Hgt);
	for (var i = 0; i < iterations; ++i)
	{
		for (var r = 0; r < GetLength(runners); ++r)
		{
			var runner = runners[r];
			if (!runner) continue;
			
			// Get direction.
			var x_dir = 0, y_dir = 0;
			var r_dir = 0;
			if (!Random(9)) r_dir = r % possible_dir_count;
			else r_dir = Random(possible_dir_count);
			x_dir = possible_x_dirs[r_dir];
			y_dir = possible_y_dirs[r_dir];
			
			var x = runner[0] + x_dir;
			var y = runner[1] + y_dir;
			var solid = GetPixel(x, y) != tunnel;
			var out_of_bounds = (x < 0) || (x > this.Wdt) || (y < 0) || (y > this.Hgt);
			var destroy = out_of_bounds;
			
			runners[r] = [x, y];
			
			if (solid)
			{
				++dug_out;
				this->SetPixel(x, y, tunnel);
				if (!Random(1 + 1 * runner_count) && (runner_count < 32))
				{
					var pos = -1;
					for (var c = 0; c < GetLength(runners); ++c)
					{
						if (runners[c]) continue;
						pos = c;
						break;
					}
					if (pos == -1) PushBack(runners, [x, y]);
					else runners[pos] = [x, y];
					
					runner_count += 1;
				}
			}
			else
			{
				destroy = destroy || !Random(10);
			}
			
			if (destroy)
			{
				if (runner_count <= 1)
				{
					runners[r] = [center_x, center_y];
				}
				else
				{
					runners[r] = nil;
					runner_count -= 1;
				}
			}
		}
	}
	
	// Now perform a smoothing step. Just do it in place, meh.
	for (var smooth = 0; smooth < 1; ++smooth)
	{
		for (var x = 1; x < this.Wdt - 1; ++x)
		for (var y = 1; y < this.Hgt - 1; ++y)
		{
			var free_count = 0;
			if (GetPixel(x - 1, y) == tunnel) ++free_count;
			if (GetPixel(x + 1, y) == tunnel) ++free_count;
			if (GetPixel(x, y - 1) == tunnel) ++free_count;
			if (GetPixel(x, y + 1) == tunnel) ++free_count;
		
			if (free_count >= 3) SetPixel(x, y, tunnel);
		}
	}
	return dug_out;
}

public func DrawSkyland(int x, int y)
{
	x = P2W(x);
	y = P2H(y);
	var width = P2W(RandomX(15, 30));
	var height = P2H(RandomX(5, 20));
	var surface = {Algo = MAPALGO_Ellipsis, X = x, Y = y, Wdt = width, Hgt = height};
	surface = {Algo = MAPALGO_Turbulence, Op = surface, Amplitude = [5, 5], Scale = 10, Iterations = 2};
	Draw("Earth", surface);
	
	if (!Random(2))
		DrawCave(x - width/2, y - height/2, width, height);
}

public func DrawMountain(int center_x)
{
	var center_y = GROUND_Y_PROMILLE + RandomX(-10, 10);
	var width = 25;
	var height = 150 + RandomX(-25, 25);
	var x = A2W([center_x - 2 * width, center_x - width, center_x, center_x + width, center_x + 2 * width, center_x]);
	var y = A2H([center_y + 20, center_y, center_y - height, center_y, center_y + 20, center_y + height]);
	var surface = {Algo = MAPALGO_Polygon, X = x, Y = y};
	surface = {Algo = MAPALGO_Turbulence, Op = surface, Amplitude = [5, 5], Scale = 10, Iterations = 5};
	Draw("Rock", surface);
	var border = {Algo = MAPALGO_Border, Op = surface,  Left = 3, Right = 3, Top = 0, Bottom = 10};
	Draw("Granite", border);
	
	var dug_out = 0;
	while (dug_out < 1000)
	{
		var x = P2W(center_x + RandomX(-10, 10) - 10);
		var y = P2H(center_y + RandomX(-height, height/2));
		dug_out += DrawCave(x, y, P2W(RandomX(5, 15)), P2H(RandomX(5, 15)));
	}
}

public func DrawDesert(proplist layer, int p_width)
{
	var x = A2W([50, p_width - 50, p_width / 2 + 100, p_width/2, p_width / 2 - 100]);
	var y = A2H([GROUND_Y_PROMILLE - 5, GROUND_Y_PROMILLE - 5, GROUND_Y_PROMILLE + 200, GROUND_Y_PROMILLE + 300, GROUND_Y_PROMILLE + 200]);
	var surface = {Algo = MAPALGO_Polygon, X = x, Y = y};
	surface = {Algo = MAPALGO_Turbulence, Op = surface, Amplitude = [50, 10], Scale = 5, Iterations = 1};
	layer->Draw("Sand", surface);
	var border = {Algo = MAPALGO_Border, Op = surface,  Left = 20, Right = 20, Top = 0, Bottom = 100};
	border = {Algo = MAPALGO_Turbulence, Op = border, Amplitude = [25, 0], Scale = 2, Iterations = 5};
	layer->Draw("Sand", border);
}

public func DrawOcean(int dir)
{
	var x = A2W([-50, 100, 50, 25, 12, 0]);
	var y = A2H([GROUND_Y_PROMILLE, GROUND_Y_PROMILLE, GROUND_Y_PROMILLE + 10, GROUND_Y_PROMILLE + 100, GROUND_Y_PROMILLE + 200, GROUND_Y_PROMILLE + 200]);
	if (dir == DIR_Right)
		x = MirrorX(x, P2W(1000));
	var surface = {Algo = MAPALGO_Polygon, X = x, Y = y};
	surface = {Algo = MAPALGO_Turbulence, Op = surface, Amplitude = [25, 0], Scale = 10, Iterations = 3};
	Draw("Water", surface);
}
