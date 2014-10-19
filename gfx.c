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
    SDL_FreeSurface( background ); 
    SDL_FreeSurface( score ); 

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
    //I wanted a braaahn baby
    SDL_Color textColor = { 171, 100, 0 };
    score_string = render_score_to_string (12345678901);
    score = TTF_RenderText_Solid( fonts[1], score_string, textColor );
    TTF_SizeText (fonts[1], score_string, &textWidth, &textHeight);
    xpos = (SCREEN_WIDTH - textWidth) / 2;
    //ypos = (SCREEN_HEIGHT - textHeight) / 2;
    ypos = 177;

    if (score == NULL)
        fprintf (stderr, "Error rendering text %s\n", SDL_GetError());
    apply_surface( xpos, ypos, score, screen );
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

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE|SDL_FULLSCREEN);
    if (screen == NULL)
        fprintf (stderr, "Error setting video mode %s\n", SDL_GetError());
    
    background = load_image ("images/background.bmp");
    if (background == NULL)
    {       
        fprintf (stderr, "Error loading background image\n");    
    }
  

    while (running)
    {
        apply_surface( 0, 0, background, screen );
        render_score ();
        if (SDL_Flip (screen) != 0)
            fprintf (stderr, "Error in SDL_Flip %s\n", SDL_GetError());
    }
    return NULL;
}
