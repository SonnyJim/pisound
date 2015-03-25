#include "pisound.h"
#include "gfx.h"
#include "scene.h"
#include "scene_trans.h"

void trans_hor_wipe1_init (void)
{
    if (verbose)
        fprintf (stdout, "scene_draw: hor_wipe1_init\n");
    trans_rect1.x = 0;
    trans_rect1.y = 0;
    trans_rect1.w = 10;
    trans_rect1.h = SCREEN_HEIGHT;
}

void trans_hor_wipe1_run (void)
{
    SDL_RenderClear (renderer);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
    SDL_RenderCopy (renderer, trans_scene2, NULL, &trans_rect1);
    trans_rect1.w += 10;
    if (trans_rect1.w >= SCREEN_WIDTH)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: hor_wipe1 finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}

void trans_hor_wipe2_init (void)
{
    if (verbose)
        fprintf (stdout, "scene_draw: hor_wipe2_init\n");
    trans_rect1.x = 0;
    trans_rect1.y = 0;
    trans_rect1.w = 10;
    trans_rect1.h = SCREEN_HEIGHT;
}

void trans_hor_wipe2_run (void)
{
    SDL_RenderClear (renderer);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
    SDL_RenderCopy (renderer, trans_scene2, &trans_rect1, &trans_rect1);
    trans_rect1.w += 10;
    if (trans_rect1.w >= SCREEN_WIDTH)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: hor_wipe2 finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}

void trans_hor_wipe3_init (void)
{
    if (verbose)
        fprintf (stdout, "scene_draw: hor_wipe3_init\n");
    trans_rect1.x = 0;
    trans_rect1.y = 0;
    trans_rect1.w = 10;
    trans_rect1.h = SCREEN_HEIGHT;

    trans_rect2.x = 0;
    trans_rect2.y = 0;
    trans_rect2.w = SCREEN_WIDTH;
    trans_rect2.h = SCREEN_HEIGHT;
}

void trans_hor_wipe3_run (void)
{
    SDL_RenderClear (renderer);
    trans_rect2.x = trans_rect1.w;
    SDL_RenderCopy (renderer, trans_scene1, NULL, &trans_rect2);
    SDL_RenderCopy (renderer, trans_scene2, &trans_rect1, &trans_rect1);
    trans_rect1.w += 10;
    if (trans_rect1.w >= SCREEN_WIDTH)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: hor_wipe3 finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}


void trans_vert_wipe1_init (void)
{
    if (verbose)
        fprintf (stdout, "scene_draw: vert_wipe1_init\n");
    trans_rect1.x = 0;
    trans_rect1.y = 0;
    trans_rect1.w = SCREEN_WIDTH;
    trans_rect1.h = 10;
}

void trans_vert_wipe1_run (void)
{
    SDL_RenderClear (renderer);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
    SDL_RenderCopy (renderer, trans_scene2, NULL, &trans_rect1);
    trans_rect1.h += 10;
    if (trans_rect1.h >= SCREEN_HEIGHT)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: vert_wipe1 finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}

void trans_vert_wipe2_init (void)
{
    if (verbose)
        fprintf (stdout, "scene_draw: vert_wipe2_init\n");
    trans_rect1.x = 0;
    trans_rect1.y = 0;
    trans_rect1.w = SCREEN_WIDTH;
    trans_rect1.h = 10;
}

void trans_vert_wipe2_run (void)
{
    SDL_RenderClear (renderer);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
    SDL_RenderCopy (renderer, trans_scene2, &trans_rect1, &trans_rect1);
    trans_rect1.h += 10;
    if (trans_rect1.h >= SCREEN_HEIGHT)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: trans_vert_wipe2 finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}

void trans_vert_wipe3_init (void)
{
    if (verbose)
        fprintf (stdout, "scene_draw: vert_wipe3_init\n");
    trans_rect1.x = 0;
    trans_rect1.y = 0;
    trans_rect1.w = SCREEN_WIDTH;
    trans_rect1.h = 10;

    trans_rect2.x = 0;
    trans_rect2.y = 0;
    trans_rect2.w = SCREEN_WIDTH;
    trans_rect2.h = SCREEN_HEIGHT;
}

void trans_vert_wipe3_run (void)
{
    SDL_RenderClear (renderer);
    trans_rect2.y = trans_rect1.h;
    SDL_RenderCopy (renderer, trans_scene1, NULL, &trans_rect2);
    SDL_RenderCopy (renderer, trans_scene2, &trans_rect1, &trans_rect1);
    trans_rect1.h += 10;
    if (trans_rect1.h >= SCREEN_HEIGHT)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: trans_vert_wipe3 finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}


