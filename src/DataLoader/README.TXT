While this doesn't load maps graphically yet, nor does it use LUA, it DOES
pull in the data for maps.

It currently supports a "world" configuration script (under media/world#.cfg)
that lists all of the locations where data is to be found.

The makefile is "smart enough" to compile everything under the ./src directory
but not smart enough to remove the three files that include main() functions
in them: Test1_Layers.cpp, Test2_Maps.cpp, Test3_Worlds.cpp.

So you have to go in and move/rename two of those three Test sources before
running make.

When you run the generated "bin/run_me.exe" (change the extensions in the
makefile for those of you reading this on non-Windows platforms) with no
arguments, or just with "debug" or "verbose" on the command-line, it should
tell you that it's looking for some config file.  Something like this:

  ./bin/run_me

  Usage: ./bin/run_me filename [debug] [verbose]
        filename: Path to the "world_.cfg" config file.
        debug:    If specified, turn on debugging.
        verbose:  If specified, turn on verbose error/success messages.

That "world_.cfg" means that, on the command-line, specify "world0.cfg":

  ./bin/run_me world0.cfg

You can also use "verbose", which shows error messages, or "debug" which shows
non-error (but slightly more detailed) messages.

Test3_Worlds.cpp is most comprehensive of the three tests; unless you have a
reason to JUST look at individual map or level config files (like if you're
designing levels and need to check them), there's not really that much use for
them.

However, it DOES dump out several pages worth of ALL the maps, levels, layers,
obstacles and triggers that have been specified in the config files.

Lastly, if this doesn't work on others' platforms, let me know what breaks and
where.  It may not use Allegro yet, but I still want it to be more-or-less
cross-platform and be able to compile.

Maybe suggesting changes to the makefile so it will crank out three separate
binaries, with possibility of adding more as I continue to work on this?
