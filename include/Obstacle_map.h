#ifndef Obstacle_map_h
#define Obstacle_map_h

#include "Obstacle.h"

#include <vector>
#include <fstream>


class Obstacle_map:
	public Obstacle
{
	public:
		typedef std::vector<int> Column;
		typedef std::vector<Column> Tilemap;

		Obstacle_map();
		virtual ~Obstacle_map();

		bool Load(std::ifstream &data);

		virtual bool Line_collision(const Vector &line_begin, const Vector &line_end, Collision_data &data);
		virtual bool Circle_collision(const Vector &center, float radius, Collision_data &data);
		virtual bool Point_blocked(const Vector &position, const Vector &from);

	private:
		bool Tile_blocked(unsigned int x, unsigned int y, unsigned int fromX, unsigned int fromY);

		Tilemap tilemap;
		int tilesize_x;
		int tilesize_y;
};


#endif  //  Obstacle_map_h
