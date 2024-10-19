#
# unix
#

CC = gcc
LD = gcc
AR = ar
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

# iconv
# Jj commented libiconv usage
#ifneq ($(OS),Windows_NT)
CFLAGS += -D USE_ICONV
#LIBS += -liconv
#endif

#
# SDL2 stuff
#

SDL_CONFIG = sdl2-config

SDL_LIBS   := `$(SDL_CONFIG) --libs`
SDL_CFLAGS := `$(SDL_CONFIG) --cflags`

#
# iconv stuff
#

PKG_CONFIG = pkg-config

ICONV_LIBS   := `$(PKG_CONFIG) --libs iconv`
ICONV_CFLAGS := `$(PKG_CONFIG) --cflags iconv`

