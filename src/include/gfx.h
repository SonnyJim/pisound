#include <SDL2/SDL_ttf.h>
#include <locale.h> //Used to set locale for score separator
#include <SDL2/SDL_image.h>

#include "fonts.h"

//#define SCORE_SEPARATOR ','
#define MAX_FONTS 255
#define CFG_FONT_FILE "fonts.cfg"

TTF_Font *fonts[255];
SDL_Color textColor;

SDL_DisplayMode videomode;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_RendererInfo drinfo; 

//Textures for transition effects
SDL_Texture *trans_scene1;
SDL_Texture *trans_scene2;

int SCREEN_HEIGHT;
int SCREEN_WIDTH;
int SCREEN_BPP;

int load_fonts (void);
int load_gfx_resources (void);

//Some vars for score_render
int player_num;
int old_player_num;
long long score;
long long old_score;

//FPS gubbins
int fpsStart;
int fpsFrames;
float fpsAvg;

void init_gfx_vars (void);

char *render_score_to_string (long long score);
SDL_Texture* load_image_to_texture (char *filename);

void gfx_init_game_vars (void);
