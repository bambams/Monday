#include "Game.h"
#include "Debug.h"
#include "Entity.h"
#include "Keyboard_menu.h"
#include "Main_menu.h"
#include "Options_menu.h"
#include "Vector.h"
#include "Main.h"

#include <iostream>


GAME_CONTROL_DEFS game_control_defs[] = {
    /* func name,    pri. key,           sec. key,           pri. joy,            sec. joy  */
	{ "MOVE LEFT",   ALLEGRO_KEY_LEFT,   ALLEGRO_KEY_A,      0,0,GCD_AXIS_LOW,0,  0,0,GCD_BUTTON,1  },
	{ "MOVE RIGHT",  ALLEGRO_KEY_RIGHT,  ALLEGRO_KEY_D,      0,0,GCD_AXIS_HIGH,0, 0,0,GCD_BUTTON,2  },
	{ "MOVE UP",     ALLEGRO_KEY_UP,     ALLEGRO_KEY_W,      0,0,GCD_AXIS_LOW,0,  0,0,GCD_BUTTON,3  },
	{ "MOVE DOWN",   ALLEGRO_KEY_DOWN,   ALLEGRO_KEY_S,      0,0,GCD_AXIS_LOW,0,  0,0,GCD_BUTTON,4  },
	{ "FIRE",        ALLEGRO_KEY_LCTRL,  ALLEGRO_KEY_SPACE,  0,0,GCD_BUTTON, 7,   0,0,GCD_BUTTON,7  }
};


int Game_get_player(lua_State* state)
{
	Game* game = Lua_wrapper::Get_game(state);
	lua_pushlightuserdata(state, game->Get_player());
	return 1;
}


int Game_load_map(lua_State* state)
{
	Monday_out(VERBOSE_LEVEL2, std::cout, "Load map call from lua\n");
	Game* game = Lua_wrapper::Get_game(state);
	const char* s = lua_tostring(state, 1);
	Monday_out(VERBOSE_LEVEL2, std::cout, "- Map name: %s\n", s);
	game->Load_map(s);
	return 0;
}


int Game_get_map(lua_State* state)
{
	Game* game = Lua_wrapper::Get_game(state);
	lua_pushlightuserdata(state, game->Get_map());
	return 1;
}


Game::Game()
:map(NULL)
,menu(NULL)
{
	next_game_event = GAME_EVENT_NONE;
	quit = false;
}


Game::~Game()
{
}


void Game::Run()
{
	al_init();
	al_install_keyboard();
	al_iio_init();
	al_font_init();

	ALLEGRO_DISPLAY *display;
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	display = al_create_display(640, 480);

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)display);
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)al_get_keyboard());

	font = al_ttf_load_font("media/DejaVuSans.ttf", 20, 0);
	mon_assert(NULL != font && "Could not load media/DejaVuSans.ttf.\n");

	lua_wrapper.Init(this);

	Init();

	double last_time = al_current_time();

	while(1)
	{
		if (quit)
			break;
		ALLEGRO_EVENT event;
		if (al_get_next_event(event_queue, &event))
		{
			// quit when the ESC key is pressed, placeholder until menus are in place.
		  	if (!menu && ALLEGRO_EVENT_KEY_DOWN == event.type)
			{
				if (ALLEGRO_KEY_ESCAPE == event.keyboard.keycode)
				{
					break;
				}
			}
			if (ALLEGRO_EVENT_DISPLAY_CLOSE == event.type)
			{
				break;
			}
			Event(event);
		}

		game_event_n gameEvent = Get_next_game_event();
		if (GAME_EVENT_NONE != gameEvent)
		{
			if (GAME_EVENT_EXIT == gameEvent)
			{
				break;
			}
			if (GAME_EVENT_PLAY_GAME == gameEvent)
			{
				Close_menu();
			}
			// I'm not sure why this was commented out--is there a problem with it?
			// Adding it back in (+ a few other things) fixed the options menu problem,
			// so that's what I did.
			Game_event(next_game_event);
			next_game_event = GAME_EVENT_NONE;	/* purge 'queue' */
		}


		double current_time = al_current_time();
		double dt = current_time - last_time;
		last_time = current_time;
		Update(dt);

		al_clear(al_map_rgb(0, 0, 0));
		Render();
		al_flip_display();

		al_rest(0.001);
	}

	lua_wrapper.Deinit();
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
}


void Game::Init()
{
	lua_State* lua_state = lua_wrapper.Get_state();
	lua_register(lua_state, "Get_player", Game_get_player);
	lua_register(lua_state, "Load_map", Game_load_map);
	lua_register(lua_state, "Get_map", Game_get_map);

	Player_register_lua_callbacks(lua_state);

	Entity_manager_register_lua_callbacks(lua_state);
	Entity_register_lua_callbacks(lua_state);
	entity_manager.Set_game(this);

	player.Set_game(this);
	player.Set_solid(true);
	Animations animations = Load_animations("media/Gryphon_fly.adf", resource_manager);
	player.Set_animations(animations);
	entity_manager.Set_player(&player);
	entity_manager.Set_game(this);
	obstacle_manager.Add_obstacle(&player);

	menu = new Main_menu(this);

	lua_wrapper.Load_script("media/Init.lua");

	Load_map("media/test.map");

	lua_wrapper.Load_script("media/Scripts.lua");

	camera.Set_viewport(Vector(0, 0), Vector(640, 480));
}


