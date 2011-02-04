/**
 * Load all of the Levels for only the CURRENT Layer in from a file.
 *
 *  The Layer Manager handles all of the actual disk IO, leaving "Layer" to
 *  simply be a regular class object.
 *
 */

#include "Layer_Manager.h"

/* Defines show_verbose() */
#include "Debug.h"

/* Defines fileExists function */
#include "DiskAccess.h"

/* Defines tile_type, obstacle_type, ObstacleLine, ObstacleMap, LayerLine,
 * LayerMap, LayerData, LayerArray
 */
#include "LayerData.h"

/* Defines LayerArray */
#include "Level.h"

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
 * Constructor: Layer_Manager, passes base directory to the Manager superclass
 */
Layer_Manager::Layer_Manager(const std::string &working_directory)
	: workingDirectory(working_directory)
{
	numLayersSpecified = 0;

	numRows = 0;
	numColumns = 0;
}


/**
 * Destructor: Layer_Manager
 */
Layer_Manager::~Layer_Manager()
{
}


/**
 * Function: The working directory is considered the "root" directory for all
 * derived subdirectories.
 *
 * Returns: std::string Working directory
 */
std::string Layer_Manager::Get_WorkingDirectory()
{
	return workingDirectory;
}


/**
 * Function: Set the path to the config file.  The Manager superclass receives
 * the FULL config file path, which includes the working directory as well.
 */
void Layer_Manager::Set_ConfigFile(const std::string &config_file)
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
std::string Layer_Manager::Get_ConfigFile()
{
	return configFile;
}


/**
 * Function: Set the number of rows that are expected within the config file
 * ("layer#.cfg").
 */
void Layer_Manager::Set_Num_Rows(const u32 &rows)
{
	numRows = rows;
}


/**
 * Function: Return the number of rows ACTUALLY found within the config file
 * (even if not the same number as was specified).
 *
 * Returns: unsigned int Number of rows found
 */
u32 Layer_Manager::Get_Num_Rows()
{
	return numRows;
}


/**
 * Function: Set the number of columns that are expected within the config
 * file ("layer#.cfg").
 */
void Layer_Manager::Set_Num_Columns(const u32 &cols)
{
	numColumns = cols;
}


/**
 * Function: Return the number of columns ACTUALLY found within the config
 * file (even if not the same number as was specified).
 *
 * Returns: unsigned int Number of columns found
 */
u32 Layer_Manager::Get_Num_Columns()
{
	return numColumns;
}


/**
 * Function: Return the Layer array (contains all row x column tile entries
 * found within all layers specified in the config file).
 *
 * Returns: std::vector Layer Array
 */
