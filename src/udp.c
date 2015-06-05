#include "pisound.h"
#include "udp.h"
#include "queue.h"
#include "gfx.h"
#include "snd.h"
#include "scene.h"

static void udp_send_msg (char *msg, Uint32 cliaddr)
{
    Uint16 port = UDP_PORT + 1; 
    strcpy ((char *)pms->data, msg);
    pms->len = strlen ((char *)pms->data);
    pms->address.host = cliaddr;
    pms->address.port = SDLNet_Read16(&port);
    if (verbose)
        fprintf (stdout, "Sending msg: %s to %x %x\n", pms->data, pms->address.host, pms->address.port); 
    SDLNet_UDP_Send(sd, -1, pms);
}

static void udp_score_receive (char *msg)
{
    score = strtoll (msg + 1, NULL, 10);
    fprintf (stdout, "Score set to %lld\n", score);
}

static void udp_scene_receive (long scene, Uint32 cliaddr)
{
    if (scene >= 0 && scene <= MAX_SCENES)
    {
        if (verbose)
            fprintf (stdout, "scene change requested: %lu\n", scene);
        requested_scene = (int) scene;
    }
    else
    {
        fprintf (stderr, "Error: requested scene %lu out of range\n", scene);
        udp_send_msg (UDP_MSG_ERROR, cliaddr);
    }
}

static void udp_scene_trans (long trans)
{
    if (verbose)
        fprintf (stdout, "transition requested: %lu\n", trans);
    if (!scene_transition_running)
        scene_transition = (int) trans;
}

static void udp_player_num (long num, Uint32 cliaddr)
{
    if (num > 0 && num <= MAX_PLAYERS)
    {
        player_num = (int) num;
    }
    else
    {
        fprintf (stderr, "Error: udp_player_num out of range\n");
        udp_send_msg (UDP_MSG_ERROR, cliaddr);
    }
}

static void udp_decode_msg (char *msg, Uint32 cliaddr)
{
    int byte1, byte2;
    char cmd[3], code[3];

    
    //Grab the frst 2 bytes and convert to int
    strncpy (cmd, msg, 2);
    strncpy (code, msg + 2, 2);
    byte1 = strtol (cmd, NULL, 16);
    byte2 = strtol (code, NULL, 16);

    //Error checking
    if (strlen (msg) < 1)
    {
        fprintf (stderr, "Error in udp_decode_msg: tiny message");
        udp_send_msg (UDP_MSG_ERROR, cliaddr);
        return;
    }
    else if (strlen (msg) < 4 
            && (byte1 != UDP_SOUND_PLAY || byte1 != UDP_MUSIC_PLAY || byte1 == UDP_SCENE_CHANGE))
    {
        //fprintf (stderr, "Error in udp_decode_msg: Message too short\n");
        //fprintf (stdout, "UDP received: byte1=%i byte2=%i msg=%s\n", byte1, byte2, msg);
        //udp_send_msg (UDP_MSG_ERROR, cliaddr);
        //return;
    }

    if (byte1 > 255 || byte2 > 255)
    {
        //udp_send_msg (UDP_MSG_ERROR, cliaddr);
        fprintf (stderr, "Error in udp_decode_msg bytes bigger than 255: %i %i %s\n", byte1, byte2, msg);
    }

    if (verbose)
        fprintf (stdout, "UDP received: byte1=%i byte2=%i msg=%s\n", byte1, byte2, msg);

    switch (byte1)
    {
        case UDP_SOUND_PLAY:
            if (play_sound (byte2))
                udp_send_msg (UDP_MSG_ERROR, cliaddr);
            break;
        case UDP_MUSIC_PLAY:
            if (play_music (byte2))
                udp_send_msg (UDP_MSG_ERROR, cliaddr);
            break;
        case UDP_MUSIC_STOP:
            if (play_music (MUSIC_OFF))
                udp_send_msg (UDP_MSG_ERROR, cliaddr);
            break;
        case UDP_VOLUME_UP:
            volume_up ();
            break;
        case UDP_VOLUME_DOWN:
            volume_down ();
            break;
        case UDP_VOLUME_SET:
            volume_set (byte2);
            break;
        case UDP_PING:
            udp_send_msg (UDP_MSG_PING, cliaddr);
            break;
        case UDP_VERSION:
            udp_send_msg (UDP_MSG_VERSION, cliaddr);
            break;
        case UDP_SCORE:
            udp_score_receive (msg);
            break;
        case UDP_PLAYER_NUM:
            udp_player_num (byte2, cliaddr);
            break;
        case UDP_SHUTDOWN:
            running = 0;
            break;
        case UDP_SCENE_CHANGE:
            udp_scene_receive (byte2, cliaddr);
            break;
        case UDP_SCENE_TRANS:
            udp_scene_trans (byte2);
            break;
        default:
            fprintf (stderr, "Unrecognised udp_decode_msg: %i %i %s\n", byte1, byte2, msg);
            udp_send_msg (UDP_MSG_ERROR, cliaddr);
            break;
    }
}

int udp_thread (void *ptr)
{
    /* Open a UDP socket */
    if (!(sd = SDLNet_UDP_Open(UDP_PORT)))
    {
        fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        return 1;
    }

    /* Make space for the recv and sending packet */
    if (!(pm = SDLNet_AllocPacket(UDP_BUFFLEN)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        return 1;
    }
    
    if (!(pms = SDLNet_AllocPacket(UDP_BUFFLEN)))
    {
        fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
        return 1;
    }

    while (running)
    {
        if (SDLNet_UDP_Recv(sd, pm))
        {
            udp_decode_msg ((char *)pm->data, pm->address.host);
        }
    }
    
    if (verbose)
        fprintf (stdout, "Shutting down udp server\n");
    SDLNet_FreePacket(pm);
    SDLNet_FreePacket(pms);
    SDLNet_UDP_Close (sd);
    SDLNet_Quit ();
    return 0;
}
