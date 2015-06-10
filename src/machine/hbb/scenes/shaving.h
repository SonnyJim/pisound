#define GRAVITY 0.5
#define NUM_SHAVINGS 6

SDL_Rect shaving_rect[NUM_SHAVINGS];

struct shaving_t
{
    int xpos;
    int ypos;
    float xvel;
    float yvel;
    int active;
};

struct shaving_t shaving[NUM_SHAVINGS];

void init_shaving_all (int xpos, int ypos);
void draw_shaving (void);
