#include "pisound.h"
#include "gfx.h"
#include "scene.h"

SDL_Texture * texture;
SDL_Rect dstrect;

static void init_gameover_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[requested_scene]);

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Blended ( FON_CHIZ_BOLD_80, "Game Over", color);
    texture = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    SDL_QueryTexture (texture, NULL, NULL, &dstrect.w, &dstrect.h);
    dstrect.x = (SCREEN_WIDTH / 2) - (dstrect.w / 2);
    dstrect.y = (SCREEN_HEIGHT / 2) - (dstrect.h / 2);
}

int draw_gameover (void)
{
    if (requested_scene != running_scene)
        init_gameover_scene ();

    SDL_RenderCopy (renderer, texture, NULL, &dstrect); 
    return 0;
}

