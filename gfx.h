//const int SCREEN_WIDTH = 1024;
//const int SCREEN_HEIGHT = 600;
//const int SCREEN_BPP = 16;
#include <SDL/SDL_ttf.h>

#define DEFAULT_FONT "fonts/lazy.ttf"

SDL_Surface *message = NULL; 
SDL_Surface *background = NULL; 
SDL_Surface *screen = NULL;

TTF_Font *font = NULL;
SDL_Color textColor;


