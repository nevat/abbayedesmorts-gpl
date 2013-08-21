/* gameover.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"
# include "SDL2/SDL_ttf.h"

void gameover (SDL_Window *screen,uint *state) {

	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, 256, 192);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	
  SDL_Color fgcolor = {255,255,255};
	TTF_Font *font = TTF_OpenFont("../fonts/VeniceClassic.ttf", 18);
	SDL_Texture *gameoversurf = NULL;
	SDL_Surface *gameover = NULL;
	SDL_Rect destext = {0,0,0,0};
	Mix_Music *bso = Mix_LoadMUS("../sounds/GameOverV2N.ogg");
	int height = 0;
	int width = 0;
  char text[16];

	/* TTF_SetFontHinting(font, TTF_HINTING_NORMAL); */

	sprintf (text, "G a m e  O v e r");

	gameover = TTF_RenderText_Blended(font, text, fgcolor);
	gameoversurf = SDL_CreateTextureFromSurface(renderer,gameover);
	TTF_SizeText(font, text, &width, &height);
	destext.x = 50;
	destext.y = 60;
	destext.w = 100;
	destext.h = 100;
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer,gameoversurf,NULL,&destext);

	/* Flip */
	SDL_RenderPresent(renderer);
	Mix_PlayMusic(bso, 0);

	/* Wait */
	sleep(12);

	/* Cleaning */
	Mix_FreeMusic (bso);
	TTF_CloseFont(font);
	SDL_DestroyTexture(gameoversurf);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(gameover);

	*state = 0;

}