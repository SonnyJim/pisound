#include <stdbool.h>
#define QUEUESIZE       1024

typedef struct queue_t{
    int q[QUEUESIZE+1];
    int first;
    int last;
    int count;
} queue_t;

//Input/Output queues
struct queue_t gpio_input_q;
struct queue_t gpio_output_q;
struct queue_t sfx_q;

void queue_init (queue_t *q);
int queue_add (queue_t *q, int val);
int queue_remove (queue_t *q);
int queue_empty (queue_t *q);

void music_request (int music_code);
int music_requested;
