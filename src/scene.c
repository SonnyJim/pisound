#include "pisound.h"
#include "scene.h"
#include "gfx.h"

int scene_transition = 0;

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

void scene_update_current (void)
{
    if (scene_transition == 0)
        running_scene = current_scene;
            
}

void scene_render_texture (SDL_Texture *texture, SDL_Rect dstrect)
{
    if (scene_transition == 0)
        SDL_RenderCopy (renderer, texture, NULL, &dstrect);

}

int scene_draw (void)
{
    int ret;
    ret = (*scene_p[current_scene]) ();
    /*
    switch (current_scene)
    {
            case BOOT:
                ret = draw_boot ();
                break;
            case AMODE:
                ret = draw_amode ();
                break;
            case GAME:
                ret = draw_game ();
                break;
            case GAMEOVER:
                ret = draw_gameover ();
                break;
            case HSENTRY:
                ret = draw_hsentry ();
                break;
            case TEST:
                ret = draw_test ();
                break;
            case TILT:
                ret = draw_tilt ();
                break;
            default:
                ret = draw_amode ();
                break;
    }
    */
    return ret;
}
