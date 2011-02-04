/**
 * Handles loading all of the Maps (rooms) for the current World.
 *
 *  The World Manager handles all of the actual disk IO, leaving "Map" to
 *  simply be a regular class object.
 *
 */

#include "Map_Manager.h"

/* Defines verbose_good, verbose_bad */
#include "Debug.h"

/* Defines fileExists function */
#include "DiskAccess.h"

/* Defines MapArray */
#include "World.h"

/* Magic function that scans a config file for the first non-linecomment */
#include "Parse_Lines.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>


/**
 * Constructor: Map_Manager, passes base directory to the Manager superclass
 */
Map_Manager::Map_Manager(const std::string &working_directory)
	: workingDirectory(working_directory)
{
	configFile = "";
	tileset_path = "";
	numMapsSpecified = 0;
	invalidID = "MAP_ID_NONE";

	tileWidth = 0;
	tileHeight = 0;
}


/**
 * Destructor: Map_Manager
 */
Map_Manager::~Map_Manager()
{
}


/**
 * Function: The working directory is considered the "root" directory for all
 * derived subdirectories.
 *
 * Returns: std::string Working directory
 */
std::string Map_Manager::Get_WorkingDirectory()
{
	return workingDirectory;
}


/**
 * Function: Set the path to the config file.  The Manager superclass receives
 * the FULL config file path, which includes the working directory as well.
 */
void Map_Manager::Set_ConfigFile(const std::string &config_file)
{
	configFile = config_file;
	Manager::Set_ConfigFile(workingDirectory + config_file);
}


/**
 * Function: Return only the config file's path as was given to this object:
 * not including the working directory.
 *
 * Returns: std::string Config file
 */
std::string Map_Manager::Get_ConfigFile()
{
	return configFile;
}


/**
 * Function: Return the subdirectory specified in the config file
 * (specifically, via the Read_Map_Path() function).
 *
 * Returns: std::string Subdirectory
 */
std::string Map_Manager::Get_SubDirectory()
{
	return subDirectory;
}


/**
 * Function: Set the width of all tiles to be found within this Map.
 */
void Map_Manager::Set_Tile_Width(const u32 &width)
{
	tileWidth = width;
}


/**
 * Function: Return the width of all tiles found within this Map.
 *
 * Returns: unsigned int Tile width
 */
u32 Map_Manager::Get_Tile_Width()
{
	return tileWidth;
}


/**
 * Function: Set the height of all tiles to be found within this Map.
 */
void Map_Manager::Set_Tile_Height(const u32 &height)
{
	tileHeight = height;
}


/**
 * Function: Return the height of all tiles found within this Map.
 *
 * Returns: unsigned int Tile height
 */
u32 Map_Manager::Get_Tile_Height()
{
	return tileHeight;
}


/**
 * Function: Return the Map array (contains an entry for each Level that can
 * be found within this Map: data includes Level ID, tile dimensions, number
 * of maps in this World, IDs of each Map, and linkage between Maps),
 * specifically, via the Read_Map_Config() function.
 *
 * Returns: std::vector Map Array
 */
