#define CFG_FONT_FILE "fonts.cfg"
#define OUTPUT_FILE "src/include/fonts.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main (void)
{
    int i;
    FILE *cfg_file, *output_file;
    char cfg_line[1024] = "", out_line[1024] = "", *fontname, *fontsize;
    const char delim[] = ",";

    fprintf (stdout, "fontgen: Generating font headers from %s\n", CFG_FONT_FILE);
    //Load font list
    cfg_file = fopen (CFG_FONT_FILE, "r");
    
    if (cfg_file == NULL)
        return 1;
    output_file = fopen (OUTPUT_FILE, "w");
    if (output_file == NULL)
        return 1;

    i = 0;
    while (fgets (cfg_line, 1024, cfg_file) != NULL)
    {
        //Ignore remarks
        if (cfg_line[0] != '#')
        {
            fontname = strtok (cfg_line, delim);
            fontsize = strtok (NULL, delim);
            if (fontsize == NULL || fontname == NULL)
            {
                fprintf (stdout, "fontgen: EOF\n");
                break;
            }
            sprintf (out_line, "#define FON_%s_%i fonts[%i]\n", fontname, atoi(fontsize), i);
            fputs (out_line, output_file);
            i++;
        }
    }
    fclose (cfg_file);
    fclose (output_file);
    fprintf (stdout, "fontgen: Finished\n");
    return 0;
}
