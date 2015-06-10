#include "pisound.h"
#include "gfx.h"
#include "shaving.h"
SDL_Texture *shaving_tex = NULL;
SDL_Surface *shaving_srf = NULL;

static void init_shaving (int num, int xpos, int ypos)
{
    shaving[num].xpos = xpos;
    shaving[num].ypos = ypos;
    shaving[num].xvel = rand() % 20 - 10;
    shaving[num].yvel = rand() % 20 - 10;
    shaving[num].active = rand ()%2;

    shaving_rect[num].w = 15;
    shaving_rect[num].h = 15;
}

static int init_shaving_gfx (void)
{
    shaving_srf = IMG_Load ("data/hbb/gfx/shaving.png");
    if (shaving_srf == NULL)
    {
        fprintf (stderr, "Error loading shaving sprite\n");
        return 1;
    }

    shaving_tex = SDL_CreateTextureFromSurface(renderer, shaving_srf);
    SDL_FreeSurface(shaving_srf);
    if (shaving_tex == NULL)
    {
        fprintf (stderr, "Error creating shaving texture\n");
        return 1;
    }
    return 0;
}

void init_shaving_all (int xpos, int ypos)
{
    int i;
    if (shaving_tex == NULL)
        init_shaving_gfx ();
    for (i = 0; i < NUM_SHAVINGS; i++)
    {
        init_shaving (i, xpos, ypos);
    }
}

static void update_shaving (int num)
{
    shaving[num].yvel += GRAVITY;
    shaving[num].xpos += shaving[num].xvel;
    shaving[num].ypos += shaving[num].yvel;

    shaving_rect[num].x = shaving[num].xpos;
    shaving_rect[num].y = shaving[num].ypos;

    if (shaving[num].xpos <= 0 || shaving[num].xpos >= SCREEN_WIDTH|| shaving[num].ypos >= SCREEN_HEIGHT)
        shaving[num].active = 0;
}

void draw_shaving (void)
{
    int i;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

    for (i = 0; i < NUM_SHAVINGS; i++)
    {
        if (shaving[i].active)
        {
            update_shaving (i);
            SDL_RenderCopy (renderer, shaving_tex, NULL, &shaving_rect[i]);
            //SDL_RenderDrawRect(renderer, &shaving_rect[i]);
        }
    }
}
