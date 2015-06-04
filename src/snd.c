#include "pisound.h"
#include "queue.h"
#include "volume.h"

//Initialise the sound array with NULL pointers
void init_sounds (void)
{
    int i;
    for (i = 0; i < MAX_SOUNDS; i++)
        sounds[i] = NULL;
    
    for (i = 0; i < MAX_MUSIC; i++)
	    music[i] = NULL;

    music_requested = MUSIC_OFF;
    music_current = MUSIC_OFF;
    
}

//Release the memory allocated to our sounds
void free_sounds (void)
{
    int i;
    for (i = 0; i < MAX_SOUNDS; i++)
	    Mix_FreeChunk (sounds[i]);

    for (i = 0; i < MAX_MUSIC; i++)
	    Mix_FreeMusic (music[i]);
    
    music_requested = MUSIC_OFF;
    music_current = MUSIC_OFF;
}

//Check the sound_queue and play sounds if needed
static void sound_check (void)
{
    int sound_code;
    if (!queue_empty (&sfx_q))
    {
        sound_code = queue_remove (&sfx_q);
        if (verbose)
            fprintf (stderr, "Playing sound_code %i\n", sound_code);
        channel = Mix_PlayChannel(-1, sounds[sound_code], 0);
        if(channel == -1)
	        fprintf(stderr, "Warning: Unable to play WAV file: %s\n", Mix_GetError());
    }
}

static void music_check (void)
{    
    if (music_current != music_requested)
    {
        if (verbose)
            fprintf (stdout, "Playing music_code %i\n", music_requested);

        music_current = music_requested;
        
        if (music_current == MUSIC_OFF)
            Mix_HaltMusic ();
        else if (Mix_PlayMusic (music[music_current], -1) == -1)
            fprintf (stderr, "Unable to play Music file: %s\n", Mix_GetError());
    }
}

//Main sound loop
int snd_thread (void *ptr)
{
    while (running)
    {
        music_check ();
        sound_check ();
    }
    free_sounds ();
    Mix_CloseAudio();
	
    fprintf (stdout, "Audio thread stopped\n");
    return 0;
}

int init_audio (void)
{ 
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

    //Configure maximum amount of simultaenous voices
    Mix_AllocateChannels(max_voices);
    if (verbose)
        fprintf (stdout, "Max voices %i\n", max_voices);
    
    //Initialise the sound_queue
    queue_init (&sfx_q);
    
    //Initialise volume
    init_volume ();

    return 0;
}


