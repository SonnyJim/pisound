#include "pisound.h"
#include "gfx.h"

//#include "bcm_host.h"

void free_gfx (void) 
{ 
    int i;

    if(renderer) 
        SDL_DestroyRenderer (renderer);
    if(window) 
        SDL_DestroyWindow (window);
    
    //Close the fonts that were used 
    for (i = 0; i < MAX_FONTS; i++)
        TTF_CloseFont(fonts[i]);
    TTF_Quit();
}

//Puts thousand separators into string
char *render_score_to_string (long long score)
{
    static char newstring[255];
    sprintf (newstring, "%'lld", score);
    return newstring;
}

static void render_score (void)
{
    int textWidth, textHeight, xpos, ypos;
    char *score_string;
    SDL_Color textColor;
  
    //Only update the score if it has changed
    if (score != old_score)// || 
           // (score == 0 && old_score == 0))
    {
        score_string = render_score_to_string (score);
        TTF_SizeText (fonts[0], score_string, &textWidth, &textHeight);
        xpos = (SCREEN_WIDTH - textWidth) / 2;
        ypos = 120;
        score_rect.x = xpos;
        score_rect.y = ypos;
        score_rect.w = textWidth;
        score_rect.h = textHeight;
        
        //Inner text
        textColor = (SDL_Color) { 82, 42, 0};
        score_srf = TTF_RenderText_Solid( fonts[0], score_string, textColor );
        score_tex = SDL_CreateTextureFromSurface(renderer, score_srf);
        SDL_FreeSurface (score_srf);
        score_srf = NULL;
        SDL_SetTextureBlendMode (score_tex, SDL_BLENDMODE_BLEND);    
        SDL_SetTextureAlphaMod (score_tex, 150);
        
        //Outline
        textColor = (SDL_Color) { 42, 22, 0};
        score_srf = TTF_RenderText_Solid( fonts[1], score_string, textColor );
        score_outline_tex = SDL_CreateTextureFromSurface(renderer, score_srf);
        SDL_FreeSurface (score_srf);
        score_srf = NULL;
        SDL_SetTextureBlendMode (score_outline_tex, SDL_BLENDMODE_BLEND);    
        SDL_SetTextureAlphaMod (score_outline_tex, 200);
        
        old_score = score;
    }
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
    SDL_RenderCopy (renderer, score_outline_tex, NULL, &score_rect);
   
    //Render player number
    if (player_num != old_player_num)
    {
        sprintf (score_string, "Player %d", player_num);
        TTF_SizeText (fonts[2], score_string, &textWidth, &textHeight);

        xpos = (SCREEN_WIDTH - textWidth) / 2;
        ypos = 85;
        
        player_rect.x = xpos;
        player_rect.y = ypos;
        player_rect.w = textWidth;
        player_rect.h = textHeight;
        
        textColor = (SDL_Color) { 0, 0, 0};
        score_srf = TTF_RenderText_Solid( fonts[2], score_string, textColor );
        player_tex = SDL_CreateTextureFromSurface(renderer, score_srf);
        SDL_FreeSurface (score_srf);
        SDL_SetTextureBlendMode (score_tex, SDL_BLENDMODE_BLEND);    
        SDL_SetTextureAlphaMod (score_tex, 200);
        old_player_num = player_num;
    }
    SDL_RenderCopy (renderer, player_tex, NULL, &player_rect);
}

SDL_Texture* load_image_to_texture (char *filename)
{
    SDL_Texture* dst_texture = NULL;

    if (verbose)
        fprintf (stdout, "Loading %s\n", filename);
    sprite_srf = IMG_Load(filename);
    
    if (sprite_srf == NULL)
        fprintf (stderr, "Error loading sprite: %s\n", IMG_GetError());
    else
    {
        dst_texture = SDL_CreateTextureFromSurface (renderer, sprite_srf);
        if (dst_texture == NULL)
            fprintf (stderr, "Error loading image to texture: %s %s\n", filename, SDL_GetError());
        SDL_FreeSurface (sprite_srf);
    }
    return dst_texture;
}

void load_gfx (void)
{
    sprite_srf = IMG_Load("images/shaving.png");
    if (sprite_srf == NULL)
        fprintf (stderr, "Error loading sprite: %s\n", IMG_GetError());
    sprite_tex = SDL_CreateTextureFromSurface (renderer, sprite_srf);
    SDL_FreeSurface (sprite_srf);
    sprite_srf = NULL;
    SDL_QueryTexture (sprite_tex, NULL, NULL, &sprite_rect.w, &sprite_rect.h);

    sprite_srf = IMG_Load("images/hbb_logo.png");
    if (sprite_srf == NULL)
        fprintf (stderr, "Error loading sprite: %s\n", IMG_GetError());
    logo_tex = SDL_CreateTextureFromSurface (renderer, sprite_srf);
    SDL_FreeSurface (sprite_srf);
    sprite_srf = NULL;
    SDL_QueryTexture (logo_tex, NULL, NULL, &logo_rect.w, &logo_rect.h);

    billybob_tex = load_image_to_texture (IMG_BILLYBOB);
    SDL_QueryTexture (billybob_tex, NULL, NULL, &billybob_rect.w, &billybob_rect.h);
    
    bubba_tex = load_image_to_texture (IMG_BUBBA);
    SDL_QueryTexture (bubba_tex, NULL, NULL, &bubba_rect.w, &bubba_rect.h);

    grandpa_tex = load_image_to_texture (IMG_GRANDPA);
    SDL_QueryTexture (grandpa_tex, NULL, NULL, &grandpa_rect.w, &grandpa_rect.h);

    peggysue_tex = load_image_to_texture (IMG_PEGGYSUE);
    SDL_QueryTexture (peggysue_tex, NULL, NULL, &peggysue_rect.w, &peggysue_rect.h);
}

