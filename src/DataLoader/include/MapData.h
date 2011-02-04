#ifndef MapData_h
#define MapData_h


/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

#include <string>
#include <vector>


/* Assuming there won't be more than 65536 tiles found in a single tilemap */
typedef u16 tilemap_type;


/* For now, these can be strings; may be simplified by just being ints */
typedef std::string map_id_type;


/* Used to hold the Map's config file path, ID, and IDs of maps found in all
 * four directions (N, E, S, W)
 */
struct MapData
{
	std::string mappath;
	map_id_type id;
	map_id_type ids_surrounding[4];
};

typedef std::vector<MapData> MapArray;


#endif  //  MapData_h
