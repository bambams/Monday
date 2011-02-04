#ifndef Main_h
#define Main_h


/* All this file will do is include the appropriate Allegro and dependency
 * files: these will ONLY be the ones found in your compiler's directory
 * (#includes will be defined with <...>, not "...").  This file will not
 * include any files from the local project.
 */
extern "C"
{
#include <allegro5/allegro5.h>
#include <allegro5/internal/aintern_bitmap.h>
#include <allegro5/bitmap_new.h>
#include <allegro5/a5_ttf.h>
#include <allegro5/a5_iio.h>

#include <lua.h>
}


#endif  // Main_h
