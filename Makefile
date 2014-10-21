CC	= gcc
INCLUDE	= -I/usr/local/include 
#GLES_INCLUDE = -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads/ -I/opt/vc/include/interface/vmcs_host/linux/
#GLESLIBS = -L/opt/vc/lib
DEBUG = -g
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe -std=gnu99 -O3
LDFLAGS	= -L/usr/local/lib 
LDLIBS	= -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lpthread
OBJS = src/pisound.o src/getopts.o src/queue.o src/cfg.o src/udp.o src/volume.o src/pidfile.o src/gfx.o src/gfx_font.o

all: pisound

pisound: $(OBJS)
	@echo [link]
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf src/*.o
	rm -rf pisound
