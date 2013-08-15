/* Abbaye des Morts */
/* Version 2.0 */

/* (c) 2010 - Locomalito & Gryzor87 */
/* 2013 - David "Nevat" Lara */

/* GPL v3 license */

main () {

	uint exit = 0;
	uint state = 0; /* 0-intro,1-introduction */
	uint grapset = 0; /* 0-8bits, 1-16bits */

	/* Creating window */
	SDL_Window *screen = SDL_CreateWindow("Abbaye des Morts v2.0",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,512,384,SDL_WINDOW_OPENGL);
	/* Renderer (with VSync, nice !) */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, 256, 192);

	while (exit != 1) {
		switch (state) {
			case 0: startscreen(screen,renderer,&state,&grapset);
							break;
			case 1: exit = 1;
							break;
		}
	}

	/* Cleaning */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);

	/* Exiting normally */
	return 0;

}

void history(SDL_Renderer *renderer,uint *state,uint *grapset) {

	uint exit = 0;

}