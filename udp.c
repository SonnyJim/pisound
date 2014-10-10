#include <sys/socket.h>
#include <netinet/in.h>

#include "pisound.h"
#include "udp.h"
static void decode_udp_msg (char *msg)
{
    int byte1, byte2;
    char cmd[3], code[3];
    printf ("decode: %s\n", msg);
   
    strncpy (cmd, msg, 2);
    strncpy (code, msg + 2, 2);
    printf ("cmd=%s code=%s\n", cmd, code);

    byte1 = strtol (cmd, NULL, 16);
    byte2 = strtol (code, NULL, 16);

    if (byte1 > 255 || byte2 > 255)
        fprintf (stderr, "Error in udp_msg decode: %i %i %s\n", byte1, byte2, msg);

    switch (byte1)
    {
        case UDP_SOUND_PLAY:
            sound_queue_add (byte2);
            break;
        case UDP_MUSIC_PLAY:
            music_request (byte2);
            break;
        case UDP_MUSIC_STOP:
            music_request (MUSIC_OFF);
            break;
        default:
            fprintf (stderr, "Unrecognised udp_msg decode: %i %i %s\n", byte1, byte2, msg);
            break;
    }
}

void* udp_thread (void *ptr)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char mesg[1024];


   if (verbose)
       fprintf (stdout, "Starting udp server\n");
   
   if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1)
   {
        fprintf (stderr, "Can't create socket: %s\n", strerror(errno));
   }

   bzero(&servaddr,sizeof(servaddr));
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
   servaddr.sin_port=htons(UDP_PORT);
   if (bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) == -1)
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
           n = recvfrom(sockfd,mesg,1024,0,(struct sockaddr *)&cliaddr,&len);
           sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
           mesg[n] = 0;
           if (verbose)
               fprintf (stdout, "UDP received: %s\n", mesg);
           decode_udp_msg (mesg);
       }
   }
   if (verbose)
       fprintf (stdout, "Shutting down udp server\n");
   
   return NULL;
}
