/** Test the functionality of the Layer loader.
 *
 *  The purpose of this file is just to make sure that loading a Level in from
 *  a file works properly.
 *
 *  We have to hard-code any values that we don't have which we need; the
 *  values usually come from config files.
 *
 */

#include "Test2_Maps.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

/* Defines show_verbose() */
#include "Debug.h"

/* Magic function that scans a config file for the first non-linecomment */
#include "Parse_Lines.h"

/* Defines parallax_type, LevelData, LevelArray
 */
#include "LevelData.h"

#include "Map.h"
#include "Level.h"

#include "Level_Manager.h"
#include "Layer_Manager.h"

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>


bool enableDebug;
bool enableVerbose;


std::vector<std::string> parseArgs(int argc, char *argv[])
{
	/* Inital values */
	enableDebug = false;
	enableVerbose = false;

	std::vector<std::string> filenames;
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		/* Abbreviated options begin with "-", and full options begin with
		 * "--" to conform to standards.
		 */
		if (arg[0] == '-')
		{
			/* DEBUG */
			if (arg == "-d" || arg == "--debug")
			{
				enableDebug = true;
				verbose_good(std::cerr, "Setting debug ON\n");
				continue;
			}
			else if (arg == "-nd" || arg == "--nodebug")
			{
				verbose_good(std::cerr, "Setting debug OFF\n");
				enableDebug = false;
				continue;
			}

			/* VERBOSE */
			else if (arg == "-v" || arg == "--verbose")
			{
				enableVerbose = true;
				verbose_bad(std::cerr, "Setting verbose ON\n");
				continue;
			}
			else if (arg == "-nv" || arg == "--noverbose")
			{
				verbose_bad(std::cerr, "Setting verbose OFF\n");
				enableVerbose = false;
				continue;
			}
		}

		/* If we got here, we probably have a valid filename. */
		filenames.push_back(arg);
	}

	return filenames;
}


void usage(const std::string &filename)
{
	std::cout << "Usage: " << filename << " filename [-d|-nd|-v|-nv] [--debug|--nodebug|--verbose|--noverbose]\n";
	std::cout << "\tfilename:         Path to the \"world_.cfg\" config file.\n";
	std::cout << "\t-d, --debug:      Turns on extra non-error messages.\n";
	std::cout << "\t-nd, --nodebug:   (default) Turns off extra non-error messages.\n";
	std::cout << "\t-v, --verbose:    Turns on extra error messages.\n";
	std::cout << "\t-nv, --noverbose: (default) Turns off extra error messages.\n";
}


int main(int argc, char *argv[])
{
	std::vector<std::string> filenames = parseArgs(argc, argv);
	if (filenames.size() == 0)
	{
		usage(argv[0]);
		return 0;
	}

	/* Set some initial values for this test program that the program will
	 * need to properly extract the files we need.
	 *
	 * This data is typically pulled in from the config file:
	 *   "./media/world#/map#.cfg"
	 */

	u32 numLevelRows = 14;
	u32 numLevelColumns = 21;

	u32 tileWidth = 32;
	u32 tileHeight = 32;

	/* Points to the "./media/world0/" base path */
	std::string currentDirectory = "./media/world0/";

	for (u32 inputFiles = 0; inputFiles < filenames.size(); ++inputFiles)
	{
		/* Tell the Map where the file is (relative to the Map's base) where
		 * the "map#.cfg" file can be found.
		 */
		MapData mapData;
		mapData.mappath = filenames[inputFiles];

		/* Load the Map:
		 * - Load each Level through the Map_Manager
		 * - Then call the manager's Show_All() function to display everything
		 *   to the screen
		 */
		Map map(tileWidth, tileHeight);
		map.Set_MapData(mapData);

		std::string mapBasePath = currentDirectory;

		/* Tells the Map what the base directory is:
		 *   "./media/world#/"
		 */
		map.Set_Base_Directory(mapBasePath);

		/* Now that the level knows where to pull the data from,
		 * actually load the data into the level (pass in the tile dimensions)
		 */
		map.Load_Levels();

		Level_Manager *levelManager = map.Get_Level_Manager();
		if (levelManager != NULL)
		{
			levelManager->Show_All(std::cout);

			/* The manager has parsed the Map's config file.  Now, loop
			 * through each "level path" found and load that level.
			 *
			 * This should be a fairly large printout, since it will show
			 * this manager's Show_All() as well as each of the 3 levels'
			 * manager Show_All() readouts.
			 */


			LevelArray levelArray = levelManager->Get_LevelArray();

			u32 numLevels = levelArray.size();
			for (u32 i = 0; i < numLevels; ++i)
			{
				LevelData levelData = levelArray[i];

				/* Load the Level:
				 * - Load each Layer through the Level_Manager
				 * - Then call the manager's Show_All() function to display everything
				 *   to the screen
				 */
				Level level(numLevelRows, numLevelColumns);
				level.Set_LevelData(levelData);

				std::string levelBasePath = levelManager->Get_WorkingDirectory() + levelManager->Get_SubDirectory();

				/* Tells the Level what the base directory is:
				 *   "./media/world#/map#/"
				 */
				level.Set_Base_Directory(levelBasePath);

				/* Now that the Level knows where to pull the data from,
				 * actually load the data into the level
				 */
				level.Load_Layers();
				level.Load_Triggers();


				Layer_Manager *layerManager = level.Get_Layer_Manager();
				if (layerManager != NULL)
				{
					layerManager->Show_All(std::cout);
				}

				Trigger_Manager *triggerManager = level.Get_Trigger_Manager();
				if (triggerManager != NULL)
				{
					triggerManager->Show_All(std::cout);
				}
			}
		}
	}
	return 0;
}
