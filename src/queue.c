#include "pisound.h"

void music_request (int music_code)
{
    if (music_code < MAX_MUSIC || music_code == MUSIC_OFF)
        music_requested = music_code;
    else
        fprintf (stderr, "Error: music_request out of range %i\n", music_code);
}

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
        sound_queue.first = (sound_queue.first + 1) % SOUND_QUEUE_SIZE;
        sound_queue.count = sound_queue.count - 1;
    }
    if (i > MAX_SOUNDS || i > MAX_MUSIC)
    {
        fprintf (stderr, "Error:  sound_queue_read tried to return %i, which is bigger than MAX_SOUNDS/MAX_MUSIC\n", i);
        exit (1);
    }
    else
        return (i);
}

//Add an item to the sound_queue
void sound_queue_add (int sound_code)
{
    if (verbose)
        fprintf (stdout, "sound_queue_add %i\n", sound_code);

    if (sound_code > MAX_SOUNDS || sound_code > MAX_MUSIC)
    {
        fprintf (stderr, "Error:  sound_queue_add tried to add %i, which is bigger than MAX_SOUNDS/MAX_MUSIC\n", sound_code);
        return;
    }

    if (sound_queue.count >= SOUND_QUEUE_SIZE)
        fprintf (stderr, "Error: sound_queue overflow\n");
    else
    {
        sound_queue.last = (sound_queue.last + 1) % SOUND_QUEUE_SIZE;
        sound_queue.data[sound_queue.last] = sound_code;
        sound_queue.count = sound_queue.count + 1;
    }
}
