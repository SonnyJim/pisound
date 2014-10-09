#include "pisound.h"

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
    int i;
    if (sound_queue.count <= 0) 
    {
        fprintf(stderr, "Warning: attempting to read from an empty queue.\n");
        return (QUEUE_POS_EMPTY);
    }
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
        sound_queue.last = (sound_queue.last + 1) % SOUND_QUEUE_SIZE;
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

//Parse the cfg and load WAVs into memory
int cfg_load (void)
{
    FILE *sound_list;
    char cfg_line[1024];
    int sound_num;
    char *sound_file;
    int len;
    const char delim[1] = "=";

    //Initilise sound array
    init_sounds ();

    //Load the sound FX
    fprintf (stdout, "Attempting to load sound list %s\n", DEFAULT_SOUND_FILE);
    sound_list = fopen (DEFAULT_SOUND_FILE, "r");
    
    if (sound_list == NULL)
        return errno;
    
    while (fgets (cfg_line, 1024, sound_list) != NULL)
    {
        //Ignore remarks
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
    fclose (sound_list);


    //Load the music WAVs
    fprintf (stdout, "Attempting to load music list %s\n", DEFAULT_MUSIC_FILE);
    sound_list = fopen (DEFAULT_MUSIC_FILE, "r");
    
    if (sound_list == NULL)
        return errno;
    
    while (fgets (cfg_line, 1024, sound_list) != NULL)
    {
        //Ignore remarks
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
            music[sound_num] = Mix_LoadMUS (sound_file);
	        if(music[sound_num] == NULL) 
            {
		        printf("\nUnable to load file %s: %s\n", sound_file, Mix_GetError());
                return 1;
            }
        }
    }
    fclose (sound_list);
    return 0;
}

void music_check (void)
{    
    if (music_current != music_requested)
    {
        music_current = music_requested;
        if (Mix_PlayMusic (music[music_current], -1) == -1)
        {
            fprintf (stderr, "Unable to play Music file: %s\n", Mix_GetError());
        }
    }
}

//Play some sounds
void play_sounds (void)
{
    while (1)
    {
        music_check ();
        if (sound_queue.count > 0)
        {
            channel = Mix_PlayChannel(-1, sounds[sound_queue_read ()], 0);

            if(channel == -1) 
            {
	        	fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
	        }
	    //Wait until the sound has stopped playing
	    //while(Mix_Playing(channel) != 0);
        }
    }
}

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
        }
    }
}

int main(int argc, char *argv[])
{
	int audio_rate = 44100;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 1;
	int audio_buffers = 4096;
    int ret;
    pthread_t thread1;

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

    ret = pthread_create (&thread1, NULL, gpio_thread, &music_requested);
    if (ret)
    {
        fprintf(stderr,"Error creating gpio_thread: %i\n",ret);
    }
    play_sounds ();
    free_sounds ();
	//Need to make sure that SDL_mixer and SDL have a chance to clean up
	Mix_CloseAudio();
	SDL_Quit();	
	
	//Return success!
	return 0;
}
