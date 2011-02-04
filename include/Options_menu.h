#ifndef Options_menu_h
#define Options_menu_h

#include "Menu.h"

class Options_menu: public Menu
{
public:
	Options_menu(Game* game);
	void Render();
	game_event_n Event(ALLEGRO_EVENT event);
private:
	int option;
};

#endif
