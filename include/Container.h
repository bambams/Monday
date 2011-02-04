#ifndef Container_h
#define Container_h


#include "main.h"
#include "Camera.h"

#include <vector>


class Game;
class Entity;

typedef std::vector<Entity *> Entities;


class Container
{
	public:
		Container();
		virtual ~Container();

		void Set_game(Game *game);
		void Add_entity(Entity *e);
		void Remove_entity(Entity *e);
		int Number_of_entities();
		void Render(const Camera *camera);
		void Pick_up(Vector from, float r, Container *to, bool all);
		void Interact(Vector from, float r, Container *to);

		Entities &Get_entities();

	protected:
		void Process_removals();
		Game *game;

	private:
		Entities removals;
		Entities entities;
};


#endif  //  Container_h
