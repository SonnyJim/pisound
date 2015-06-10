#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <SDL_thread.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

#include "udp.h"

#define NUM_CMDS 10
#define IP "127.0.0.1"

UDPsocket socket;
UDPpacket *send_p;
IPaddress srvaddr;
SDL_Event event;
char score_str[16] = "1234";
long long score;

SDL_Window *win = NULL;
SDL_Renderer *ren = NULL;
TTF_Font    *font = NULL;

char byte1, byte2;
int cmd_index, cmd_value, cmd_mode;
int running, score_entry;
enum { MODE_CMD, MODE_VALUE};

struct udp_cmd
{
    char *name;
    int udp_byte;
    int data_size;
};

struct udp_cmd udp_cmds[NUM_CMDS] = {
    { "Play sound", UDP_SOUND_PLAY, 1},
    { "Play music", UDP_MUSIC_PLAY, 1},
    { "Stop music", UDP_MUSIC_STOP, 0},
    { "Volume set", UDP_VOLUME_SET, 1},
    { "Set score", UDP_SCORE, sizeof(score)},
    { "Set player", UDP_PLAYER_NUM, 1},
    { "Set scene", UDP_SCENE_CHANGE, 1},
    { "Set subscene", UDP_SUBSCENE_CHANGE, 1},
    { "Set transition", UDP_SCENE_TRANS, 1},
    { "Shutdown", UDP_SHUTDOWN, 0},
};

void sig_handler (int signo)
{
    if (signo == SIGINT)
    {
        running = 0;
        fprintf (stdout, "\nSIGINT detected\n");
    }
}

/*
int recv_thread (void *ptr)
{
    UDPpacket *recv_p;

    recv_p = SDLNet_AllocPacket (UDP_BUFFLEN);
    if (!recv_p)
    {
        fprintf (stderr, "Error allocing recv_packet: %s\n", SDLNet_GetError());
        return 1;
    }

    while (running)
    {
        if (SDLNet_UDP_Recv (socket, recv_p))
        {
            fprintf (stdout, "recv: %s\n", (char *)recv_p->data);
        }
    }
    fprintf (stdout, "receive thread stopped\n");
    return 0;
}
*/

static void send_packet (char *msg, int data_size)
{
    send_p->address.host = srvaddr.host;
    send_p->address.port = srvaddr.port;
    memcpy (send_p->data, msg, data_size);
    send_p->len = data_size;

    SDLNet_UDP_Send (socket, -1, send_p);
}

static void mode_set (void)
{
    //Automatically set MODE_CMD if there's no data to send with the command
    if (udp_cmds[cmd_index].data_size == 0)
        cmd_mode = MODE_CMD;

    if (udp_cmds[cmd_index].udp_byte == UDP_SCORE)
    {
        SDL_StartTextInput();
        score_entry = 1;
    }
    else
    {
        SDL_StopTextInput();
        score_entry = 0;
    }
}

static void cmd_index_inc (void)
{
    if (cmd_mode == MODE_CMD)
    {
        if (cmd_index < NUM_CMDS - 1)
            cmd_index++;
    }
    else if (cmd_mode == MODE_VALUE)
    {
        if (cmd_value < 255)
            cmd_value++;
    }

    mode_set ();
}

static void cmd_index_dec (void)
{
    if (cmd_mode == MODE_CMD)
    {
        if (cmd_index > 0)
            cmd_index--;
    }
    else if (cmd_mode == MODE_VALUE)
    {
        if (cmd_value > 0)
        cmd_value--;
    }
    mode_set ();
}

static void draw_cmd (void)
{
    SDL_Color colour = { 255, 255, 255 };
   // SDL_Color rectcolour = {100, 100, 100};

    SDL_Surface *cmdsrf = NULL;
    SDL_Texture *cmdtex = NULL;
    SDL_Rect dstrect;
    char value[4] = "000";

    sprintf (value, "%03i", cmd_value);

    dstrect.x = 10;
    dstrect.y = 10;
    //Draw cmd name

    cmdsrf = TTF_RenderText_Solid (font, udp_cmds[cmd_index].name, colour);
    cmdtex = SDL_CreateTextureFromSurface(ren, cmdsrf);
    SDL_QueryTexture(cmdtex, NULL, NULL, &dstrect.w, &dstrect.h);

    //Draw red box around cmd
    if (cmd_mode == MODE_CMD)
    {
        dstrect.y -= 1;
        SDL_SetRenderDrawColor (ren, 255, 0, 0, 0);
        SDL_RenderDrawRect(ren, &dstrect);
        dstrect.y += 1;
    }


    SDL_RenderCopy(ren, cmdtex, NULL, &dstrect);

    if (udp_cmds[cmd_index].data_size == 0)
        return;

    if (score_entry)
    {
        cmdsrf = TTF_RenderText_Solid (font, score_str, colour);
        cmdtex = SDL_CreateTextureFromSurface(ren, cmdsrf);
        dstrect.x = 300;
        SDL_QueryTexture(cmdtex, NULL, NULL, &dstrect.w, &dstrect.h);

        SDL_RenderCopy(ren, cmdtex, NULL, &dstrect);
        return;
    }

    //Draw data value
    cmdsrf = TTF_RenderText_Solid (font, value, colour);
    cmdtex = SDL_CreateTextureFromSurface(ren, cmdsrf);
    dstrect.x = 400;
    SDL_QueryTexture(cmdtex, NULL, NULL, &dstrect.w, &dstrect.h);

    //Draw red box around value
    if (cmd_mode == MODE_VALUE)
    {
        dstrect.y -= 1;
        SDL_SetRenderDrawColor (ren, 255, 0, 0, 0);
        SDL_RenderDrawRect(ren, &dstrect);
        dstrect.y += 1;
    }

    SDL_RenderCopy(ren, cmdtex, NULL, &dstrect);
}

