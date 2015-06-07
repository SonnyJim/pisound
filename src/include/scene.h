#define MAX_SCENES 7

int running_scene, requested_scene, current_subscene, running_subscene;

int scene_draw (void);

int draw_boot (void);
int draw_amode (void);
int draw_game (void);
int draw_gameover (void);
int draw_hsentry (void);
int draw_test (void);
int draw_tilt (void);

extern const char *scene_names[];
enum scenes {
    BOOT,
    AMODE,
    GAME,
    GAMEOVER,
    HSENTRY,
    TEST,
    TILT,
    INVALID_SCENE
};


int scene_transition;
int scene_transition_running;

enum game_subscenes {
    GAME_SIGN,
    GAME_MINE,
    GAME_OUTHOUSE,
    GAME_RIVER
};

struct subscene_ops {
    void (*init) (void);
    char *name;
    char *background;
};

void game_sign_init (void);

