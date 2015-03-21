#define MAX_SCENES 6

int current_scene, running_scene, current_subscene, running_subscene;

void scene_draw (void);

void draw_boot (void);
void draw_amode (void);
void draw_game (void);
void draw_gameover (void);
void draw_hsentry (void);
void draw_test (void);
void draw_tilt (void);

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
