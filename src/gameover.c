/* gameover.c */

#include "gameover.h"
#include "loading.h"

void gameover (SDL_Window *screen,uint8_t *state) {

	SDL_Texture *gameover = loadtexture(renderer, "gameover");
	Mix_Music *bso = loadmus("GameOverV2N");

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer,gameover,NULL,NULL);

	/* Flip */
	SDL_RenderPresent(renderer);
	Mix_PlayMusic(bso, 0);

	/* Wait */
	SDL_Delay(12 * 1000);

	/* Cleaning */
	Mix_FreeMusic(bso);
	SDL_DestroyTexture(gameover);

	*state = 0;

}
