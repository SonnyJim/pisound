#include "pisound.h"
#include "gfx.h"
#include "scene.h"
#include <locale.h>

char *commaprint(unsigned long n)
{
	static int comma = ',';
	static char retbuf[30];
	char *p = &retbuf[sizeof(retbuf)-1];
	int i = 0;

	*p = '\0';

	do {
		if(i%3 == 0 && i != 0)
			*--p = comma;
		*--p = '0' + n % 10;
		n /= 10;
		i++;
	} while(n != 0);

	return p;
}

char *render_score_to_string (long long score)
{
#ifdef linux
    //Set locale for thousand separator in render_score
    setlocale(LC_ALL, "");
    static char newstring[255];
    sprintf (newstring, "%'lld", score);
    return newstring;
#else
    return commaprint(score);
#endif
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

//Setup the transition textures
void init_trans_textures (void)
{
    trans_scene1 = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    trans_scene2 = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (trans_scene1 == NULL || trans_scene2 == NULL)
    {
        fprintf (stderr, "Error setting up trans_textures: %s\n", SDL_GetError());
        running = 0;
    }
}

void show_pisound_logo (void)
{
#define FROG_SIZE 64
    int anim_frame = 0, anim_step = 0;
    load_gfx_resources ();

    SDL_Texture *bootlogo_tex = NULL;

    SDL_Texture *frog_tex = load_image_to_texture ("data/gfx/frog.png");
    SDL_Rect    frog_srcrect;
    SDL_Rect    frog_dstrect;
    bootlogo_tex = load_image_to_texture ("data/gfx/pisound_logo.png");

    frog_dstrect.x = SCREEN_WIDTH - FROG_SIZE;
    frog_dstrect.y = (SCREEN_HEIGHT / 2) + 130;
    frog_dstrect.w = FROG_SIZE;
    frog_dstrect.h = FROG_SIZE;

    while (loading_resources != 0)
    {
        frog_srcrect.x = 0 + (anim_frame * FROG_SIZE);
        frog_srcrect.y = 0;
        frog_srcrect.w = FROG_SIZE;
        frog_srcrect.h = FROG_SIZE;

        SDL_RenderClear (renderer);
        SDL_RenderCopy (renderer, bootlogo_tex, NULL, NULL);
        SDL_RenderCopy (renderer, frog_tex, &frog_srcrect, &frog_dstrect);
        SDL_RenderPresent (renderer);
        SDL_Delay (150);

        if (anim_step < 3)
        {
            anim_frame++;
            frog_dstrect.y -= 20;
        }
        else if (anim_step < 6)
        {
            anim_frame--;
            frog_dstrect.y += 20;
        }
        anim_step++;
        if (anim_step == 7)
        {
            anim_step = 0;
            anim_frame = 0;
        }

        if (anim_frame != 0)
            frog_dstrect.x -= 10;
        if (frog_dstrect.x <= 0)
            frog_dstrect.x = SCREEN_WIDTH - FROG_SIZE;



    }
    //Clean up textures
    SDL_DestroyTexture (bootlogo_tex);
    SDL_DestroyTexture (frog_tex);
}

int init_screen (void)
{
    int i, numdrivers;

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


    if (verbose)
        fprintf (stdout, "Current screen mode: %ix%i\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    if (cfg_fullscreen)
    {
        SCREEN_WIDTH = videomode.w;
        SCREEN_HEIGHT = videomode.h;
        window = SDL_CreateWindow("Pisound", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
    }
    else
    {
        SCREEN_WIDTH = 656;
        SCREEN_HEIGHT = 384;
        window = SDL_CreateWindow("Pisound", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    }

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
        fprintf (stdout, "Using %s\n", SDL_GetCurrentVideoDriver());
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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

    if (renderer == NULL)
    {
        fprintf (stderr, "Error:  Couldn't find a hardware renderer that works, trying a software renderer\n");
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE |  SDL_RENDERER_TARGETTEXTURE);
        if (renderer == NULL)
        {
            fprintf (stderr, "Error creating software renderer: %s\n", SDL_GetError());
            return 1;
        }
    }

    SDL_GetRendererInfo (renderer, &drinfo);
    fprintf (stdout, "Using %s driver\n", drinfo.name);

    return 0;
}

int load_gfx_resources (void)
{
    load_fonts ();
    return 0;
}

static void sdl_poll_event (void)
{
    SDL_Event event;

    while (SDL_PollEvent (&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    running = 0;
                    break;
                }
                break;
/*
            case (SDL_WINDOWEVENT):
                fprintf (stdout, "SDL_WINDOWEVENT %i\n", event.window.event);
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_MINIMIZED:
                        fprintf (stdout, "Window minimised\n");
                        break;
                    case SDL_WINDOWEVENT_MAXIMIZED:
                        fprintf (stdout, "Window maximised\n");
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        fprintf (stdout, "Window restored\n");

                        break;
                }
                break;
*/
        }
    }
}

int gfx_thread (void *ptr)
{
    int font_num;

    if (init_screen () != 0)
    {
        fprintf (stderr, "Error setting up screen\n");
        running = 0;
        return 1;
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
    init_trans_textures ();
    gfx_init_game_vars ();
    requested_scene = BOOT;
    running_scene = INVALID_SCENE;
    SDL_ShowCursor(SDL_DISABLE);

    while (running)
    {
        if (scene_draw ())
        {
            fprintf (stderr, "Error in scene_draw()\n");
            running = 0;
        }

        if (cfg_show_fps)
        {
            fpsFrames++;
            fpsAvg = (fpsFrames / (float) (SDL_GetTicks () - fpsStart)) * 1000;
            if (fpsFrames % 1000 == 0)
                fprintf (stdout, "Average FPS = %f\n", fpsAvg);
        }
        sdl_poll_event ();
    }

    //Close the fonts that were used
    for (font_num = 0; font_num < MAX_FONTS; font_num++)
        TTF_CloseFont(fonts[font_num]);
    TTF_Quit();

    SDL_QuitSubSystem (SDL_INIT_VIDEO);

    if (verbose)
       fprintf (stdout, "GFX thread stopped\n");

    return 0;
}
