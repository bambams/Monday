#ifndef Keyboard_menu_h
#define Keyboard_menu_h


#include "main.h"
#include "Menu.h"

#include <string>


typedef struct KEYBOARD_POS
{
	int x;
	int y;
	int w;
	int h;
} KEYBOARD_POS;


extern KEYBOARD_POS keyb_pos_104[];   /* in Keyboard_menu.cpp */


typedef struct KEYBOARD_DEF
{
	int numkeys;
	std::string name;
	int w, h;
	KEYBOARD_POS *pos;
} KEYBOARD_DEF;


class Keyboard_menu
	: public Menu
{
	public:
		Keyboard_menu(Game *game);
		void Render();
		game_event_n Event(ALLEGRO_EVENT event);

	private:
		KEYBOARD_DEF keyboard_defs;
		int option;
};


#endif  //  Keyboard_menu_h
