#ifndef Menu_h
#define Menu_h

#include <allegro5/allegro5.h>

extern "C"
{
	#include "lua.h"
}
#include "Game_events.h"
#include "Vector.h"

class Gui;
class Button;

class Game;

class Menu
{
public:
	Menu(Game* game);
	virtual ~Menu();
	void Open_child(Menu* menu);
	void Base_event(ALLEGRO_EVENT event);
	virtual void Render();
	void Add_button(Vector pos);
	void Add(Gui * g);
protected:
	Game* game;
private:
	virtual game_event_n Event(ALLEGRO_EVENT event);
	void Close_child();
	void Close_buttons();
	Menu* parent;
	Menu* child;
	Button* button;
	Gui ** gui_list;
	int gui_num;
};

void Menu_register_lua_callbacks(lua_State* state);

#endif
