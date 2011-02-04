Note: Added to GitHub by Brandon McCaig. The original repository's life
was coming to an end so I added it to GitHub to preserve it.

Building

Dependencies

    allegro 4.9.x
        Follow instructions in allegro, consult forums as http://www.allegro.cc/ if you have problems.

        NOTE:
        Both MinGW and MSVC ports of 4.9.6 (1/2) have been made at http://www.allegro5.org/
        - runtime DLLs
        - precompiled libraries
        - Allegro5 include files

        Therefore, if you use these compilers, you may not need to compile allegro5 yourself.


    lua 5.1.4
        http://www.lua.org/download.html


    There are two ways to build monday, both are very simple as long as you have the tools installed.

        Using scons
            Just type "scons" and press ENTER, that's it.

            Options
                debug=1

        Using make
            Just as simple as scons, just type "make" and press ENTER.
