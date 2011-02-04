#ifndef Tileset_h
#define Tileset_h

#include <string>
#include <allegro5/allegro5.h>
#include "Resource_manager.h"

class Tile
{
public:
	Tile(int i)
	:index(i){}
	int index;
};

class Tileset
{
public:
	Tileset();
	~Tileset();
	int Tile_width();
	int Tile_height();
	void Load(const std::string& filename, Resource_manager& resource_manager);
	void Render(Tile &tile, float x, float y) const;
private:
	int tile_width;
	int tile_height;
	int num_tiles_x;
	int num_tiles_y;
	ALLEGRO_BITMAP* tileset;
};

#endif
