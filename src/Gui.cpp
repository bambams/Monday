#include "Gui.h"

#include <allegro5/allegro5.h>

Gui::Gui(Game * game)
{
}

ALLEGRO_FONT * Gui::Get_font()
{
	return font;
}

void Gui::Set_font(ALLEGRO_FONT * font)
{
	this->font = font;
}


char * gen_button_tag()
{
	return ustrdup("IDM_");
} 



Button::Button()
{
	this->gui = gui;
	this->x = 0;
	this->y = 0;
	this->width = 80;
	this->height = 8;
	this->tag = NULL;
	this->string = ustrdup("[button]");
};

Button::Button(Gui * gui)
{
	this->gui = gui;
}

Button::Button(Menu * menu)
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
	this->tag = (tag) ? ustrdup(tag) : gen_button_tag();
	this->string = (string) ? ustrdup(string) : ustrdup("[button]");
}


Button::~Button()
{
	if (tag) free (tag);
	if (string) free (string);
}


void Button::SetText(char * s) { string = s; }

char * Button::GetText() {return string;}


void Button::Shape(int x, int y, int w, int h) {
	this->x =x;
	this->y =y;
	this->width =w;
	this->height =h;
} 

/* from MASkinG api */
void Button::MakeExit() {

	this->action = GAME_EVENT_EXIT;
}

void Button::Set_game_event(game_event_n ge) { this->action = ge; } 


void Button::Render()
{
	Draw_Centre(0,0,selected);
}


void Button::Draw(int x, int y, bool selected)
{
	

}


void Button::Draw_Centre(int x, int y, bool selected)
{
    ALLEGRO_COLOR green = al_map_rgba_f(0, 0.5, 0, 1);
    ALLEGRO_COLOR bright = al_map_rgba_f(0.25, 0.75, 0.25, 1);
    ALLEGRO_FONT * font = gui->Get_font();

	al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (selected) ? bright : green );
	al_draw_rectangle(this->x, this->y, this->x + this->width, this->y + this->height, al_map_rgb(100, 0, 0), 0);
	al_font_textout_centre(font,  this->x + this->width/2, this->y + this->height/2, this->string, -1);
}

