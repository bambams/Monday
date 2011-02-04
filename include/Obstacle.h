#ifndef Obstacle_h
#define Obstacle_h

#include "Vector.h"

class Obstacle;


class Collision_data
{
	public:
		Vector adjust;
		Obstacle *obstacle;
	private:
};


class Obstacle
{
	public:
		enum Obstacle_Directions
		{
			NORTH = 0x01,
			WEST  = 0x02,
			EAST  = 0x04,
			SOUTH = 0x08
		};

		/* Most of these won't ever be used, except BLOCK_NONE, BLOCK_ALL, or
		 * BLOCK_NSWE; it's easiest just to bitwise-AND them with the
		 * Obstacle_Directions above:
		 *    BLOCK_NE is the same as (target & (NORTH | EAST))
		 *    BLOCK_NSW is the same as (target & (NORTH | WEST | SOUTH))
		 */
		enum Obstacle_Types
		{
			BLOCK_NONE = 0,  /* Nothing blocked South, West, East or North */

			BLOCK_N    = 1,  /* 0001, Nwes */
			BLOCK_W    = 2,  /* 0010, nWes */
			BLOCK_NW   = 3,  /* 0011, NWes */
			BLOCK_E    = 4,  /* 0100, nwEs */
			BLOCK_NE   = 5,  /* 0101, NwEs */
			BLOCK_WE   = 6,  /* 0110, nWEs */
			BLOCK_NWE  = 7,  /* 0111, NWEs */
			BLOCK_S    = 8,  /* 1000, nweS */
			BLOCK_NS   = 9,  /* 1001, NweS */
			BLOCK_SW   = 10, /* 1010, nWeS */
			BLOCK_NSW  = 11, /* 1011, NWeS */
			BLOCK_SE   = 12, /* 1100, nwES */
			BLOCK_NSE  = 13, /* 1101, NwES */
			BLOCK_SWE  = 14, /* 1110, nWES */
			BLOCK_NSWE = 15, /* 1111, NWES */

			BLOCK_ALL = BLOCK_NSWE,

			NUM_OBSTACLES    /* Must always be last entry in enum */
		};

	public:
		virtual ~Obstacle();
		virtual bool Line_collision(const Vector &line_begin, const Vector &line_end, Collision_data &data) = 0;
		virtual bool Circle_collision(const Vector &center, float radius, Collision_data &data) = 0;
		virtual bool Point_blocked(const Vector &position, const Vector &from) = 0;
	private:
};


#endif  //  Obstacle_h
