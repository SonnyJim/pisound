CC	= gcc
INCLUDE	= -I/usr/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe -std=gnu99 -O3
LDFLAGS	= -L/usr/lib 
LDLIBS	= -lSDL -lSDL_mixer -lSDL_ttf -lpthread
OBJS = src/pisound.o src/queue.o src/cfg.o src/udp.o src/volume.o src/pidfile.o src/gfx.o src/gfx_font.o

all: pisound

pisound: $(OBJS)
	@echo [link]
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf src/*.o
	rm -rf pisound
