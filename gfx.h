#include <SDL/SDL_ttf.h>

#define SCORE_SEPARATOR ','
#define TEST_SCORE "1,234,567,890"
#define MAX_FONTS 255
#define CFG_FONT_FILE "fonts.cfg"

SDL_Surface *score; 
SDL_Surface *background; 
SDL_Surface *screen;

TTF_Font *fonts[255];
SDL_Color textColor;

int SCREEN_HEIGHT;
int SCREEN_WIDTH;
int SCREEN_BPP;

int load_fonts (void);
