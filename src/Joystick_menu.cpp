#include "Joystick_menu.h"
extern "C"
{
#include "allegro5/a5_ttf.h"
}
#include "Game.h"



JOYSTICK_POS joyb_pos_ps2[]= {
	{40,20,4,4},  {44,16,4,4},
	{36,16,4,4},  {40,12,4,4},
	{20,4,4,3},  {26,4,4,3}
};
	

void joyrenderGL() {

};



static ALLEGRO_FONT *large_font, *small_font;

static int choose_mode = 0;  /* 0 = table,  1 = sequence (choose all in turn) */


void draw_joybutton_text(float x, float y, int c) {
    ALLEGRO_COLOR white = al_map_rgba_f(1.0, 1.0, 1.0, 1);

	al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, white );

	if (c >= ALLEGRO_KEY_0 && c<= ALLEGRO_KEY_9) al_font_textout(small_font, x+2, y+16, al_keycode_to_name(c), -1 );
	if (c >= ALLEGRO_KEY_A && c<= ALLEGRO_KEY_Z) al_font_textout(large_font, x+2, y, al_keycode_to_name(c), -1 );
}



void 
draw_virtual_joystick(JOYSTICK_POS * joypos, float x_pos, float y_pos, float x_scale, float y_scale) {

    ALLEGRO_COLOR green = al_map_rgba_f(0, 0.75, 0, 1);
    ALLEGRO_COLOR bright = al_map_rgba_f(0.50, 1.0, 0.50, 1);
	ALLEGRO_COLOR white = al_map_rgba_f(1, 1, 1, 1);

	ALLEGRO_KEYBOARD_STATE state;
	al_get_keyboard_state(&state);


	int n=0; float x,y,w,h;
	while ((joypos->w !=0) && n<128) {

		x=joypos->x * x_scale + x_pos;
		y=joypos->y * y_scale + y_pos;
		w=joypos->w * x_scale;
		h=joypos->h * y_scale;

	//	if (n==1) printf("x %f,%f,%f,%f ", x,y,w,h);	

		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (al_key_down(&state,n)) ? white : bright );
		al_draw_rectangle( x, y, x+w-2, y+h-2, al_map_rgb(200, 200, 200), 1);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, green );
		al_draw_rectangle( x+1, y+1, x+w-1, y+h-1, al_map_rgb(200, 200, 200), 0);
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, white );
		al_draw_rectangle( x, y, x+w-2, y+h-2, al_map_rgb(200, 200, 200), 0);
		draw_joybutton_text(x+2, y, n);
		joypos++; n++;
	}

	//printf("\n");


}


char joystick_control_short_name[20]="J  S  A  B    ";

char * gamejoyctl_to_name( int joy, int stick, int ab, int ab_n) {
	char * ret = joystick_control_short_name; 
	char * ab_name;
	
	switch (ab) {
		case GCD_BUTTON: ab_name= "B";	break; 
		case GCD_AXIS_LOW: ab_name="A-"; break;
        	case GCD_AXIS_HIGH: ab_name="A+"; break;
		default: ab_name = "."; 
	}
		uszprintf(ret,20,"J%1d S%d %s%d", joy, stick,ab_name,ab_n);
	return ret;
} 


void
draw_joyselect_table(GAME_CONTROL_DEFS * defs, int n, int scroll_n, 
					 ALLEGRO_FONT * font, int x_pos, int y_pos) {
    int a;
    int c1,c2,c3;
    ALLEGRO_COLOR bright = al_map_rgba_f(0.50, 1.0, 0.50, 1);
	ALLEGRO_COLOR white = al_map_rgba_f(1, 1, 1, 1);


	c1 = x_pos -150;
	c2 = x_pos +20;
	c3 = x_pos +150;

	int y= y_pos;

	al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, bright);
	al_font_textout_centre(font, c1, y, "Action", -1);
	al_font_textout_centre(font, c2, y, "Control1", -1);
	al_font_textout_centre(font, c3, y, "Control2", -1);

	y+=24;	

	for (a = scroll_n;  a< (n-scroll_n) ; a++, y+=20) {
	    al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (a==0) ? bright : white );
		al_font_textout_centre(font, c1, y, defs[a].name, -1);
		
		al_font_textout_centre(font, c2, y,
			gamejoyctl_to_name( defs[a].joy1, defs[a].stick1, defs[a].ab_1, defs[a].ab_n_1 ), -1);
		al_font_textout_centre(font, c3, y,
			gamejoyctl_to_name( defs[a].joy2, defs[a].stick2, defs[a].ab_2, defs[a].ab_n_2 ), -1);
	}

}



Joystick_menu::Joystick_menu(Game* game)
:Menu(game)
,option(0)
{
	joystick_defs.pos = joyb_pos_ps2;
	joystick_defs.numkeys = 6;
	joystick_defs.name = "PS2 Dual Analog";
	joystick_defs.w = 92;   //in 1/4 key units 
	joystick_defs.h = 27;
	
	large_font = al_ttf_load_font("media/DejaVuSans.ttf", 14, 0);	
	small_font = al_ttf_load_font("media/DejaVuSans.ttf", 10, 0);

}

int Joystick_menu::selected = 1;

void Joystick_menu::Render()
{
   ALLEGRO_COLOR green = al_map_rgba_f(0, 0.75, 0, 1);
   ALLEGRO_COLOR bright = al_map_rgba_f(0.50, 1.0, 0.50, 1);
   ALLEGRO_FONT* font = game->Get_font();
   ALLEGRO_JOYSTICK * selected_joystick = NULL;

	int cx,y;

	if (al_num_joysticks()>0) {

		selected_joystick = al_get_joystick(selected);
		cx = al_get_display_width()/2;
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, bright );
		al_font_textout_centre(font, cx, 20, al_get_joystick_name(selected_joystick), -1);
		
		draw_virtual_joystick(joystick_defs.pos,  32,200,
		                  600.0/ joystick_defs.w, 160.0/ joystick_defs.h );
	}

	draw_joyselect_table(game_control_defs, 5, 0, font, 320, 40);

	int fh = 20,n=4;

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



game_event_n Joystick_menu::Event(ALLEGRO_EVENT event)
{
game_event_n ret = GAME_EVENT_NONE;

	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		
		switch (event.keyboard.keycode)
		{
		case ALLEGRO_KEY_UP:
			--option;
			if (option<0) option=3;
			break;
		case ALLEGRO_KEY_DOWN:
			++option;
			if (option==4) option=0;
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

