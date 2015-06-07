#include "pisound.h"
#include "volume.h"

void init_volume (void)
{
    Mix_Volume (-1, volume);
    Mix_VolumeMusic (volume);
}

void volume_set (int volume)
{
    if (volume < MIN_VOLUME || volume > MAX_VOLUME)
        fprintf (stderr, "Warning: volume_set %i is out of range\n", volume);
    
    if (volume > MAX_VOLUME)
        volume = MAX_VOLUME;
    else if (volume < MIN_VOLUME)
        volume = MIN_VOLUME;

    if (verbose)
        fprintf (stdout, "Setting volume %i\n", volume);
    Mix_Volume (-1, volume);
    Mix_VolumeMusic (volume);
}

void volume_up (void)
{
    if (volume < MAX_VOLUME)
    {
        if (verbose)
            fprintf (stdout, "Volume up %i\n", volume);
        volume++;
        Mix_Volume (-1, volume);    
        Mix_VolumeMusic (volume);
    }
    else
        fprintf (stderr, "volume:  At max already\n");
}

void volume_down (void)
{
    if (volume > 0 && volume > MIN_VOLUME)
    {
        if (verbose)
            fprintf (stdout, "Volume down %i\n", volume);
        volume--;
        Mix_Volume (-1, volume);  
        Mix_VolumeMusic (volume);
    }
}


