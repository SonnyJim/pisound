#include "pisound.h"
#include "gfx.h"
int load_fonts (void)
{
    int i, len, fontsize;
    FILE *cfg_file;
    char cfg_line[1024], *filename;
    const char delim[] = ",";

    if (TTF_Init () != 0)
    {
        fprintf (stderr, "Error initialising SDL_ttf %s\n", TTF_GetError());
    }

    //Initiliase font array
    for (i = 0; i < MAX_FONTS; i++)
        fonts[i] = NULL;

    //Load font
    if (verbose)
        fprintf (stdout, "Attempting to load font list %s\n", CFG_FONT_FILE);
    cfg_file = fopen (CFG_FONT_FILE, "r");
    
    if (cfg_file == NULL)
        return errno;
    
    i = 0;
    while (fgets (cfg_line, 1024, cfg_file) != NULL)
    {
        //Ignore remarks
        if (cfg_line[0] != '#')
        {
            //TODO This will probably break if font.cfg has Windows line endings
            //Strip off newline
            len = strlen (cfg_line);
            if (cfg_line[len - 1] == '\n')
                cfg_line[len - 1] = '\0';

            fontsize = atoi (strtok (cfg_line, delim));
            filename = strtok (NULL, delim);
          
            if (verbose)
                fprintf (stdout, "Loading %s | position %i | size %i\n", filename, i, fontsize);
            
            fonts[i] = TTF_OpenFont(filename, fontsize);
            if (fonts[i] == NULL)
            {
                fprintf (stderr, "Error loading %s: %s\n", filename, TTF_GetError());
                return 1;
            }
            i++;
        }
    }
    fclose (cfg_file);
    return 0;
}
