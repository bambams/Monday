/**
 * Handles loading all of the Levels (floors) for the current Map
 *
 *  The Map Manager handles all of the actual disk IO, leaving "Level" to
 *  simply be a regular class object.
 *
 */

#include "Level_Manager.h"

/* Defines verbose_bad, verbose_good ) */
#include "Debug.h"

/* Defines fileExists function */
#include "DiskAccess.h"

/* Defines LevelArray */
#include "Map.h"

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
 * Constructor: Level_Manager, passes base directory to the Manager superclass
 */
Level_Manager::Level_Manager(const std::string &working_directory)
	: workingDirectory(working_directory)
{
	subDirectory = "";
	numLevelsSpecified = 0;

	numRows = 0;
	numColumns = 0;
}


/**
 * Destructor: Level_Manager
 */
Level_Manager::~Level_Manager()
{
}


/**
 * Function: The working directory is considered the "root" directory for all
 * derived subdirectories.
 *
 * Returns: std::string Working directory
 */
std::string Level_Manager::Get_WorkingDirectory()
{
	return workingDirectory;
}


/**
 * Function: Set the path to the config file.  The Manager superclass receives
 * the FULL config file path, which includes the working directory as well.
 */
void Level_Manager::Set_ConfigFile(const std::string &config_file)
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
std::string Level_Manager::Get_ConfigFile()
{
	return configFile;
}


/**
 * Function: Return the subdirectory specified in the config file
 * (specifically, via the Read_Map_Path() function).
 *
 * Returns: std::string Subdirectory
 */
std::string Level_Manager::Get_SubDirectory()
{
	return subDirectory;
}


/**
 * Function: Return the number of rows specified in the config file
 * (specifically, via the Read_Map_Dimensions() function).
 *
 * Returns: unsigned int Number of Rows
 */
u32 Level_Manager::Get_Num_Rows()
{
	return numRows;
}


/**
 * Function: Return the number of columns specified in the config file
 * (specifically, via the Read_Map_Dimensions() function).
 *
 * Returns: unsigned int Number of Columns
 */
u32 Level_Manager::Get_Num_Columns()
{
	return numColumns;
}


/**
 * Function: Return the Level array (contains an entry for each Layer that can
 * be found within this Level: data includes Layer ID, path to its config
 * file, path to the Level's Trigger config file, and parallax information),
 * specifically, via the Read_Levels_Config() function.
 *
 * Returns: std::vector Level Array
 */
