#include <wiringPi.h>
#include "pisound.h"
#include "gpio.h"
#include "gpio_queue.h"

//IC6 = Output buffer to WPC
#define IC6_DIR     8
#define IC6_IN      1
#define IC6_OUT     0

//IC3 = Input buffer to Pi
#define IC3_CE      9
#define IC3_ENABLE  0
#define IC3_DISABLE 1

#define IN_IRQ      10
#define OUT_CLK     11
#define WPC_RST     12
#define STATUS_LED  13

//Whether the input buffer has been read by the WPC CPU
bool input_waiting, output_waiting;

struct queue_t gpio_input_q;
struct queue_t gpio_output_q;

//Read an 8bit value from GPIO 0-7
static uint8_t read_gpio (void)
{
    uint8_t input = 0;
    int i;

    for (i = 0; i < 8; i++)
        input |= (digitalRead (i) << i);

    return input;
}

//IRQ received from WPC, copy data into input queue
void irq_recv (void)
{
    int val;
    val = read_gpio();
    if (verbose)
        fprintf (stdout, "Received %i on GPIO\n", val);
    queue_add (&gpio_input_q, val);
    //Assuming that the WPC CPU reads U29 before it tries to send data to U30
    input_waiting = false;
}

void rst_recv (void)
{
    //Send reset signal to main code
}

//Change the GPIO buffer to output
static void set_gpio_output (void)
{
    int i;
    for (i = 0; i < 8; i++)
        pinMode (i, OUTPUT);
    //Change the Pi buffer direction
    digitalWrite (IC6_DIR, IC6_OUT);
    //Disable the WPC input buffer
    digitalWrite (IC3_CE, IC3_DISABLE);
}

//Change the GPIO buffer to input
static void set_gpio_input (void)
{
    int i;
    for (i = 0; i < 8; i++)
        pinMode (i, INPUT);
    //Change the Pi buffer direction
    digitalWrite (IC6_DIR, IC6_IN);
    //Enable the WPC input buffer
    digitalWrite (IC3_CE, IC3_ENABLE);
}

//Generate a CLK pulse for IC4
static void out_clk (void)
{
    digitalWrite (OUT_CLK, 0);
    digitalWrite (OUT_CLK, 1);
}

static int init_gpio (void)
{
    //Init the input and output queues
    queue_init (&gpio_input_q);
    queue_init (&gpio_output_q);
    
    wiringPiSetup ();
    pinMode (WPC_RST, INPUT);
    pinMode (IN_IRQ, INPUT);
    pinMode (WPC_RST, INPUT);

    pinMode (STATUS_LED, OUTPUT);
    pinMode (OUT_CLK, OUTPUT);
    pinMode (IC6_DIR, OUTPUT);
    pinMode (IC3_CE, OUTPUT);
    
    wiringPiISR (IN_IRQ, INT_EDGE_FALLING, &irq_recv);
    wiringPiISR (WPC_RST, INT_EDGE_FALLING, &rst_recv);
 
    
    set_gpio_input ();

    digitalWrite (STATUS_LED, 1);
    return 0;
}

static void write_gpio (uint8_t val)
{
    //Set GPIO buffer to output
    set_gpio_output ();
    //Write to output buffer
    digitalWriteByte (val);
    input_waiting = true;
    //Clock the data into IC4, ready for the WPC to pick it up
    out_clk ();
    //Set GPIO buffer back to input
    set_gpio_input ();
}

//Called from main code to add data to the output queue
void gpio_send_out (int val)
{
    queue_add (&gpio_output_q, val);
}

void *gpio_thread(void *ptr)
{
    init_gpio ();
    while (running)
    {
        if (!queue_empty (&gpio_output_q) && !input_waiting)
            write_gpio (queue_remove (&gpio_output_q));

        if (!queue_empty (&gpio_input_q))
            sound_queue_add (queue_remove (&gpio_input_q));
    }
    digitalWrite (STATUS_LED, 0);
    return NULL;
}


