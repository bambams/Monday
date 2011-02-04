#include "Keyboard_menu.h"

#include "main.h"
#include "Game.h"


KEYBOARD_POS keyb_pos_104[]={

  { -1, -1,92,27},

  { 7,13,4,4},  //A
  {25,17,4,4},  //B
  {17,17,4,4},  //C
  {15,13,4,4},  //D
  {14, 9,4,4},  //E
  {19,13,4,4},  //F
  {23,13,4,4},  //G
  {27,13,4,4},  //H
  {34, 9,4,4},  //I
  {31,13,4,4},  //J
  {35,13,4,4},  //K
  {39,13,4,4},  //L
  {33,17,4,4},  //M
  {29,17,4,4},  //N
  {38, 9,4,4},  //O
  {42, 9,4,4},  //P
  { 6, 9,4,4},  //Q
  {18, 9,4,4},  //R
  {11,13,4,4},  //S
  {22, 9,4,4},  //T
  {30, 9,4,4},  //U
  {21,17,4,4},  //V
  {10, 9,4,4},  //W
  {13,17,4,4},  //X
  {26, 9,4,4},  //Y
  { 9,17,4,4},  //Z

  {40, 5,4,4},  //0
  { 4, 5,4,4},  //KEY_1
  { 8, 5,4,4},
  {12, 5,4,4},
  {16, 5,4,4},
  {20, 5,4,4},
  {24, 5,4,4},
  {28, 5,4,4},
  {32, 5,4,4},
  {36, 5,4,4},  //9


  {74,21,8,4},  //KEY_0_PAD
  {74,17,4,4},  //KEY_1
  {78,17,4,4},
  {82,17,4,4},
  {74,13,4,4},
  {78,13,4,4},
  {82,13,4,4},
  {74, 9,4,4},
  {78, 9,4,4},
  {82, 9,4,4},  //KEY_9_PAD


  { 8, 0,4,4},  //F1
  {12, 0,4,4},  //F2
  {16, 0,4,4},  //F3
  {20, 0,4,4},  //F4

  {26, 0,4,4},  //F5
  {30, 0,4,4},  //F6
  {34, 0,4,4},  //F7
  {38, 0,4,4},  //F8

  {44, 0,4,4},  //F9
  {48, 0,4,4},  //F10
  {52, 0,4,4},  //F11
  {56, 0,4,4},  //F12


  { 0, 0,4,4},  //esc
  { 0, 5,4,4},  //KEY_TILDE
  {44, 5,4,4},  //KEY_MINUS
  {48, 5,4,4},  //KEY_EQUALS
  {56, 5,4,4},  //KEY_BACKSPACE

  { 0, 9,6,4},  //KEY_TAB

  {46, 9,4,4},  //KEY_OPENBRACE [
  {50, 9,4,4},  //KEY_CLOSEBRACE ]

  {54, 9,6,8},  //KEY_ENTER

  {43,13,4,4},  //KEY_COLON
  {47,13,4,4},  //KEY_QUOTE

  {52, 5,4,4},  //KEY_BACKSLASH
//  {51,13,4,4},  //KEY_BACKSLASH2
  { 5,17,4,4},  // backslash2

  {37,17,4,4},  //KEY_COMMA ,
  {41,17,4,4},  //KEY_STOP .
  {45,17,4,4},  //KEY_SLASH /
  {15,21,24,4}, //KEY_SPACE

  {61, 5,4,4},  //ins
  {61, 9,4,4},  //del
  {65, 5,4,4},  //home
  {65, 9,4,4},  //end
  {69, 5,4,4},  //pgup
  {69, 9,4,4},  //pgdn

  {61,21,4,4},  //left
  {69,21,4,4},  //right
  {65,17,4,4},  //up
  {65,21,4,4},  //down

  {78, 5,4,4},  //KEY_SLASH_PAD
  {82, 5,4,4},  //KEY_ASTERISK_PAD
  {86, 5,4,4},  //KEY_MINUS_PAD
  {86, 9,4,8},  //KEY_PLUS_PAD
  {82,21,4,4},  //KEY_DEL_PAD
  {86,17,4,8},  //KEY_ENTER_PAD

  {61, 0,4,4},  //Print Screen/Sys Rq
  {69, 0,4,4},  //Pause/Break

  {74, 0,1,1},  //KEY_ABNT_C1
  {75, 0,1,1},  //KEY_YEN
  {76, 0,1,1},  //KEY_KANA
  {77, 0,1,1},  //KEY_CONVERT
  {78, 0,1,1},  //KEY_NOCONVERT

  {79, 0,1,1},  //KEY_AT
  {80, 0,1,1},  //KEY_CIRCUMFLEX
  {81, 0,1,1},  //KEY_COLON2
  {82, 0,1,1},  //KEY_KANJI

//modifiers

  { 0,17,5,4},  //left shift
  {49,17,7,4},  //R SHIFT
  { 0,21,5,4},  //left CTL
  {55,21,5,4},  //right CTL
  {10,21,5,4},  //left ALT
  {39,21,5,4},  //right ALT
  { 5,21,5,4},  //left WIN
  {44,21,5,4},  //right WIN
  {49,21,6,4},  //LIST
  {65, 0,4,4},  //Scroll Lock
  {74, 5,4,4},  //KEY_NUMLOCK
  {0,13,7,4},  //KEY_CAPSLOCK

  {0,0,0,0}	//end of list   marker. do not remove


};



void keyrenderGL() {

};


ALLEGRO_FONT *large_font, *small_font;

int choose_mode = 0;  /* 0 = table,  1 = sequence (choose all in turn) */


