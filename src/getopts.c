#include "pisound.h"

void show_args (void)
{
    fprintf (stdout, "Command line options:\n");
    fprintf (stdout, "      -v          Verbose output\n");
#ifdef BUILD_GFX
    fprintf (stdout, "      -g          Enable GFX \n");
#endif
    fprintf (stdout, "      -a          Enable audio \n");
#ifdef BUILD_GPIO
    fprintf (stdout, "      -G          Enable GPIO\n");
#endif
    fprintf (stdout, "      -u          Enable UDP server\n");
    fprintf (stdout, "      -f          Show average frames per second\n");
    fprintf (stdout, "      -l          Don't show Pisound logo on startup\n");
    fprintf (stdout, "      -h          This help screen\n");
}

//read command line arguments
int getopts (int argc, char *argv[])
{
    int c;

    cfg_gfx_engine = 0;
    cfg_audio_engine = 0;
    cfg_gpio_engine = 0;
    cfg_udp_engine = 0;
    cfg_show_fps = 0;
    cfg_show_logo = 1;

    if (argv[1][0] != '-')
    {
        fprintf (stderr, "Unrecognised option: %s\n", argv[1]);
        return 1;
    }

    while ((c = getopt (argc, argv, "vhgaGufl")) != -1)
    {
        switch (c)
        {
        case 'v':
            verbose = 1;
            break;
#ifdef BUILD_GFX
        case 'g':
            cfg_gfx_engine = 1;
            break;
#endif
        case 'a':
            cfg_audio_engine = 1;
            break;
#ifdef BUILD_GPIO
        case 'G':
            cfg_gpio_engine = 1;
            break;
#endif
        case 'u':
            cfg_udp_engine = 1;
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
