#ifndef Obstacle_map_h
#define Obstacle_map_h

#include "Obstacle.h"

#include <vector>
#include <fstream>

/* Class: Obstacle_map
 * Provides collision information for a tilemap.
 */
class Obstacle_map:
	public Obstacle
{
	public:
		typedef std::vector<bool> Column;
		typedef std::vector<Column> Tilemap;

		/* Constructor: Obstacle_map
		 */
		Obstacle_map();

		/* Function: Load
		 * Parameters:
		 * data - filestream to load from
		 */
		void Load(std::ifstream& data);

		/* Function: Line_collision
		 * For collision handling
		 */
		virtual bool Line_collision(const Vector& line_begin, const Vector& line_end, Collision_data& data);

		/* Function: Circle_collision
		 * For collision handling
		 */
		virtual bool Circle_collision(const Vector& center, float radius, Collision_data& data);

		/* Function: Point_blocked
		 * For collision handling
		 */
		virtual bool Point_blocked(const Vector& position);

		/* Function: Set_Dimensions
		 * Sets the width and height of tiles within this obstacle map
		 */
		void Set_Dimensions(const int& tilew, const int& tileh);

		/* Section: Private
		 */
	private:

		/* Function: Tile_blocked
		 * Helper function checking if a tile is blocked.
		 */
		bool Tile_blocked(unsigned int x, unsigned int y);

		Tilemap tilemap;
		int tile_w;
		int tile_h;
};

#endif
