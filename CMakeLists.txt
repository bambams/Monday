#-----------------------------------------------------------------------------#
#
#   Build system setup
#
#-----------------------------------------------------------------------------#

cmake_minimum_required(VERSION 2.6)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

option(USE_CXX "Build C++ portions." on)


#-----------------------------------------------------------------------------#
#
#   Debug and Release builds
#
#-----------------------------------------------------------------------------#

if(GRADE_STANDARD)
    set(CMAKE_CONFIGURATION_TYPES "Release" CACHE INTERNAL "internal")
endif(GRADE_STANDARD)

if(GRADE_DEBUG)
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE INTERNAL "internal")
endif(GRADE_DEBUG)

if(GRADE_STANDARD AND GRADE_DEBUG)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE INTERNAL "internal")
endif(GRADE_STANDARD AND GRADE_DEBUG)


#-----------------------------------------------------------------------------#
#
#   C or C++ options
#
#-----------------------------------------------------------------------------#

if(USE_CXX)
    project(ALLEGRO C CXX)
else(USE_CXX)
    project(ALLEGRO C)
endif(USE_CXX)

set(ALLEGRO_VERSION 4.9.5)


#-----------------------------------------------------------------------------#
#
#   Search in the `cmake' directory for additional CMake modules.
#
#-----------------------------------------------------------------------------#

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)


#-----------------------------------------------------------------------------#
#
#   Search for C header files in these directories.
#
#-----------------------------------------------------------------------------#

include_directories(
    include
    # For out-of-source builds.
    ${CMAKE_BINARY_DIR}/include
)


#-----------------------------------------------------------------------------#
#
#   Lists of all the source files.
#
#-----------------------------------------------------------------------------#

include(FileList)


#-----------------------------------------------------------------------------#
#
#   Build options
#
#-----------------------------------------------------------------------------#

option(GRADE_STANDARD "Build the standard non-debug version" on)
option(GRADE_DEBUG "Build the debug version" on)
option(GRADE_PROFILE "Build the profiling version" off)

#
# For developers.
#

option(STRICT_WARN "Halt at warnings" off)


#-----------------------------------------------------------------------------#
#
#   Platform configuration
#
#-----------------------------------------------------------------------------#

include(CheckCSourceCompiles)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckTypeSize)

#
# Configure for MSVC
#

if(MSVC_IDE)
    set(COMPILER_MSVC 1)
endif(MSVC_IDE)

#
# Configure for gcc
#

if(CMAKE_COMPILER_IS_GNUCC)
    set(COMPILER_GCC 1)
endif(CMAKE_COMPILER_IS_GNUCC)

#
# Configure compilers or platforms
#

if(MINGW)
    set(ALLEGRO_MINGW32 1)
endif(MINGW)

if(UNIX)
    set(ALLEGRO_UNIX 1)
endif(UNIX)

if(COMPILER_MSVC)
    set(ALLEGRO_MSVC 1)
endif(COMPILER_MSVC)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX 1)
    set(ALLEGRO_MACOSX 1)
    set(ALLEGRO_UNIX 0)
    set(WANT_X11 off)
    # This flag is required on some versions of Mac OS X to avoid linker
    # problems with global variables which are not explicitly initialised.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(BORLAND)
    set(ALLEGRO_BCC32 1)
endif(BORLAND)

#
# Begin tests.
#

include(TestBigEndian)
test_big_endian(ALLEGRO_BIG_ENDIAN)
if(NOT ALLEGRO_BIG_ENDIAN)
    set(ALLEGRO_LITTLE_ENDIAN 1)
endif(NOT ALLEGRO_BIG_ENDIAN)

check_type_size("_Bool" ALLEGRO_HAVE__BOOL)

#-----------------------------------------------------------------------------#
#
#   Libraries that we always need to link against on this platform
#
#-----------------------------------------------------------------------------#

set(PLATFORM_LIBS)

if(ALLEGRO_UNIX) # not MACOSX
    list(APPEND PLATFORM_LIBS m ${CMAKE_THREAD_LIBS_INIT})
endif(ALLEGRO_UNIX)

if(SUPPORT_X11)
    list(APPEND PLATFORM_LIBS ${X11_LIBRARIES})
endif(SUPPORT_X11)

if(WIN32)
    list(APPEND PLATFORM_LIBS
        kernel32 user32 gdi32 comdlg32 ole32 dinput8 dxguid winmm
        )
endif(WIN32)

if(MACOSX)
    list(APPEND PLATFORM_LIBS "-framework AppKit")
    list(APPEND PLATFORM_LIBS "-framework IOKit")
endif(MACOSX)

#-----------------------------------------------------------------------------#

