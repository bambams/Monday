#ifndef Map_h
#define Map_h


/* Defines MapData, MapArray */
#include "MapData.h"

#include "Level_Manager.h"

#include <string>
#include <vector>


class Map
{
	public:
		Map(const tilemap_type &tileW, const tilemap_type &tileH);
		virtual ~Map();

		virtual void Set_Base_Directory(const std::string &dir);
		virtual void Set_Config_Path(const std::string &path);

		virtual bool Load_Levels();

		Level_Manager *Get_Level_Manager();

		void Set_MapData(const MapData &data);
		MapData Get_MapData();

	protected:
		tilemap_type tileWidth, tileHeight;

		/* Contains "./media/world#/" */
		std::string baseDirectory;

		/* Contains "map#.cfg" */
		std::string levelpath;

		Level_Manager *levelManager;

		MapData mapData;
};


#endif  //  Map_h
