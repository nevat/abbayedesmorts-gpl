/* Abbaye des Morts */
/* Version 2.0 */

/* (c) 2010 - Locomalito & Gryzor87 */
/* 2013 - David "Nevat" Lara */

/* GPL v3 license */

#include "main.h"

main () {

	uint exit = 0;
	uint state = 0; /* 0-intro,1-history,2-game */
	uint grapset = 0; /* 0-8bits, 1-16bits */

	/* Creating window */
	SDL_Window *screen = SDL_CreateWindow("Abbaye des Morts v2.0",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,512,384,SDL_WINDOW_OPENGL);

	/* Init audio */
	Mix_OpenAudio (44100, AUDIO_S16, 2, 4096);

	while (exit != 1) {
		switch (state) {
			case 0: startscreen(screen,&state,&grapset);
							break;
			case 1: history(screen,&state,&grapset);
							break;
			case 2: game(screen,&state,&grapset);
							break;
			case 6: exit = 1;
							break;
		}
	}

	/* Cleaning */
	SDL_DestroyWindow(screen);

	/* Exiting normally */
	return 0;

}