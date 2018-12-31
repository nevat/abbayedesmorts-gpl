/* ending.c */

# include "base.h"

void ending (SDL_Window *screen,uint8_t *state) {

	SDL_Texture *tiles = IMG_LoadTexture(renderer, DATADIR "/graphics/tiles.png");
	SDL_Texture *text = IMG_LoadTexture(renderer, DATADIR "/graphics/ending.png");

	SDL_Rect srcdoor = {600,72,64,48};
	SDL_Rect desdoor = {96,72,64,48};

	Mix_Music *bso = Mix_LoadMUS(DATADIR "/sounds/PrayerofHopeN.ogg");

	int16_t x = 0;

	Mix_PlayMusic(bso,0);

	for (int16_t i=0;i<951;i++) {

		/* Cleaning the renderer */
		SDL_RenderClear(renderer);

		if (i<360)
			x = i/60;
		else
			x = 5;
		
		if (i > 365)
			SDL_RenderCopy(renderer,text,NULL,NULL);

		srcdoor.x = 600 + (64 * x);
		SDL_RenderCopy(renderer,tiles,&srcdoor,&desdoor);

		/* Flip */
		SDL_RenderPresent(renderer);

	}

	/* Cleaning */
	SDL_DestroyTexture(tiles);
	SDL_DestroyTexture(text);
	Mix_FreeMusic(bso);

	*state = 0;

}
