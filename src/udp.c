#include "pisound.h"
#include "udp.h"
#include "queue.h"
#include "gfx.h"

static void udp_send_pong (struct sockaddr_in cliaddr)
{
    sendto(sockfd, UDP_PONG, strlen (UDP_PONG),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
}

static void udp_send_version (struct sockaddr_in cliaddr)
{
    sendto(sockfd, UDP_VERSION_STRING, strlen (UDP_VERSION_STRING),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
}

static void udp_send_cmd_error (struct sockaddr_in cliaddr)
{
    sendto(sockfd, UDP_CMD_ERROR, strlen (UDP_CMD_ERROR),0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
}

static void udp_score_receive (char *msg)
{
    score = strtoll (msg + 1, NULL, 10);
    fprintf (stdout, "Score set to %lld\n", score);
}

static void udp_player_num (int num)
{
    if (num > 0 && num <= MAX_PLAYERS)
    {
        player_num = num;
    }
    else
        fprintf (stderr, "Error: udp_player_num out of range\n");
}

static void udp_decode_msg (char *msg, struct sockaddr_in cliaddr)
{
    int byte1, byte2;
    char cmd[3], code[3];

    
    //Grab the frst 2 bytes and convert to int
    strncpy (cmd, msg, 2);
    strncpy (code, msg + 2, 2);
    byte1 = strtol (cmd, NULL, 16);
    byte2 = strtol (code, NULL, 16);

    //Error checking
    if (strlen (msg) < 2)
    {
        fprintf (stderr, "Error in udp_decode_msg: empty message");
        udp_send_cmd_error (cliaddr);
        return;
    }
    else if (strlen (msg) < 4 
            && (byte1 == UDP_SOUND_PLAY || byte1 == UDP_MUSIC_PLAY))
    {
        fprintf (stderr, "Error in udp_decode_msg: Message too short\n");
        fprintf (stdout, "UDP received: byte1=%i byte2=%i msg=%s\n", byte1, byte2, msg);
        udp_send_cmd_error (cliaddr);
        return;
    }

    if (byte1 > 255 || byte2 > 255)
    {
        udp_send_cmd_error (cliaddr);
        fprintf (stderr, "Error in udp_decode_msg: %i %i %s\n", byte1, byte2, msg);
    }

    if (verbose)
        fprintf (stdout, "UDP received: byte1=%i byte2=%i msg=%s\n", byte1, byte2, msg);

    switch (byte1)
    {
        case UDP_SOUND_PLAY:
            queue_add (&sfx_q, byte2);
            break;
        case UDP_MUSIC_PLAY:
            music_request (byte2);
            break;
        case UDP_MUSIC_STOP:
            music_request (MUSIC_OFF);
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
            udp_send_pong (cliaddr);
            break;
        case UDP_VERSION:
            udp_send_version (cliaddr);
            break;
        case UDP_SCORE:
            udp_score_receive (msg);
            break;
        case UDP_PLAYER_NUM:
            udp_player_num (byte2);
            break;
        case UDP_QUIT:
            running = 0;
            break;
        default:
            fprintf (stderr, "Unrecognised udp_decode_msg: %i %i %s\n", byte1, byte2, msg);
            udp_send_cmd_error (cliaddr);
            break;
    }
}

void* udp_thread (void *ptr)
{
   if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
   {
        fprintf (stderr, "Can't create socket: %s\n", strerror(errno));
   }

   memset (&servaddr, 0, sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
   servaddr.sin_port = htons (UDP_PORT);

   if (bind (sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1)
   {
       fprintf (stderr, "Error starting udp server: %s\n", strerror(errno));
   }
   else
   {
       if (verbose)
           fprintf (stdout, "udp server running on %i\n", UDP_PORT);
       while (running)
       {
           len = sizeof(cliaddr);
           n = recvfrom (sockfd, mesg, UDP_BUFFLEN, 0, (struct sockaddr *) &cliaddr,&len);
           //Echo request
           //sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
           mesg[n] = 0;
           udp_decode_msg (mesg, cliaddr);
           memset (mesg, 0, sizeof(mesg));
       }
       if (verbose)
           fprintf (stdout, "Shutting down udp server\n");
   }
   
   return NULL;
}
