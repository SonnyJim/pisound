#include "pisound.h"
#include "gfx.h"

//#include "bcm_host.h"

void free_gfx (void) 
{ 
    int i;
    //Free the surfaces 
   // SDL_FreeSurface( background_srf ); 
   // SDL_FreeSurface( score_srf ); 

    if(renderer) 
        SDL_DestroyRenderer (renderer);
    if(window) 
        SDL_DestroyWindow (window);
    
    //Close the fonts that were used 
    for (i = 0; i < MAX_FONTS; i++)
        TTF_CloseFont(fonts[i]);
    TTF_Quit();
}

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
    SDL_SetTextureBlendMode (score_tex, SDL_BLENDMODE_BLEND);    
    SDL_SetTextureAlphaMod (score_tex, 150);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);
    
    //Outline
    textColor = (SDL_Color) { 42, 22, 0};
    score_srf = TTF_RenderText_Solid( fonts[1], score_string, textColor );
    score_tex = SDL_CreateTextureFromSurface(renderer, score_srf);
    SDL_FreeSurface (score_srf);
    SDL_SetTextureBlendMode (score_tex, SDL_BLENDMODE_BLEND);    
    SDL_SetTextureAlphaMod (score_tex, 200);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);

    //Render player number
    sprintf (score_string, "Player %d", player_num);
    TTF_SizeText (fonts[2], score_string, &textWidth, &textHeight);

    xpos = (SCREEN_WIDTH - textWidth) / 2;
    ypos = 80;

    score_rect.x = xpos;
    score_rect.y = ypos;
    score_rect.w = textWidth;
    score_rect.h = textHeight;

    textColor = (SDL_Color) { 0, 0, 0};
    score_srf = TTF_RenderText_Solid( fonts[2], score_string, textColor );
    score_tex = SDL_CreateTextureFromSurface(renderer, score_srf);
    SDL_FreeSurface (score_srf);
    SDL_SetTextureBlendMode (score_tex, SDL_BLENDMODE_BLEND);    
    SDL_SetTextureAlphaMod (score_tex, 200);
    SDL_RenderCopy (renderer, score_tex, NULL, &score_rect);


   
}

void* gfx_thread (void *ptr)
{
    //Needed on the Pi to initialise the GPU
    //bcm_host_init ();
    load_fonts ();
    
    player_num = 1;

    //Set locale for thousand separator in render_score
    setlocale(LC_NUMERIC, "");

    if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
        fprintf (stderr, "\nError initialising SDL video: %s\n", SDL_GetError ());
        pthread_exit (NULL);
    }
    
    if (SDL_GetCurrentDisplayMode (0, &videomode) != 0)
    {
        fprintf (stderr, "Error getting current display mode: %s\n", SDL_GetError ());
        pthread_exit (NULL);
    }

    if (verbose)
        fprintf (stdout, "Current screen mode: %ix%i\n", videomode.w, videomode.h);

    SCREEN_WIDTH = videomode.w;
    SCREEN_HEIGHT = videomode.h;
    

    window = SDL_CreateWindow("Pisound", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
    if (window == NULL)
    {
        fprintf (stderr, "Error creating window: %s\n", SDL_GetError());
        pthread_exit (NULL);
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        fprintf (stderr, "Error creating renderer: %s\n", SDL_GetError());
        pthread_exit (NULL);
    }
    
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
    
    SDL_RenderCopy (renderer, background_tex, NULL, NULL);
    while (running)
    {
        SDL_RenderCopy (renderer, background_tex, NULL, NULL);
        render_score ();
        SDL_RenderPresent (renderer);
    }
    pthread_exit (NULL);
    return NULL;
}
