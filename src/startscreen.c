/* startscreen.c */

#include "startscreen.h"
#include "loading.h"
#include "main.h"

void startscreen(SDL_Window *screen,uint8_t *state,uint8_t *grapset,uint8_t *fullscreen) {

	uint8_t exit = 0;
	uint8_t musicplay = 0;
	uint8_t change = 1;

	SDL_Rect srcintro = {0,0,SCREEN_W,SCREEN_H};
	SDL_Rect desintro = {0,0,SCREEN_W,SCREEN_H};

	SDL_Event keyp;

	/* Loading PNG */
	SDL_Texture *intro = loadtexture(renderer, "intro");
	SDL_Texture *intromd = loadtexture(renderer, "intromd");

	/* Load audio */
	Mix_Music *music = loadmus("MainTitleN");

	while (exit != 1) {
		gameoption_t option = OPT_NONE;
        
        if (change == 1) { /* Only redraw when change is made */

            /* Cleaning the renderer */
            SDL_RenderClear(renderer);

            /* Put image on renderer */
            if (*grapset == 0)
                SDL_RenderCopy(renderer, intro, &srcintro, &desintro);
            else
                SDL_RenderCopy(renderer, intromd, &srcintro, &desintro);

            /* Flip ! */
            renderpresent(renderer);
            
            change = 0;
            
        }

		/* Play music if required */
		if (musicplay == 0) {
			musicplay = 1;
			Mix_PlayMusic(music, 0);
		}

		/* Check input (keyboard/gamepad) */
		if ( SDL_PollEvent(&keyp) ) {
			switch (keyp.type) {
				case SDL_KEYDOWN:
					switch (keyp.key.keysym.sym) {
						case SDLK_SPACE:
							option = OPT_STARTGAME;
							break;
						case SDLK_i:
							option = OPT_SHOWINSTRUCT;
							break;
						case SDLK_f:
							option = OPT_FULLSCREEN;
							break;
						case SDLK_c:
							option = OPT_GRAPHICS;
							break;
						case SDLK_ESCAPE:
							option = OPT_QUIT;
							break;
					}
					break;
				case SDL_CONTROLLERBUTTONDOWN:
					switch (keyp.cbutton.button) {
						case SDL_CONTROLLER_BUTTON_A:
						case SDL_CONTROLLER_BUTTON_START:
							option = OPT_STARTGAME;
							break;
						case SDL_CONTROLLER_BUTTON_X:
							option = OPT_SHOWINSTRUCT;
							break;
						case SDL_CONTROLLER_BUTTON_Y:
							option = OPT_GRAPHICS;
							break;
						case SDL_CONTROLLER_BUTTON_BACK:
							option = OPT_QUIT;
							break;
					}
					break;
				case SDL_WINDOWEVENT:
					change = 1;
					break;
				case SDL_CONTROLLERDEVICEADDED:
					gamepad_init();
					break;
				case SDL_CONTROLLERDEVICEREMOVED:
					gamepad_remove(keyp.cdevice.which);
					break;
				case SDL_QUIT:
					option = OPT_QUIT;
					break;
			}
		}

		switch (option) {
			case OPT_STARTGAME:
				*state = 1;
				exit = 1;
				break;
			case OPT_SHOWINSTRUCT:	// Show instructions
				change = 1; // enable redraw
				if (srcintro.y == 0)
					srcintro.y = SCREEN_H;
				else {
					srcintro.y = 0;
					musicplay = 0;
				}
				break;
			case OPT_GRAPHICS:	// Change graphics set
				change = 1;	// enable redraw
				if (*grapset == 0)
					*grapset = 1;
				else
					*grapset = 0;
				break;
			case OPT_FULLSCREEN:	// Switch fullscreen/windowed
				if (*fullscreen == 0) {
					SDL_SetWindowFullscreen(screen,SDL_WINDOW_FULLSCREEN_DESKTOP);
					*fullscreen = 1;
				} else {
					SDL_SetWindowFullscreen(screen,0);
					*fullscreen = 0;
				}
				break;
			case OPT_QUIT:
				*state = 6;
				exit = 1;
			default:
				break;
		}
	}

	/* Cleaning */
	SDL_DestroyTexture(intro);
	SDL_DestroyTexture(intromd);
	Mix_FreeMusic(music);

}
