//This scene is shown whilst we are waiting for the machine to boot up
#include "pisound.h"
#include "gfx.h"
#include "scene.h"

int txtW, txtH;
SDL_Texture * boot_tex;
SDL_Rect dstrect;

static int init_boot_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[requested_scene]);
    
    SDL_Surface * surface = IMG_Load ("data/hbb/gfx/hbb_logo.png");
    
    if (surface == NULL)
    {
        fprintf (stderr, "Error loading hbb_logo.png %s\n", SDL_GetError ());
        return 1;
    }

    boot_tex = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    if (boot_tex == NULL)
    {
        fprintf (stderr, "Error creating boot_tex from surface\n");
        return 1;
    }
    
    SDL_QueryTexture (boot_tex, NULL, NULL, &dstrect.w, &dstrect.h);

    dstrect.x = (SCREEN_WIDTH / 2) - (dstrect.w / 2);
    dstrect.y = (SCREEN_HEIGHT / 2) - (dstrect.h / 2);
 
    return 0;
}

int draw_boot (void)
{
    int ret = 0;
    if (requested_scene != running_scene)
        ret = init_boot_scene ();

    SDL_RenderCopy (renderer, boot_tex, NULL, &dstrect); 
    return ret;
}

