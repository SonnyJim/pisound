#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "string.h"

#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"


//WPC uses 8bit sound code
#define MAX_SOUNDS 255
#define DEFAULT_CFG_FILE "pisound.cfg"

//List of all the pointers to the sounds that can be called
Mix_Chunk *sounds[MAX_SOUNDS];

void init_sounds (void)
{
    int i;
    
    for (i = 0; i < MAX_SOUNDS; i++)
        sounds[i] = NULL;
}

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
   
            //Strip off newline
            len = strlen (sound_file);
            if (sound_file[len - 1] == '\n')
                sound_file[len - 1] = '\0';
            fprintf (stdout, "Loading %s @ position %i\n", sound_file, sound_num);
            sounds[sound_num] = Mix_LoadWAV (sound_file);
	        if(sounds[sound_num] == NULL) 
            {
		        printf("Unable to load WAV file: %s\n", Mix_GetError());
                return 1;
	        }
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
	int channel;				//Channel on which our sound is played
	  
	int audio_rate = 44100;			//Frequency of audio playback
	Uint16 audio_format = AUDIO_S16SYS; 	//Format of the audio we're playing
	int audio_channels = 1;			//2 channels = stereo
	int audio_buffers = 4096;		//Size of the audio buffers in memory

    int i;
	
    

    //Initialize SDL audio
	if (SDL_Init(SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	//Initialize SDL_mixer with our chosen audio settings
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}
	
    //Initilise sound array
    init_sounds ();
    
    //Load sounds into array
    if (cfg_load () != 0)
    {
        fprintf (stderr, "An error occured reading the config file!\n");
        fprintf (stderr, "%s\n", strerror(errno));
        exit (1);
    }

	//Play our sound file, and capture the channel on which it is played
	channel = Mix_PlayChannel(-1, sounds[0], -1);
	channel = Mix_PlayChannel(-1, sounds[1], -1);
	channel = Mix_PlayChannel(-1, sounds[2], -1);
	channel = Mix_PlayChannel(-1, sounds[3], -1);
	channel = Mix_PlayChannel(-1, sounds[4], -1);
	if(channel == -1) {
		printf("Unable to play WAV file: %s\n", Mix_GetError());
	}
	
	//Wait until the sound has stopped playing
	while(Mix_Playing(channel) != 0);
	
	//Release the memory allocated to our sound
    for (i = 0; i < 4; i++)
    {
	    Mix_FreeChunk(sounds[i]);
    }

	//Need to make sure that SDL_mixer and SDL have a chance to clean up
	Mix_CloseAudio();
	SDL_Quit();	
	
	//Return success!
	return 0;
}
