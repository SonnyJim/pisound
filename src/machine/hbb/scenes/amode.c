#include "pisound.h"
#include "gfx.h"
#include "scene.h"

SDL_Texture * texture;
SDL_Rect dstrect;

SDL_Texture *scroller;
SDL_Rect scrlrect;
int line = 0;

const char *scroller_text[4] = {"This is a really long piece of text to demonstrate what scrolling text looks like.",  
                               "How awesome! I think I'm about to mess my pants it's that cool.",  
                               "Really gives you that 1980's demo vibe, doesn't it Oh well, should stop round about now",
                               "I'm rambling and there's work to be done.  Toodlepips!"};

static void init_amode_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[current_scene]);

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Blended ( FON_CHIZ_BOLD_80, scene_names[current_scene], color);
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
    SDL_Surface * surface = TTF_RenderText_Solid ( FON_ALFPHA_20, scroller_text[line], color);
    scroller = SDL_CreateTextureFromSurface (renderer, surface);
    if (scroller == NULL)
        fprintf (stdout, "Scroller error %s\n", scroller_text[line]);
    SDL_FreeSurface (surface);
    SDL_QueryTexture (scroller, NULL, NULL, &scrlrect.w, &scrlrect.h);
    scrlrect.y = SCREEN_HEIGHT - (scrlrect.h * 2);
    scrlrect.x = SCREEN_WIDTH;

}

static void draw_scroller (void)
{
    SDL_RenderCopy (renderer, scroller, NULL, &scrlrect);
    scrlrect.x -= 2;
    //scrlrect.x goes minus, innit, so when it hits 0, wrap to the next line
    if (scrlrect.w + scrlrect.x <= 0)
    {
        line++;
        if (line > 3)
            line = 0;
        create_scroller_texture ();
    }
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

