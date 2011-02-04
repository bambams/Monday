#ifndef Joystick_menu_h
#define Joystick_menu_h

#include "Menu.h"

typedef struct JOYSTICK_POS { int x,y,w,h; } JOYSTICK_POS;
extern JOYSTICK_POS joyb_pos_ps2[];   /* in Joystick_menu.cpp */

typedef struct JOYSTICK_DEF { 
 int numkeys;	
 char * name;
 int w,h; 
 JOYSTICK_POS * pos;
} JOYSTICK_DEF;

class Joystick_menu: public Menu
{
public:
	Joystick_menu(Game* game);
	void Render();
	game_event_n Event(ALLEGRO_EVENT event);
private:
	JOYSTICK_DEF joystick_defs;
	int option;
	static int selected; 
};

#endif
