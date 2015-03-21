#include "pisound.h"
#include "gfx.h"
#include "scene.h"
#include "game.h"

void gfx_init_game_vars (void)
{
    old_player_num = 0;
    player_num = 1;
    old_score = 1;
    score = 0;
}

void game_sign_init (void)
{
    fprintf (stdout, "Initialising game_sign subscene\n");
}

static void init_game_scene (void)
{
    if (verbose)
        fprintf (stdout, "Initialising %s scene\n", scene_names[current_scene]);
    
    
    background_srf = IMG_Load ("data/hbb/gfx/backgrounds/sign.png");
    if (background_srf == NULL)
        fprintf (stderr, "Error loading background: %s\n", SDL_GetError());
    
    background_tex = SDL_CreateTextureFromSurface(renderer, background_srf);
   
    if (background_tex == NULL)
        fprintf (stderr, "Error creating background texture: %s\n", SDL_GetError());
    SDL_FreeSurface (background_srf);
    
    running_scene = current_scene;
}

static void render_score (void)
{
    int textWidth, textHeight, xpos, ypos;
    char *score_string = NULL;
    SDL_Color textColor;
    SDL_Surface * score_srf = NULL;
  
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
        SDL_SetTextureBlendMode (score_tex, SDL_BLENDMODE_BLEND);    
        SDL_SetTextureAlphaMod (score_tex, 150);
        
        //Outline
        textColor = (SDL_Color) { 42, 22, 0};
        score_srf = TTF_RenderText_Solid( fonts[1], score_string, textColor );
        score_outline_tex = SDL_CreateTextureFromSurface(renderer, score_srf);
        SDL_FreeSurface (score_srf);
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

/*
void load_gfx (void)
{
    sprite_srf = IMG_Load("images/shaving.png");
    if (sprite_srf == NULL)
        fprintf (stderr, "Error loading sprite: %s\n", IMG_GetError());
    sprite_tex = SDL_CreateTextureFromSurface (renderer, sprite_srf);
    SDL_FreeSurface (sprite_srf);
    sprite_srf = NULL;
    SDL_QueryTexture (sprite_tex, NULL, NULL, &sprite_rect.w, &sprite_rect.h);


    billybob_tex = load_image_to_texture (IMG_BILLYBOB);
    SDL_QueryTexture (billybob_tex, NULL, NULL, &billybob_rect.w, &billybob_rect.h);
    
    bubba_tex = load_image_to_texture (IMG_BUBBA);
    SDL_QueryTexture (bubba_tex, NULL, NULL, &bubba_rect.w, &bubba_rect.h);

    grandpa_tex = load_image_to_texture (IMG_GRANDPA);
    SDL_QueryTexture (grandpa_tex, NULL, NULL, &grandpa_rect.w, &grandpa_rect.h);

    peggysue_tex = load_image_to_texture (IMG_PEGGYSUE);
    SDL_QueryTexture (peggysue_tex, NULL, NULL, &peggysue_rect.w, &peggysue_rect.h);
}
*/

void draw_game (void)
{
    if (current_scene != running_scene)
        init_game_scene ();

   
    SDL_RenderCopy (renderer, background_tex, NULL, NULL);
    render_score ();
}

