#include <SDL2/SDL.h>

//SDL_Surface *score_srf;
SDL_Surface *score_outline_srf;
SDL_Texture *score_tex;
SDL_Texture *score_outline_tex;
SDL_Rect    score_rect;
SDL_Surface *background_srf;
SDL_Texture *background_tex;
SDL_Rect    background_rect;

SDL_Surface *sprite_srf;

SDL_Texture *sprite_tex;
SDL_Rect    sprite_rect;

SDL_Texture *player_tex;
SDL_Rect    player_rect;

#define MAX_SUBSCENES 4

enum game_subscenes {
    GAME_SIGN,
    GAME_MINE,
    GAME_OUTHOUSE,
    GAME_RIVER
};

struct subscene_ops {
    int (*init) (void);
    char *name;
    char *background;
};

int game_sign_init (void);
int game_mine_init (void);
int game_outhouse_init (void);
int game_river_init (void);

struct subscene_ops subscenes[MAX_SUBSCENES] =
{
    { game_sign_init, "Sign", "data/hbb/gfx/backgrounds/sign.png"},
    { game_mine_init, "Mine", "data/hbb/gfx/backgrounds/mine.png"},
    { game_outhouse_init, "Outhouse", "data/hbb/gfx/backgrounds/outhouse.png"},
    { game_river_init, "River", "data/hbb/gfx/backgrounds/river.png"}
};
