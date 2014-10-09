#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "string.h"

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

//How many different sounds we can play
#define MAX_SOUNDS 255
//How long the sound queue is
#define SOUND_QUEUE_SIZE 16
#define QUEUE_POS_EMPTY 256

#define DEFAULT_CFG_FILE "pisound.cfg"

//List of all the pointers to the sounds that can be called
Mix_Chunk *sounds[MAX_SOUNDS];

//Channel on which our sound is played
int channel;

//sound queue
struct sound_queue_t {
    int data[SOUND_QUEUE_SIZE + 1];
    int first;
    int last;
    int count;
} sound_queue;

struct sound_queue_t sound_queue;

//Initialise the sound_queue
void sound_queue_init (void)
{
    int i;
    sound_queue.first = 0;
    sound_queue.last = SOUND_QUEUE_SIZE - 1;
    sound_queue.count = 0;

    for (i = 0; i < SOUND_QUEUE_SIZE; i++)
    {
        sound_queue.data[i] = QUEUE_POS_EMPTY;
    }
}

//Read the next item from the sound_queue
int sound_queue_read (void)
{
    int i = QUEUE_POS_EMPTY;
    if (sound_queue.count <= 0) 
        fprintf(stderr, "Warning: attempting to read from an empty queue.\n");
    else 
    {
        i = sound_queue.data[sound_queue.first];
        sound_queue.data[sound_queue.first] = QUEUE_POS_EMPTY;
        sound_queue.first = (sound_queue.first+1) % SOUND_QUEUE_SIZE;
        sound_queue.count = sound_queue.count - 1;
    }
    return (i);
}

//Add an item to the sound_queue
void sound_queue_add (int sound_code)
{
    if (sound_queue.count >= SOUND_QUEUE_SIZE)
        fprintf (stderr, "Error: sound_queue overflow\n");
    else
    {
        sound_queue.last = (sound_queue.last+1) % SOUND_QUEUE_SIZE;
        sound_queue.data[sound_queue.last] = sound_code;
        sound_queue.count = sound_queue.count + 1;
    }
}

void sound_queue_display (void)
{
    int i;

    for (i = 0; i < SOUND_QUEUE_SIZE; i++)
        fprintf (stdout, "%i=%i ", i, sound_queue.data[i]);
    fprintf (stdout, "front=%i rear=%i count=%i\n", sound_queue.first, sound_queue.last, sound_queue.count);
}

//Initialise the sound array with NULL pointers
void init_sounds (void)
{
    int i;
    for (i = 0; i < MAX_SOUNDS; i++)
        sounds[i] = NULL;
}

//Release the memory allocated to our sounds
void free_sounds (void)
{
    int i;
    for (i = 0; i < MAX_SOUNDS; i++)
    {
	    Mix_FreeChunk(sounds[i]);
    }
}

//Parse the cfg and load WAVs into memory
int cfg_load (void)
{
    FILE *cfg_file;
    char cfg_line[1024];
    int sound_num;
    char *sound_file;
    int len;
    const char delim[2] = "=";

    fprintf (stdout, "Attempting to load config file %s\n", DEFAULT_CFG_FILE);
    
    cfg_file = fopen (DEFAULT_CFG_FILE, "r");
    
    if (cfg_file == NULL)
        return errno;
    
    while (fgets (cfg_line, 1024, cfg_file) != NULL)
    {
        if (cfg_line[0] != '#')
        {
            sound_num = atoi (strtok (cfg_line, delim));
            sound_file = strtok (NULL, delim);
   
            //TODO This will probably break if pisound.cfg has Windows line endings
            //Strip off newline
            len = strlen (sound_file);
            if (sound_file[len - 1] == '\n')
                sound_file[len - 1] = '\0';
           
            fprintf (stdout, "Loading %s @ position %i\n", sound_file, sound_num);
            sounds[sound_num] = Mix_LoadWAV (sound_file);
	        if(sounds[sound_num] == NULL) 
            {
		        printf("\nUnable to load file %s: %s\n", sound_file, Mix_GetError());
                return 1;
	        }
        }
    }
    return 0;
}

void play_sounds (void)
{
    while (sound_queue.count > 0)
    {
        //Play some sounds
    	channel = Mix_PlayChannel(-1, sounds[sound_queue_read ()], 0);
        if(channel == -1) {
	    	fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	    }
	    //Wait until the sound has stopped playing
	    while(Mix_Playing(channel) != 0);
    }
}

int main(int argc, char *argv[])
{
	int audio_rate = 44100;			//Frequency of audio playback
	Uint16 audio_format = AUDIO_S16SYS; 	//Format of the audio we're playing
	int audio_channels = 1;
	int audio_buffers = 4096;		//Size of the audio buffers in memory

    fprintf (stdout, "=======\n");
    fprintf (stdout, "PiSound\n");
    fprintf (stdout, "=======\n\n");
    
    //Initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	//Initialize SDL_mixer with our chosen audio settings
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		return 1;
	}
	
    //Initilise sound array
    init_sounds ();
    
    //Load sounds into array
    if (cfg_load () != 0)
    {
        fprintf (stderr, "An error occured reading the config file!\n");
        fprintf (stderr, "%s\n", strerror(errno));
        return 1;
    }
    
    //Initialise the sound_queue
    sound_queue_init ();

    sound_queue_add (0);
    sound_queue_add (1);
    sound_queue_add (2);
    sound_queue_add (3);
    sound_queue_add (4);
    sound_queue_add (5);
    sound_queue_add (4);
    sound_queue_add (3);
    sound_queue_add (2);
    sound_queue_add (1);

    play_sounds ();
    free_sounds ();
	//Need to make sure that SDL_mixer and SDL have a chance to clean up
	Mix_CloseAudio();
	SDL_Quit();	
	
	//Return success!
	return 0;
}
