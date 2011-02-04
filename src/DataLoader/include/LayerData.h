#ifndef LayerData_h
#define LayerData_h


/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

#include <string>
#include <vector>


/* Defined here, because only layers are worried about what "type" of data
 * the individual tiles are.  Change it here once, and we can save space or
 * allow for much larger tilemaps.
 */
typedef u32 tile_type;
typedef u8  obstacle_type;


/* Used to hold the 2-dimensional Obstacle map array */
typedef std::vector<obstacle_type> ObstacleLine;
typedef std::vector<ObstacleLine> ObstacleMap;


/* Used to hold the 2-dimensional Layer map array */
typedef std::vector<tile_type> LayerLine;
typedef std::vector<LayerLine> LayerMap;


/* For now, these can be strings; may be simplified by just being ints */
typedef std::string layer_id_type;


/* Used to hold the obstacles, tiles, and ID */
struct LayerData
{
	std::vector<ObstacleLine> obstacle;

	layer_id_type id;
	std::vector<LayerMap> layer;
};

typedef std::vector<LayerData> LayerArray;


#endif  //  LayerData_h
