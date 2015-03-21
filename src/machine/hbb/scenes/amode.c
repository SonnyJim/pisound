#include "pisound.h"
#include "gfx.h"
#include "scene.h"

SDL_Texture * texture;
SDL_Rect dstrect;

SDL_Texture *scroller;
SDL_Rect scrlrect;

static char scroller_text[] = "This is a really long piece of text to demonstrate what scrolling text looks like.  How awesome!\
                                I think I'm about to mess my pants it's that cool.  Really gives you that 1980's demo vibe, doesn't it?\
                                 Oh well, should stop round about now, I'm rambling and there's work to be done.  Toodlepips!";

static void init_amode_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[current_scene]);

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid ( FON_CHIZ_BOLD_80, scene_names[current_scene], color);
    texture = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    SDL_QueryTexture (texture, NULL, NULL, &dstrect.w, &dstrect.h);
    dstrect.x = (SCREEN_WIDTH / 2) - (dstrect.w / 2);
    dstrect.y = (SCREEN_HEIGHT / 2) - (dstrect.h / 2);
 
    running_scene = current_scene;
}

static void create_scroller_texture (void)
{
    SDL_Color color = { 255, 0, 0 };
    SDL_Surface * surface = TTF_RenderText_Solid ( FON_ALFPHA_36, scroller_text, color);
    scroller = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);
    SDL_QueryTexture (texture, NULL, NULL, &scrlrect.w, &scrlrect.h);
    scrlrect.y = SCREEN_HEIGHT - scrlrect.h;
    scrlrect.x = SCREEN_WIDTH;

}

static void draw_scroller (void)
{
    SDL_RenderCopy (renderer, scroller, NULL, &scrlrect);
    scrlrect.x--;
}

int draw_amode (void)
{
    if (current_scene != running_scene)
    {
        init_amode_scene ();
        create_scroller_texture ();
    }
    SDL_RenderCopy (renderer, texture, NULL, &dstrect); 
    draw_scroller ();

    return 0;
}