void trans_zoom_init (void)
{
    if (verbose)
        fprintf (stdout, "scene_draw: zoom_init\n");
    trans_rect1.w = 10 * ((float) SCREEN_WIDTH / (float) SCREEN_HEIGHT);
    trans_rect1.h = 10;
    trans_rect1.x = (SCREEN_WIDTH / 2) - (trans_rect1.w / 2);
    trans_rect1.y = (SCREEN_HEIGHT / 2) - (trans_rect1.h / 2);
}

void trans_zoom_run (void)
{
    
    SDL_RenderClear (renderer);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
    SDL_RenderCopy (renderer, trans_scene2, NULL, &trans_rect1);
    
    trans_rect1.w += 5 * ((float) SCREEN_WIDTH / (float) SCREEN_HEIGHT);
    trans_rect1.h += 5;
    trans_rect1.x = (SCREEN_WIDTH / 2) - (trans_rect1.w / 2);
    trans_rect1.y = (SCREEN_HEIGHT / 2) - (trans_rect1.h / 2);
   
    if (trans_rect1.w >= SCREEN_WIDTH || trans_rect1.h >= SCREEN_HEIGHT)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: trans_zoom finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}

void trans_fade_init (void)
{
    int ret;
    if (verbose)
        fprintf (stdout, "scene_draw: trans_fade init\n");
    ret = SDL_SetTextureBlendMode (trans_scene2, SDL_BLENDMODE_BLEND);
    if (ret)
        fprintf (stderr, "trans_fade: %s\n", SDL_GetError());
    trans_alpha = 0;

    trans_rect1.x = 0;
    trans_rect1.y = 0;
    trans_rect1.w = SCREEN_WIDTH;
    trans_rect1.h = SCREEN_HEIGHT;
}

void trans_fade_run (void)
{
    SDL_RenderClear (renderer);
    SDL_SetTextureAlphaMod (trans_scene2, trans_alpha);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
    SDL_RenderCopy (renderer, trans_scene2, NULL, &trans_rect1);
    if (++trans_alpha > 255)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: trans_fade finished\n");
        SDL_SetTextureBlendMode (trans_scene2, SDL_BLENDMODE_NONE);
        scene_transition = 0;
        scene_transition_running = 0;
    }
}

#define NUM_BLINDS 8
#define BLIND_WIDTH (SCREEN_WIDTH / NUM_BLINDS)
void trans_blinds_init (void)
{
    trans_rect1.y = 0;
    trans_rect1.w = 1;
    trans_rect1.h = SCREEN_HEIGHT;

    trans_rect2.y = 0;
    trans_rect2.w = 1;
    trans_rect2.h = SCREEN_HEIGHT;

}

void trans_blinds_run (void)
{
    int i;
    SDL_RenderClear (renderer);
    SDL_RenderCopy (renderer, trans_scene1, NULL, NULL);
   
    trans_rect1.x = 0;
    trans_rect2.x = 0;
    for (i = 0; i < NUM_BLINDS; i++)
    {
        SDL_RenderCopy (renderer, trans_scene2, &trans_rect2, &trans_rect1);
        trans_rect1.x += BLIND_WIDTH;
        trans_rect2.x = trans_rect1.x;
    }
    
    trans_rect1.w += 1;
    trans_rect2.w = trans_rect1.w;

    if (trans_rect1.w >= BLIND_WIDTH)
    {
        if (verbose)
            fprintf (stdout, "scene_draw: trans_blinds finished\n");
        scene_transition = 0;
        scene_transition_running = 0;
    }
}

struct trans_ops trans_effects[] = { 
    { .init = trans_hor_wipe1_init, .run = trans_hor_wipe1_run},
    { .init = trans_hor_wipe2_init, .run = trans_hor_wipe2_run},
    { .init = trans_hor_wipe3_init, .run = trans_hor_wipe3_run},
    { .init = trans_vert_wipe1_init, .run = trans_vert_wipe1_run},
    { .init = trans_vert_wipe2_init, .run = trans_vert_wipe2_run},
    { .init = trans_vert_wipe3_init, .run = trans_vert_wipe3_run},
    { .init = trans_zoom_init, .run = trans_zoom_run},
    { .init = trans_blinds_init, .run = trans_blinds_run},
    { .init = trans_fade_init, .run = trans_fade_run}
};


