#include "Debug.h"
#include "Game.h"


/**
 * Function: Check the command line for settings, such as turning on or off
 * debugging output, etc.
 */
bool parseArgs(int argc, char* argv[])
{
	/* Inital values */
	showDebug = 0;
	bool playGame = true;

	std::vector<std::string> filenames;
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		/* Abbreviated options begin with "-", and full options begin with
		 * "--" to conform to standards.
		 */
		if ('-' == arg[0])
		{
			/* DEBUG */
			if ("-d" == arg || "--debug" == arg)
			{
				/* Level of debugging needs to be indicated next */
				if (i + 1 < argc)
				{
					int level = atoi(argv[i + 1]);
					debugOn(std::cout, level);

					/* Forces the for..loop to skip over the next argument */
					++i;
				}
				else
				{
					debugOn(std::cout, WARNING_LEVEL);
					Monday_out(WARNING_LEVEL, std::cerr, "*WARNING: You must specify a debugging level with the \"%s\" switch.\n", arg.c_str());
					Monday_out(WARNING_LEVEL, std::cerr, "Assuming level %d.\n", WARNING_LEVEL);
				}
				continue;
			}
			else if ("-nd" == arg || "--nodebug" == arg)
			{
				debugOff(std::cout);
				continue;
			}

			else if ("-h" == arg || "--help" == arg)
			{
				/* Indicate that the user wishes to view the help without
				 * playing the game.
				 */
				playGame = false;
				continue;
			}
		}

		/* If we got here, we didn't recognize the argument.  We can show some
		 * sort of "that command doesn't exist..." or show the help() if we
		 * want...
		 */
	}

	/* Indicate that the user will continue and play the game.
	 */
	return playGame;
}

void help(const std::string &filename)
{
	std::cout << "Usage: " << filename << " [-d #|--debug #] [-nd|--nodebug] [-h | --help]\n";
	std::cout << "\t-d #, --debug #:  Turns on debugging messages to indicated level.\n";
	std::cout << "\t-nd, --nodebug:   (default) Turns off extra non-error messages.\n";
	std::cout << "\t-h, --help:       Show this help info.\n";
}


int main(int argc, char *argv[])
{
	/* Parse command-line arguments, if there are any */
	if (argc > 1)
	{
		if (false == parseArgs(argc, argv))
		{
			/* User specified "-h" or "--help", so show usage and exit without
			 * running the game.
			 */
			help(argv[0]);
			return 0;
		}
	}

	Game game;
	game.Run();
	return 0;
}
END_OF_MAIN()
