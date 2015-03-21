#include "pisound.h"
#include "gfx.h"
#include "scene.h"

int txtW, txtH;
SDL_Texture * texture;
SDL_Rect dstrect;

static void init_boot_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[current_scene]);
    
    SDL_Surface * surface = IMG_Load ("images/hbb_logo.png");
    
    if (surface == NULL)
    {
        fprintf (stderr, "Error loading hbb_logo.png %s\n", SDL_GetError ());
        return;
    }

    texture = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    if (texture == NULL)
    {
        fprintf (stderr, "Error creating texture from surface\n");
        return;
    }
    
    SDL_QueryTexture (texture, NULL, NULL, &dstrect.w, &dstrect.h);

    dstrect.x = (SCREEN_WIDTH / 2) - (dstrect.w / 2);
    dstrect.y = (SCREEN_HEIGHT / 2) - (dstrect.h / 2);
 
    running_scene = current_scene;
}

void draw_boot (void)
{
    if (current_scene != running_scene)
        init_boot_scene ();

    SDL_RenderCopy (renderer, texture, NULL, &dstrect); 

}

