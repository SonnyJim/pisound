GAME = hbb
#BUILD_GPIO = y
BUILD_GFX = y
CC	= gcc
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LDFLAGS := $(shell sdl2-config --libs)
CFLAGS = -O3 -Wall -Winline -pipe -std=gnu99 $(SDL2_CFLAGS) -I./src/include
LDLIBS = -lpthread -lSDL2_mixer $(SDL2_LDFLAGS)

OBJS = src/pisound.o src/getopts.o src/queue.o src/cfg.o src/udp.o src/volume.o src/pidfile.o src/snd.o src/scene.o
#HDRS = src/pisound.h src/scene.h src/volume.h src/queue.h src/udp.h src/cfg.h

ifdef BUILD_GPIO
CFLAGS += -DBUILD_GPIO
LDLIBS += -lwiringPi
OBJS += src/gpio.o
HDRS += src/gpio.h
endif

ifdef BUILD_GFX
CFLAGS += -DBUILD_GFX
LDLIBS += -lSDL2_ttf -lSDL2_image
OBJS += src/gfx.o src/gfx_font.o src/scenes/boot.o src/scenes/test.o 
ifeq ($(GAME), hbb)
OBJS += src/machine/hbb/scenes/amode.o src/machine/hbb/scenes/game.o 
OBJS += src/machine/hbb/scenes/gameover.o src/machine/hbb/scenes/hsentry.o src/machine/hbb/scenes/tilt.o
endif
#HDRS += src/gfx.h 
endif

all: pisound

debug: CFLAGS += -g
debug: pisound

OBJS += $(HDRS)

listdir: tools/listdir.o
	@echo [link]
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

pisound: $(OBJS)
	@echo [link]
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf src/*.o
	rm -rf tools/*.o
	rm -f pisound
	rm -f listdir

