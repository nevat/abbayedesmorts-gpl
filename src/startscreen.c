/* startscreen.c */

#include "base.h"

void startscreen(SDL_Window *screen,uint8_t *state,uint8_t *grapset,uint8_t *fullscreen) {

	uint8_t exit = 0;
	uint8_t musicplay = 0;
    uint8_t change = 1;

	SDL_Rect srcintro = {0,0,256,192};
	SDL_Rect desintro = {0,0,256,192};

	SDL_Event keyp;

	/* Loading PNG */
	SDL_Texture *intro = IMG_LoadTexture(renderer, DATADIR "/graphics/intro.png");
	SDL_Texture *intromd = IMG_LoadTexture(renderer, DATADIR "/graphics/intromd.png");

	/* Load audio */
	Mix_Music *music = Mix_LoadMUS(DATADIR "/sounds/MainTitleN.ogg");

	while (exit != 1) {
        
        if (change == 1) { /* Only redraw when change is made */

            /* Cleaning the renderer */
            SDL_RenderClear(renderer);

            /* Put image on renderer */
            if (*grapset == 0)
                SDL_RenderCopy(renderer, intro, &srcintro, &desintro);
            else
                SDL_RenderCopy(renderer, intromd, &srcintro, &desintro);

            /* Flip ! */
            SDL_RenderPresent(renderer);
            
            change = 0;
            
        }

		/* Play music if required */
		if (musicplay == 0) {
			musicplay = 1;
			Mix_PlayMusic(music, 0);
		}

		/* Check keyboard */
		if ( SDL_PollEvent(&keyp) ) {
			if (keyp.type == SDL_KEYDOWN) { /* Key pressed */
				if (keyp.key.keysym.sym == SDLK_c) { /* Change graphic set */
                    change = 1; /* enable redraw */
					if (*grapset == 0)
						*grapset = 1;
					else
						*grapset = 0;
				}
				if (keyp.key.keysym.sym == SDLK_i) { /* Show instructions */
                    change = 1; /* enable redraw */
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
			
			if (keyp.type == SDL_JOYBUTTONDOWN) {
				if (keyp.jbutton.button == JUMP_JOYBUTTON || keyp.jbutton.button == START_JOYBUTTON) {
					*state = 1;
					exit = 1;
				}
				if (keyp.jbutton.button == SELECT_JOYBUTTON) {
                    change = 1; /* enable redraw */
					if (*grapset == 0)
						*grapset = 1;
					else
						*grapset = 0;
				}
			}

			if (keyp.type == SDL_QUIT) { /* Exit game */
				exit = 1;
				*state = 6;
			}

		}
		
	}

	/* Cleaning */
	SDL_DestroyTexture(intro);
	SDL_DestroyTexture(intromd);
    Mix_FreeMusic(music);
    
}
