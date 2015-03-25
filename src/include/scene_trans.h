#define NUM_TRANS_FX 2

SDL_Rect transrect;
struct trans_ops 
{
    void (*init) (void);
    void (*run) (void);
};

void trans_hor_wipe_init (void);
void trans_hor_wipe_run (void);

void trans_ver_wipe_init (void);
void trans_ver_wipe_run (void);

enum trans_fx {
    TRANS_HOR_WIPE,
    TRANS_VER_WIPE
};

struct trans_ops trans_effects[NUM_TRANS_FX];
