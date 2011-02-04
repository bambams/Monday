#ifndef Game_h
#define Game_h

#include "Menu.h"
#include "Player.h"
#include "Lua_wrapper.h"
#include "Entity_manager.h"
#include "Map.h"
#include "Camera.h"
#include "Obstacle_manager.h"
#include "Main.h"

enum GCD_AXISTYPE
{
	GCD_BUTTON,
	GCD_AXIS_LOW,
	GCD_AXIS_HIGH
};


typedef struct GAME_CONTROL_DEFS {
	/* Displayed name of function in game menus */
	const char* name;

	/* Primary key */
	int  key1;

	/* Secondary key */
	int  key2;

	/* Primary joysick */
	int  joy1, stick1, axistype1, axis_or_button_1;

	/* Secondary joystick */
	int  joy2, stick2, axistype2, axis_or_button_2;
} GAME_CONTROL_DEFS;

extern GAME_CONTROL_DEFS game_control_defs[];


/* Class: Game
 * The hub of the whole system, handling, initialization, cleanup, main loop, events and services.
 */
class Game
{
public:

	/* Constructor: Game
	 */
	Game();

	/* Destructor: Game
	 */
	~Game();

	/* Function: Run
	 * Runs the whole game, doesn't return until game exits.
	 */
	void Run();

	/* Function: Close_menu
	 * Closes the top menu.
	 */
	void Close_menu();

	/* Function: Get_font
	 * Returns:
	 * A common font
	 */
	ALLEGRO_FONT* Get_font();

	/* Function: Get_player
	 * Returns:
	 * The player entity.
	 */
	Player* Get_player();

	/* Function: Get_entity_manager
	 * Returns:
	 * The <Entity_manager> instance.
	 */
	Entity_manager& Get_entity_manager();

	/* Function: Get_lua_wrapper
	 * Returns:
	 * The <Lua_wrapper> instance.
	 */
	Lua_wrapper& Get_lua_wrapper();
	void Queue_game_event(game_event_n ge);

	/* Function: Get_obstacle_manager
	 * Returns:
	 * The <Obstacle_manager> instance.
	 */
	Obstacle_manager& Get_obstacle_manager();

	/* Function: Get_resource_manager
	 * Returns:
	 * The <Resource_manager> instance.
	 */
	Resource_manager& Get_resource_manager();

	/* Function: Get_map
	 * Returns:
	 * The <Map>.
	 */
	Map* Get_map();

	/* Function: Load_map
	 */
	void Load_map(const std::string& mapname);

	/* Function: Quit_game
	   Quits the game.
	*/
	void Quit_game();

	/* Section: Private
	 */

private: //Functions

	/* Function: Init
	 * Initialize game
	 */
	void Init();

	/* Function: Update
	 * Updates everything
	 *
	 * Parameters:
	 * dt - Time passed since last update.
	 */
	void Update(double dt);

	/* Function: Render
	 * Renders everything
	 */
	void Render();

	/* Function: Event
	 * Distrubutes events
	 */
	void Event(ALLEGRO_EVENT event);
	void Game_event(game_event_n event);
	game_event_n Get_next_game_event();

private: //Members
	Entity_manager entity_manager;
	Obstacle_manager obstacle_manager;
	Resource_manager resource_manager;

	Camera camera;
	Lua_wrapper lua_wrapper;
	Map* map;
	Menu* menu;
	Player player;

	game_event_n next_game_event;
	ALLEGRO_FONT* font;
	bool quit;
};


#endif  //  Game_h
