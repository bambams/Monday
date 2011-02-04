#######################################################################
#
# Instructions:
#
# make
#   Compiles all .c and .cpp files in the src directory to .o
#   files in the obj directory, and links them into an
#   executable named 'game' or 'game.exe' in the currect directory.
#
# make clean
#   Removes all .o files from the obj directory.
#
# make veryclean
#   Removes all .o files and the game executable.
#
# Optional parameters:
#
# STATICLINK=1
#   Compiles/removes a statically linked version of the game without
#   DLL dependencies. The static object files are put in obj/static
#   and the executable has '_static' appended to the name.
#
# DEBUG=1
#   Compiles/removes debugging information.
#
# NAME=game_name
#   Sets the name of the game executable. By default the game
#   executable is called 'game' or 'game.exe'.
#
# If you use add-on libraries, add them to the lines starting with
# 'LIBS='. Make sure you enter the libraries in both lines, for the
# normal and static version!
#
#######################################################################

CC = gcc
CXX = g++
LD = g++
CFLAGS = -Iinclude -I/usr/include/lua5.1 -s -W -Wall

# Add-on libraries go here
ifdef STATICLINK
	LIBS =
else
	LIBS =
endif

ifdef PROFILE
	CFLAGS += -g3 -pg
	LFLAGS += -g3 -pg
else
	ifdef DEBUG
		CFLAGS += -g3 -ggdb3 -O0
		LFLAGS += -g3 -ggdb3 -O0
	else
		CFLAGS += -O3
	endif
endif

ifndef NAME
	NAME = monday
endif

ifndef WINDOWS
ifdef MINGDIR
	WINDOWS = 1
endif
endif

LIBS += -la5_iio -la5_font -la5_ttf -llua5.1


ifdef WINDOWS
#	RM = del /q
	CFLAGS += -D__GTHREAD_HIDE_WIN32API
	LFLAGS = -Wl,--subsystem,windows
	ifdef STATICLINK

		ifdef DEBUG
			LIBS += -lallegd_s-4.9.5
		else
			LIBS += -lalleg_s-4.9.5
		endif

		CFLAGS += -DSTATICLINK
		LIBS += -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
		OBJDIR = obj/static
		BIN = $(NAME)_static.exe
	else
		ifdef DEBUG
			CFLAGS += -DDEBUG
			LIBS += -lallegd-4.9.5
			OBJDIR = obj/debug
			BIN = $(NAME)_debug.exe
		else
			LIBS += -lalleg-4.9.5
			OBJDIR = obj/release
			BIN = $(NAME).exe
		endif
	endif
else
	RM = rm -f
	ifdef STATICLINK
		LIBS += `allegro-config --libs --static` -lXrender
		OBJDIR = obj/static
		BIN = $(NAME)_static
	else
		LIBS += `allegro5-config --libs`
		OBJDIR = obj
		BIN = $(NAME)
	endif
endif

OBJ_CPP := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.cpp,%.o,$(wildcard src/*.cpp))))
OBJ_C := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.c,%.o,$(wildcard src/*.c))))

all: game

$(OBJDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

game: $(OBJ_C) $(OBJ_CPP)
	$(LD) -o $(BIN) $(OBJ_C) $(OBJ_CPP) $(LIBS) $(LFLAGS)

clean:
ifdef WINDOWS
ifneq ($(OBJ_C),)
	-$(RM) $(subst /,\,$(OBJ_C))
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(subst /,\,$(OBJ_CPP))
endif
else
ifneq ($(OBJ_C),)
	-$(RM) $(OBJ_C)
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(OBJ_CPP)
endif
endif

veryclean: clean
	-$(RM) $(BIN)
