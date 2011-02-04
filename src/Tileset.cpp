#include "Tileset.h"
#include <fstream>
#include <iostream>
#include "Debug.h"
#include "Image_resource.h"

Tileset::Tileset()
:tileset(NULL)
{
}


Tileset::~Tileset()
{
}


int Tileset::Tile_width()
{
	return tile_width;
}


int Tileset::Tile_height()
{
	return tile_height;
}


void Tileset::Load(const std::string &filename, Resource_manager *resource_manager)
{
	std::ifstream fs;
	fs.open(filename.c_str());
	std::string line;
	std::getline(fs, line);
	Image_resource *image = resource_manager->Load<Image_resource>(line);
	if (image != NULL)
	{
		tileset = image->Get_allegro_bitmap();
	}
	fs >> tile_width;
	fs >> tile_height;
	fs.close();

	num_tiles_x = al_get_bitmap_width(tileset) / tile_width;
	num_tiles_y = al_get_bitmap_height(tileset) / tile_height;

	std::cout << num_tiles_x << " " << num_tiles_y << " " << line << std::endl;

	assert(tileset);
}


void Tileset::Render(Tile &tile, float x, float y) const
{
	int index = tile.index;
	if (index>=num_tiles_x *num_tiles_y)
		return;
	int tile_y = index/num_tiles_x;
	int tile_x = index-tile_y *num_tiles_x;
	tile_y *= tile_height;
	tile_x *= tile_width;

	al_draw_bitmap_region(tileset, tile_x, tile_y, tile_width, tile_height, x, y, 0);
}
