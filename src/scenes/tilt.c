#include "../pisound.h"
#include "../gfx.h"
#include "../scene.h"

int scale, zoom;
SDL_Texture * texture;
SDL_Rect dstrect;
SDL_Rect zoom_rect;

static void init_tilt_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[current_scene]);

    SDL_Color color = { 255, 0, 0 };
    SDL_Surface * surface = TTF_RenderText_Solid ( fonts[0], scene_names[current_scene], color);
    texture = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    SDL_QueryTexture (texture, NULL, NULL, &dstrect.w, &dstrect.h);
    dstrect.x = (SCREEN_WIDTH / 2) - (dstrect.w / 2);
    dstrect.y = (SCREEN_HEIGHT / 2) - (dstrect.h / 2);

    scale = 0;
    zoom = 1;

    running_scene = current_scene;
}

void draw_tilt (void)
{
    if (current_scene != running_scene)
        init_tilt_scene ();

    if (zoom)
    {
        scale += 25;
        if (scale > 400)
            zoom = 0;
        
    }
    else
    {
        scale -= 25;
        if (scale <= 0)
            zoom = 1;
    }

    zoom_rect.w = dstrect.w + scale;
    zoom_rect.h = dstrect.h + scale;
    zoom_rect.x = (SCREEN_WIDTH / 2) - (zoom_rect.w / 2);
    zoom_rect.y = (SCREEN_HEIGHT / 2) - (zoom_rect.h / 2);

    if (zoom_rect.y + zoom_rect.h > SCREEN_HEIGHT)
        zoom = 0;

    SDL_RenderCopy (renderer, texture, NULL, &zoom_rect); 
}

