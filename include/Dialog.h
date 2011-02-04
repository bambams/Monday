#ifndef Dialog_h
#define Dialog_h

#include <list>
#include <string>
#include <allegro5/allegro5.h>

//Placeholder, we can advance this as a class later
typedef std::string Speechbubble;

typedef std::list<Speechbubble> Speechbubbles;

class Dialog
{
public:
	void Add_speechbubble(Speechbubble bubble);
	void Clear();
	void Event(ALLEGRO_EVENT event);
	void Render();
private:
	Speechbubbles speechbubbles;
};

#endif
