//How many different sounds we can play
#define MAX_SOUNDS 255
//How long the sound queue is
#define SOUND_QUEUE_SIZE 16
#define QUEUE_POS_EMPTY 256

#define DEFAULT_CFG_FILE "pisound.cfg"

//List of all the pointers to the sounds that can be called
Mix_Chunk *sounds[MAX_SOUNDS];

//Channel on which our sound is played
int channel;

//sound queue
struct sound_queue_t {
    int data[SOUND_QUEUE_SIZE + 1];
    int first;
    int last;
    int count;
} sound_queue;


