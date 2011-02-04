#ifndef Menu_h
#define Menu_h


#include "Main.h"
#include "Game_events.h"
#include "Vector.h"

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

protected:
	Game* game;

private:
	virtual game_event_n Event(ALLEGRO_EVENT event);
	void Close_child();
	Menu* parent;
	Menu* child;
	Button* button;
};

void Menu_register_lua_callbacks(lua_State* state);


#endif  //  Menu_h
