//SDL_Surface *score_srf; 
SDL_Surface *score_outline_srf; 
SDL_Texture *score_tex; 
SDL_Texture *score_outline_tex; 
SDL_Rect    score_rect;
SDL_Surface *background_srf; 
SDL_Texture *background_tex; 

SDL_Surface *sprite_srf;

SDL_Texture *sprite_tex;
SDL_Rect    sprite_rect;

SDL_Texture *player_tex;
SDL_Rect    player_rect;

#define IMG_BILLYBOB    "images/headshots/billybob.png"
#define IMG_BUBBA       "images/headshots/bubba.png"
#define IMG_GRANDPA     "images/headshots/grandpa.png"
#define IMG_PEGGYSUE    "images/headshots/peggysue.png"

#define DIR_L 0
#define DIR_D 1
#define DIR_R 2
#define DIR_U 3

SDL_Texture *billybob_tex;
SDL_Rect    billybob_rect;

SDL_Texture *bubba_tex;
SDL_Rect    bubba_rect;

SDL_Texture *grandpa_tex;
SDL_Rect    grandpa_rect;

SDL_Texture *peggysue_tex;
SDL_Rect    peggysue_rect;

int sprite_x;
int sprite_y;
int x_up;
int y_up;


