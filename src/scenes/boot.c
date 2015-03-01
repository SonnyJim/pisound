#include "../pisound.h"
#include "../gfx.h"
#include "../scene.h"

int txtW, txtH;
SDL_Texture * texture;
SDL_Rect dstrect;

static void init_boot_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[current_scene]);

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid ( fonts[0], scene_names[current_scene], color);
    texture = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    SDL_QueryTexture (texture, NULL, NULL, &txtW, &txtH);
    dstrect.x = SCREEN_WIDTH / 2;
    dstrect.y = SCREEN_HEIGHT / 2;
    dstrect.w = txtW;
    dstrect.h = txtH;
 
    running_scene = current_scene;
}

void draw_boot (void)
{
    if (current_scene != running_scene)
        init_boot_scene ();

    SDL_RenderCopy (renderer, texture, NULL, &dstrect); 

}

