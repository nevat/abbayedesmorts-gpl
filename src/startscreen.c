/* startscreen.c */

# include "startscreen.h"

void startscreen(SDL_Window *screen,uint *state,uint *grapset) {

	/* Renderer (with VSync, nice !) */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, 256, 192);

	uint exit = 0;
	uint musicplay = 0;

	SDL_Rect srcintro = {0,0,256,192};
	SDL_Rect desintro = {0,0,256,192};

	SDL_Event keyp;

	/* Loading PNG */
	SDL_Texture *intro = IMG_LoadTexture(renderer,"../graphics/intro.png");
	SDL_Texture *intromd = IMG_LoadTexture(renderer,"../graphics/intromd.png");

	/* Load audio */
	Mix_Music *music = Mix_LoadMUS("../sounds/MainTitleN.ogg");

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
				if (keyp.key.keysym.sym == SDLK_f) /* Switch to fullscreen */
					SDL_SetWindowFullscreen(screen,SDL_WINDOW_FULLSCREEN);
				if (keyp.key.keysym.sym == SDLK_SPACE) { /* Start game */
					*state = 1;
					exit = 1;
				}
			}
		}

	}

	/* Cleaning */
	SDL_DestroyTexture(intro);
	SDL_DestroyTexture(intromd);
	SDL_DestroyRenderer(renderer);

}