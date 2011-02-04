#include "Dialog.h"

void Dialog::Add_speechbubble(Speechbubble bubble)
{
	speechbubbles.push_back(bubble);
}

void Dialog::Clear()
{
	speechbubbles.clear();
}

void Dialog::Event(ALLEGRO_EVENT event)
{
}

void Dialog::Render()
{
}