static void volume_up (void)
{
    char msg[1];
    msg[0] = UDP_VOLUME_UP;
    send_packet (msg, 1);
}

static void volume_down (void)
{
    char msg[1];
    msg[0] = UDP_VOLUME_DOWN;
    send_packet (msg, 1);
}

static void send_cmd (void)
{
    char msg[UDP_BUFFLEN];
    memset (msg, 0, UDP_BUFFLEN);
    msg[0] = udp_cmds[cmd_index].udp_byte;

    if (score_entry)
    {
        score = strtoll (score_str, NULL, 10);
        memcpy (msg + 1, &score, sizeof(score));
    }
    else if (udp_cmds[cmd_index].data_size == 0)
    {
        msg[1] = '\0';
    }
    else if (udp_cmds[cmd_index].data_size > 0)
    {
        msg[1] = cmd_value;
        msg[2] = '\0';
    }

    //fprintf (stdout, "Sending command: %s %i\n", udp_cmds[cmd_index].name, cmd_value);
    send_packet(msg, udp_cmds[cmd_index].data_size + 1);
}

static void remove_score_digit (void)
{
    int len;
    len = strlen (score_str);
    if (len > 0)
    {
        score_str[len - 1] = '\0';
    }
}

void read_input (void)
{

    while (SDL_PollEvent (&event))
    {
        if (event.type == SDL_TEXTINPUT && score_entry)
        {
            if ( event.text.text[0] >= '0' && event.text.text[0] <= '9')
            {
                if (strlen (score_str) < 10)
                    sprintf (score_str, "%s%s", score_str, event.text.text);
            }
        }

        if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    fprintf (stderr, "Quit\n");
                    running = 0;
                    break;
                case SDLK_RIGHTBRACKET:
                    volume_up ();
                    break;
                case SDLK_LEFTBRACKET:
                    volume_down ();
                    break;
                case SDLK_UP:
                    cmd_index_inc ();
                    break;
                case SDLK_DOWN:
                    cmd_index_dec ();
                    break;
                case SDLK_LEFT:
                    cmd_mode = MODE_CMD;
                    break;
                case SDLK_RIGHT:
                    if (udp_cmds[cmd_index].data_size > 0 && score_entry != 1)
                        cmd_mode = MODE_VALUE;
                    break;
                case SDLK_RETURN:
                    send_cmd ();
                    break;
                case SDLK_BACKSPACE:
                    if (score_entry)
                        remove_score_digit ();
                    break;
            }
        }
        if (event.type == SDL_QUIT)
        {
            running = 0;
        }
    }
}

int sdl_init (void)
{
    if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    win = SDL_CreateWindow ("Pisound client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 50, SDL_WINDOW_SHOWN);

    if (win == NULL)
    {
        fprintf (stderr, "Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL)
    {
        fprintf (stderr, "Error creating renderer: %s\n", SDL_GetError());
        return 1;
    }

    // Initilise networking
    if (SDLNet_Init () != 0)
    {
        fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
        return 1;
    }

    socket = SDLNet_UDP_Open (0);
    if (!socket)
    {
        fprintf (stderr, "Error opening socket: %s\n", SDLNet_GetError());
        return 1;
    }

    send_p = SDLNet_AllocPacket (UDP_BUFFLEN);
    if (!send_p)
    {
        fprintf (stderr, "Error allocing send_packet: %s\n", SDLNet_GetError());
        running = 0;
        return 1;
    }

    if (SDLNet_ResolveHost (&srvaddr, IP, UDP_PORT))
    {
        fprintf (stderr, "Error resolving host: %s\n", SDLNet_GetError());
        return 1;
    }

    if (TTF_Init () == -1)
    {
        fprintf (stderr, "Error initialising TTF library\n");
        return 1;
    }
    font = TTF_OpenFont("data/fonts/Alfphabet-IV.ttf",28);
    if (font == NULL)
    {
        fprintf (stderr, "Error loading font\n");
        return 1;
    }

    return 0;
}

int main (int argc, char *argv[])
{
    SDL_Thread *thread = NULL;

    //setup signal handler
    if ( signal(SIGINT, sig_handler) == SIG_ERR)
    {
        fprintf(stderr, "can't catch SIGINT\n");
        return 1;
    }

    running = 1;
    cmd_index = 0;
    cmd_value = 0;
    cmd_mode = MODE_CMD;
    score_entry = 0;

    if (sdl_init ())
        return 1;
/*
    thread = SDL_CreateThread (recv_thread, "RecvThread", (void *) NULL);
    if (thread == NULL)
    {
        fprintf (stderr, "SDL_CreateThread failed: %s\n", SDL_GetError());
        return 1;
    }
*/


    while (running)
    {
        SDL_SetRenderDrawColor (ren, 0, 0, 0, 0);
        SDL_RenderClear(ren);
        draw_cmd ();
        SDL_RenderPresent (ren);
        read_input ();
    }

    SDLNet_UDP_Close (socket);
    SDL_WaitThread (thread, NULL);
    SDLNet_Quit ();
    TTF_Quit ();
    SDL_Quit ();

    fprintf (stdout, "Exiting\n");
    return 0;
}
