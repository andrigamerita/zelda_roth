# Type 'make MIYOO=1' to build for Miyoo

APP_NAME = zelda_roth
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

ifdef MIYOO
    CHAINPREFIX := /opt/miyoo
    CROSS_COMPILE := $(CHAINPREFIX)/usr/bin/arm-linux-
    AR := $(CROSS_COMPILE)ar
    CC := $(CROSS_COMPILE)gcc
    CXX := $(CROSS_COMPILE)g++
    STRIP := $(CROSS_COMPILE)strip
    RANLIB := $(CROSS_COMPILE)ranlib
endif

SYSROOT := $(shell $(CC) --print-sysroot)

ifdef MIYOO
    export CHAINPREFIX CROSS_COMPILE AR CC CXX STRIP RANLIB SYSROOT
endif

SDL_CFLAGS := $(shell $(SYSROOT)/usr/bin/sdl-config --cflags)
SDL_LIBS := $(shell $(SYSROOT)/usr/bin/sdl-config --libs)

CXXFLAGS = -O2 $(SDL_CFLAGS)
LDFLAGS = $(SDL_LIBS) -lSDL_mixer -lSDL_gfx -lSDL_image

ifdef MIYOO
    CXXFLAGS += -DDINGUX -march=armv5tej
endif

all : $(APP_NAME)

$(APP_NAME) : $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean :
	rm -rf src/*.o $(APP_NAME)
