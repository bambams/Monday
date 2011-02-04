#include "Menu.h"
#include "Game.h"
#include "Gui.h"

Menu::Menu(Game* game)
:game(game)
,parent(NULL)
,child(NULL)
{
}

Menu::~Menu()
{
	if(child)
		delete child;
}

void Menu::Open_child(Menu* menu)
{
	child = menu;
}

void Menu::Base_event(ALLEGRO_EVENT event)
{
	if(child)
	{
		child->Base_event(event);
	}
	else
	{
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				if(parent)
					parent->Close_child();
				else
					game->Close_menu();
				return;
			}
		}
		game->Queue_game_event(Event(event));
	}
}

game_event_n Menu::Event(ALLEGRO_EVENT event)
{
   return GAME_EVENT_NONE;
}

void Menu::Render()
{
}

void Menu::Close_child()
{
	delete child;
	child = NULL;
}

void Menu::Add_button(Vector position)
{
	Button button(this);

}

/* LUA callbacks */
int Menu_add_button(lua_State* state)
{
//	assert(lua_gettop(state) == 2);

	Menu* menu = static_cast<Menu*>(lua_touserdata(state, 1));

	int x = lua_tonumber(state, 2);
	int y = lua_tonumber(state, 3);

	menu->Add_button(Vector(x, y));

	printf("[LUA] Menu_add_button\n");

	return 0;
}

void Menu_register_lua_callbacks(lua_State* state)
{
	lua_register(state, "Menu_add_button", Menu_add_button);
//	lua_register(state, "Menu_background_pic", Menu_background_pic);
}

