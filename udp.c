#include <sys/socket.h>
#include <netinet/in.h>

#include "pisound.h"

void* udp_thread (void *ptr)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr;
   socklen_t len;
   char mesg[1000];


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
           n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
           sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
           printf("-------------------------------------------------------\n");
           mesg[n] = 0;
           printf("Received the following:\n");
           printf("%s",mesg);
           strcpy (udp_msg, mesg);
           printf("-------------------------------------------------------\n");
       }
   }
   if (verbose)
       fprintf (stdout, "Shutting down udp server\n");
   
   return NULL;
}
