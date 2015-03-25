#include "pisound.h"
#include "scene.h"
#include "scene_trans.h"
#include "gfx.h"

int scene_transition = 0;
int scene_transition_running = 0;

const char *scene_names[] = {
    "Boot",
    "Attract mode",
    "Game",
    "Game Over",
    "High Score Entry",
    "Test mode",
    "TILT"
};

int (*scene_p[MAX_SCENES]) () = {draw_boot, draw_amode, draw_game, draw_gameover, draw_hsentry, draw_test, draw_tilt};

struct subscene_ops game_sign = 
{
    .init = game_sign_init,
    .name = "Sign",
    .background = "images/backgrounds/sign.png"
};

static void scene_render_transition (void)
{
    if (!scene_transition  || !scene_transition_running)
    {
        fprintf (stderr, "scene_draw:  Error, ended up in scene_render_transition when we have no transition to show\n");
        return;
    }
    if (scene_transition_running == 1)
    {
        trans_effects[scene_transition - 1].init ();
        scene_transition_running = 2;
    }
    trans_effects[scene_transition - 1].run ();
}

int scene_draw (void)
{
    int ret = 0;
   

    SDL_RenderClear (renderer);
    //If no transition, just draw the current scene
    if (!scene_transition && !scene_transition_running && running_scene != requested_scene)
    {
        ret = (*scene_p[requested_scene]) ();
        running_scene = requested_scene;
    }
    else if (scene_transition && !scene_transition_running && running_scene != requested_scene)
    {
        //Setup the scene transition
        fprintf (stdout, "trans=%i current=%i running =%i\n", scene_transition, requested_scene, running_scene);
        if (scene_transition > NUM_TRANS_FX)
        {
            fprintf (stderr, "scene_draw:  Requested scene_transition out of range\n");
            running_scene = requested_scene;
            return 0;
        }
        //Copy the running_scene to trans_scene1 texture
        SDL_SetRenderTarget (renderer, trans_scene1);
        SDL_RenderClear (renderer);
        (*scene_p[running_scene]) ();

        //Copy the requested_scene to trans_scene2 texture
        SDL_SetRenderTarget (renderer, trans_scene2);
        SDL_RenderClear (renderer);
        (*scene_p[requested_scene]) ();
       
        //Set the render target back to default
        SDL_SetRenderTarget (renderer, NULL);
        scene_transition_running = 1;
        scene_render_transition ();
    }
    else if (scene_transition_running)
        scene_render_transition ();
    else
        ret = (*scene_p[running_scene]) ();
    

    SDL_RenderPresent (renderer);
    return ret;
}
