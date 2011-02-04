g++ -c -o obj/Animation.o      -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Animation.cpp
g++ -c -o obj/Animator.o       -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Animator.cpp
g++ -c -o obj/Dialog.o         -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Dialog.cpp
g++ -c -o obj/Entity.o         -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Entity.cpp
g++ -c -o obj/Entity_manager.o -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Entity_manager.cpp
g++ -c -o obj/Game.o           -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Game.cpp
g++ -c -o obj/Lua_wrapper.o    -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Lua_wrapper.cpp
g++ -c -o obj/main.o           -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/main.cpp
g++ -c -o obj/Main_menu.o      -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Main_menu.cpp
g++ -c -o obj/Menu.o           -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Menu.cpp
g++ -c -o obj/Player.o         -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Player.cpp
g++ -c -o obj/Vector.o         -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude src/Vector.cpp
g++    -o Monday.exe           -g3 -ggdb3 -W -Wall -Wno-unused -Iinclude obj/*.o -lallegd-4.9.5 -llua -Wl,--subsystem,windows -la5_iiod -la5_font -la5_ttfd
