#ifndef LevelData_h
#define LevelData_h


/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

#include <string>
#include <vector>


/* Not sure yet what kind of data type parallax needs: sticking with float */
typedef float parallax_type;


/* For now, these can be strings; may be simplified by just being ints */
typedef std::string level_id_type;


/* Used to hold the Level's ID, config file path, trigger config file path,
 * and parallax options
 */
struct LevelData
{
	level_id_type id;
	std::string levelpath;
	std::string triggerpath;
	parallax_type parallaxHorz;
	parallax_type parallaxVert;
};

typedef std::vector<LevelData> LevelArray;


#endif  //  LevelData_h
