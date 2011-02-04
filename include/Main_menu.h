#ifndef Main_menu_h
#define Main_menu_h

#include "Menu.h"
#include "Gui.h"


class Main_menu: public Menu
{
private:

	Button playButton;
	Button optionsButton;
	Button exitButton;


public:
	Main_menu(Game* game);
	void Render();
	game_event_n Event(ALLEGRO_EVENT event);
private:
	int option;
};

#endif
