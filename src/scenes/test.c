#include "pisound.h"
#include "gfx.h"
#include "scene.h"

SDL_Texture * texture;
SDL_Rect dstrect;

static void init_test_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[requested_scene]);

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid ( fonts[0], scene_names[requested_scene], color);
    texture = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    SDL_QueryTexture (texture, NULL, NULL, &dstrect.w, &dstrect.h);
    dstrect.x = (SCREEN_WIDTH / 2) - (dstrect.w / 2);
    dstrect.y = (SCREEN_HEIGHT / 2) - (dstrect.h / 2);
}

int draw_test (void)
{
    if (requested_scene != running_scene)
        init_test_scene ();

    SDL_RenderCopy (renderer, texture, NULL, &dstrect); 
    return 0;
}

