#include "pisound.h"

void sig_handler (int signo)
{
    if (signo == SIGINT)
    { 
        fprintf (stdout, "\nSIGINT detected, shutting down\n");
        free_sounds ();
	    Mix_CloseAudio();
        remove_pid ();
        running = 0;
        free_gfx ();
	    SDL_Quit();	
    }
}

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
void sound_check (void)
{
    int sound_code;
    if (sound_queue.count > 0)
    {
        sound_code = sound_queue_read ();
        if (verbose)
            fprintf (stderr, "Playing sound_code %i\n", sound_code);
        channel = Mix_PlayChannel(-1, sounds[sound_code], 0);
        if(channel == -1)
	        fprintf(stderr, "Warning: Unable to play WAV file: %s\n", Mix_GetError());
    }
}

void music_check (void)
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
void play_sounds (void)
{
    fprintf (stdout, "Audio thread started\n");
    while (running)
    {
        music_check ();
        sound_check ();
    }
}

//Read the GPIO
void *gpio_thread(void *ptr)
{
        /*
    int i;
    while (1)
    {
        for (i = 0; i < 5; i++)
        {
            music_requested = i;
            sleep (2);
            sound_queue_add (i);
            sleep (2);
            sound_queue_add (i);
            music_requested = MUSIC_OFF;
            sleep (2);
        }

        //Decode sound_code from GPIO
        switch (sound_code)
        {
            case SND_VOL_UP:
                volume_up ();
                break;
            case SND_VOL_DOWN:
                volume_down ();
                break;
            case SND_STOP_MUSIC:
                music_requested = MUSIC_OFF;
                break;
        }
    }
        */
    return NULL;
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

    //Load sounds and music files
    if (cfg_load_audio () != 0)
        return 1;

    //Configure maximum amount of simultaenous voices
    Mix_AllocateChannels(max_voices);
    
    //Initialise the sound_queue
    sound_queue_init ();
    
    //Initialise volume
    init_volume ();

    return 1;
}

int main(int argc, char *argv[])
{
    int ret;

    fprintf (stdout, "=========\n");
    fprintf (stdout, "|PiSound|\n");
    fprintf (stdout, "=========\n\n");
  
    running = 0;
    
    //Check PID file
    if (check_pid () != 0)
    {
        fprintf (stderr, "Error opening or creating PID file\n");
        return 1;
    }

    //Read cmd line args
    if (getopts (argc, argv) != 0)
        return 1;
    
   //Load configuration and sounds in memory
    if (cfg_load () != 0)
    {
        fprintf (stderr, "An error occured reading the config file!\n");
        fprintf (stderr, "%s\n", strerror(errno));
        return 1;
    }
    
    //setup signal handler
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        fprintf(stderr, "\ncan't catch SIGINT\n");

    init_audio ();
    running = 1;
   
    //Start the GPIO thread
    ret = pthread_create (&thread1, NULL, gpio_thread, &music_requested);
    if (ret)
    {
        fprintf(stderr,"Error creating gpio_thread: %i\n",ret);
    }
    else
        fprintf (stdout, "GPIO thread started\n");

    //Start the udp server
    ret = pthread_create (&thread2, NULL, udp_thread, NULL);
    if (ret)
    {
        fprintf(stderr,"Error creating udp_thread: %i\n",ret);
    }
    else
 
    //Start the gfx thread
    if (cfg_gfx_engine)
    {
        ret = pthread_create (&thread3, NULL, gfx_thread, NULL);
        if (ret)
        {
            fprintf(stderr,"Error creating gfx_thread: %i\n",ret);
        }
        else
            fprintf (stdout, "GFX thread started\n");
    }

    //Start main audio thread
    play_sounds ();
   
    fprintf (stdout, "\nExiting\n");
	return 0;
}
