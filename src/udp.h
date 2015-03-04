#include "pisound.h"
#include <sys/socket.h>
#include <netinet/in.h>

/*
 * UDP message format:
 * 1 byte command followed by 1 byte code
 *
 */

#define UDP_SOUND_PLAY      0x00
#define UDP_MUSIC_PLAY      0x01
#define UDP_MUSIC_STOP      0x02
#define UDP_VOLUME_UP       0x03
#define UDP_VOLUME_DOWN     0x04
#define UDP_VOLUME_SET      0x05

//Message format:
//send UDP_SCORE_START, followed by the player score as a string
#define UDP_SCORE           0xe0
#define UDP_PLAYER_NUM      0xe1

#define UDP_SCENE_CHANGE    0xfb
#define UDP_SHUTDOWN        0xfc
#define UDP_PING            0xfe
#define UDP_VERSION         0xff

#define UDP_PORT            8008
#define UDP_BUFFLEN         256

//Sent to client when an error occurs
#define UDP_MSG_ERROR   "ERROR"
#define UDP_MSG_PING    "PONG"
#define UDP_MSG_VERSION "Pisound V0.1"

int sockfd,n;
struct sockaddr_in servaddr,cliaddr;
socklen_t len;
char mesg[UDP_BUFFLEN];
