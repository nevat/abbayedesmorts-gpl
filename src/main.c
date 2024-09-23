/* Abbaye des Morts */
/* Version 2.0 */

/* (c) 2010 - Locomalito & Gryzor87 */
/* 2013-2023 - David "Nevat" Lara */
/* 2015-2023 - Carsten Teibes */

/* GPL v3 license */

 
#include "main.h"
#include "startscreen.h"
#include "history.h"
#include "game.h"
#include "gameover.h"
#include "ending.h"

SDL_Renderer *renderer;

int main (int argc, char** argv) {

	// TODO: support arguments for fullscreen, etc.
	(void) argc;
	(void) argv;

	uint8_t exit = 0;
	uint8_t state = 0; /* 0-intro,1-history,2-game,3-gameover,4-ending,5-exit */
	uint8_t grapset = 0; /* 0-8bits, 1-16bits */
	uint8_t fullscreen = 0; /* 0-Windowed,1-Fullscreen */

	/* SDL2 initialization */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

#ifdef __SWITCH__
	romfsInit();
#endif

	/* Missing data files, exit with error */
	if(!check_data()) return 1;

	/* Creating window */
	SDL_Window *screen = SDL_CreateWindow(GAME_TITLE,
		SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_W * 3,SCREEN_H * 3,
		fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_RESIZABLE);

	/* Create renderer (with VSync, nice !) */
	renderer = SDL_CreateRenderer(screen, -1,
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	SDL_RenderSetLogicalSize(renderer, SCREEN_W, SCREEN_H);

	/* Create a render target for smooth scaling */
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_Texture *target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, SCREEN_W, SCREEN_H);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
	SDL_SetRenderTarget(renderer, target);

	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);

	/* Init joystick if there's one connected */
	SDL_Joystick *joystick = NULL;

	if (SDL_Init(SDL_INIT_JOYSTICK) >= 0) {
		joystick = SDL_NumJoysticks() > 0 ? SDL_JoystickOpen(0) : NULL; 
		SDL_JoystickEventState(SDL_ENABLE);
	}

	/* Hide mouse cursor */
	SDL_ShowCursor(SDL_DISABLE);

	/* Init audio */
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,4096);
	Mix_AllocateChannels(5);

	while (exit != 1) {
		switch (state) {
			case 0:
				update_title(screen, NULL);
				startscreen(screen,&state,&grapset,&fullscreen);
				break;
			case 1:
				update_title(screen, "History");
				history(screen,&state,&grapset,&fullscreen);
				break;
			case 2:
				game(screen,&state,&grapset,&fullscreen);
				break;
			case 3:
				update_title(screen, "GAME OVER");
				gameover(screen,&state);
				break;
			case 4:
				update_title(screen, "THE END");
				ending(screen,&state);
				break;
			case 6:
				exit = 1;
				break;
		}
	}

	/* Cleaning */
	SDL_JoystickClose(joystick);
	SDL_DestroyTexture(target);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);
	SDL_Quit();	

#ifdef __SWITCH__
	romfsExit();
#endif

	/* Exiting normally */
	return 0;

}

int check_data() {
	const int NUM_CHECKS = 4;

	/* Check every kind of data */
	const char* files[] = {
		DATADIR "/graphics/tiles.png",
		DATADIR "/sounds/MainTitleN.ogg",
		DATADIR "/data/map.txt",
		DATADIR "/data/enemies.txt"
	};

	/* Try to open each file */
	int missing = NUM_CHECKS;
	SDL_RWops *rw;
	for(int i = 0; i < NUM_CHECKS; i++) {
		rw = SDL_RWFromFile(files[i], "rb");
		if (rw) {
			SDL_RWclose(rw);
			missing--;
		}
	}

	/* Everything found */
	if (!missing) return 1;

	const char* errormsg = "Cannot find data!\nFiles are expected under \"" DATADIR
		"\",\nbut nothing was found.\n\nPlease check your installation.";

	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, GAME_TITLE, errormsg, NULL)) {
		/* Fallback error message to terminal */
		printf("ERROR: %s\n", errormsg);
	}

	return 0;
}

void update_title(SDL_Window *screen, const char* title) {
	static char buf[64];
	strcpy(buf, GAME_TITLE);
	if (title) {
		strcat(buf, " - ");
		strcat(buf, title);
	}
	SDL_SetWindowTitle(screen, buf);
}

void renderpresent(SDL_Renderer *renderer) {
	SDL_Texture *target = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, NULL);

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, target, NULL, NULL);
	SDL_RenderPresent(renderer);

	SDL_SetRenderTarget(renderer, target);
}
