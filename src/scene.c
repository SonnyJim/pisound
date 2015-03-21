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

void scene_draw (void)
{
    switch (current_scene)
    {
            case BOOT:
                draw_boot ();
                break;
            case AMODE:
                draw_amode ();
                break;
            case GAME:
                draw_game ();
                break;
            case GAMEOVER:
                draw_gameover ();
                break;
            case HSENTRY:
                draw_hsentry ();
                break;
            case TEST:
                draw_test ();
                break;
            case TILT:
                draw_tilt ();
                break;
            default:
                draw_amode ();
                break;
        }
}
