#include "Options_menu.h"
extern "C"
{
#include "allegro5/a5_ttf.h"
}
#include "Game.h"

Options_menu::Options_menu(Game* game)
:Menu(game)
,option(0)
{
}

void Options_menu::Render()
{
    ALLEGRO_COLOR green = al_map_rgba_f(0, 0.5, 0, 1);
    ALLEGRO_COLOR bright = al_map_rgba_f(0.25, 0.75, 0.25, 1);
	ALLEGRO_FONT* font = game->Get_font();
	int fh = 20;
	int n = 3, y, cx; 
	
	y = al_get_display_height()/2 - (fh * n) /2;

	cx = al_get_display_width()/2;

	if(font)
	{
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (option==0) ? bright : green );
		al_font_textout_centre(font, cx,y, "Keyboard", -1);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (option==1) ? bright : green );
		al_font_textout_centre(font, cx, y+fh, "Joystick", -1);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (option==2) ? bright : green );
		al_font_textout_centre(font,cx, y+fh*2, "Exit", -1);

		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, bright );
		al_font_textout_centre(font, cx - 120, y+fh*option, "▶⚔", -1);
		al_font_textout_centre(font, cx + 120, y+fh*option, "⚔◀", -1);
/*		al_font_textout(font, "Some unicode symbols:", 50, 240);
		al_font_textout(font, "■□▢▣▤▥▦▧▨▩▪▫▬▭▮▯▰▱", 50, 260);
		al_font_textout(font, "▲△▴▵▷▸▹►▻▼▽▾▿◀◁◂◃◄◅◆◇◈◉◊", 50, 280);
		al_font_textout(font, "○◌◍◎●◐◑◒◓◔◕◖◗◘◙", 50, 300);
*/	}
	al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, al_map_rgba_f(1, 1, 1, 1));
}

game_event_n Options_menu::Event(ALLEGRO_EVENT event)
{
game_event_n ret = GAME_EVENT_NONE;

	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		
		switch (event.keyboard.keycode)
		{
		case ALLEGRO_KEY_UP:
			--option;
			if (option<0) option=2;
			break;
		case ALLEGRO_KEY_DOWN:
			++option;
			if (option==3) option=0;
			break;
		case ALLEGRO_KEY_SPACE:
			switch (option) 
			{
				case 0: ret = GAME_EVENT_KEYBOARD_MENU; break;
				case 1: ret = GAME_EVENT_JOYSTICK_MENU; break;
				case 2: ret = GAME_EVENT_MAIN_MENU; break;
			}	
			break;
		}
	}

	return ret;
}
