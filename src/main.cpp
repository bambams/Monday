#include "Game.h"

int main(int argc, const char *argv[])
{

int i;
bool windowed = false, fullscreen = false, debugmode=false;

   for (i = 1; i < argc; i++) {
      if (!strcmp (argv[i], "-f") || !strcmp (argv[i], "--fullscreen"))
         fullscreen = true;

      if (!strcmp (argv[i], "-w") || !strcmp (argv[i], "--window"))
         windowed = true;

      if (!strcmp (argv[i], "-d") || !strcmp (argv[i], "--debug"))
         debugmode = true;

      /* -p x y w h	sets window position & size (overrides config file) */
      if (!strcmp (argv[i], "-p")) {
		if (i < argc-4) {	
		 	int x = strtol(argv[++i],NULL,0);
		 	int y = strtol(argv[++i],NULL,0);
		 	int w = strtol(argv[++i],NULL,0);
		 	int h = strtol(argv[++i],NULL,0);
		}
	}	

      if (!strcmp (argv[i], "--help")) {
         printf("Usage - ./monday [params]\n"
		  "\n"
		  "-f	forces fullscreen\n"
		  "-w	forces windowed\n"
		  "-d or --debug	opens debug window and adds extra options to menus\n"
		  "-p x y w h	sets window position & size (overrides config file)\n"
		  "--harder, --better, --stronger, --faster  makes it better.\n"	
		);
         return EXIT_SUCCESS;
      }
   }


/* Instantiate the Game Object and run it */
	Game game;
	game.Set_debugconsole(debugmode);
	game.Run();
	return 0;
}
END_OF_MAIN()