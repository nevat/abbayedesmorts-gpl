/* ending.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"

void ending (SDL_Window *screen,uint *state) {

	/* Creating renderer */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
	SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "0");
	SDL_RenderSetLogicalSize(renderer, 256, 192);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	SDL_Texture *tiles = IMG_LoadTexture(renderer,"/usr/share/abbayev2/graphics/tiles.png");
	SDL_Texture *text = IMG_LoadTexture(renderer,"/usr/share/abbayev2/graphics/ending.png");

	SDL_Rect srcdoor = {600,72,64,48};
	SDL_Rect desdoor = {96,72,64,48};

	Mix_Music *bso = Mix_LoadMUS("/usr/share/abbayev2/sounds/PrayerofHopeN.ogg");

	int i = 0;
	int x = 0;
	int height = 0;
	int width = 0;
	char message[25];

	Mix_PlayMusic (bso,0);

	for (i=0;i<951;i++) {

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
	SDL_DestroyTexture (tiles);
	SDL_DestroyTexture(text);
	Mix_FreeMusic (bso);
	SDL_DestroyRenderer (renderer);

	*state = 0;

}