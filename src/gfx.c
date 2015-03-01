#include "pisound.h"
#include "gfx.h"
#include "scene.h"

//Puts thousand separators into string
char *render_score_to_string (long long score)
{
    static char newstring[255];
    sprintf (newstring, "%'lld", score);
    return newstring;
}

SDL_Texture* load_image_to_texture (char *filename)
{
    SDL_Texture* dst_texture = NULL;
    SDL_Surface* src_surface = NULL;

    if (verbose)
        fprintf (stdout, "Loading %s\n", filename);
    src_surface = IMG_Load(filename);
    
    if (src_surface == NULL)
        fprintf (stderr, "Error loading sprite: %s\n", IMG_GetError());
    else
    {
        dst_texture = SDL_CreateTextureFromSurface (renderer, src_surface);
        if (dst_texture == NULL)
            fprintf (stderr, "Error loading image to texture: %s %s\n", filename, SDL_GetError());
        SDL_FreeSurface (src_surface);
    }
    return dst_texture;
}


void show_pisound_logo (void)
{
    int textWidth, textHeight, xpos, ypos, i=0;
    char loading_text[4][12] = { "Loading |", "Loading /", "Loading -", "Loading \\" };
    load_gfx_resources ();

    SDL_Texture *logo_tex = NULL;
    SDL_Rect    logo_rect;

    SDL_Texture *loading_tex = NULL;
    SDL_Rect    loading_rect;
    SDL_Surface *loading_srf;

    SDL_Texture *loading_txt_tex = NULL;
    SDL_Rect    loading_txt_rect;
    SDL_Surface *loading_txt_srf;


    SDL_Texture *frog_tex = load_image_to_texture ("images/frog.png");
    SDL_Rect    frog_srcrect;
    SDL_Rect    frog_dstrect;
    loading_tex = load_image_to_texture ("images/pisound_logo.png");
    
    logo_tex = load_image_to_texture ("images/hbb_logo.png");
   
    SDL_QueryTexture (logo_tex, NULL, NULL, &logo_rect.w, &logo_rect.h);
    SDL_QueryTexture (loading_tex, NULL, NULL, &loading_rect.w, &loading_rect.h);
    
    frog_dstrect.x = SCREEN_WIDTH / 2;
    frog_dstrect.y = SCREEN_HEIGHT - 64;
    frog_dstrect.w = 64;
    frog_dstrect.h = 64;

    while (loading_resources != 0)
    {
        TTF_SizeText (fonts[0], loading_text[i], &textWidth, &textHeight);
        xpos = (SCREEN_WIDTH - textWidth) / 2;
        ypos = SCREEN_HEIGHT - textHeight - 70;
        loading_txt_rect.x = xpos;
        loading_txt_rect.y = ypos;
        loading_txt_rect.w = textWidth;
        loading_txt_rect.h = textHeight;
 
        textColor = (SDL_Color) { 0, 0, 255};
        loading_txt_srf = TTF_RenderText_Solid( fonts[0], loading_text[i], textColor );
        loading_txt_tex = SDL_CreateTextureFromSurface(renderer, loading_txt_srf);
        SDL_FreeSurface (loading_srf);
        loading_srf = NULL;

        frog_srcrect.x = 0 + (i * 64);
        frog_srcrect.y = 0;
        frog_srcrect.w = 64;
        frog_srcrect.h = 64;

        SDL_RenderClear (renderer);    
        SDL_RenderCopy (renderer, loading_tex, NULL, NULL);
        SDL_RenderCopy (renderer, loading_txt_tex, NULL, &loading_txt_rect);
        SDL_RenderCopy (renderer, frog_tex, &frog_srcrect, &frog_dstrect);
        SDL_RenderPresent (renderer);
        SDL_Delay (500);

        if (i < 3)
            i++;
        else
            i = 0;

    }
    //Clean up textures
    SDL_DestroyTexture (logo_tex);
    SDL_DestroyTexture (loading_tex);
    SDL_DestroyTexture (loading_txt_tex);
    SDL_DestroyTexture (frog_tex);
}

