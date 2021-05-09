/* Abbaye des Morts */
/* Version 2.0 */

/* (c) 2010 - Locomalito & Gryzor87 */
/* 2013 - David "Nevat" Lara */

/* GPL v3 license */

 
#include "main.h"
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

	/* Creating window */
	SDL_Window *screen; 
	if (fullscreen)
		screen = SDL_CreateWindow(NULL, 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else	
		screen = SDL_CreateWindow("Abbaye des Morts v2.0.1",
			SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,768,576,0);

	/* Create renderer (with VSync, nice !) */
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, 256, 192);
	SDL_SetRenderDrawColor(renderer,0,0,0,255);

	/* Init joystick if there's one connected */
	SDL_Joystick *joystick = NULL;

	if ( SDL_Init(SDL_INIT_JOYSTICK) >= 0 )
	{
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
				startscreen(screen,&state,&grapset,&fullscreen);
				break;
			case 1:
				history(screen,&state,&grapset,&fullscreen);
				break;
			case 2:
				game(screen,&state,&grapset,&fullscreen);
				break;
			case 3:
				gameover(screen,&state);
				break;
			case 4:
				ending(screen,&state);
				break;
			case 6:
				exit = 1;
				break;
		}
	}

	/* Cleaning */
	SDL_JoystickClose(joystick);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);
	SDL_Quit();	

#ifdef __SWITCH__
	romfsExit();
#endif

	/* Exiting normally */
	return 0;

}
