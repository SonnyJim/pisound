#include "pisound.h"
#include "gfx.h"
#include "scene.h"
#include "math.h"

int scale, zoom, zoom2;
SDL_Texture * texture;
SDL_Texture * circle_tex;
SDL_Rect dstrect;
SDL_Rect zoom_rect;
SDL_Rect circle_rect;

int length = 80;
float angle = 0.0;
float angle_stepsize = 0.3;

static int init_tilt_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[current_scene]);

    SDL_Color color = { 255, 0, 0 };
    SDL_Surface * surface = TTF_RenderText_Blended ( FON_CHIZ_BOLD_80, scene_names[current_scene], color);
    texture = SDL_CreateTextureFromSurface (renderer, surface);
    if (texture == NULL)
    {
        fprintf (stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_FreeSurface (surface);
    SDL_QueryTexture (texture, NULL, NULL, &dstrect.w, &dstrect.h);
    dstrect.x = (SCREEN_WIDTH / 2) - (dstrect.w / 2);
    dstrect.y = (SCREEN_HEIGHT / 2) - (dstrect.h / 2);

    scale = 0;
    zoom = 1;

    running_scene = current_scene;
    return 0;
}

static int init_draw_circle (void)
{
    SDL_Color color = { 255, 0, 0 };
    SDL_Surface * surface = TTF_RenderText_Blended (FON_ALFPHA_20, "TILT", color);
    circle_tex = SDL_CreateTextureFromSurface (renderer, surface);
    if (circle_tex == NULL)
    {
        fprintf (stderr, "Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_FreeSurface (surface);
    SDL_QueryTexture (circle_tex, NULL, NULL, &circle_rect.w, &circle_rect.h);
    zoom2 = 0;
    return 0;
}

#define CENTRE_X ((SCREEN_WIDTH / 2) - (circle_rect.w / 2))
#define CENTRE_Y ((SCREEN_HEIGHT / 2) - (circle_rect.h / 2))
static void draw_circle (void)
{
    // go through all angles from 0 to 2 * PI radians
    if (angle > 2 * M_PI)
    {
        angle = 0;
        
        if (zoom2 == 0)
            length += 20;
        else
            length -= 20;
    }
    // calculate x, y from a vector with known length and angle
    circle_rect.x = (length * cos (angle)) + CENTRE_X;
    circle_rect.y = (length * sin (angle)) + CENTRE_Y;
    if (CENTRE_Y + length > SCREEN_HEIGHT)
        zoom2 = 1;
    else if (length <= 80)
        zoom2 = 0;
    angle += angle_stepsize;
}

static void draw_tilt_zoom (void)
{
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


}

int draw_tilt (void)
{
    if (current_scene != running_scene)
    {
        init_tilt_scene ();
        init_draw_circle ();
    }

    draw_tilt_zoom ();
    draw_circle ();

    //SDL_RenderCopy (renderer, texture, NULL, &zoom_rect); 
    //SDL_RenderCopy (renderer, circle_tex, NULL, &circle_rect); 
    scene_render_texture (texture, zoom_rect);
    scene_render_texture (circle_tex, circle_rect);
    return 0;
}

