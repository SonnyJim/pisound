#include "pisound.h"

void show_args (void)
{
    fprintf (stdout, "Command line options:\n");
    fprintf (stdout, "      -v          Verbose output\n");
    fprintf (stdout, "      -g          Enable GFX engine\n");
    fprintf (stdout, "      -f          Show average frames per second\n");
    fprintf (stdout, "      -l          Don't show Pisound logo on startup\n");
    fprintf (stdout, "      -h          This help screen\n");
}

//read command line arguments
int getopts (int argc, char *argv[])
{
    int c;

    cfg_gfx_engine = 0;
    cfg_show_fps = 0;
    cfg_show_logo = 1;
   
    while ((c = getopt (argc, argv, "vhgfl")) != -1)
    {
        switch (c)
        {
        case 'v':
            verbose = 1;
            break;
        case 'g':
            cfg_gfx_engine = 1;
            break;
        case 'f':
            cfg_show_fps = 1;
            break;
        case 'l':
            cfg_show_logo = 0;
            break;
        case 'h':
            show_args ();
            return 1;
        case '?':
            if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            show_args ();
            return 1;
        default:
            return 1;
        }
    }
    return 0;
}
