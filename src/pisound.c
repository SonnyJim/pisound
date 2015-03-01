#include "pisound.h"
#include "queue.h"
#include "scene.h"

static void shutdown_pisound (void)
{
        free_sounds ();
	    Mix_CloseAudio();
	    SDL_Quit();	
        remove_pid ();
}

void sig_handler (int signo)
{
    if (signo == SIGINT)
    {
        fprintf (stdout, "\nSIGINT detected, shutting down\n");
        running = 0;
        //Wait for threads to die
        if (cfg_gpio_engine)
        {
            if (verbose)
                fprintf (stdout, "Waiting for GPIO to shutdown\n");
            while (pthread_kill (thread1, 0) == 0){}
        }
        
        /* FIXME UDP never exits?
        if (cfg_udp_engine)
        {
            if (verbose)
                fprintf (stdout, "Waiting for UDP server to shutdown\n");
            while (pthread_kill (thread2, 0) == 0) {
            printf ("WAITING\n");
            sleep (1);
            }

        }
        */
        if (cfg_gfx_engine)
        {
            if (verbose)
                fprintf (stdout, "Waiting for GFX to shutdown\n");
            while (pthread_kill (thread3, 0) == 0){}
        }

        if (verbose)
            fprintf (stdout, "Waiting for sound to shutdown\n");
        while (pthread_kill (thread4, 0) == 0){}
        shutdown_status = 1;
    }
}

int main(int argc, char *argv[])
{
    int ret;

    fprintf (stdout, "=========\n");
    fprintf (stdout, "|PiSound|\n");
    fprintf (stdout, "=========\n\n");
  
    running = 0;
    shutdown_status = 0;
    loading_resources = 1;
    current_scene = BOOT;
    
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

#ifdef BUILD_GFX    
    //Start the gfx thread
    if (cfg_gfx_engine)
    {
        ret = pthread_create (&thread3, NULL, gfx_thread, NULL);
        if (ret)
        {
            fprintf(stderr,"Error creating gfx_thread: %i\n", ret);
            return 1;
        }
        else
            fprintf (stdout, "GFX thread started\n");
    }
#endif

#ifdef BUILD_GPIO
    //Start the GPIO thread
    if (cfg_gpio_engine)
    {
        ret = pthread_create (&thread1, NULL, gpio_thread, &music_requested);
        if (ret)
        {
            fprintf(stderr,"Error creating gpio_thread: %i\n", ret);
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
    ret = pthread_create (&thread4, NULL, snd_thread, NULL);
    if (ret)
    {
        fprintf(stderr,"Error creating sound thread: %i\n",ret);
        return 1;
    }
    else
        fprintf (stdout, "Sound thread started\n");
   

    while (running) 
    {
        sleep (1);
    }

    int counter = 0;
    while (shutdown_status == 0)
    {
        raise (SIGINT);
        fprintf (stdout, "Waiting for threads to shutdown %i\n", counter);
        sleep (1);
        //If threads havent responded by 3 seconds, shutdown now
        if (++counter > 3)
            shutdown_status = 1;
    }

    shutdown_pisound ();

    fprintf (stdout, "\nExiting\n");
	return 0;
}