LayerArray Layer_Manager::Get_LayerArray()
{
	return layerArray;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "obstacles_config"
 *
 * Format:
 * - [col x row] obstacles
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Layer_Manager::Read_Obstacles_Config(std::ifstream &fs)
{
	std::string line_obstacle_map;

	for (u32 row = 0; row < numRows; ++row)
	{
		if (seek_non_comment_line(fs, line_obstacle_map))
		{
			ObstacleLine obstacleColumn;

			std::stringstream ss(line_obstacle_map);
			for (u32 col = 0; col < numColumns; ++col)
			{
				/* These have to be read in as an integer, then cast to
				 * whatever type "tile_type" is, else this only reads in
				 * single characters, making "15" a "1" followed by a "5".
				 */
				u32 single_obstacle;
				ss >> single_obstacle;
				if (ss.fail())
				{
					verbose_bad(std::cerr, "*ERROR: Could not read in Obstacle data.\n");
					return false;
				}

				obstacleColumn.push_back((obstacle_type)single_obstacle);
			}
			obstacles.push_back(obstacleColumn);
		}
		else
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in data at row %d.\n", row);
			return false;
		}
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "num_layers"
 *
 * Format:
 * - [number of layers]
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Layer_Manager::Read_Num_Layers(std::ifstream &fs)
{
	std::string line_num_layers;
	if (seek_non_comment_line(fs, line_num_layers))
	{
		std::stringstream ss(line_num_layers);

		ss >> numLayersSpecified;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: invalid value after \"num_layers\"\n");
			return false;
		}
		else
		{
			verbose_good(std::cerr, "Map has specified %d layer%s will be found within the \"%s\" config file.\n",
			             numLayersSpecified,
			             (numLayersSpecified == 1 ? "" : "s"),
			             Manager::Get_ConfigFile().c_str());
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"num_layers\" specified, but could not read in any more data.\n");
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "layers_config"
 *
 * Format:
 * - layer ID
 * - [col x row] tiles
 *
 * Returns: true if successfully read/parsed, false on errors
 */
bool Layer_Manager::Read_Layers_Config(std::ifstream &fs)
{
	std::string line_layer_id;

	LayerData layerData;

	if (seek_non_comment_line(fs, line_layer_id))
	{
		layerData.id = line_layer_id;

		LayerMap layerMap;

		for (u32 row = 0; row < numRows; ++row)
		{
			LayerLine layerLine;
			for (u32 col = 0; col < numColumns; ++col)
			{
				tile_type single_tile;
				fs >> single_tile;
				if (fs.bad())
				{
					verbose_bad(std::cerr, "*ERROR: could not read in tile data from row %d, col %d\n", row, col);
					return false;
				}

				layerLine.push_back(single_tile);
			}
			layerMap.push_back(layerLine);
		}
		layerData.layer.push_back(layerMap);

		layerArray.push_back(layerData);
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: \"layers_config\" specified, but could not read in any more data.\n");
		return false;
	}

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
bool Layer_Manager::Load_Settings_From_Config()
{
	/* Assume success unless otherwise indicated */
	bool success = true;

	std::string fullFilePath = Manager::Get_ConfigFile();

	/* If the file doesn't exist, or if the number of rows/columns are not
	 * defined, return with an error.
	 */
	if (fileExists(fullFilePath) == false)
	{
		verbose_bad(std::cerr, "fileExists(%s) returned false.\n", fullFilePath.c_str());
		success = false;
		return success;
	}

	std::ifstream fs(fullFilePath.c_str());
	if (!fs.is_open())
	{
		verbose_bad(std::cerr, "Could not load file: %s", fullFilePath.c_str());
		success = false;
	}

	std::string line;
	while (success != false && seek_non_comment_line(fs, line))
	{
		if (line == "obstacles_config")
		{
			success = Read_Obstacles_Config(fs);
			continue;
		}

		else if (line == "num_layers")
		{
			success = Read_Num_Layers(fs);
			continue;
		}

		else if (line == "layers_config")
		{
			success = Read_Layers_Config(fs);
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
void Layer_Manager::Show_All(std::ostream &os)
{
	u32 layerArraySize = layerArray.size();
	u32 numObstacles = obstacles.size();
	if (layerArraySize == 0 && numObstacles == 0)
	{
		/* No Layer or Obstacle data were loaded; no reason to print anything
		 * here
		 */
		return;
	}

	verbose_good(os, "Layer_Manager:\n");

	/* Show the main manager's Show_All() function before our own. */
	Manager::Show_All(os);

	os << "\nLayer_Manager::Show_All()\n";

	verbose_good(os, "\tDimensions for layers: %d rows, %d columns\n", numRows, numColumns);

	if (numObstacles > 0)
	{
		/* Display the obstacle map data */
		os << "\n\tObstacle map:\n";
		for (u32 obstacleIndex = 0; obstacleIndex < numObstacles; ++obstacleIndex)
		{
			os << "\t\t";

			ObstacleLine obstacleLine = obstacles[obstacleIndex];

			u32 numCols = obstacleLine.size();
			for (u32 col = 0; col < numCols; ++col)
			{
				os << std::setw(2) << (int)obstacleLine[col] << " ";
			}

			os << std::endl;
		}
		os << std::endl;
	}

	/* Display each of the layers */
	verbose_good(os, "\n\tLayers:\n");

	if (layerArraySize != numLayersSpecified)
	{
		/* Number specified, and number found were not equal.  Print debug
		 * info stating as such.
		 */
		verbose_good(os, "\t\tNumber of layers specified in config file: %d\n", numLayersSpecified);
		verbose_good(os, "\t\tActual number of layers found: %d\n\n", layerArraySize);
	}
	else
	{
		verbose_good(os, "\t\t%d layer%s found\n", layerArraySize, (layerArraySize == 1 ? "" : "s"));
	}

	/* Display the entire LayerData structure */
	for (u32 layerArrayIndex = 0; layerArrayIndex < layerArraySize; ++layerArrayIndex)
	{
		LayerData layerData = layerArray[layerArrayIndex];

		/* Display each of the layer IDs along with the actual tile data */
		os << "\tLayer " << layerArrayIndex << ", ID: " << layerData.id << std::endl;

		std::vector<LayerMap> singleLayer = layerData.layer;

		u32 numLayers = singleLayer.size();
		for (u32 layerIndex = 0; layerIndex < numLayers; ++layerIndex)
		{
			LayerMap entireRow = singleLayer[layerIndex];

			u32 numRows = entireRow.size();
			for (u32 row = 0; row < numRows; ++row)
			{
				os << "\t\t";

				LayerLine entireColumn = entireRow[row];

				u32 numCols = entireColumn.size();
				for (u32 col = 0; col < numCols; ++col)
				{
					tile_type currentTile = entireColumn[col];
					os << std::setw(2) << currentTile << " ";
				}

				os << std::endl;
			}
		}
	}
}
