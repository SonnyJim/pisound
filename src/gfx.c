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

void free_gfx (void) 
{ 
    int i;
    //Free the surfaces 
    SDL_FreeSurface( background_srf ); 
    SDL_FreeSurface( score_srf ); 

    //Close the fonts that were used 
    for (i = 0; i < MAX_FONTS; i++)
        TTF_CloseFont(fonts[i]);
    TTF_Quit();
}

char *render_score_to_string (long long score)
{
    static char oldstring[255], newstring[255];
    int len, newlen, i, j, offset;

    //Clear both strings
    memset (oldstring, 0, 255);
    memset (newstring, 0, 255);

    //Convert the score to a string
    sprintf (oldstring, "%lld", score);

    //Get the current and resulting string lengths
    len = strlen (oldstring);
    newlen = len + (len / 3);

    //Don't forget the string terminator
    i = len + 1;
    j = newlen + 1;
    offset = 0;

    while (i != 0)
    {
        i--;
        j--;
        newstring[j] = oldstring[i];
        if (offset == 3)
        {
            offset = 0;
            j--;
            newstring[j] = SCORE_SEPARATOR;
        }
        offset++;
    }
    return newstring;
}

static void render_score (void)
{
    int textWidth, textHeight, xpos, ypos;
    char *score_string;
    SDL_Color textColor;
    
    score_string = render_score_to_string (score);

    //I wanted a braaahn baby
    textColor = (SDL_Color) { 82, 42, 0};
    score_srf = TTF_RenderText_Solid( fonts[0], score_string, textColor );
    SDL_SetAlpha (score_srf, SDL_SRCALPHA, 128);
    TTF_SizeText (fonts[0], score_string, &textWidth, &textHeight);
    xpos = (SCREEN_WIDTH - textWidth) / 2;
    //ypos = (SCREEN_HEIGHT - textHeight) / 2;
    ypos = 100;

    if (score_srf == NULL)
        fprintf (stderr, "Error rendering text %s\n", SDL_GetError());
    apply_surface( xpos, ypos, score_srf, screen_srf );
}

void* gfx_thread (void *ptr)
{
    const SDL_VideoInfo* videoInfo;

    if (SDL_Init (SDL_INIT_VIDEO) != 0)
    {
        fprintf (stderr, "\nError initialising SDL video: %s\n", SDL_GetError ());
    }
    
    load_fonts ();

    videoInfo = SDL_GetVideoInfo ();
    SCREEN_WIDTH = videoInfo->current_w;
    SCREEN_HEIGHT = videoInfo->current_h;
    SCREEN_BPP = 32;

    if (verbose)
        fprintf (stdout, "Screen width = %i Screen height = %i\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    screen_srf = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE|SDL_FULLSCREEN);
    if (screen_srf == NULL)
        fprintf (stderr, "Error setting video mode %s\n", SDL_GetError());
    
    background_srf = load_image ("images/background.bmp");
    if (background_srf == NULL)
    {       
        fprintf (stderr, "Error loading background image\n");    
    }
  

    while (running)
    {
        apply_surface( 0, 0, background_srf, screen_srf );
        render_score ();
        if (SDL_Flip (screen_srf) != 0)
            fprintf (stderr, "Error in SDL_Flip %s\n", SDL_GetError());
    }
    return NULL;
}