int init_screen (void)
{
    int i, numdrivers;
    SDL_RendererInfo drinfo; 

    if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
        fprintf (stderr, "\nError initialising SDL video: %s\n", SDL_GetError ());
        return 1;
    }
    
    if (SDL_GetCurrentDisplayMode (0, &videomode) != 0)
    {
        fprintf (stderr, "Error getting current display mode: %s\n", SDL_GetError ());
        return 1;
    }
    
    SCREEN_WIDTH = videomode.w;
    SCREEN_HEIGHT = videomode.h;

    if (verbose)
        fprintf (stdout, "Current screen mode: %ix%i\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    window = SDL_CreateWindow("Pisound", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
    if (window == NULL)
    {
        fprintf (stderr, "Error creating window: %s\n", SDL_GetError());
        return 1;
    }

    numdrivers = SDL_GetNumVideoDrivers ();

    if (verbose)
    {
        fprintf (stdout, "Found %i video drivers\n", numdrivers);
        for (i = 0; i < numdrivers; i++)
        {
            fprintf (stdout, "Driver %i: %s\n", i,  SDL_GetVideoDriver(i));
        }
    }

    numdrivers = SDL_GetNumRenderDrivers ();

    if (verbose)
    {
        fprintf (stdout, "Found %i render drivers\n", numdrivers);
        for (i = 0; i < numdrivers; i++)
        {
            SDL_GetRenderDriverInfo (i, &drinfo);
            fprintf (stdout, "Driver %i: %s\n", i, drinfo.name);
        }
    }
    
    //Try all the different SDL2 drivers until we find one that works ;-)
    for (i = 0; i < numdrivers; i++)
    {
        if (verbose)
            fprintf (stdout, "Trying render driver %i\n", i);
        renderer = SDL_CreateRenderer(window, i, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        
        if (renderer == NULL)
            fprintf (stderr, "Error creating renderer: %s\n", SDL_GetError());
        else
            break;
    }
    
    if (renderer == NULL)
    {
        fprintf (stderr, "Error:  Couldn't find a hardware renderer that works, trying a software renderer\n");
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer == NULL)
        {
            fprintf (stderr, "Error creating renderer: %s\n", SDL_GetError());
            return 1;
        }
    }
    
    SDL_GetRenderDriverInfo (i, &drinfo);
    fprintf (stdout, "Using %s driver\n", drinfo.name);
    return 0;
}

int load_gfx_resources (void)
{
    //load_gfx ();
    load_fonts ();
    //init_gfx_vars ();
    return 0;
}

void* gfx_thread (void *ptr)
{
    int font_num;

    //Set locale for thousand separator in render_score
    setlocale(LC_NUMERIC, "");

    if (init_screen () != 0)
    {
        fprintf (stderr, "Error setting up screen\n");
        return NULL;
    }

    //show logo also loads resources
    if (cfg_show_logo)
        show_pisound_logo ();
    else
        load_gfx_resources ();

    fprintf (stdout, "Finished loading resources\n");

   // if (play_video () != 0)
   //     fprintf (stderr, "Something broke whilst playing a video\n");
    
    if (cfg_show_fps)
    {
        fpsFrames = 0;
        fpsStart = SDL_GetTicks ();
    }

    gfx_init_game_vars ();
    while (running)
    {
        SDL_RenderClear(renderer);
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

       SDL_RenderPresent (renderer);
        
        if (cfg_show_fps)
        {
            fpsFrames++;
            fpsAvg = (fpsFrames / (float) (SDL_GetTicks () - fpsStart)) * 1000;
            if (fpsFrames % 1000 == 0)
                fprintf (stdout, "Average FPS = %f\n", fpsAvg);
        }
    }
   
    //Close the fonts that were used 
    for (font_num = 0; font_num < MAX_FONTS; font_num++)
        TTF_CloseFont(fonts[font_num]);
    TTF_Quit();

    SDL_QuitSubSystem (SDL_INIT_VIDEO);
    
    if (verbose)
       fprintf (stdout, "GFX thread stopped\n");

    return NULL;
}
