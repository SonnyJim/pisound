#BUILD_GPIO = y
BUILD_GFX = y
CC	= gcc
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LDFLAGS := $(shell sdl2-config --libs)
CFLAGS = -O3 -Wall -Winline -pipe -std=gnu99 $(SDL2_CFLAGS)
LDLIBS = -lpthread -lSDL2_mixer $(SDL2_LDFLAGS)

OBJS = src/pisound.o src/getopts.o src/queue.o src/cfg.o src/udp.o src/volume.o src/pidfile.o src/snd.o 

ifdef BUILD_GPIO
CFLAGS += -DBUILD_GPIO
LDLIBS += -lwiringPi
OBJS += src/gpio.o
endif

ifdef BUILD_GFX
CFLAGS += -DBUILD_GFX
LDLIBS += -lSDL2_ttf -lSDL2_image 
OBJS += src/gfx.o src/gfx_font.o
endif

all: pisound

debug: CFLAGS += -g
debug: pisound


pisound: $(OBJS)
	@echo [link]
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf src/*.o
	rm -rf pisound

