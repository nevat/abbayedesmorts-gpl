/* gameover.c */

#include "gameover.h"

void gameover (SDL_Window *screen,uint8_t *state) {

	SDL_Texture *gameover = IMG_LoadTexture(renderer, DATADIR "/graphics/gameover.png");
	
	Mix_Music *bso = Mix_LoadMUS(DATADIR "/sounds/GameOverV2N.ogg");

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer,gameover,NULL,NULL);

	/* Flip */
	SDL_RenderPresent(renderer);
	Mix_PlayMusic(bso, 0);

	/* Wait */
	sleep(12);

	/* Cleaning */
	Mix_FreeMusic(bso);
	SDL_DestroyTexture(gameover);

	*state = 0;

}
