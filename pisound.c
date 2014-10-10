#include "pisound.h"

void sig_handler (int signo)
{
    if (signo == SIGINT)
    { 
        fprintf (stdout, "\nSIGINT detected, shutting down\n");
        pthread_cancel (thread1);
        free_sounds ();
	    Mix_CloseAudio();
	    SDL_Quit();	
        running = 0;
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

void music_check (void)
{    
    if (music_current != music_requested)
    {
        music_current = music_requested;
        
        if (music_current == MUSIC_OFF)
            Mix_HaltMusic ();
        else if (Mix_PlayMusic (music[music_current], -1) == -1)
            fprintf (stderr, "Unable to play Music file: %s\n", Mix_GetError());
    }
}

//Scale the volume range between SDL/WPC
//0 - 128 vs 0 - 32
void volume_up (void)
{
    if (volume < MAX_VOLUME)
    {
        volume++;
        Mix_Volume (-1, volume * 8);    
    }
}

void volume_down (void)
{
    if (volume > 0)
    {
        volume--;
        Mix_Volume (-1, volume * 8);  
    }
}

//Play some sounds
void play_sounds (void)
{
    fprintf (stdout, "Audio thread started\n");
    while (running)
    {
        music_check ();
        if (sound_queue.count > 0)
        {
            channel = Mix_PlayChannel(-1, sounds[sound_queue_read ()], 0);

            if(channel == -1) 
            {
	        	fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	        }
        }
    }
}

//Read the GPIO
void *gpio_thread(void *ptr)
{
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
        /*

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
        */
    }
}

int main(int argc, char *argv[])
{
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 1;
	int audio_buffers = 4096;
    int ret, c;

    fprintf (stdout, "=========\n");
    fprintf (stdout, "|PiSound|\n");
    fprintf (stdout, "=========\n\n");
  
    verbose = 0;
    running = 0;
   
    //read command line arguments
    while ((c = getopt (argc, argv, "v")) != -1)
    {
        switch (c)
        {
        case 'v':
            verbose = 1;
            break;
        }
    }

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
	
    
    //Load sounds into array
    if (cfg_load () != 0)
    {
        fprintf (stderr, "An error occured reading the config file!\n");
        fprintf (stderr, "%s\n", strerror(errno));
        return 1;
    }
    
    //Initialise the sound_queue
    sound_queue_init ();

    int j;
    for (j = 0; j < 4;j++)
        sound_queue_add (j);

    music_requested = 1;

    //setup signal handler
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        fprintf(stderr, "\ncan't catch SIGINT\n");
   
    running = 1;
    
    //Start the GPIO thread
    ret = pthread_create (&thread1, NULL, gpio_thread, &music_requested);
    if (ret)
    {
        fprintf(stderr,"Error creating gpio_thread: %i\n",ret);
    }
    else
        fprintf (stdout, "GPIO thread started\n");

    //Start audio thread
    play_sounds ();
   
    fprintf (stdout, "\nExiting\n");
	return 0;
}
