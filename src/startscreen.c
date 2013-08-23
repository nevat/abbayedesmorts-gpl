/* startscreen.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"

void startscreen(SDL_Window *screen,uint *state,uint *grapset,uint *fullscreen) {

	/* Renderer (with VSync, nice !) */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
	SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY", "0");
	SDL_RenderSetLogicalSize(renderer, 256, 192);

	uint exit = 0;
	uint musicplay = 0;

	SDL_Rect srcintro = {0,0,256,192};
	SDL_Rect desintro = {0,0,256,192};

	SDL_Event keyp;

	/* Loading PNG */
	SDL_Texture *intro = IMG_LoadTexture(renderer,"/usr/share/abbayev2/graphics/intro.png");
	SDL_Texture *intromd = IMG_LoadTexture(renderer,"/usr/share/abbayev2/graphics/intromd.png");

	/* Load audio */
	Mix_Music *music = Mix_LoadMUS("/usr/share/abbayev2/sounds/MainTitleN.ogg");

	while (exit != 1) {

		/* Cleaning the renderer */
		SDL_RenderClear(renderer);

		/* Put image on renderer */
		if (*grapset == 0)
			SDL_RenderCopy(renderer, intro, &srcintro, &desintro);
		else
			SDL_RenderCopy(renderer, intromd, &srcintro, &desintro);

		/* Flip ! */
		SDL_RenderPresent(renderer);

		/* Play music if required */
		if (musicplay == 0) {
			musicplay = 1;
			Mix_PlayMusic(music, 0);
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
				if (keyp.key.keysym.sym == SDLK_i) { /* Show instructions */
					if (srcintro.y == 0)
						srcintro.y = 192;
					else {
						srcintro.y = 0;
						musicplay = 0;
					}
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
					*state = 1;
					exit = 1;
				}
				if (keyp.key.keysym.sym == SDLK_ESCAPE) { /* Exit game */
      		exit = 1;
					*state = 6;
				}
			}
		}

	}

	/* Cleaning */
	SDL_DestroyTexture(intro);
	SDL_DestroyTexture(intromd);
	SDL_DestroyRenderer(renderer);

}