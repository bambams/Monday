#include "Gui.h"
#include "Main.h"


Gui::Gui(Game* game)
{
}


ALLEGRO_FONT* Gui::Get_font()
{
	return font;
}


void Gui::Set_font(ALLEGRO_FONT* font)
{
	this->font = font;
}


char* gen_button_tag()
{
	return strdup("IDM_");
}


/**
 * Function: Button constructor, setting default gui
 */
Button::Button(Gui* gui)
{
	this->gui = gui;
}


/**
 * Function: Button constructor, setting default menu
 */
Button::Button(Menu* menu)
{
	this->menu = menu;
}


Button::Button(Gui * gui, int cx, int cy, int w, int h, char * string, char * tag)
{
	this->gui = gui;
	this->x = cx - w/2;
	this->y = cy - h/2;
	this->width = w;
	this->height = h;
	this->tag = (tag) ? strdup(tag) : gen_button_tag();
	this->string = (string) ? strdup(string) : strdup("[button]");
}


Button::~Button()
{
	if (tag) free (tag);
	if (string) free (string);
}


void Button::Draw(int x, int y, bool selected)
{
	
}


void Button::Draw_Centre(int x, int y, bool selected)
{
	ALLEGRO_COLOR green = al_map_rgba_f(0, 0.5, 0, 1);
	ALLEGRO_COLOR bright = al_map_rgba_f(0.25, 0.75, 0.25, 1);
	ALLEGRO_FONT* font = gui->Get_font();

	al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (selected) ? bright : green );
	al_draw_rectangle(this->x, this->y, this->x + this->width, this->y + this->height, al_map_rgb(100, 0, 0), 0);
	al_font_textout_centre(font,  this->x + this->width/2, this->y + this->height/2, this->string, -1);
}

