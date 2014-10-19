CC	= gcc
INCLUDE	= -I/usr/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe -std=gnu99 -O3
LDFLAGS	= -L/usr/lib 
LDLIBS	= -lSDL -lSDL_mixer -lSDL_ttf -lpthread
OBJS = pisound.o queue.o cfg.o udp.o volume.o pidfile.o gfx.o gfx_font.o

all: pisound

pisound: $(OBJS)
	@echo [link]
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf *.o
	rm -rf pisound
