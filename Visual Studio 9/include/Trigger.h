#ifndef Trigger_h
#define Trigger_h

#include "Main.h"
#include "Entity.h"


typedef enum
{
	/* Trigger type is uninitialized/undefined */
	TRIGGER_TYPE_NONE = 0,

	/* Trigger responds when someone/something moves close to it (like a motion
	 * sensor, edge of map, etc.)
	 */
	TRIGGER_TYPE_PROXIMITY,

	/* Trigger responds when an Entity performs an ACTION on it (like a switch,
	 * button, etc.)
	 */
	TRIGGER_TYPE_INTERACT,

	/* Number of defined trigger types */
	NUM_TRIGGER_TYPES
} trigger_t;


class Trigger
{
	public:
		Trigger();
		virtual ~Trigger();

		bool CheckCollision(Entity *e);

	private:
		trigger_t trigger_type;
//		Action* action;
};


class Trigger_proximity :
public Trigger
{
	private:
		Item* item1;
		Item* item2;
		float distance;
};


#endif  //  Trigger_h
