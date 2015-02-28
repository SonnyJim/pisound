#include "pisound.h"
#include "queue.h"

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

int main(int argc, char *argv[])
{
    int ret;

    fprintf (stdout, "=========\n");
    fprintf (stdout, "|PiSound|\n");
    fprintf (stdout, "=========\n\n");
  
    running = 0;
    loading_resources = 1;
    
    //Check PID file
    if (check_pid () != 0)
    {
        fprintf (stderr, "Error opening or creating PID file\n");
        return 1;
    }

    //Read cmd line args
    if (getopts (argc, argv) != 0)
        return 1;
   
   //Load configuration
    if (cfg_load () != 0)
    {
        fprintf (stderr, "An error occured reading the config file!\n");
        fprintf (stderr, "%s\n", strerror(errno));
        return 1;
    }
    
    //setup signal handler
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        fprintf(stderr, "\ncan't catch SIGINT\n");

    running = 1;
    
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

#ifdef BUILD_GPIO
    //Start the GPIO thread
    if (cfg_gpio_engine)
    {
        ret = pthread_create (&thread1, NULL, gpio_thread, &music_requested);
        if (ret)
        {
            fprintf(stderr,"Error creating gpio_thread: %i\n",ret);
            return 1;
        }
        else
            fprintf (stdout, "GPIO thread started\n");
    }
#endif

    //Start the udp server
    if (cfg_udp_engine)
    {
        ret = pthread_create (&thread2, NULL, udp_thread, NULL);
        if (ret)
        {
            fprintf(stderr,"Error creating udp_thread: %i\n",ret);
            return 1;
        }
        else
            fprintf (stdout, "UDP thread started\n");
    }
 
    if (init_audio () != 0)
    {
        fprintf (stderr, "Error initialising audio\n");
        return 1;
    }

    //Load sounds and music files
    if (cfg_load_audio () != 0)
    {
        fprintf (stderr, "Error loading sound files\n");
        return 1;
    }

    //Start main audio thread
    play_sounds ();
   
    fprintf (stdout, "\nExiting\n");
	return 0;
}
