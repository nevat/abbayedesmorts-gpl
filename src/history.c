/* history.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"

void history(SDL_Window *screen,uint *state,uint *grapset,uint *fullscreen) {

	/* Renderer */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
	SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "0");
	SDL_RenderSetLogicalSize(renderer, 256, 192);

	SDL_Event keyp;

	/* Load audio */
	Mix_Music *music = Mix_LoadMUS("/usr/share/abbayev2/sounds/ManhuntN.ogg");

	/* Loading PNG */
	SDL_Texture *tiles = IMG_LoadTexture(renderer,"/usr/share/abbayev2/graphics/tiles.png");
	SDL_Texture *text = IMG_LoadTexture(renderer,"/usr/share/abbayev2/graphics/history.png");

	SDL_Rect srcjean = {384,88,16,24};
	SDL_Rect desjean = {0,100,16,24};
	SDL_Rect srcenem = {96,64,16,24};
	SDL_Rect desenem = {0,100,16,24};

	uint exit = 0;
	float posjean = -16;
	float posenem[4] = {-17,-17,-17,-17};
	uint animation = 0;
	uint i = 0;
	uint musicload = 0;

	while (exit != 1) {

		/* Cleaning the renderer */
		SDL_RenderClear(renderer);

		/* Play music at start */
		if (musicload == 0) {
			musicload = 1;
			Mix_PlayMusic(music, 0);
		}

		/* Show text */
		SDL_RenderCopy(renderer,text,NULL,NULL);

		/* Animation control */
		if (animation < 13)
			animation ++;
		else
			animation = 0;

		/* Jean running */
		if (posjean < 257) {
			posjean += 0.75;
			desjean.x = posjean;
			srcjean.x = 384 + ((animation / 7) * 16); /* Walking animation */
			srcjean.y = 88 + (*grapset * 120); /* 8 or 16 bits sprite */
			SDL_RenderCopy(renderer,tiles,&srcjean,&desjean);
		}

		/* Crusaders running */
		/* When start running */
		for (i=0;i<4;i++) {
			if (posjean > (35 + (30 * i)))
				posenem[i] += 0.65;
		}
		/* Draw */
		for (i=0;i<4;i++) {
			if ((posenem[i] > -17) && (posenem[i] < 257)) {
				desenem.x = posenem[i];
				srcenem.x = 96 + ((animation / 7) * 16);
				srcenem.y = 64 + (*grapset * 120);
				SDL_RenderCopy(renderer,tiles,&srcenem,&desenem);
			}
		}

		/* Check keyboard */
		if ( SDL_PollEvent(&keyp) ) {
			if (keyp.type == SDL_KEYDOWN) { /* Key pressed */
				if (keyp.key.keysym.sym == SDLK_c) { /* Change graphic set */
					if (*grapset == 0)
						*grapset = 1;
					else
						*grapset = 0;
				}
				if (keyp.key.keysym.sym == SDLK_f) { /* Switch fullscreen/windowed */
					if (*fullscreen == 0) {
						SDL_SetWindowFullscreen(screen,SDL_WINDOW_FULLSCREEN_DESKTOP);
						*fullscreen = 1;
					}
					else {
						SDL_SetWindowFullscreen(screen,0);
						*fullscreen = 0;
					}
				}
				if (keyp.key.keysym.sym == SDLK_SPACE) { /* Start game */
					*state = 2;
					exit = 1;
				}
				if (keyp.key.keysym.sym == SDLK_ESCAPE) { /* Exit game */
      		exit = 1;
					*state = 6;
				}
			}
		}

		if (posenem[3] > 256) { /* Ending history */
			exit = 1;
			*state = 2;
		}

		/* Flip ! */
		SDL_RenderPresent(renderer);

	}

	/* Cleaning */
	SDL_DestroyTexture(tiles);
	SDL_DestroyTexture(text);
	SDL_DestroyRenderer(renderer);
	Mix_FreeMusic(music);

}