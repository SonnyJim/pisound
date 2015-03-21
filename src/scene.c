#include "scene.h"

const char *scene_names[] = {
    "Boot",
    "Attract mode",
    "Game",
    "Game Over",
    "High Score Entry",
    "Test mode",
    "TILT"
};

struct subscene_ops game_sign = 
{
    .init = game_sign_init,
    .name = "Sign",
    .background = "images/backgrounds/sign.png"
};

int scene_draw (void)
{
    int ret;
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
    return ret;
}
