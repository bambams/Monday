#include "Game.h"
#include <allegro5/allegro5.h>
#include <allegro5/a5_iio.h>
#include "Main_menu.h"
#include "Options_menu.h"
#include "Keyboard_menu.h"
#include "Joystick_menu.h"
#include <iostream>


/* global pointer to currently running game */
Game* game;


GAME_CONTROL_DEFS game_control_defs[] = {
    /* func name,    pri. key,        sec. key,      pri. joy,            sec. joy  */ 
	{ "MOVE LEFT",  ALLEGRO_KEY_LEFT, ALLEGRO_KEY_A, 0,0,GCD_AXIS_LOW,0,  0,0,GCD_BUTTON,1  },  
	{ "MOVE RIGHT", ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_D,0,0,GCD_AXIS_HIGH,0, 0,0,GCD_BUTTON,2  },
	{ "MOVE UP",    ALLEGRO_KEY_UP, ALLEGRO_KEY_W,   0,0,GCD_AXIS_HIGH,1,  0,0,GCD_BUTTON,3  },
	{ "MOVE DOWN",  ALLEGRO_KEY_DOWN, ALLEGRO_KEY_S, 0,0,GCD_AXIS_LOW,1,  0,0,GCD_BUTTON,4  },
	{ "FIRE",  ALLEGRO_KEY_LCTRL, ALLEGRO_KEY_SPACE,0,0,GCD_BUTTON, 1,   0,0,GCD_BUTTON,7  }
};


int Game_get_player(lua_State* state)
{
//	assert(lua_gettop(state) == 0);

    lua_pushlightuserdata(state, game->Get_player());

	return 1;
}

Game::Game()
:menu(NULL)
,dialog(NULL)
{
	game = this;
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

	ALLEGRO_DISPLAY *display, *console = NULL;
	display = al_create_display(640, 480);

	if (debugconsole) console = al_create_display(640, 480);

	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)display);
	al_register_event_source(event_queue, (ALLEGRO_EVENT_SOURCE *)al_get_keyboard());

	font = al_ttf_load_font("media/DejaVuSans.ttf", 20, 0);

	lua_wrapper.Init();

	Init();

	double last_time = al_current_time();

	while(1)
	{
		ALLEGRO_EVENT event;
		if (al_get_next_event(event_queue, &event))
		{
			// quit when the ESC key is pressed, placeholder until menus are in place.
			if (!menu && event.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
					break;
			}
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				break;
			Event(event);
		}

		if (Get_next_game_event() != GAME_EVENT_NONE)
		{   
			if (Get_next_game_event() == GAME_EVENT_EXIT) break;
                     	Game_event(game_event);
			game_event = GAME_EVENT_NONE;	
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
	if (debugconsole) al_destroy_display(console);
}

void Game::Set_debugconsole(bool mode) { debugconsole = mode;}

game_event_n Game::Get_next_game_event()
{
	return game_event;	
}

void Game::Queue_game_event(game_event_n ge)
{
	game_event = ge;
}


void Game::Init()
{
	lua_register(lua_wrapper.Get_state(), "Get_player", Game_get_player);

	Player_register_lua_callbacks(lua_wrapper.Get_state());
	Character_register_lua_callbacks(lua_wrapper.Get_state());

	Menu_register_lua_callbacks(lua_wrapper.Get_state());

	player.Initiate();

	character.Set_update_script("Albert_update");

	menu = new Main_menu(this);

	lua_wrapper.Load_script("media/Init.lua");
	lua_wrapper.Load_script("media/Scripts.lua");
/*	if (luaL_loadfile(lua_state, "media/Init.lua") == 0) // all is well
	{
		int errors = lua_pcall(lua_state, 0, 0, 0);

		if (errors)
		{
			std::cout << lua_tostring(lua_state, -1) << std::endl;
		}
	}
	else
	{
		std::cout << "media/Init.lua is missing" << std::endl;
	}

	if (luaL_loadfile(lua_state, "media/Scripts.lua") == 0)
	{
		int errors = lua_pcall(lua_state, 0, 0, 0);

		if (errors)
		{
			std::cout << lua_tostring(lua_state, -1) << std::endl;
		}
	}
	else
	{
		std::cout << "media/Scripts.lua is missing or broken" << std::endl;
	}
*/
}

void Game::Update(double dt)
{
	player.Update(dt);
	character.Update(dt, this);
}

void Game::Render()
{
	//Hide this when in main menu somehow.
	player.Render();
	character.Render();
	item_manager.Render();

	if(menu)
		menu->Render();
	else if(dialog)
		dialog->Render();


}



void Game::Event(ALLEGRO_EVENT event)
{
	if(menu)
		menu->Base_event(event);
	else if(dialog)
		dialog->Event(event);
	else
	{
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_A)
				item_manager.Add(new Item, player.Get_position());
			if (event.keyboard.keycode == ALLEGRO_KEY_Z)
			{
				Item* item = item_manager.Remove(player.Get_position());
			}
		}
		player.Event(event);
	}
}

void Game::Game_event(game_event_n event)
{
	switch (event) {
		case GAME_EVENT_MAIN_MENU:
			if (menu) delete menu;
			menu = new Main_menu(this);	
			break;

		case GAME_EVENT_OPTIONS_MENU:
			if (menu) delete menu;
			menu = new Options_menu(this);	
			break;

		case GAME_EVENT_KEYBOARD_MENU:
			if (menu) delete menu;
			menu = new Keyboard_menu(this);	
			break;

		case GAME_EVENT_JOYSTICK_MENU:
			if (menu) delete menu;
			menu = new Joystick_menu(this);	
			break;

		case GAME_EVENT_PLAY_GAME:
			if (menu) delete menu;
			break;

		default:
			break;
	}

}

void Game::Switch_to_menu(Menu * menu)
{
	this->Close_menu();
	this->menu = menu;
	//menu.Init()
}

void Game::Close_menu()
{
	delete menu;
	menu = NULL;
}

ALLEGRO_FONT* Game::Get_font()
{
	return font;
}

Player* Game::Get_player()
{
	return &player;
}

Lua_wrapper& Game::Get_lua_wrapper()
{
	return lua_wrapper;
}
