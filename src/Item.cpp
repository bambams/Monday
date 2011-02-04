#include "Item.h"
#include "main.h"


void Item::Render(const Vector &p)
{
	float x = p.X();
	float y = p.Y();
	al_draw_rectangle(x - 5, y - 5, x + 5, y + 5, al_map_rgb(100, 100, 100), 0);
}
