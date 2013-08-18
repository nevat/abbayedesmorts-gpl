/* game.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"
# include "SDL2/SDL_ttf.h"

# include "structs.h"

void game(SDL_Window *screen,uint *state,uint *grapset) {

	/* Renderer */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, 256, 192);

	/* Sounds */
	Mix_Music *bso[7];
	Mix_Chunk *fx[7];

	/* Init Font */
	TTF_Font *fuente = TTF_OpenFont("../fonts/VeniceClassic.ttf", 17);
	TTF_SetFontHinting(font, TTF_HINTING_NORMAL);

	uint stagedata[25][22][32];
	int enemydata[25][7][15];
	uint room[2] = {5,5}; /* Room, previous room */
	uint exit = 0;
	int coord[2] = {0,0}; /* Coord X and Y */
	uint changeflag = 1; /* Screen change */
	uint counter[3] = {0,0,0}; /* Counters */
	uint changetiles=*grapset;
	uint i = 0;

	/* Loading PNG */
	SDL_Texture *tiles = IMG_LoadTexture(renderer,"../graphics/tiles.png");

	/* Loading musics */
	loadingmusic(bso,fx);

	/* Load data */
	loaddata(stagedata,enemydata);

	/* Init struct */
	struct enem enemies = {
		.type = {0,0,0,0,0,0,0},
		.x = {0,0,0,0,0,0,0},
		.y = {0,0,0,0,0,0,0},
		.direction = {0,0,0,0,0,0,0},
		.tilex = {0,0,0,0,0,0,0},
		.tiley = {0,0,0,0,0,0,0},
		.animation = {0,0,0,0,0,0,0},
		.limleft = {0,0,0,0,0,0,0},
		.limright = {0,0,0,0,0,0,0},
		.speed = {0,0,0,0,0,0,0},
		.fire = {0,0,0,0,0,0,0},
		.adjustx1 = {0,0,0,0,0,0,0},
		.adjustx2 = {0,0,0,0,0,0,0},
		.adjusty1 = {0,0,0,0,0,0,0},
		.adjusty2 = {0,0,0,0,0,0,0}
	};

	struct hero jean = {
		.x = 72,
		.y = 136,
		.direction = 1,
		.jump = 0,
		.height = 0,
		.animation = 0,
		.gravity = 1.9,
		.points = {0,0,0,0,0,0,0,0},
		.ground = 0,
		.collision = {0,0,0,0},
		.ducking = 0,
		.checkpoint = {5,72,136,5},
		.state = {9,0},
		.flags = {0,0,0,0,0,0,0},
		.death = 0,
		.push = {0,0,0,0},
		.temp = 0
	};

	/* Game loop */
	while (exit != 1) {

		/* Manage counters */
		counters(counter);

		/* Cleaning the renderer */
		SDL_RenderClear(renderer);

		/* Animation of fire and water */
		animation(stagedata,room,counter);

		/* Draw screen */
		drawscreen(renderer,stagedata,tiles,room,counter,changeflag,fx,changetiles);

		/* Draw statusbar */
		if (room[0] != 4)
			statusbar(renderer,tiles,room,jean.lifes,jean.crosses,font,changetiles);

		/* Draw Jean */
		if (jean.flags[6] < 8)
			drawjean (renderer,tiles,&jean,counter,fx,changetiles);

		/* Flip ! */
		SDL_RenderPresent(renderer);

	}

	/* Cleaning */
	SDL_DestroyTexture(tiles);
	SDL_DestroyRenderer(renderer);
	for (i=0;i<7;i++) {
		Mix_FreeMusic(bso[i]);
		Mix_FreeChunk(fx[i]);
	}
	TTF_CloseFont(font);

}