void render_sprite (void)
{
    if (x_up)
        sprite_x -= 2;
    else
        sprite_x += 2;
    
    if (sprite_x >= SCREEN_WIDTH - sprite_rect.w)
        x_up = 1;
    else if (sprite_x <= 0)
        x_up = 0;
    sprite_y = SCREEN_HEIGHT /2 + 100;

    sprite_rect.x = sprite_x;
    sprite_rect.y = sprite_y;
    SDL_RenderCopy (renderer, sprite_tex, NULL, &sprite_rect);
}

double rotate_angle;

void render_logo (void)
{
    logo_rect.x = (SCREEN_WIDTH - logo_rect.w) / 2;
    logo_rect.y = ((SCREEN_HEIGHT - logo_rect.h) / 2) + 40;

    billybob_rect.x = 0;
    billybob_rect.y = 0;
    
    bubba_rect.x = SCREEN_WIDTH - bubba_rect.w;
    bubba_rect.y = 0;

    grandpa_rect.x = SCREEN_WIDTH - grandpa_rect.w;
    grandpa_rect.y = SCREEN_HEIGHT -  grandpa_rect.h;

    peggysue_rect.x = 0;
    peggysue_rect.y = SCREEN_HEIGHT -  peggysue_rect.h;

    if (++rotate_angle >= 360)
        rotate_angle = 0;
    SDL_RenderCopyEx (renderer, logo_tex, NULL, &logo_rect, rotate_angle, NULL, 0);
    SDL_RenderCopyEx (renderer, billybob_tex, NULL, &billybob_rect, rotate_angle, NULL, 0);
    SDL_RenderCopyEx (renderer, bubba_tex, NULL, &bubba_rect, 360 - rotate_angle, NULL, 0);
    SDL_RenderCopyEx (renderer, grandpa_tex, NULL, &grandpa_rect, rotate_angle, NULL, 0);
    SDL_RenderCopyEx (renderer, peggysue_tex, NULL, &peggysue_rect, 360 - rotate_angle, NULL, 0);
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
        renderer = SDL_CreateRenderer(window, 2, SDL_RENDERER_SOFTWARE);
        if (renderer == NULL)
            return 1;
    }
    
    SDL_GetRenderDriverInfo (i, &drinfo);
    fprintf (stdout, "Using %s\n", drinfo.name);
    return 0;
}

void init_gfx_vars (void)
{
    old_player_num = 0;
    player_num = 1;
    old_score = 1;

    sprite_x = 0;
    sprite_y = 0;
    x_up = 0;
    y_up = 0;
    rotate_angle = 0;
}

void* gfx_thread (void *ptr)
{


    //Set locale for thousand separator in render_score
    setlocale(LC_NUMERIC, "");

    if (init_screen () != 0)
    {
        fprintf (stderr, "Error setting up screen\n");
        pthread_exit (NULL);
    }

   //Load all the resources we want to use
    load_gfx ();
    load_fonts ();
    init_gfx_vars ();

    background_srf = SDL_LoadBMP("images/background.bmp");
    if (background_srf == NULL)
    {
        fprintf (stderr, "Error loading background: %s\n", SDL_GetError());
        pthread_exit (NULL);
    }
    
    background_tex = SDL_CreateTextureFromSurface(renderer, background_srf);
    if (background_tex == NULL)
    {
        fprintf (stderr, "Error creating background texture: %s\n", SDL_GetError());
        pthread_exit (NULL);
    }
    
    SDL_FreeSurface (background_srf);
    background_srf = NULL;
  
    fpsFrames = 0;
    fpsStart = SDL_GetTicks ();
    while (running)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy (renderer, background_tex, NULL, NULL);
        render_score ();
        render_logo ();
        render_sprite ();
        SDL_RenderPresent (renderer);
        
        if (verbose)
        {
            fpsFrames++;
            fpsAvg = (fpsFrames / (float) (SDL_GetTicks () - fpsStart)) * 1000;
            if (fpsFrames % 1000 == 0)
                fprintf (stdout, "Average FPS = %f\n", fpsAvg);
        }
    }
    free_gfx ();
    pthread_exit (NULL);
    return NULL;
}