void Game::Update(double dt)
{
	player.Update(dt);

	/* This centers the screen on the player */
	camera.Set_position(player.Get_position());
	camera.Update(dt);

	entity_manager.Update(dt);
}


/**
 * Function: Determine who should have focus, and call its Render() function.
 *
 * If a menu is being displayed, it should have precidence before rendering
 * maps or players.
 */
void Game::Render()
{
	if (menu)
		menu->Render();
	else
	{
		if (map)
			map->Render(camera);
		player.Render(camera);
		// entity_manager.Render(camera); // so the maps handles entities now?
	}
}


/**
 * Function: Handle and pass along events.
 *
 * If a menu is active, it will receive the events; none will be passed up to
 * other event subscribers, such as the Player event handler.
 */
void Game::Event(ALLEGRO_EVENT event)
{
	if (menu)
	{
		/* The game's menu screen is visible, so pass the event to the menu */
		menu->Base_event(event);
	}
	else
	{
		/* Check to see if the event was from pressing a key */
		if (ALLEGRO_EVENT_KEY_DOWN == event.type)
		{
			switch (event.keyboard.keycode)
			{
				/* Camera rumble effect */
				case ALLEGRO_KEY_R:
					camera.Set_rumble(10);
					break;

				/* Temporary Player movement speed increase */
				case ALLEGRO_KEY_LSHIFT:
					player.Set_speed_boost(50);
					break;
			}
		}

		/* Check to see if the event was from releasing a key */
		else if (ALLEGRO_EVENT_KEY_UP == event.type)
		{
			switch (event.keyboard.keycode)
			{
				/* Stop camera shaking effect */
				case ALLEGRO_KEY_R:
					camera.Set_rumble(0);
					break;

				/* Set temporary Player speed back to normal */
				case ALLEGRO_KEY_LSHIFT:
					player.Set_speed_boost(0);
					break;
			}
		}

		/* Pass the event to the player's event handler */
		player.Event(event);
	}
}


void Game::Game_event(game_event_n event)
{
	Monday_out(SUGGESTION_LEVEL, std::cout, "Game::Game_event: event = "); // No endline
	switch (event) {
		case GAME_EVENT_MAIN_MENU:
			Monday_out(SUGGESTION_LEVEL, std::cout, "GAME_EVENT_MAIN_MENU\n");
			if (menu)
			{
				delete menu;
			}
			menu = new Main_menu(this);
			break;

		case GAME_EVENT_OPTIONS_MENU:
			Monday_out(SUGGESTION_LEVEL, std::cout, "GAME_EVENT_OPTIONS_MENU\n");
			if (menu)
			{
				delete menu;
			}
			menu = new Options_menu(this);
			break;

		case GAME_EVENT_KEYBOARD_MENU:
			Monday_out(SUGGESTION_LEVEL, std::cout, "GAME_EVENT_KEYBOARD_MENU\n");
			if (menu)
			{
				delete menu;
			}
			menu = new Keyboard_menu(this);
			break;

		default:
			Monday_out(SUGGESTION_LEVEL, std::cout, "[unspecified]\n");
			break;
	}
}


game_event_n Game::Get_next_game_event()
{
	return next_game_event;
}


void Game::Queue_game_event(game_event_n ge)
{
	next_game_event = ge;
}

void Game::Close_menu()
{
	delete menu;
	menu = NULL;
}

void Game::Quit_game()
{
	quit = true;
}

ALLEGRO_FONT* Game::Get_font()
{
	return font;
}


Player* Game::Get_player()
{
	return &player;
}


Entity_manager& Game::Get_entity_manager()
{
	return entity_manager;
}


Lua_wrapper& Game::Get_lua_wrapper()
{
	return lua_wrapper;
}


Obstacle_manager& Game::Get_obstacle_manager()
{
	return obstacle_manager;
}


Resource_manager& Game::Get_resource_manager()
{
	return resource_manager;
}


Map* Game::Get_map()
{
	return map;
}


void Game::Load_map(const std::string& mapname)
{
	Monday_out(VERBOSE_LEVEL2, std::cout, "Deleting old map\n");
	if (map)
	{
		delete map;
		map = NULL;
	}

	//Todo: Use resource manager
	Monday_out(VERBOSE_LEVEL2, std::cout, "Loading new map\n");
	map = new Map;
	Monday_out(VERBOSE_LEVEL2, std::cout, "Game::Load_map()\n\tmapname: \"%s\"\n", mapname.c_str());
	map->Load(mapname, &obstacle_manager, resource_manager, &entity_manager);
	Monday_out(VERBOSE_LEVEL2, std::cout, "New map loaded\n");

	Monday_out(VERBOSE_LEVEL3, std::cout, "Updating tile dimensions for player\n");
	player.Set_Dimensions(map->Get_TileWidth(), map->Get_TileHeight());
}
