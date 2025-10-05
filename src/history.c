/* history.c */

#include "history.h"
#include "loading.h"
#include "main.h"

void history(SDL_Window *screen,uint8_t *state,uint8_t *grapset,uint8_t *fullscreen) {

	SDL_Event keyp;

	/* Load audio */
	Mix_Music *music = loadmus("ManhuntN");

	/* Loading PNG */
	SDL_Texture *tiles = loadtexture(renderer, "tiles");
	SDL_Texture *text = loadtexture(renderer, "history");

	SDL_Rect srcjean = {320,88,16,24};
	SDL_Rect desjean = {0,100,16,24};
	SDL_Rect srcenem = {64,64,16,24};
	SDL_Rect desenem = {0,100,16,24};

	uint8_t exit = 0;
	float posjean = -16;
	float posenem[4] = {-17,-17,-17,-17};
	uint8_t animation = 0;
	uint8_t musicload = 0;

	while (exit != 1) {
		gameoption_t option = OPT_NONE;

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
			srcjean.x = 320 + ((animation / 7) * 16); /* Walking animation */
			srcjean.y = 88 + (*grapset * 120); /* 8 or 16 bits sprite */
			SDL_RenderCopyEx(renderer,tiles,&srcjean,&desjean,0,0,1);
		}

		/* Crusaders running */
		/* When start running */
		for (uint16_t i=0;i<4;i++) {
			if (posjean > (35 + (30 * i)))
				posenem[i] += 0.65;
		}
		/* Draw */
		for (uint16_t i=0;i<4;i++) {
			if ((posenem[i] > -17) && (posenem[i] < 257)) {
				desenem.x = posenem[i];
				srcenem.x = 64 + ((animation / 7) * 16);
				srcenem.y = 64 + (*grapset * 120);
				SDL_RenderCopyEx(renderer,tiles,&srcenem,&desenem,0,0,1);
			}
		}

		/* Check input (keyboard/gamepad) */
		if ( SDL_PollEvent(&keyp) ) {
			switch (keyp.type) {
				case SDL_KEYDOWN:
					switch (keyp.key.keysym.sym) {
						case SDLK_SPACE:
							option = OPT_STARTGAME;
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
						case SDL_CONTROLLER_BUTTON_Y:
							option = OPT_GRAPHICS;
							break;
						case SDL_CONTROLLER_BUTTON_BACK:
							option = OPT_QUIT;
							break;
					}
					break;
				case SDL_QUIT:
					option = OPT_QUIT;
					break;
			}
		}

		switch (option) {
			case OPT_STARTGAME:
				*state = 2;
				exit = 1;
				break;
			case OPT_GRAPHICS:	// Change graphics set
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

		if (posenem[3] > SCREEN_W) { /* Ending history */
			exit = 1;
			*state = 2;
		}

		/* Flip ! */
		renderpresent(renderer);

	}

	/* Cleaning */
	SDL_DestroyTexture(tiles);
	SDL_DestroyTexture(text);
	Mix_FreeMusic(music);

}
