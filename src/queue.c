#include "pisound.h"
#include "queue.h"

void queue_init (queue_t *q)
{
        q->first = 0;
        q->last = QUEUESIZE-1;
        q->count = 0;
}

int queue_add (queue_t *q, int val)
{
        if (q->count >= QUEUESIZE)
        {
            fprintf(stderr, "Error: queue overflow  val=%d\n",val);
            return 1;
        }
        else 
        {
                q->last = (q->last+1) % QUEUESIZE;
                q->q[ q->last ] = val;    
                q->count = q->count + 1;
        }
        return 0;
}

int queue_remove (queue_t *q)
{
        int val;

        if (q->count <= 0) 
        {
            fprintf(stderr, "Error: Tried to queue_remove on an empty queue.\n");
            val = 0;
        }
        else 
        {
                val = q->q[ q->first ];
                q->first = (q->first+1) % QUEUESIZE;
                q->count = q->count - 1;
        }

        return (val);
}

int queue_empty (queue_t *q)
{
        if (q->count <= 0) 
            return true;
        else 
            return false;
}