void animation (uint stagedata[][22][32],int room[],int counter[]) {

	int i = 0;
	int j = 0;

	uint data = 0;

	for (j=0; j<=21; j++) {
		for (i=0; i<=31; i++) {

			data = stagedata[room][j][i];

			/* Fire animation */
			if ((data == 59) || (data == 60)) {
				if ((counter[0] == 1) || (counter[0] == 11) || (counter[0] == 21)) {
					if (data == 59)
						data = 60;
					else
						data = 59;
				}
			}

			/* Water animation */
			if ((data > 500) && (data < 533)) {
				if (data < 532)
					data ++;
				else
					data = 501;
			}

			stagedata[room][j][i] = data;

		}
	}
}

void counters (uint counter[]) {

	if (counter[0] < 29)
		counter[0] ++;
	else
		counter[0] = 0;

	if (counter[1] < 90)
		counter[1] ++;
	else
		counter[1] = 0;

	if (counter[2] < 8)
		counter[2] ++;
	else
		counter[2] = 0;

}

void drawscreen (SDL_Renderer *renderer,uint stagedata[][22][32],SDL_Texture *tiles,uint room[],uint counter[],uint changeflag,Mix_Chunk *fx[],uint changetiles) {

	int coordx = 0;
	int coordy = 0;
	SDL_Rect srctiles = {0,0,8,8};
	SDL_Rect destiles = {0,0,0,0};
	uint data = 0;

	for (coordy=0; coordy<=21; coordy++) {
		for (coordx=0; coordx<=31; coordx++) {
			data = stagedata[room][coordy][coordx];
			if ((data > 0) && (data != 99)) {
				destiles.x = coordx * 8;
				destiles.y = coordy * 8;
				if (data < 200) {
					srctiles.w = 8;
					srctiles.h = 8;
					if (data < 101) {
						srctiles.y = 0;
						if (data == 84) /* Cross brightness */
							srctiles.x = (data - 1) * 8 + (counter[0]/8 * 8);
						else
							srctiles.x = (data - 1) * 8;
					}
					else {
						if (data == 154) {
							srctiles.x=600 + ((counter[0] / 8) * 16);
							srctiles.y=0;
							srctiles.w=16;
							srctiles.h=24;
						}
						else {
							srctiles.y = 8;
							srctiles.x = (data - 101) * 8;
						}
					}
				}
				if ((data > 199) && (data < 300)) {
					srctiles.x = (data - 201) * 48;
					srctiles.y = 16;
					srctiles.w = 48;
					srctiles.h = 48;
				}
				if ((data > 299) && (data < 399)) {
					srctiles.x = 96 + ((data - 301) * 8);
					srctiles.y = 16;
					srctiles.w = 8;
					srctiles.h = 8;
					/* Door movement */
					if ((room == 7) && ((counter[1] > 59) && (counter[1] < 71))) {
						if ((data == 347) || (data == 348) || (data == 349) || (data == 350)) {
							destiles.x += 2;
							if ((data == 350) && (counter[1] == 70))
								Mix_PlayChannel(-1, fx[3], 0); /* Sound of door */
						}
					}
				}
				/* Hearts */
				if ((data > 399) && (data < 405)) {
					srctiles.x = 96 + ((data - 401) * 8) + (32 * (counter[0] / 15));
					srctiles.y = 24;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				/* Crosses */
				if ((data > 408) && (data < 429)) {
					srctiles.x = 96 + ((data - 401) * 8) + (32 * (counter[1] / 23));
					srctiles.y = 24;
					srctiles.w = 8;
					srctiles.h = 8;
				}

				if ((data > 499) && (data < 599)) {
					srctiles.x = 96 + ((data - 501) * 8);
					srctiles.y = 32;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				if ((data > 599) && (data < 650)) {
					srctiles.x = 96 + ((data - 601) * 8);
					srctiles.y = 56;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				if (data == 650) { /* Cup */
					srctiles.x = 584;
					srctiles.y = 87;
					srctiles.w = 16;
					srctiles.h = 16;
				}
				if ((data == 152) || (data == 137) || (data == 136)) {
					if (changeflag == 0) {
						srctiles.y = srctiles.y + (changetiles * 120);
						SDL_RenderCopy(renderer,tiles,&srctiles,&destiles);
					}
				}
				else {
					srctiles.y = srctiles.y + (changetiles * 120);
					SDL_RenderCopy(renderer,tiles,&srctiles,&destiles);
				}
			}
		}
	}

}

void statusbar (SDL_Renderer *renderer,SDL_Texture *tiles,int room[],int lifes,int crosses,TTF_Font *font,uint changetiles) {

	SDL_Rect srcbar = {448,104,13,12};
	SDL_Rect desbar = {0,177,0,0};
	SDL_Color fgcolor = {255,255,255}; /* Color de la fuente, blanco */
	SDL_Texture *score = NULL;
	SDL_Rect desscore = {0,0,0,0};
	int i = 0;

	char data[1];
	char screenname[18];
	int width = 0;
	int height = 0;

	/* Show heart and crosses sprites */
	if (changetiles == 1)
		srcbar.y = 224;
	SDL_RenderCopy(renderer,tiles,&srcbar,&desbar);
	srcbar.x = 461;
	srcbar.w = 12;
	desbar.x = 32;
	SDL_RenderCopy(renderer,tiles,&srcbar,&desbar);

	desmarcador.y = 174;

	for (i=0; i<=2; i++) {
		switch (i) {
			case 0: sprintf(data, "%d", lifes);
									 score = TTF_RenderText_Solid(font, data, fgcolor);
									 desscore.x = 18;
									 break;
			case 1: sprintf(data, "%d", crosses);
									 score = TTF_RenderText_Solid(font, data, fgcolor);
									 desscore.x = 50;
									 break;
			case 2:
				if (room[0] == 1)
					sprintf (screenname, "A prayer of Hope");
				if (room[0] == 2)
					sprintf (screenname, "Tower of the Bell");
				if (room[0] == 3)
					sprintf (screenname, "Wine supplies");
				if (room[0] == 5)
					sprintf (screenname, "Escape !!!");
				if (room[0] == 6)
					sprintf (screenname, "Death is close");
				if (room[0] == 7)
					sprintf (screenname, "Abandoned church");
				if (room[0] == 8)
					sprintf (screenname, "The Altar");
				if (room[0] == 9)
					sprintf (screenname, "Hangman tree");
				if (room[0] == 10)
					sprintf (screenname, "Pestilent Beast");
				if (room[0] == 11)
					sprintf (screenname, "Cave of illusions");
				if (room[0] == 12)
					sprintf (screenname, "Plagued ruins");
				if (room[0] == 13)
					sprintf (screenname, "Catacombs");
				if (room[0] == 14)
					sprintf (screenname, "Hidden garden");
				if (room[0] == 15)
					sprintf (screenname, "Gloomy tunels");
				if (room[0] == 16)
					sprintf (screenname, "Lake of despair");
				if (room[0] == 17)
					sprintf (screenname, "The wheel of faith");
				if (room[0] == 18)
					sprintf (screenname, "Banquet of Death");
				if (room[0] == 19)
					sprintf (screenname, "Underground river");
				if (room[0] == 20)
					sprintf (screenname, "Unexpected gate");
				if (room[0] == 21)
					sprintf (screenname, "Evil church");
				if (room[0] == 22)
					sprintf (screenname, "Tortured souls");
				if (room[0] == 23)
					sprintf (screenname, "Ashes to ashes");
				if (room[0] == 24)
					sprintf (screenname, "Satan !!!");

				score = TTF_RenderText_Solid(font, screenname, fgcolor);
				TTF_SizeText(font, screenname, &width, &height);
				desscore.x = 256 - width;
				break;
		}

		SDL_RenderCopy(renderer,score,NULL,&desscore);
		SDL_DestroyTexture(score);

	}

}
