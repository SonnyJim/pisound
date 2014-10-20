#include <SDL2/SDL_ttf.h>

#define SCORE_SEPARATOR ','
#define TEST_SCORE "1,234,567,890"
#define MAX_FONTS 255
#define CFG_FONT_FILE "fonts.cfg"

SDL_Surface *score_srf; 
SDL_Texture *score_tex; 
SDL_Texture *score_outline; 
SDL_Rect    score_rect;
SDL_Surface *background_srf; 
SDL_Texture *background_tex; 

TTF_Font *fonts[255];
SDL_Color textColor;

SDL_DisplayMode videomode;
SDL_Window *window;
SDL_Renderer *renderer;

int SCREEN_HEIGHT;
int SCREEN_WIDTH;
int SCREEN_BPP;

int load_fonts (void);

int player_num;
long long score;

int transparency, direction;
