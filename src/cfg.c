#include "pisound.h"
#include "volume.h"

FILE *sound_list, *cfg_file;
char cfg_line[1024];
int sound_num;
char *sound_file;
int len;
const char delim[] = "=";

static int cfg_load_sounds (void)
{
    //Initilise sound array
    init_sounds ();

    //Load the sound FX
    if (verbose)
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
            if (sound_num > MAX_SOUNDS)
            {
                fprintf (stderr, "Error: sound_num larger than maximum allowed: %i max=%i\n", sound_num, MAX_SOUNDS);
                return 1;
            }

            sound_file = strtok (NULL, delim);
   
            //TODO This will probably break if pisound.cfg has Windows line endings
            //Strip off newline
            len = strlen (sound_file);
            if (sound_file[len - 1] == '\n')
                sound_file[len - 1] = '\0';
            /* 
            if (verbose)
                fprintf (stdout, "Loading %s @ position %i\n", sound_file, sound_num);
            */
            sounds[sound_num] = Mix_LoadWAV (sound_file);
	       
            if(sounds[sound_num] == NULL) 
            {
		        fprintf(stderr, "\nUnable to load file %s: %s\n", sound_file, Mix_GetError());
                return 1;
            }
        }
    }
    fclose (sound_list);
    return 0;
}

static int cfg_load_music (void)
{

    //Load the music WAVs
    if (verbose)
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
            if (sound_num > MAX_MUSIC)
            {
                fprintf (stderr, "Error: music_num larger than maximum allowed: %i max=%i\n", sound_num, MAX_MUSIC);
                return 1;
            }

            sound_file = strtok (NULL, delim);
   
            //TODO This will probably break if pisound.cfg has Windows line endings
            //Strip off newline
            len = strlen (sound_file);
            if (sound_file[len - 1] == '\n')
                sound_file[len - 1] = '\0';
            /*      
            if (verbose)
                fprintf (stdout, "Loading %s @ position %i\n", sound_file, sound_num);
            */
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

int cfg_load (void)
{
    //Load the general configuration
    //Set some default values before attempting to load config
    volume = DEFAULT_VOLUME;
    max_voices = DEFAULT_MAXVOICES;
    audio_channels = DEFAULT_AUDIO_CHANNELS;
    //audio_format = DEFAULT_AUDIO_FORMAT;
    audio_format = AUDIO_S16SYS;
    audio_buffers = DEFAULT_AUDIO_BUFFERS;
    audio_rate = DEFAULT_AUDIO_RATE;

    if (verbose)
        fprintf (stdout, "Attempting to load configuration file %s\n", DEFAULT_CFG_FILE);
    cfg_file = fopen (DEFAULT_CFG_FILE, "r");
    
    if (cfg_file == NULL)
        return errno;
   
    while (fgets (cfg_line, 1024, cfg_file) != NULL)
    {
        //Ignore remarks
        if (cfg_line[0] != '#')
        {
            //Find the volume
            if (strncmp (cfg_line, CFG_VOLUME, strlen(CFG_VOLUME)) == 0)
            {
                volume = atoi(cfg_line + strlen(CFG_VOLUME));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_VOLUME, volume);
            }
            
            if (strncmp (cfg_line, CFG_MAXVOICES, strlen(CFG_MAXVOICES)) == 0)
            {
                max_voices = atoi(cfg_line + strlen(CFG_MAXVOICES));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_MAXVOICES, max_voices);
            }

            if (strncmp (cfg_line, CFG_AUDIO_CHANNELS, strlen(CFG_AUDIO_CHANNELS)) == 0)
            {
                audio_channels = atoi(cfg_line + strlen(CFG_AUDIO_CHANNELS));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_AUDIO_CHANNELS, audio_channels);
            }
            
            if (strncmp (cfg_line, CFG_AUDIO_BUFFERS, strlen(CFG_AUDIO_BUFFERS)) == 0)
            {
                audio_buffers = atoi(cfg_line + strlen(CFG_AUDIO_BUFFERS));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_AUDIO_BUFFERS, audio_buffers);
            }
      
            /*
            if (strncmp (cfg_line, CFG_AUDIO_FORMAT, strlen(CFG_AUDIO_FORMAT)) == 0)
            {
                audio_format = atol (cfg_line + strlen(CFG_AUDIO_FORMAT));
                if (verbose)
                    fprintf (stdout, "Reading audio_format from configuration file, %i\n", audio_format);
            }
            */
            if (strncmp (cfg_line, CFG_AUDIO_RATE, strlen(CFG_AUDIO_RATE)) == 0)
            {
                audio_rate = atoi(cfg_line + strlen(CFG_AUDIO_RATE));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_AUDIO_RATE, audio_rate);
            }
           
#ifdef BUILD_GFX
            if (strncmp (cfg_line, CFG_GFX_ENGINE, strlen(CFG_GFX_ENGINE)) == 0)
            {
                cfg_gfx_engine = atoi(cfg_line + strlen (CFG_GFX_ENGINE));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_GFX_ENGINE, cfg_gfx_engine);
            }
#endif

            if (strncmp (cfg_line, CFG_AUDIO_ENGINE, strlen(CFG_AUDIO_ENGINE)) == 0)
            {
                cfg_audio_engine = atoi(cfg_line + strlen (CFG_AUDIO_ENGINE));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_AUDIO_ENGINE, cfg_audio_engine);
            }

#ifdef BUILD_GPIO 
            if (strncmp (cfg_line, CFG_GPIO_ENGINE, strlen(CFG_GPIO_ENGINE)) == 0)
            {
                cfg_gpio_engine = atoi(cfg_line + strlen (CFG_GPIO_ENGINE));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_GPIO_ENGINE, cfg_gpio_engine);
            }
#endif

            if (strncmp (cfg_line, CFG_UDP_ENGINE, strlen(CFG_UDP_ENGINE)) == 0)
            {
                cfg_udp_engine = atoi(cfg_line + strlen (CFG_UDP_ENGINE));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_UDP_ENGINE, cfg_udp_engine);
            }
            
            if (strncmp (cfg_line, CFG_SHOW_LOGO, strlen(CFG_SHOW_LOGO)) == 0)
            {
                cfg_show_logo= atoi(cfg_line + strlen (CFG_SHOW_LOGO));
                if (verbose)
                    fprintf (stdout, "%s%i\n", CFG_SHOW_LOGO, cfg_show_logo);
            }
        }
    }
    fclose (cfg_file);
    return 0;
}

int cfg_load_audio (void)
{
    loading_resources = 1;
    if (cfg_load_sounds () != 0)
    {
        fprintf (stderr, "Error loading sfx\n");
        return 1;
    }
    if (cfg_load_music () != 0)
    {
        fprintf (stderr, "Error loading music\n");
        return 1;
    }
    loading_resources = 0;
    return 0;
}
