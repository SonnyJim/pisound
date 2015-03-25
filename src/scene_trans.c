#include "pisound.h"
#include "gfx.h"
#include "scene.h"
#include "scene_trans.h"

struct trans_ops trans_effects[] = { 
    { .init = trans_hor_wipe_init, .run = trans_hor_wipe_run},
    { .init = trans_ver_wipe_init, .run = trans_ver_wipe_run}
};

void trans_ver_wipe_init (void)
{
    fprintf (stdout, "scene_draw: ver_wipe_init\n");
    transrect.x = 0;
    transrect.y = 0;
    transrect.w = SCREEN_WIDTH;
    transrect.h = 10;
}

void trans_ver_wipe_run (void)
{
    SDL_RenderClear (renderer);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
    SDL_RenderCopy (renderer, trans_scene2, NULL, &transrect);
    transrect.h += 1;
    if (transrect.h >= SCREEN_HEIGHT)
    {
        fprintf (stdout, "scene_draw: ver_wipe transition finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}

void trans_hor_wipe_init (void)
{
    fprintf (stdout, "scene_draw: hor_wipe_init\n");
    transrect.x = 0;
    transrect.y = 0;
    transrect.w = 10;
    transrect.h = SCREEN_HEIGHT;
}

void trans_hor_wipe_run (void)
{
    SDL_RenderClear (renderer);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
    SDL_RenderCopy (renderer, trans_scene2, NULL, &transrect);
    transrect.w += 1;
    if (transrect.w >= SCREEN_WIDTH)
    {
        fprintf (stdout, "scene_draw: hor_wipe transition finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}
