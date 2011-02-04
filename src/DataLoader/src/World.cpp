#include "World.h"
#include "Map_Manager.h"

#include "Debug.h"

#include <string>


/**
 * Constructor: World
 */
World::World()
	: mapManager(NULL)
{
	baseDirectory = "";
	mappath = "";
}


/**
 * Destructor: World
 */
World::~World()
{
	if (mapManager != NULL)
	{
		delete mapManager;
		mapManager = NULL;
	}
}


/**
 * Function: Set the working, or base, directory under which the Maps will
 * be found (this is what is passed to the Map Manager when the config file
 * will be parsed).
 */
void World::Set_Base_Directory(const std::string &dir)
{
	baseDirectory = dir;
}


/**
 * Function: Set where the "world#.cfg" config file will be found.
 */
void World::Set_Config_Path(const std::string &path)
{
	mappath = path;
}


/**
 * Function: Call the Map Manager to parse the config file and extract all
 * data needed to load individual Maps.  Must be called before
 * "Load(world_id)".
 *
 * Returns: true if all Maps were successfully loaded, false on errors
 */
bool World::Load_Maps(const u32 &tileWidth, const u32 &tileHeight)
{
	bool success = false;

	/* If no Map Manager was created before, create it now */
	if (mapManager == NULL)
	{
		mapManager = new Map_Manager(baseDirectory);
	}

	mapManager->Set_ConfigFile(mappath);
	mapManager->Set_Tile_Width(tileWidth);
	mapManager->Set_Tile_Height(tileHeight);

	if (mapManager->Load_Settings_From_Config() == true)
	{
		mapArray = mapManager->Get_MapArray();
		success = true;
	}

	return success;
}


/**
 * Function: Return the Map Manager used to extract the Map data from the
 * config file.
 *
 * Returns: Pointer to Map Manager
 */
Map_Manager *World::Get_Map_Manager()
{
	return mapManager;
}


/**
 * Function: Return the World Array (type std::vector) which holds all of the
 * Maps contained within the World.
 *
 * Returns: std::vector Map array
 */
MapArray World::GetMapArray()
{
	return mapArray;
}
