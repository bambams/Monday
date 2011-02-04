#ifndef Tileset_h
#define Tileset_h

#include <string>
#include <allegro5/allegro5.h>
#include "Resource_manager.h"

/* Class: Tile
 * Holds information about a single tile.
 * The <Map> passes these to the <Tileset> for rendering.
 * */
class Tile
{
public:
	Tile(int i)
	:index(i){}
	int index;
};

/* Class: Tileset
 * Holds a set of tile graphics.
 * */
class Tileset
{
public:
	/* Constructor: Tileset
	 * */
	Tileset();

	/* Destructor: Tileset
	 * */
	~Tileset();

	/* Function: Set_Dimensions
	 * Sets the width and height of tiles within this tileset
	 */
	void Set_Dimensions(const int& tilew, const int& tileh);
	
	/* Function: Load
	 * Loads tileset from file
	 * 
	 * Parameters:
	 * filename - file to load
	 * resource_manager - used to load images
	 * */
	void Load(const std::string& filename, Resource_manager& resource_manager);

	/* Function: Render
	 * Renders a tile at a given position
	 * */
	void Render(Tile &tile, float x, float y) const;
private:
	int tile_w;
	int tile_h;
	int num_tiles_x;
	int num_tiles_y;
	ALLEGRO_BITMAP* tileset;
};

#endif
