#include "Map.h"
#include "Level_Manager.h"

/* Defines show_verbose() */
#include "Debug.h"

#include <string>


/**
 * Constructor: Map
 */
Map::Map(const tilemap_type &tileW, const tilemap_type &tileH)
	: levelManager(NULL)
{
	tileWidth = tileW;
	tileHeight = tileH;

	baseDirectory = "";
	levelpath = "";
}


/**
 * Destructor: Map
 */
Map::~Map()
{
	if (levelManager != NULL)
	{
		delete levelManager;
		levelManager = NULL;
	}
}


/**
 * Function: Set the working, or base, directory under which the Levels will
 * be found (this is what is passed to the Level Manager when the config file
 * will be parsed).
 */
void Map::Set_Base_Directory(const std::string &dir)
{
	baseDirectory = dir;
}


/**
 * Function: Set where the "map#.cfg" config file will be found.
 */
void Map::Set_Config_Path(const std::string &path)
{
	levelpath = path;
}


/**
 * Function: Call the Level Manager to parse the config file and extract all
 * data needed to load individual Levels.  Must be called before
 * "Load(map_id)".
 *
 * Returns: true if all Levels were successfully loaded, false on errors
 */
bool Map::Load_Levels()
{
	bool success = true;

	/* If no Level Manager was created before, create it now */
	if (levelManager == NULL)
	{
		levelManager = new Level_Manager(baseDirectory);
	}

	levelManager->Set_ConfigFile(mapData.mappath);

	if (levelManager->Load_Settings_From_Config() == false)
	{
		verbose_bad(std::cerr, "*ERROR: Load_Settings_From_Config for Level_Manager returned false.\n");
		success = false;
	}

	return success;
}


/**
 * Function: Return the Level Manager used to extract the Level data from the
 * config file.
 *
 * Returns: Pointer to Level Manager
 */
Level_Manager *Map::Get_Level_Manager()
{
	return levelManager;
}


/**
 * Function: Set the Map Data (type std::vector)
 */
void Map::Set_MapData(const MapData &data)
{
	mapData = data;
}


/**
 * Function: Return the Map Data (type std::vector)
 *
 * Returns: std::vector Map Data
 */
MapData Map::Get_MapData()
{
	return mapData;
}
