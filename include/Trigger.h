#ifndef Trigger_h
#define Trigger_h

#include "main.h"



typedef enum
{
	TRIGGER_TYPE_NONE = 0,
	TRIGGER_TYPE_PROXIMITY,

	TRIGGER_TYPE_USER,

	NUM_TRIGGER_TYPES
} trigger_t;


class Trigger
{
	private:
		trigger_t trigger_type;
		Action *action;
};


class Trigger_proximity
	: public Trigger()
{
	private:
		Item *item1;
		Item *item2;
		float distance;
};


#endif  //  Trigger_h
