#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

//How many different sounds we can play
#define MAX_SOUNDS 255
#define MAX_MUSIC 255
//How long the sound queue is
#define SOUND_QUEUE_SIZE 16
#define MUSIC_QUEUE_SIZE 16
#define QUEUE_POS_EMPTY 256
#define MUSIC_OFF 256

#define DEFAULT_SOUND_FILE "sounds.cfg"
#define DEFAULT_MUSIC_FILE "music.cfg"

//List of all the pointers to the sounds that can be called
Mix_Chunk *sounds[MAX_SOUNDS];
Mix_Music *music[MAX_MUSIC];

//Channel on which our sound is played
int channel;

//Currently playing music
int music_current;

//Music requested
int music_requested;

int running;

//sound queue
struct sound_queue_t {
    int data[SOUND_QUEUE_SIZE + 1];
    int first;
    int last;
    int count;
} sound_queue;
    
//GPIO thread
pthread_t thread1;

struct sound_queue_t sound_queue;

void sound_queue_init (void);
int sound_queue_read (void);
void sound_queue_add (int sound_code);

int cfg_load (void);

void init_sounds (void);
void free_sounds (void);
