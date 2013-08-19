/* ending.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"
# include "SDL2/SDL_ttf.h"

void ending (SDL_Window *screen,uint *state) {

	/* Creating renderer */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, 256, 192);

	SDL_Texture *black = IMG_LoadTexture(renderer,"../graphics/black.png");
	SDL_Texture *tiles = IMG_LoadTexture(renderer,"../graphics/tiles.png");
	SDL_Surface *text = NULL;
	SDL_Texture *textsurf = NULL;

	SDL_Color fgcolor = {255,255,255};
	TTF_Font *font = TTF_OpenFont("../fonts/VeniceClassic.ttf",18);
	TTF_SetFontHinting(font, TTF_HINTING_NORMAL);

	SDL_Rect destext = {0,0,0,0};

	SDL_Rect srcdoor = {600,72,64,48};
	SDL_Rect desdoor = {96,72,64,48};

	Mix_Music *bso = Mix_LoadMUS("../sounds/PrayerofHopeN.ogg");

	int i = 0;
	int x = 0;
	int height = 0;
	int width = 0;
	char message[25];

	Mix_PlayMusic (bso,0);

	for (i=0;i<951;i++) {

		SDL_RenderCopy(renderer,black,NULL,NULL);

		if (i<360)
			x = i/60;
		else
			x = 5;

		srcdoor.x = 600 + (64 * x);
		SDL_RenderCopy(renderer,tiles,&srcdoor,&desdoor);
		if (i > 365) {
			sprintf (message, "Your body has burned");
			text = TTF_RenderText_Blended(font, message, fgcolor);
			textsurf = SDL_CreateTextureFromSurface(renderer,text);
			TTF_SizeText(font, message, &width, &height);
			destext.x = 120 - (width / 2);
			destext.y = 20 - (height / 2);
			SDL_RenderCopy(renderer,textsurf,NULL,&destext);
			sprintf (message, "in the flames,");
			text = TTF_RenderText_Blended(font, message, fgcolor);
			textsurf = SDL_CreateTextureFromSurface(renderer,text);
			TTF_SizeText(font, message, &width, &height);
			destext.x = 125 - (width / 2);
			destext.y = 50 - (height / 2);
			SDL_RenderCopy(renderer,textsurf,NULL,&destext);
			sprintf (message, "but your soul has found");
			text = TTF_RenderText_Blended(font, message, fgcolor);
			textsurf = SDL_CreateTextureFromSurface(renderer,text);
			TTF_SizeText(font, message, &width, &height);
			destext.x = 122 - (width / 2);
			destext.y = 125 + (height / 2);
			SDL_RenderCopy(renderer,textsurf,NULL,&destext);
			sprintf (message, "a place in Heaven");
			text = TTF_RenderText_Blended(font, message, fgcolor);
			textsurf = SDL_CreateTextureFromSurface(renderer,text);
			TTF_SizeText(font, message, &width, &height);
			destext.x = 121 - (width / 2);
			destext.y = 155 + (height / 2);
			SDL_RenderCopy(renderer,textsurf,NULL,&destext);
		}

		/* Flip */
		SDL_RenderPresent(renderer);

	}

	/* Cleaning */
	SDL_DestroyTexture (tiles);
	SDL_DestroyTexture (black);
	Mix_FreeMusic (bso);
	TTF_CloseFont (font);
	SDL_DestroyTexture (textsurf);
	SDL_DestroyRenderer (renderer);
	SDL_FreeSurface(text);

	*state = 0;

}