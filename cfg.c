#include "pisound.h"
#include "cfg.h"
#include "volume.h"

//Parse the cfg and load WAVs/volume settings into memory
int cfg_load (void)
{
    FILE *sound_list, *cfg_file;
    char cfg_line[1024];
    int sound_num;
    char *sound_file;
    int len;
    const char delim[1] = "=";

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
          
            if (verbose)
                fprintf (stdout, "Loading %s @ position %i\n", sound_file, sound_num);
            
            sounds[sound_num] = Mix_LoadWAV (sound_file);
	       
            if(sounds[sound_num] == NULL) 
            {
		        fprintf(stderr, "\nUnable to load file %s: %s\n", sound_file, Mix_GetError());
                return 1;
            }
        }
    }
    fclose (sound_list);

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
          
            if (verbose)
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

    //Load the general configuration
    //Set some default values before attempting to load config
    volume = DEFAULT_VOLUME;

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
                    fprintf (stdout, "Reading volume from configuration file, %i\n", volume);
            }
        }
    }
    fclose (cfg_file);
    return 0;
}
