#include <wiringPi.h>
#include <pisound.h>

#define IC6_DIR     8
#define IC6_IN      1
#define IC6_OUT     0
#define IC3_CE      9
#define IN_CLK      10
#define OUT_CLK     11
#define WPC_RST     12
#define STATUS_LED  13
#define IN_IRQ      14


char input_buff, output_buff;

//IRQ received from WPC, copy data into input_buff
void irq_recv (void)
{
    input_buff = read_gpio();
    if (verbose)
        fprintf (stdout, "Received %c on GPIO\n", input_buff);
}

void rst_recv (void)
{
    //Send reset signal to pisound
}

static void set_gpio_output (void)
{
    int i;
    for (i = 0; i < 7; i++)
        pinMode (i, OUTPUT);
    digitalWrite (IC6_DIR, IC6_OUT);
    digitalWrite (IC3_CE, 0);
}

static void set_gpio_input (void)
{
    int i;
    for (i = 0; i < 7; i++)
        pinMode (i, INPUT);
    digitalWrite (IC6_DIR, IC6_IN);
    digitalWrite (IC3_CE, 1);
}

static int init_gpio (void)
{
    wiringPiSetup ();
    set_gpio_input ();
    pinMode (WPC_RST, INPUT);
    pinMode (IN_IRQ, INPUT);
    pinMode (IN_CLK, INPUT);
    pinMode (WPC_RST, INPUT);

    pinMode (STATUS_LED, OUTPUT);
    pinMode (OUT_CLK, OUTPUT);
    pinMode (IC6_DIR, OUTPUT);
    pinMode (IC3_CE, OUTPUT);
    
    wiringPiISR (IN_IRQ, INT_EDGE_FALLING, &irq_recv);
    wiringPiISR (WPC_RST, INT_EDGE_FALLING, &rst_recv);
}

static int shutdown_gpio (void)
{

}

static char* read_gpio (void)
{
    char input;
    int i;

    for (i = 0; i < 7; i++)
    {
        
    }

}

static void write_gpio (char val)
{
    set_gpio_output ();
    //Write to output buffer
    //Set GPIO back to input
    set_gpio_input ();
}

void *gpio_thread(void *ptr)
{
    init_gpio ();
    while (running)
    {

    }
    shutdown_gpio ();
    return NULL;
}


