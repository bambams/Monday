#ifndef Level_h
#define Level_h


/* Defines LevelData, LevelArray */
#include "LevelData.h"

#include "Layer_Manager.h"
#include "Trigger_Manager.h"

#include <string>
#include <vector>


class Level
{
	public:
		Level(const u32 &rows, const u32 &columns);
		virtual ~Level();

		virtual void Set_Base_Directory(const std::string &dir);
		virtual void Set_Config_Path(const std::string &path);

		virtual bool Load_Layers();
		virtual bool Load_Triggers();

		Layer_Manager *Get_Layer_Manager();
		Trigger_Manager *Get_Trigger_Manager();

		void Set_LevelData(const LevelData &data);
		LevelData Get_LevelData();

	protected:
		u32 numRows, numColumns;

		/* Contains "./media/world#/map#/" */
		std::string baseDirectory;

		/* Contains "level#.cfg" */
		std::string layerpath;

		Layer_Manager *layerManager;
		Trigger_Manager *triggerManager;

		LevelData levelData;
};


#endif  //  Level_h