void draw_keycap_text(float x, float y, int c) {
    ALLEGRO_COLOR white = al_map_rgba_f(1.0, 1.0, 1.0, 1);

	al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, white );

	if (c >= ALLEGRO_KEY_0 && c<= ALLEGRO_KEY_9) al_font_textout(small_font, x+2, y+16, al_keycode_to_name(c), -1 );
	if (c >= ALLEGRO_KEY_A && c<= ALLEGRO_KEY_Z) al_font_textout(large_font, x+2, y, al_keycode_to_name(c), -1 );
}





void 
draw_virtual_keyboard(KEYBOARD_POS * keypos, float x_pos, float y_pos, float x_scale, float y_scale) {
    ALLEGRO_COLOR green = al_map_rgba_f(0, 0.75, 0, 1);
    ALLEGRO_COLOR bright = al_map_rgba_f(0.50, 1.0, 0.50, 1);
	ALLEGRO_COLOR white = al_map_rgba_f(1, 1, 1, 1);

	ALLEGRO_KEYBOARD_STATE state;
	al_get_keyboard_state(&state);

	int n=0; float x,y,w,h;
	while ((keypos->w !=0) && n<128) {

		x=keypos->x * x_scale + x_pos;
		y=keypos->y * y_scale + y_pos;
		w=keypos->w * x_scale;
		h=keypos->h * y_scale;

	//	if (n==1) printf("x %f,%f,%f,%f ", x,y,w,h);	

		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (al_key_down(&state,n)) ? white : bright );
		al_draw_rectangle( x, y, x+w-2, y+h-2, al_map_rgb(200, 200, 200), 1);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, green );
		al_draw_rectangle( x+1, y+1, x+w-1, y+h-1, al_map_rgb(200, 200, 200), 0);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, white );
		al_draw_rectangle( x, y, x+w-2, y+h-2, al_map_rgb(200, 200, 200), 0);
		draw_keycap_text(x+2, y, n);
		keypos++; n++;
	}

	//printf("\n");


}

void
draw_keyselect_table(GAME_CONTROL_DEFS * defs, int n, int scroll_n, 
					 ALLEGRO_FONT * font, int x_pos, int y_pos) {
    int a;
    int c1,c2,c3;
    ALLEGRO_COLOR bright = al_map_rgba_f(0.50, 1.0, 0.50, 1);
	ALLEGRO_COLOR white = al_map_rgba_f(1, 1, 1, 1);


	c1 = x_pos -100;
	c2 = x_pos +20;
	c3 = x_pos +100;

	int y= y_pos;

	al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, bright);
	al_font_textout_centre(font, c1, y, "Action", -1);
	al_font_textout_centre(font, c2, y, "Key1", -1);
	al_font_textout_centre(font, c3, y, "Key2", -1);

	y+=24;	

	for (a = scroll_n;  a< (n-scroll_n) ; a++, y+=20) {
	    al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (a==0) ? bright : white );
		al_font_textout_centre(font, c1, y, defs[a].name, -1);
		al_font_textout_centre(font, c2, y, al_keycode_to_name(defs[a].key1), -1);
		al_font_textout_centre(font, c3, y, al_keycode_to_name(defs[a].key2), -1);
	}

}



Keyboard_menu::Keyboard_menu(Game* game)
:Menu(game)
,option(0)
{
	keyboard_defs.pos = keyb_pos_104;
	keyboard_defs.numkeys = 104;
	keyboard_defs.name = "English (UK) 104 key";
	keyboard_defs.w = 92;   //in 1/4 key units 
	keyboard_defs.h = 27;
	
	large_font = al_ttf_load_font("media/DejaVuSans.ttf", 14, 0);	
	small_font = al_ttf_load_font("media/DejaVuSans.ttf", 10, 0);

}


void Keyboard_menu::Render()
{
    ALLEGRO_COLOR green = al_map_rgba_f(0, 0.75, 0, 1);
    ALLEGRO_COLOR bright = al_map_rgba_f(0.50, 1.0, 0.50, 1);
	ALLEGRO_FONT* font = game->Get_font();

	draw_virtual_keyboard(keyboard_defs.pos,  32,200,
		                  600.0/ keyboard_defs.w, 160.0/ keyboard_defs.h );

	draw_keyselect_table(game_control_defs, 5, 0, font, 320, 0);


	int fh = 20;
	int cx,n=4,y;

	y = al_get_display_height() - fh * n;
	cx = al_get_display_width()*3/4;

	if(font)
	{
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (option==0) ? bright : green );
		al_font_textout_centre(font, cx, y, "Choose all", -1);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (option==1) ? bright : green );
		al_font_textout_centre(font, cx, y+fh, "Load", -1);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (option==2) ? bright : green );
		al_font_textout_centre(font,  cx, y+fh*2, "Save", -1);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (option==3) ? bright : green );
		al_font_textout_centre(font, cx, y+fh*3, "Exit", -1);

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



game_event_n Keyboard_menu::Event(ALLEGRO_EVENT event)
{
game_event_n ret = GAME_EVENT_NONE;

	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		
		switch (event.keyboard.keycode)
		{
		case ALLEGRO_KEY_UP:
			option--;
			if (option<0) option=3;
			break;
		case ALLEGRO_KEY_DOWN:
			option++;
			if (option>3) option=0;
			break;
		case ALLEGRO_KEY_SPACE:
			switch (option) 
			{
				case 0: choose_mode = 1; break;
				case 1: ret = GAME_EVENT_OPTIONS_MENU; break;
				case 2: ret = GAME_EVENT_OPTIONS_MENU; break;
				case 3: ret = GAME_EVENT_OPTIONS_MENU; break;
			}	
			break;
		}
	}

	return ret;
}

