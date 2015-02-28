CC	= gcc
INCLUDE	= -I/usr/local/include 
#GLES_INCLUDE = -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads/ -I/opt/vc/include/interface/vmcs_host/linux/
#GLESLIBS = -L/opt/vc/lib
DEBUG = -g -DDEBUG
CFLAGS	= -Wall $(INCLUDE) -Winline -pipe -std=gnu99 -O3
LDFLAGS	= -L/usr/local/lib 
LDLIBS	= -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lpthread
GPIO_LIBS = -lwiringPi
LDLIBS += $(GPIO_LIBS)
GPIO_OBJS = src/gpio.o
OBJS += $(GPIO_OBJS)
OBJS = src/pisound.o src/getopts.o src/queue.o src/cfg.o src/udp.o src/volume.o src/pidfile.o src/snd.o src/gfx.o src/gfx_font.o src/gpio.o 

all: pisound

debug: CFLAGS += -g
debug: pisound


pisound: $(OBJS)
	@echo [link]
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf src/*.o
	rm -rf pisound

