#ifndef Game_h
#define Game_h

#include "Game_events.h"
#include "Menu.h"
#include "Dialog.h"
#include "Player.h"
#include "Item_manager.h"
#include "Character.h"
#include "Lua_wrapper.h"

extern "C"
{
#include "allegro5/a5_ttf.h"
}

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

enum GCD_AXISTYPE { GCD_BUTTON, GCD_AXIS_LOW, GCD_AXIS_HIGH };


typedef struct GAME_CONTROL_DEFS {
	const char * name;   /* diplayed name of function in game menus */
	int  key1;     /* primary key */
	int  key2;     /* secondary key */
	int  joy1, stick1, ab_1, ab_n_1;  /* primary joysick */
	int  joy2, stick2, ab_2, ab_n_2;  /* secondary joystick */
} GAME_CONTROL_DEFS;

extern GAME_CONTROL_DEFS game_control_defs[];


class Menu;

class Game
{
public:
	Game();
	~Game();
	void Run();
	void Close_menu();
	ALLEGRO_FONT* Get_font();
	Player* Get_player();
	//lua_State* Get_lua_state();
	Lua_wrapper& Get_lua_wrapper();
	void Set_debugconsole(bool c); 
	void Queue_game_event(game_event_n ge);

private: //Functions
	void Init();
	void Update(double dt);
	void Render();
	void Event(ALLEGRO_EVENT event);
	void Game_event(game_event_n e);
	void Switch_to_menu(Menu * menu);
	game_event_n Get_next_game_event();

private: //Members
	Menu* menu;
	game_event_n game_event;
	Dialog* dialog;
	Player player;
	Item_manager item_manager;
	ALLEGRO_FONT* font;
	Lua_wrapper lua_wrapper;
	//lua_State* lua_state;
	Character character;
	bool debugconsole;
};

#endif
