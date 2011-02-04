#include "Level.h"
#include "Layer_Manager.h"
#include "Trigger_Manager.h"


/* Defines verbose_good, verbose_bad */
#include "Debug.h"

#include <string>


/**
 * Constructor: Level
 */
Level::Level(const u32 &rows, const u32 &columns)
	: layerManager(NULL),
	  triggerManager(NULL)
{
	numRows = rows;
	numColumns = columns;

	baseDirectory = "";
	layerpath = "";
}


/**
 * Destructor: Level
 */
Level::~Level()
{
	if (layerManager != NULL)
	{
		delete layerManager;
		layerManager = NULL;
	}

	if (triggerManager != NULL)
	{
		delete triggerManager;
		triggerManager = NULL;
	}
}


/**
 * Function: Set the working, or base, directory under which the Layers and
 * Triggers will be found (this is what is passed to the Layer Manager when
 * the config file will be parsed).
 */
void Level::Set_Base_Directory(const std::string &dir)
{
	baseDirectory = dir;
}


/**
 * Function: Set where the "level#.cfg" config file will be found.
 */
void Level::Set_Config_Path(const std::string &path)
{
	layerpath = path;
}


/**
 * Function: Call the Layer Manager to parse the config file and extract all
 * data needed to load individual Layers.  Must be called before
 * "Load(level_id)".
 *
 * Returns: true if all Layers were successfully loaded, false on errors
 */
bool Level::Load_Layers()
{
	bool success = true;

	/* If no Layer Manager was created before, create it now */
	if (layerManager == NULL)
	{
		layerManager = new Layer_Manager(baseDirectory);
	}

	layerManager->Set_Num_Rows(numRows);
	layerManager->Set_Num_Columns(numColumns);
	layerManager->Set_ConfigFile(levelData.levelpath);

	if (layerManager->Load_Settings_From_Config() == false)
	{
		verbose_bad(std::cerr, "*ERROR: Layer_Manager::Load_Settings_From_Config returned false.\n");
		success = false;
	}

	return success;
}


/**
 * Function: Call the Trigger Manager to parse the config file and extract all
 * data needed to load this Level's triggers.  Must be called before
 * "Load(level_id)".
 *
 * Returns: true if all Triggers were successfully loaded, false on errors
 */
bool Level::Load_Triggers()
{
	bool success = true;

	if (triggerManager == NULL)
	{
		triggerManager = new Trigger_Manager(baseDirectory);
	}
	triggerManager->Set_Num_Rows(numRows);
	triggerManager->Set_Num_Columns(numColumns);
	triggerManager->Set_ConfigFile(levelData.triggerpath);
	if (triggerManager->Load_Settings_From_Config() == false)
	{
		verbose_bad(std::cerr, "*ERROR: Trigger_Manager::Load_Settings_From_Config returned false.\n");
		success = false;
	}

	return success;
}


/**
 * Function: Return the Layer Manager used to extract the Layer data from the
 * config file.
 *
 * Returns: Pointer to Layer Manager
 */
Layer_Manager *Level::Get_Layer_Manager()
{
	return layerManager;
}


/**
 * Function: Return the Trigger Manager used to extract the Trigger data from
 * the config file.
 *
 * Returns: Pointer to Trigger Manager
 */
Trigger_Manager *Level::Get_Trigger_Manager()
{
	return triggerManager;
}


/**
 * Function: Set the Level Data (type std::vector)
 */
void Level::Set_LevelData(const LevelData &data)
{
	levelData = data;
}


/**
 * Function: Return the Level Data (type std::vector)
 *
 * Returns: std::vector Level Data
 */
LevelData Level::Get_LevelData()
{
	return levelData;
}
