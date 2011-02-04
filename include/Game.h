#ifndef Game_h
#define Game_h

#include "Menu.h"
#include "Player.h"
#include "Lua_wrapper.h"
#include "Entity_manager.h"
#include "Map.h"
#include "Camera.h"
#include "Obstacle_manager.h"
#include "main.h"

enum GCD_AXISTYPE
{
	GCD_BUTTON,
	GCD_AXIS_LOW,
	GCD_AXIS_HIGH
};


typedef struct GAME_CONTROL_DEFS {
	const char * name;   /* diplayed name of function in game menus */
	int  key1;     /* primary key */
	int  key2;     /* secondary key */
	int  joy1, stick1, axistype1, axis_or_button_1;  /* primary joysick */
	int  joy2, stick2, axistype2, axis_or_button_2;  /* secondary joystick */
} GAME_CONTROL_DEFS;

extern GAME_CONTROL_DEFS game_control_defs[];


class Game
{
public:
	Game();
	~Game();
	void Run();
	void Close_menu();
	ALLEGRO_FONT* Get_font();
	Player* Get_player();
	Entity_manager& Get_entity_manager();
	Lua_wrapper& Get_lua_wrapper();
	void Queue_game_event(game_event_n ge);
	Obstacle_manager& Get_obstacle_manager();
	Resource_manager& Get_resource_manager();
private: //Functions
	void Init();
	void Update(double dt);
	void Render();
	void Event(ALLEGRO_EVENT event);
	void Game_event(game_event_n event);
	game_event_n Get_next_game_event();
private: //Members
	Resource_manager resource_manager;
	Map map;
	Obstacle_manager obstacle_manager;
	Menu* menu;
	game_event_n next_game_event;
	Player player;
	ALLEGRO_FONT* font;
	Lua_wrapper lua_wrapper;
	Entity_manager entity_manager;
	Camera camera;
};

#endif
