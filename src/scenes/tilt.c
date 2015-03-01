#include "../pisound.h"
#include "../gfx.h"

void draw_tilt (void)
{
    int txtW, txtH;

    SDL_Color color = { 255, 255, 255 };
    SDL_Surface * surface = TTF_RenderText_Solid ( fonts[0], "TILT", color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface (renderer, surface);
    SDL_FreeSurface (surface);

    SDL_QueryTexture (texture, NULL, NULL, &txtW, &txtH);
    SDL_Rect dstrect = { SCREEN_WIDTH / 2, SCREEN_HEIGHT /2, txtW, txtH};
    
    SDL_RenderCopy (renderer, texture, NULL, &dstrect); 
    
    SDL_DestroyTexture (texture);

}

