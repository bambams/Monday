#ifndef Main_menu_h
#define Main_menu_h


#include "Menu.h"


class Main_menu
	: public Menu
{
	public:
		Main_menu(Game *game);
		void Render();
		game_event_n Event(ALLEGRO_EVENT event);

	private:
		int option;
};


#endif  //  Main_menu_h
