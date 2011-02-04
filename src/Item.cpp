#include "Item.h"
#include <allegro5/allegro5.h>

void Item::Render(Vector p)
{
	al_draw_rectangle(p.X()-5, p.Y()-5, p.X()+5, p.Y()+5, al_map_rgb(100, 100, 100), 0);
}
