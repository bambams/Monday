/** Test the functionality of the Layer loader.
 *
 *  The purpose of this file is just to make sure that loading a Level in from
 *  a file works properly.
 *
 *  Because we don't yet have a Map or World loader, we have to hard-code the
 *  values that we should have pulled from those settings (these will be
 *  stored in the Test1_Layers.h header file).
 *
 */

#include "Test1_Layers.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

/* Defines show_verbose() */
#include "Debug.h"

/* Magic function that scans a config file for the first non-linecomment */
#include "Parse_Lines.h"

/* Defines parallax_type, LevelData, LevelArray
 */
#include "LevelData.h"

#include "Layer_Manager.h"
#include "Level.h"

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

	std::string currentDirectory = "./media/world0/map0/";

	LevelData levelData;
	levelData.id = "LEVEL_ID_0";
	levelData.triggerpath = "";
	levelData.parallaxHorz = 0;
	levelData.parallaxVert = 0;

	for (u32 inputFiles = 0; inputFiles < filenames.size(); ++inputFiles)
	{
		/* Pull in name/path for "level#.cfg" from command line */
		levelData.levelpath = filenames[inputFiles];

		/* Load the Level:
		 * - Load each Layer through the Level_Manager
		 * - Then call the manager's Show_All() function to display everything
		 *   to the screen
		 */
		Level level(numLevelRows, numLevelColumns);
		level.Set_LevelData(levelData);

		std::string levelBasePath = currentDirectory;

		/* Tells the Level what the base directory is:
		 *   "./media/world#/map#/"
		 */
		level.Set_Base_Directory(levelBasePath);

		/* Now that the Level knows where to pull the data from,
		 * actually load the data into the level
		 */
		level.Load_Layers();


		Layer_Manager *layerManager = level.Get_Layer_Manager();
		if (layerManager != NULL)
		{
			layerManager->Show_All(std::cout);
		}
	}
	return 0;
}
