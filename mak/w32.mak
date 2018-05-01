#
# Win32
#

ifneq ($(OS),Windows_NT)
PFX = i586-mingw32-
CFG_SFX = .w32
else
PFX =
CFG_SFX =
endif

CC = $(PFX)gcc
LD = $(PFX)gcc
AR = $(PFX)ar
LIBS = 
SLIBS = 
LFLAGS = 



ifdef DEBUG
CFLAGS = -g -O0
OBJDIR = obj
else
CFLAGS = -O3
OBJDIR = obj
endif

#
# SDL stuff
#

SDL_CONFIG = sdl2-config

SDL_LIBS := -mconsole 
SDL_LIBS += `$(SDL_CONFIG) --libs | sed -e "s|-mwindows||g" | \
 sed -e  "s|-lSDLmain||g" | sed -e  "s|-lmingw32||g"`
SDL_LIBS += -static-libgcc


#
# static link for w32 is currently not supported.
#
SDL_SLIBS := $(SDL_LIBS)

SDL_CFLAGS := `$(SDL_CONFIG) --cflags | sed -e "s/-Dmain=SDL_main//g"`


SLIBS += -ldinput8


