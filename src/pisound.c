#include "pisound.h"
#include "queue.h"
#include "snd.h"
#include "scene.h"

SDL_Thread *thread1 = NULL; // GPIO
SDL_Thread *thread2 = NULL; // UDP
SDL_Thread *thread3 = NULL; // GFX
SDL_Thread *thread4 = NULL; // SND

static void shutdown_pisound (void)
{
    SDL_Quit();	
    remove_pid ();
}

void sig_handler (int signo)
{
    int ret;

    if (signo == SIGINT)
    {
        fprintf (stdout, "\nSIGINT detected, shutting down\n");
        running = 0;
        //Wait for threads to die
        if (cfg_gpio_engine)
        {
            if (verbose)
                fprintf (stdout, "Waiting for GPIO to shutdown\n");
            SDL_WaitThread (thread1, &ret);
        }
       
        if (cfg_udp_engine)
        {
            if (verbose)
                fprintf (stdout, "Waiting for UDP to shutdown\n");
            SDL_WaitThread (thread2, &ret);
        }

        if (cfg_gfx_engine)
        {
            if (verbose)
                fprintf (stdout, "Waiting for GFX to shutdown\n");
            SDL_WaitThread (thread3, &ret);
        }
        
        if (verbose)
            fprintf (stdout, "Waiting for sound to shutdown\n");
        SDL_WaitThread (thread4, &ret);

        shutdown_status = 1;
    }
}

int main (int argc, char *argv[])
{
    fprintf (stdout, "=========\n");
    fprintf (stdout, "|PiSound|\n");
    fprintf (stdout, "=========\n\n");
  
    running = 0;
    shutdown_status = 0;
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

#ifdef BUILD_GPIO
    //Start the GPIO thread
    if (cfg_gpio_engine)
    {
        thread1 = SDL_CreateThread (gpio_thread, "GPIOThread", NULL);
        if (thread1 == NULL)
        {
            fprintf(stderr,"Error creating gpio_thread: %s\n", SDL_GetError());
            return 1;
        }
        else
            fprintf (stdout, "GPIO thread started\n");
    }
#endif

    //Start the udp server
    if (cfg_udp_engine)
    {
        if ( SDLNet_Init() != 0) 
        {
            fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
            return 1;
        }

        thread2 = SDL_CreateThread (udp_thread, "UDPThread", NULL);
        if (thread2 == NULL)
        {
            fprintf(stderr,"Error creating udp_thread: %s\n",SDL_GetError());
            return 1;
        }
        else
            fprintf (stdout, "UDP thread started\n");
    }

#ifdef BUILD_GFX    
    //Start the gfx thread
    if (cfg_gfx_engine)
    {
        thread3 = SDL_CreateThread (gfx_thread, "GFXThread", NULL);
        if (thread3 == NULL)
        {
            fprintf(stderr,"Error creating gfx_thread: %s\n", SDL_GetError());
            return 1;
        }
        else
            fprintf (stdout, "GFX thread started\n");
    }
#endif


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
    thread4 = SDL_CreateThread (snd_thread, "SNDThread", NULL);
    if (thread4 == NULL)
    {
        fprintf(stderr,"Error creating sound thread: %s\n", SDL_GetError());
        return 1;
    }
    else
        fprintf (stdout, "Sound thread started\n");
   

    while (running) 
    {
        SDL_Delay (1000);
        //sleep (1);
    }

    int counter = 0;
    while (shutdown_status == 0)
    {
        raise (SIGINT);
        fprintf (stdout, "Waiting for threads to shutdown %i\n", counter);
        SDL_Delay (1000);
        //sleep (1);
        //If threads havent responded by 3 seconds, shutdown now
        if (++counter > 3)
            shutdown_status = 1;
    }

    shutdown_pisound ();

    fprintf (stdout, "\nExiting\n");
	return 0;
}
