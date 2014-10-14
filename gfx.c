#include "pisound.h"
#include "gfx.h"

static SDL_Surface* load_image (char *filename)
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimisedImage = NULL;
    loadedImage = SDL_LoadBMP (filename);

    if (loadedImage != NULL)
    {
        optimisedImage = SDL_DisplayFormat (loadedImage);
        SDL_FreeSurface (loadedImage);
    }
    return optimisedImage;
}

static void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, NULL, destination, &offset );
}

static void gfx_clean_up(void) 
{ 
    //Free the surfaces 
    SDL_FreeSurface( background ); 
    SDL_FreeSurface( message ); 
    //Close the font that was used 
    TTF_CloseFont( font );
    TTF_Quit();
    SDL_Quit(); 
}
void* gfx_thread (void *ptr)
{
    const SDL_VideoInfo* videoInfo;

    if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
        fprintf (stderr, "\nError initialising SDL video: %s\n", SDL_GetError ());
    }

    if (TTF_Init () != 0)
    {
        fprintf (stderr, "Error initialising SDL_ttf %s\n", TTF_GetError());
    }

    videoInfo = SDL_GetVideoInfo ();
    
    int SCREEN_WIDTH = videoInfo->current_w;
    int SCREEN_HEIGHT = videoInfo->current_h;
    int SCREEN_BPP = 32;

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE|SDL_FULLSCREEN);
    if (screen == NULL)
        fprintf (stderr, "Error setting video mode %s\n", SDL_GetError());
    
    //message = load_image ("images/message.bmp");
    background = load_image ("images/background.bmp");
    if (background == NULL)
    {       
        fprintf (stderr, "Error loading background image\n");    
    }
  
    //Load font
    font = TTF_OpenFont( DEFAULT_FONT, 60 );
    if (font == NULL)
        fprintf (stderr, "Error loading %s font %s\n", DEFAULT_FONT, TTF_GetError());

    SDL_Color textColor = { 171, 100, 0 };

    message = TTF_RenderText_Solid( font, "1,234,567,890", textColor );
    if (message == NULL)
        fprintf (stderr, "Error rendering text %s\n", SDL_GetError());
    apply_surface( 0, 0, background, screen );
    apply_surface( SCREEN_WIDTH/2, SCREEN_HEIGHT/2, message, screen );
    while (running)
    {
        if (SDL_Flip (screen) != 0)
            fprintf (stderr, "Error in SDL_Flip %s\n", SDL_GetError());

    }
    gfx_clean_up ();
    return NULL;
}

