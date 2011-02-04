#ifndef TriggerData_h
#define TriggerData_h


/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

#include <string>
#include <vector>


/* Create the list of possible trigger types here */
enum trigger_type
{
	TRIGGER_LANDING_SPOT = 0,
	TRIGGER_ACTION,

	NUM_TRIGGER_TYPES
};


/* For now, these can be strings; may be simplified by just being ints */
typedef std::string trigger_id_type;


/* Used to hold the Trigger's ID, x coordinate, y coordinate, and trigger type
 */
struct TriggerData
{
	trigger_id_type id;
	u32 x;
	u32 y;
	trigger_type type;
};

typedef std::vector<TriggerData> TriggerArray;


#endif  //  TriggerData_h
