/*
 * UDP message format:
 * 1 byte command followed by 1 byte code
 *
 */

#define UDP_SOUND_PLAY  0x00
#define UDP_MUSIC_PLAY  0x01
#define UDP_MUSIC_STOP  0x02
#define UDP_VOLUME_UP   0x03
#define UDP_VOLUME_DOWN 0x04

#define UDP_VERSION 0xff

#define UDP_PORT 8008
#define UDP_BUFFLEN 16
#define UDP_VERSION_STRING "Pisound V0.1"

int sockfd,n;
struct sockaddr_in servaddr,cliaddr;
socklen_t len;
char mesg[UDP_BUFFLEN];
