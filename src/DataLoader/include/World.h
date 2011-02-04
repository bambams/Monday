#ifndef World_h
#define World_h


/* Defines Map_Manager */
#include "Map_Manager.h"

/* Defines parallax_type, MapData, MapArray */
#include "MapData.h"

#include <string>
#include <vector>


class World
{
	public:
		World();
		virtual ~World();

		virtual void Set_Base_Directory(const std::string &dir);
		virtual void Set_Config_Path(const std::string &path);
		virtual bool Load_Maps(const u32 &tileWidth, const u32 &tileHeight);

		Map_Manager *Get_Map_Manager();
		MapArray GetMapArray();

	protected:
		/* Contains "./media/world#/" */
		std::string baseDirectory;

		/* Contains "world#.cfg" */
		std::string mappath;

		Map_Manager *mapManager;
		MapArray mapArray;
};


#endif  //  World_h
