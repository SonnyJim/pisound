#define NUM_TRANS_FX 10

SDL_Rect trans_rect1;
SDL_Rect trans_rect2;
int trans_alpha;
int trans_var1;

struct trans_ops 
{
    void (*init) (void);
    void (*run) (void);
};

enum trans_fx {
    TRANS_HOR_WIPE,
    TRANS_VER_WIPE
};

struct trans_ops trans_effects[NUM_TRANS_FX];
