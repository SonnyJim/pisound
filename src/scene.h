#define MAX_SCENES 6

int current_scene;

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
    TILT
};


