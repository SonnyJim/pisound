#include <SDL2/SDL_ttf.h>
#include <locale.h> //Used to set locale for score separator

//#define SCORE_SEPARATOR ','
#define MAX_FONTS 255
#define CFG_FONT_FILE "fonts.cfg"

SDL_Surface *score_srf; 
SDL_Surface *score_outline_srf; 
SDL_Texture *score_tex; 
SDL_Texture *score_outline_tex; 
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
long long old_score;
