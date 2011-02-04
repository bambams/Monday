
#include "Debug.h"

#include "Tileset.h"

#include "Debug.h"
#include "Image_resource.h"

#include <fstream>
#include <iostream>


Tileset::Tileset()
:tileset(NULL)
{
}


Tileset::~Tileset()
{
}


void Tileset::Set_Dimensions(const int& tilew, const int& tileh)
{
	tile_w = tilew;
	tile_h = tileh;
}


void Tileset::Load(const std::string& filename, Resource_manager& resource_manager)
{
	Monday_out(VERBOSE_LEVEL2, std::cout, "Tileset:Load()\n\tFilename: \"%s\"\n", filename.c_str());
	Image_resource* image = resource_manager.Load<Image_resource>(filename);
	if (NULL != image)
	{
		tileset = image->Get_allegro_bitmap();
	}

	num_tiles_x = al_get_bitmap_width(tileset) / tile_w;
	num_tiles_y = al_get_bitmap_height(tileset) / tile_h;

	mon_assert(tileset);
}


void Tileset::Render(Tile &tile, float x, float y) const
{
	int index = tile.index;
	if (index >= num_tiles_x * num_tiles_y)
	{
		return;
	}

	int tile_y = index / num_tiles_x;
	int tile_x = index - tile_y * num_tiles_x;
	tile_y *= tile_h;
	tile_x *= tile_w;

	al_draw_bitmap_region(tileset, tile_x, tile_y, tile_w, tile_h, x, y, 0);
}
