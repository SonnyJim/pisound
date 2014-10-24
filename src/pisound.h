#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "cfg.h"

#define SOUND_QUEUE_SIZE 16
#define QUEUE_POS_EMPTY 256

#define MAX_SOUNDS 255
#define MAX_MUSIC 255

#define DEFAULT_MAXVOICES 32

#define MAX_PLAYERS 6

//Command to stop music
#define MUSIC_OFF 256

#define DEFAULT_AUDIO_RATE 44100
#define DEFAULT_AUDIO_CHANNELS 2
#define DEFAULT_AUDIO_FORMAT AUDIO_S16SYS
#define DEFAULT_AUDIO_BUFFERS 2048

//List of all the pointers to the sounds that can be called
Mix_Chunk *sounds[MAX_SOUNDS + 1];
Mix_Music *music[MAX_MUSIC + 1];

//How many channels we use, currently mono
int channel;
int audio_rate;
Uint16 audio_format;
int audio_channels;
int audio_buffers;

//Currently playing music
int music_current;

//Music requested
int music_requested;
int max_voices;

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
pthread_t thread1, thread2, thread3;
void* udp_thread (void *ptr);
void* gpio_thread (void *ptr);
void* gfx_thread (void *ptr);

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

int check_pid (void);
int remove_pid (void);

void free_gfx (void);

//GPIO
void *gpio_thread(void *ptr);