LevelArray Level_Manager::Get_LevelArray()
{
	return levelArray;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "map_dimensions"
 *
 * Format:
 * - [rows] [columns]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Level_Manager::Read_Map_Dimensions(std::ifstream &fs)
{
	std::string line_map_dimensions;

	if (seek_non_comment_line(fs, line_map_dimensions))
	{
		std::stringstream ss(line_map_dimensions);
		ss >> numRows;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in Map's rows.\n");
			return false;
		}
		ss >> numColumns;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in Map's columns.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"%s\" has specified an invalid value after \"map_dimensions\"\n", Manager::Get_ConfigFile().c_str());
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "num_levels"
 *
 * Format:
 * - [number of layers (integer)]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Level_Manager::Read_Num_Levels(std::ifstream &fs)
{
	std::string line_num_levels;

	if (seek_non_comment_line(fs, line_num_levels))
	{
		std::stringstream ss(line_num_levels);

		ss >> numLevelsSpecified;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in number of levels.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"%s\" has specified an invalid value after \"num_levels\"\n", Manager::Get_ConfigFile().c_str());
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "map_path"
 *
 * Format:
 * - [path to directory containing "level#.cfg" and "level#_triggers.cfg" files]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Level_Manager::Read_Map_Path(std::ifstream &fs)
{
	std::string line_map_path;

	if (seek_non_comment_line(fs, line_map_path))
	{
		std::stringstream ss(line_map_path);

		ss >> subDirectory;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: subdirectory (like \"map#/\") where \"level#.cfg\" and \"level#_triggers.cfg\" are contained could not be found.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"%s\" has specified an invalid value after \"map_path\"\n", Manager::Get_ConfigFile().c_str());
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "levels_config"
 *
 * Format:
 * - [level ID]
 * - [path to "level#.cfg" file]
 * - [path to "level#_triggers.cfg" file]
 * - [parallax horz] [parallax vert]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Level_Manager::Read_Levels_Config(std::ifstream &fs)
{
	std::string line_levels_config;

	LevelData levelData;

	if (seek_non_comment_line(fs, line_levels_config))
	{
		std::stringstream ss(line_levels_config);

		ss >> levelData.id;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in level's ID.\n");
			return false;
		}
	}

	if (seek_non_comment_line(fs, line_levels_config))
	{
		std::stringstream ss(line_levels_config);

		ss >> levelData.levelpath;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in path for \"level#.cfg\" file.\n");
			return false;
		}
	}

	if (seek_non_comment_line(fs, line_levels_config))
	{
		std::stringstream ss(line_levels_config);

		ss >> levelData.triggerpath;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in path for \"level#_triggers.cfg\" file.\n");
			return false;
		}
	}

	if (seek_non_comment_line(fs, line_levels_config))
	{
		std::stringstream ss(line_levels_config);

		ss >> levelData.parallaxHorz;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in horizontal parallax value.\n");
			return false;
		}

		ss >> levelData.parallaxVert;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in vertical parallax value.\n");
			return false;
		}
	}

	levelArray.push_back(levelData);

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Load levels from the "level#.cfg" config file
 *
 *  The "level#.cfg" file should have been saved to Manager before calling
 *  this function.
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Level_Manager::Load_Settings_From_Config()
{
	/* Assume success unless otherwise indicated */
	bool success = true;

	std::string fullFilePath = Manager::Get_ConfigFile();

	/* If the file doesn't exist, or if the dimensions of the tiles are not
	 * defined, return with an error.
	 */
	if (fileExists(fullFilePath) == false)
	{
		verbose_bad(std::cerr, "fileExists(\"%s\") returned false.\n", fullFilePath.c_str());
		success = false;
	}

	std::ifstream fs(fullFilePath.c_str());
	if (!fs.is_open())
	{
		verbose_bad(std::cerr, "Could not load file: %s\n", fullFilePath.c_str());
		success = false;
	}

	std::string line;
	while (success != false && seek_non_comment_line(fs, line))
	{
		if (line == "map_dimensions")
		{
			success = Read_Map_Dimensions(fs);
			continue;
		}

		else if (line == "num_levels")
		{
			success = Read_Num_Levels(fs);
			continue;
		}

		else if (line == "map_path")
		{
			success = Read_Map_Path(fs);
			continue;
		}

		else if (line == "levels_config")
		{
			success = Read_Levels_Config(fs);
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
void Level_Manager::Show_All(std::ostream &os)
{
	u32 numLevels = levelArray.size();
	if (numLevels == 0)
	{
		/* No Level data were loaded; no reason to print anything here */
		return;
	}

	verbose_good(os, "Level_Manager:\n");

	/* Show the main manager's Show_All() function before our own. */
	Manager::Show_All(os);

	os << "\nLevel_Manager::Show_All()\n";

	verbose_good(os, "\tDimensions (rows x columns) for layers: %d x %d\n", numRows, numColumns);

	verbose_good(os, "\t%d levels found in this config file:\n", numLevels);

	for (u32 i = 0; i < numLevels; ++i)
	{
		os << "\tLevel " << i << ":\n";

		LevelData levelData = levelArray[i];
		os << "\t\tID: " << levelData.id << std::endl;
		os << "\t\tConfig path: " << levelData.levelpath << std::endl;
		os << "\t\tTrigger path: " << levelData.triggerpath << std::endl;
		os << "\t\tParallax: " << levelData.parallaxHorz << "x" << levelData.parallaxVert << std::endl;
	}
}