if(STRICT_WARN)
    if(COMPILER_GCC)
        set(WFLAGS "-W -Wall -Wstrict-prototypes -Wno-unused-parameter -Werror")
    endif(COMPILER_GCC)
    if(COMPILER_MSVC)
        set(WFLAGS "/W4 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
    endif(COMPILER_MSVC)
else(STRICT_WARN)
    if(COMPILER_GCC)
        set(WFLAGS "-W -Wall -Wno-unused")
    endif(COMPILER_GCC)
    if(COMPILER_MSVC)
        set(WFLAGS "/W3 -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE")
    endif(COMPILER_MSVC)
endif(STRICT_WARN)

#-----------------------------------------------------------------------------#

macro(add_allegro_build nm extra_flags)

    set(LIBRARY_CFLAGS
        "-DALLEGRO_LIB_BUILD ${WFLAGS} ${extra_flags}"
        )

    # Suppress errors about _mangled_main_address being undefined
    # on Mac OS X.
    if(MACOSX)
        set(LIBRARY_LINK_FLAGS "-flat_namespace -undefined suppress")
    endif(MACOSX)

    macro(really_add_allegro_build nam target libtype more_extra_flags)
        set(OUTPUT_LIBRARY ${nam})
        add_library(${target} ${libtype} ${LIBRARY_SOURCES})
        set_target_properties(${target}
            PROPERTIES
            COMPILE_FLAGS "${more_extra_flags} ${LIBRARY_CFLAGS} -DALLEGRO_SRC"
            LINK_FLAGS "${LIBRARY_LINK_FLAGS}"
            OUTPUT_NAME ${nam}
            )

        target_link_libraries(${target} ${PLATFORM_LIBS})

        install(TARGETS ${target}
                DESTINATION lib
                LIBRARY PERMISSIONS
                    OWNER_READ OWNER_WRITE OWNER_EXECUTE
                    GROUP_READ             GROUP_EXECUTE
                    WORLD_READ             WORLD_EXECUTE
                )
    endmacro(really_add_allegro_build)

    if(SHARED)
        really_add_allegro_build(${nm}-${ALLEGRO_VERSION} ${nm}_shared SHARED "")
    endif(SHARED)

    if(STATIC)
        really_add_allegro_build(${nm}_s-${ALLEGRO_VERSION} ${nm}_static STATIC
            "-DALLEGRO_STATICLINK")
    endif(STATIC)

endmacro(add_allegro_build)

if(GRADE_STANDARD)
    if(COMPILER_GCC)
        add_allegro_build(alleg
            "-O2 -funroll-loops -ffast-math -fomit-frame-pointer")
    endif(COMPILER_GCC)
    if(COMPILER_MSVC)
        add_allegro_build(alleg "")
    endif(COMPILER_MSVC)
    if(BORLAND)
        add_allegro_build(alleg "-O2")
    endif(BORLAND)
endif(GRADE_STANDARD)

if(GRADE_DEBUG)
    if(COMPILER_GCC)
        add_allegro_build(allegd
            "-DDEBUGMODE -g2 -O0")
    endif(COMPILER_GCC)
    if(COMPILER_MSVC)
        add_allegro_build(allegd "-DDEBUGMODE")
    endif(COMPILER_MSVC)
    if(BORLAND)
        add_allegro_build(allegd "-DDEBUGMODE=1 -v")
    endif(BORLAND)
endif(GRADE_DEBUG)

if(GRADE_PROFILE)
    if(COMPILER_GCC)
        add_allegro_build(allegp
            "-pg -O2 -funroll-loops -ffast-math")
    endif(COMPILER_GCC)
    if(COMPILER_MSVC)
        add_allegro_build(allegp "")
    endif(COMPILER_MSVC)
endif(GRADE_PROFILE)

#-----------------------------------------------------------------------------#
#
#   Programs
#
#-----------------------------------------------------------------------------#

if(COMPILER_MSVC)
    set(EXECUTABLE_TYPE "WIN32")
endif(COMPILER_MSVC)

# Select libraries to link the programs with.  We choose
# standard, debug, profile in that order, and prefer statically linked
# over dynamically linked.  It would be worth making this configurable.

if(GRADE_STANDARD)
    set(LIB_TO_LINK alleg)
    set(FONT_LIB_TO_LINK a5_font)
    if(COMPILER_MSVC)
        set(EXE_CFLAGS "")
    elseif(COMPILER_GCC)
        set(EXE_CFLAGS "-O2 -funroll-loops -ffast-math -fomit-frame-pointer")
    endif(COMPILER_MSVC)
elseif(GRADE_DEBUG)
    set(LIB_TO_LINK allegd)
    set(FONT_LIB_TO_LINK a5_fontd)
    if(COMPILER_MSVC)
        set(EXE_CFLAGS "-DDEBUGMODE")
    elseif(COMPILER_GCC)
        set(EXE_CFLAGS "-DDEBUGMODE -g2 -O0")
    endif(COMPILER_MSVC)
elseif(GRADE_PROFILE)
    set(LIB_TO_LINK allegp)
    set(FONT_LIB_TO_LINK a5_fontp)
    if(COMPILER_MSVC)
        set(EXE_CFLAGS "")
    elseif(COMPILER_GCC)
        set(EXE_CFLAGS "-pg -O2 -funroll-loops -ffast-math")
    endif(COMPILER_MSVC)
endif(GRADE_STANDARD)

set(LINK_WITH ${LIB_TO_LINK}_shared)
set(FONT_LINK_WITH ${FONT_LIB_TO_LINK}_shared)

macro(example nm)
    add_allegro_executable(${nm} ${ARGN} ${nm}.c)
endmacro(example nm libs)

macro(example_cxx nm)
    add_allegro_executable(${nm} ${ARGN} ${nm}.cpp)
endmacro(example_cxx nm libs)
