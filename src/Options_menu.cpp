/**
 * The Options menu is displayed when the user wants to change the input keys
 * for the Joystick or Keyboard.  This simply shows the menu with:
 *
 *   Keyboard
 *   Joystick
 *   Exit
 *
 * It handles the UP and DOWN keys, as well as the action key (spacebar).
 */

#include "Options_menu.h"

// Moved #include directives to Options_menu.h

Options_menu::Options_menu(Game* game)
:Menu(game)
,option(0)
{
	/**
	 * Set up the list of all available options that will be displayed onto the
	 * screen.
	 *
	 * Make sure you update Options_menu::Event() to match the number of
	 * options here, else the undefined ones will have a default event of
	 * GAME_EVENT_NONE.
	 */
	optionStrings.push_back("Keyboard");
	optionStrings.push_back("Back to Main Menu");
	optionStrings.push_back("Exit");
//	optionStrings.push_back("Exit2"); // what are these for?
//	optionStrings.push_back("Exit3");
}


void Options_menu::Render()
{
    ALLEGRO_COLOR green = al_map_rgba_f(0, 0.5, 0, 1);
    ALLEGRO_COLOR bright = al_map_rgba_f(0.25, 0.75, 0.25, 1);
	ALLEGRO_FONT* font = game->Get_font();

	int fh = 20;
	int y, cx;

	/* Center horizontally */
	cx = al_get_display_width() / 2;

	/* Center vertically */
	y = al_get_display_height() / 2 - (fh * optionStrings.size()) / 2;

	if (font)
	{
		int sizeOptions = optionStrings.size();
		for (int selectionIndex = 0; selectionIndex < sizeOptions; ++selectionIndex)
		{
			al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, (option == selectionIndex) ? bright : green );
			al_font_textout_centre(font, cx, y + fh * selectionIndex,
								   optionStrings[selectionIndex].c_str(), -1);
		}

		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, bright );
		al_font_textout_centre(font, cx - 120, y + fh * option, "▶⚔", -1);
		al_font_textout_centre(font, cx + 120, y + fh * option, "⚔◀", -1);
/*		al_font_textout(font, "Some unicode symbols:", 50, 240);
		al_font_textout(font, "■□▢▣▤▥▦▧▨▩▪▫▬▭▮▯▰▱", 50, 260);
		al_font_textout(font, "▲△▴▵▷▸▹►▻▼▽▾▿◀◁◂◃◄◅◆◇◈◉◊", 50, 280);
		al_font_textout(font, "○◌◍◎●◐◑◒◓◔◕◖◗◘◙", 50, 300);
*/	}
	al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, al_map_rgba_f(1, 1, 1, 1));
}


game_event_n Options_menu::Event(ALLEGRO_EVENT event)
{
	game_event_n ret = GAME_EVENT_NONE;

	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (event.keyboard.keycode)
		{
		case ALLEGRO_KEY_UP:
			--option;
			if (option < 0)
			{
				option = optionStrings.size() - 1;
			}
			break;
			
		case ALLEGRO_KEY_DOWN:
			++option;
			if (option >= (int)optionStrings.size())
			{
				option = 0;
			}
			break;
		case ALLEGRO_KEY_ENTER:
		case ALLEGRO_KEY_SPACE:
			Monday_out(VERBOSE_LEVEL2, std::cout, "Options menu - press option %d\n", option);
			switch (option)
			{
			case 0:
				ret = GAME_EVENT_KEYBOARD_MENU;
				break;
				
			case 1:
				ret = GAME_EVENT_MAIN_MENU;
				break;
				
			case 2:
				ret = GAME_EVENT_EXIT;
				break;
				
			default:
				break;
			}
			break;
			
		default:
			break;
		}
	}
	return ret;
}
