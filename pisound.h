#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define SOUND_QUEUE_SIZE 16
#define QUEUE_POS_EMPTY 256

#define MAX_SOUNDS 255
#define MAX_MUSIC 255

//Command to stop music
#define MUSIC_OFF 256

//List of all the pointers to the sounds that can be called
Mix_Chunk *sounds[MAX_SOUNDS + 1];
Mix_Music *music[MAX_MUSIC + 1];

//How many channels we use, currently mono
int channel;

//Currently playing music
int music_current;

//Music requested
int music_requested;

int running;
int verbose;
int volume;

//sound queue
struct sound_queue_t {
    int data[SOUND_QUEUE_SIZE + 1];
    int first;
    int last;
    int count;
} sound_queue;
struct sound_queue_t sound_queue;
    
//GPIO thread
pthread_t thread1, thread2;
void* udp_thread (void *ptr);
void* gpio_thread (void *ptr);


int sound_queue_read (void);
void sound_queue_init (void);
void sound_queue_add (int sound_code);
void music_request (int music_code);

void volume_up (void);
void volume_down (void);
void volume_set (int volume);

int cfg_load (void);

void init_sounds (void);
void free_sounds (void);

void init_volume (void);
void volume_set (int volume);
void volume_up (void);
void volume_down (void);