MapArray Map_Manager::Get_MapArray()
{
	return mapArray;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "tilemap_config"
 *
 * Format:
 * - [width] [height]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Map_Manager::Read_Tilemap_Config(std::ifstream &fs)
{
	std::string line_tilemap_config;

	if (seek_non_comment_line(fs, line_tilemap_config))
	{
		std::stringstream ss(line_tilemap_config);
		ss >> tileWidth;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in World's width.\n");
			return false;
		}
		ss >> tileHeight;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in World's height.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"%s\" has specified an invalid value after \"tilemap_config\": tileset dimensions.\n", Manager::Get_ConfigFile().c_str());
		return false;
	}

	if (seek_non_comment_line(fs, line_tilemap_config))
	{
		std::stringstream ss(line_tilemap_config);

		ss >> tileset_path;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: tileset filename (like \"tileset_placeholder.png\") could not be found.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"%s\" has specified an invalid value after \"tilemap_config\": tileset filename.\n", Manager::Get_ConfigFile().c_str());
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "num_maps"
 *
 * Format:
 * - [number of levels (integer)]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Map_Manager::Read_Num_Maps(std::ifstream &fs)
{
	std::string line_num_maps;

	if (seek_non_comment_line(fs, line_num_maps))
	{
		std::stringstream ss(line_num_maps);

		ss >> numMapsSpecified;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in number of maps.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"%s\" has specified an invalid value after \"num_maps\"\n", Manager::Get_ConfigFile().c_str());
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that specifies which
 * map ID is considered "invalid", under the "invalid_map_id" header
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Map_Manager::Read_Invalid_Map_ID(std::ifstream &fs)
{
	std::string line_invalid_map_id;

	if (seek_non_comment_line(fs, line_invalid_map_id))
	{
		std::stringstream ss(line_invalid_map_id);

		ss >> invalidID;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: invalid_map_id (like \"MAP_ID_NONE\") could not be found.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"%s\" has specified an invalid value after \"invalid_map_id\"\n", Manager::Get_ConfigFile().c_str());
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "world_path"
 *
 * Format:
 * - [path to directory containing "map#.cfg" and "map#_triggers.cfg" files]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Map_Manager::Read_World_Path(std::ifstream &fs)
{
	std::string line_world_path;

	if (seek_non_comment_line(fs, line_world_path))
	{
		std::stringstream ss(line_world_path);

		ss >> subDirectory;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: subdirectory (like \"world#/\") where \"map#.cfg\" and \"map#_triggers.cfg\" are contained could not be found.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"%s\" has specified an invalid value after \"world_path\"\n", Manager::Get_ConfigFile().c_str());
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "map_config"
 *
 * Format:
 * - [map ID]
 * - [path to "map#.cfg" file]
 * - [path to "map#_triggers.cfg" file]
 * - [parallax horz] [parallax vert]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Map_Manager::Read_Map_Config(std::ifstream &fs)
{
	std::string line_maps_config;

	MapData mapData;

	if (seek_non_comment_line(fs, line_maps_config))
	{
		std::stringstream ss(line_maps_config);

		ss >> mapData.mappath;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in path for \"map#.cfg\" file.\n");
			return false;
		}
	}

	if (seek_non_comment_line(fs, line_maps_config))
	{
		std::stringstream ss(line_maps_config);

		ss >> mapData.id;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in map's ID.\n");
			return false;
		}
	}

	for (int direction = 0; direction < 4; ++direction)
	{
		// Scan in NORTH, EAST, SOUTH, then WEST IDs
		if (seek_non_comment_line(fs, line_maps_config))
		{
			std::stringstream ss(line_maps_config);

			ss >> mapData.ids_surrounding[direction];
			if (ss.fail())
			{
				verbose_bad(std::cerr, "*ERROR: Could not read in ID for one of the surrounding maps.\n");
				return false;
			}
		}
	}

	mapArray.push_back(mapData);

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Load maps from the "map#.cfg" config file
 *
 *  The "map#.cfg" file should have been saved to Manager before calling
 *  this function.
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Map_Manager::Load_Settings_From_Config()
{
	/* Assume success unless otherwise indicated */
	bool success = true;

	std::string fullFilePath = Manager::Get_ConfigFile();

	/* If the file doesn't exist, or if the dimensions of the tiles are not
	 * defined, return with an error.
	 */
	if (fileExists(fullFilePath) == false || tileWidth == 0 || tileHeight == 0)
	{
		success = false;
		return success;
	}

	std::ifstream fs(fullFilePath.c_str());
	if (!fs.is_open())
	{
		verbose_bad(std::cerr, "Could not load file: \"%s\"\n", fullFilePath.c_str());
		success = false;
	}

	std::string line;
	while (success != false && seek_non_comment_line(fs, line))
	{
		if (line == "tilemap_config")
		{
			success = Read_Tilemap_Config(fs);
			continue;
		}

		else if (line == "num_maps")
		{
			success = Read_Num_Maps(fs);
			continue;
		}

		else if (line == "invalid_map_id")
		{
			success = Read_Invalid_Map_ID(fs);
			continue;
		}

		else if (line == "world_path")
		{
			success = Read_World_Path(fs);
			continue;
		}

		else if (line == "map_config")
		{
			success = Read_Map_Config(fs);
			continue;
		}

		else
		{
			// Something was here that we weren't expecting.  Exit gracefully?  Or just return false?
			success = false;
		}
	}

	fs.close();
	return success;
}


/**
 * Function: Output all the settings saved in this manager (mostly paths and
 * stuff) to the specified stream.  This is mostly a debugging tool to show
 * that the data were collected correctly.
 *
 * Flags: - If "debug" is specified before calling this function, extra non-
 *          error debugging info is displayed.
 *        - If "verbose" is specified before calling this function, extra
 *          error debugging info is displayed (good for tracking down "what
 *          didn't load").
 */
void Map_Manager::Show_All(std::ostream &os)
{
	u32 numMaps = mapArray.size();
	if (numMaps == 0)
	{
		/* No Map data were loaded; no reason to print anything here */
		return;
	}

	verbose_good(os, "Map_Manager:\n");

	/* Show the main manager's Show_All() function before our own. */
	Manager::Show_All(os);

	os << "\nMap_Manager::Show_All()\n";

	verbose_good(os, "\tDimensions (width x height) for tiles: %d x %d\n", tileWidth, tileHeight);

	verbose_good(os, "\t%d maps found in this config file:\n", numMaps);

	for (u32 i = 0; i < numMaps; ++i)
	{
		os << "\tMap " << i << ":\n";

		MapData mapData = mapArray[i];
		os << "\t\tConfig path: " << mapData.mappath << std::endl;
		os << "\t\tID: " << mapData.id << std::endl;
		os << "\t\tMap NORTH ID: " << mapData.ids_surrounding[0] << std::endl;
		os << "\t\tMap EAST ID: " << mapData.ids_surrounding[1] << std::endl;
		os << "\t\tMap SOUTH ID: " << mapData.ids_surrounding[2] << std::endl;
		os << "\t\tMap WEST ID: " << mapData.ids_surrounding[3] << std::endl;
		os << std::endl;
	}
}
