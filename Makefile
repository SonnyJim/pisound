CC	= gcc
INCLUDE	= -I/usr/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe -std=gnu99 -O3
LDFLAGS	= -L/usr/lib 
LDLIBS	= -lSDL -lSDL_mixer -lpthread

all: pisound

tidbot: pisound.o
	@echo [link]
	@$(CC) -o $@ pisound $(LDFLAGS) $(LDLIBS)

clean:
	rm *.o
