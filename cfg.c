#include "pisound.h"

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
