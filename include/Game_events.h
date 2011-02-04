#ifndef Game_events_h
#define Game_events_h


typedef enum game_event_n
{
	GAME_EVENT_EXIT = -1,
	GAME_EVENT_NONE = 0,
	GAME_EVENT_MAIN_MENU,
	GAME_EVENT_PLAY_GAME,
	GAME_EVENT_LOAD_GAME,
	GAME_EVENT_OPTIONS_MENU,
	GAME_EVENT_KEYBOARD_MENU,

	GAME_EVENT_USER
} game_event_n;


#endif  //  Game_events_h
