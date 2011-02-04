#ifndef Dialog_h
#define Dialog_h


#include "main.h"

#include <list>
#include <string>


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

	protected:
		Speechbubbles speechbubbles;
};


#endif  //  Dialog_h
