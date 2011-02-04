#ifndef Keyboard_menu_h
#define Keyboard_menu_h

#include "Menu.h"

typedef struct KEYBOARD_POS { int x,y,w,h; } KEYBOARD_POS;
extern KEYBOARD_POS keyb_pos_104[];   /* in Keyboard_menu.cpp */

typedef struct KEYBOARD_DEF { 
 int numkeys;	
 char * name;
 int w,h; 
 KEYBOARD_POS * pos;
} KEYBOARD_DEF;

class Keyboard_menu: public Menu
{
public:
	Keyboard_menu(Game* game);
	void Render();
	game_event_n Event(ALLEGRO_EVENT event);
private:
	KEYBOARD_DEF keyboard_defs;
	int option;
};

#endif
