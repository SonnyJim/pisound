//This scene is shown whilst we are waiting for the machine to boot up
#include "pisound.h"
#include "gfx.h"
#include "scene.h"

int txtW, txtH;
SDL_Texture * texture;
SDL_Rect dstrect;

static int init_boot_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[current_scene]);
    
    SDL_Surface * surface = IMG_Load ("data/hbb/gfx/hbb_logo.png");
    
    if (surface == NULL)
    {
        fprintf (stderr, "Error loading hbb_logo.png %s\n", SDL_GetError ());
        return 0;
    }

    texture = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    if (texture == NULL)
    {
        fprintf (stderr, "Error creating texture from surface\n");
        return 0;
    }
    
    SDL_QueryTexture (texture, NULL, NULL, &dstrect.w, &dstrect.h);

    dstrect.x = (SCREEN_WIDTH / 2) - (dstrect.w / 2);
    dstrect.y = (SCREEN_HEIGHT / 2) - (dstrect.h / 2);
 
    running_scene = current_scene;
    return 1;
}

int draw_boot (void)
{
    int ret = 0;
    if (current_scene != running_scene)
        ret = init_boot_scene ();

    SDL_RenderCopy (renderer, texture, NULL, &dstrect); 
    return ret;
}

