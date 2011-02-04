#include "Game.h"

#include "Debug.h"
#include "Entity.h"
#include "Keyboard_menu.h"
#include "Main_menu.h"
#include "Options_menu.h"
#include "main.h"

#include <iostream>


GAME_CONTROL_DEFS game_control_defs[] = {
    /* func name,    pri. key,           sec. key,           pri. joy,             sec. joy  */
	{ "MOVE LEFT",   ALLEGRO_KEY_LEFT,   ALLEGRO_KEY_A,      0,0,GCD_AXIS_LOW, 0,  0,0,GCD_BUTTON,1  },
	{ "MOVE RIGHT",  ALLEGRO_KEY_RIGHT,  ALLEGRO_KEY_D,      0,0,GCD_AXIS_HIGH,0,  0,0,GCD_BUTTON,2  },
	{ "MOVE UP",     ALLEGRO_KEY_UP,     ALLEGRO_KEY_W,      0,0,GCD_AXIS_LOW, 0,  0,0,GCD_BUTTON,3  },
	{ "MOVE DOWN",   ALLEGRO_KEY_DOWN,   ALLEGRO_KEY_S,      0,0,GCD_AXIS_LOW, 0,  0,0,GCD_BUTTON,4  },
	{ "FIRE",        ALLEGRO_KEY_LCTRL,  ALLEGRO_KEY_SPACE,  0,0,GCD_BUTTON,   7,  0,0,GCD_BUTTON,7  }
};


int Game_get_player(lua_State *state)
{
	Game *game = Lua_wrapper::Get_game(state);
	lua_pushlightuserdata(state, game->Get_player());
	return 1;
}


Game::Game()
	: menu(NULL),
	  font(NULL)
{
	resource_manager = new Resource_manager();
	map              = new Map();
	obstacle_manager = new Obstacle_manager();
	player           = new Player();
	lua_wrapper      = new Lua_wrapper();
	entity_manager   = new Entity_manager();
	camera           = new Camera();

	next_game_event = GAME_EVENT_NONE;
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

	lua_wrapper->Init(this);

	Init();

	double last_time = al_current_time();

	while (1)
	{
		ALLEGRO_EVENT event;
		if (al_get_next_event(event_queue, &event))
		{
			// quit when the ESC key is pressed, placeholder until menus are in place.
			if (!menu && event.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					break;
				}
			}
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			{
				break;
			}

			Event(event);
		}

		if (Get_next_game_event() != GAME_EVENT_NONE)
		{
			if (Get_next_game_event() == GAME_EVENT_EXIT)
			{
				break;
			}
			if (Get_next_game_event() == GAME_EVENT_PLAY_GAME)
			{
				Close_menu();
			}
//            Game_event(next_game_event);
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

	lua_wrapper->Deinit();
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
}


void Game::Init()
{
	assert(lua_wrapper != NULL && "Game::Init(): *lua_wrapper is NULL.\n");

	lua_State *state = lua_wrapper->Get_state();
	if (state != NULL)
	{
		lua_register(state, "Get_player", Game_get_player);

		Player_register_lua_callbacks(state);

		Entity_manager_register_lua_callbacks(state);
		Entity_register_lua_callbacks(state);
		entity_manager->Set_game(this);

		player->Set_game(this);
		Animations animations = Load_animations("media/Gryphon_fly.adf", resource_manager);
		player->Set_animations(animations);
		entity_manager->Set_player(player);

		menu = new Main_menu(this);
		assert(menu != NULL && "Game::Init(): new Main_menu(this) returned NULL.\n");

		lua_wrapper->Load_script("media/Init.lua");
		lua_wrapper->Load_script("media/Scripts.lua");

		map->Load("media/test.map", obstacle_manager, resource_manager);
		camera->Set_viewport(Vector(0, 0), Vector(640, 480));
	}
	else
	{
		std::cerr << "Game::Init(): lua_wrapper->Get_state() returned NULL.\n";
	}
}


void Game::Update(double dt)
{
	player->Update(dt);
	camera->Set_position(player->Get_position());
	camera->Update(dt);
	entity_manager->Update(dt);
}


void Game::Render()
{
	if (menu)
		menu->Render();
	else
	{
		map->Render(camera);
		player->Render(camera);
		entity_manager->Render(camera);
	}
}


void Game::Event(ALLEGRO_EVENT event)
{
	if (menu)
	{
		/* The game's menu screen is visible, so pass the event to the menu */
		menu->Base_event(event);
	}
	else
	{
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_R)
			{
				camera->Set_rumble(10);
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_LSHIFT)
			{
				player->Set_speed_boost(50);
			}
		}
		if (event.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_R)
			{
				camera->Set_rumble(0);
			}
			if (event.keyboard.keycode == ALLEGRO_KEY_LSHIFT)
			{
				player->Set_speed_boost(0);
			}
		}
		player->Event(event);
	}
}


void Game::Game_event(game_event_n event)
{
	switch (event)
	{
		case GAME_EVENT_MAIN_MENU:
			if (menu)
			{
				delete menu;
			}
			menu = new Main_menu(this);
			break;

		case GAME_EVENT_OPTIONS_MENU:
			if (menu)
			{
				delete menu;
			}
			menu = new Options_menu(this);
			break;

		case GAME_EVENT_KEYBOARD_MENU:
			if (menu)
			{
				delete menu;
			}
			menu = new Keyboard_menu(this);
			break;

		default:
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


ALLEGRO_FONT *Game::Get_font()
{
	return font;
}


Player *Game::Get_player()
{
	return player;
}


Entity_manager *Game::Get_entity_manager()
{
	return entity_manager;
}


Lua_wrapper *Game::Get_lua_wrapper()
{
	return lua_wrapper;
}


Obstacle_manager *Game::Get_obstacle_manager()
{
	return obstacle_manager;
}


Resource_manager *Game::Get_resource_manager()
{
	return resource_manager;
}
