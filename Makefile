#BUILD_GPIO = y
BUILD_GFX = y
CC	= gcc
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LDFLAGS := $(shell sdl2-config --libs)
CFLAGS = -O3 -Wall -Winline -pipe -std=gnu99 $(SDL2_CFLAGS)
LDLIBS = -lpthread -lSDL2_mixer $(SDL2_LDFLAGS)

OBJS = src/pisound.o src/getopts.o src/queue.o src/cfg.o src/udp.o src/volume.o src/pidfile.o src/snd.o src/scene.o

ifdef BUILD_GPIO
CFLAGS += -DBUILD_GPIO
LDLIBS += -lwiringPi
OBJS += src/gpio.o
endif

ifdef BUILD_GFX
CFLAGS += -DBUILD_GFX
LDLIBS += -lSDL2_ttf -lSDL2_image -lavformat -lavcodec -lavutil -lswscale
OBJS += src/gfx.o src/gfx_font.o src/video.o
OBJS += src/scenes/boot.o src/scenes/amode.o src/scenes/game.o src/scenes/gameover.o src/scenes/hsentry.o src/scenes/test.o src/scenes/tilt.o
endif

all: pisound

debug: CFLAGS += -g
debug: pisound

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


