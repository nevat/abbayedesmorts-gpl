/* game.c */

# include "game.h"

void game(SDL_Window *screen,uint *state,uint *grapset) {

	/* Renderer */
	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(renderer, 256, 192);

	uint stagedata[25][22][32];
	int enemydata[25][7][15];
	uint room[2] = {5,5}; /* Room, previous room */
	uint exit = 0;
	int coord[2] = {0,0}; /* Coord X and Y */
	uint changeflag = 1; /* Screen change */
	uint counter[3] = {0,0,0}; /* Counters */
	uint changetiles=*grapset;

	/* Loading PNG */
	SDL_Texture *tiles = IMG_LoadTexture(renderer,"../graphics/tiles.png");

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

		/* Flip ! */
		SDL_RenderPresent(renderer);

	}

	/* Cleaning */
	SDL_DestroyTexture(tiles);
	SDL_DestroyRenderer(renderer);

}

void loaddata(uint stagedata[][22][32],int enemydata[][7][15]) {

	FILE *datafile = NULL;
	int i = 0;
	int j = 0;
	int k = 0;
	char line[129],temp[3],line2[61];

	/* Loading file */
	datafile = fopen("../data/map.txt, "r");
	fgets (line, 129, datafile);
	fgets (line, 129, datafile);

	/* Cargamos los datos del fichero en el array */
	for (i=0; i<=24; i++) {
		for (j=0; j<=21; j++) {
			for (k=0; k<=31; k++) {
				temp[0] = line[k*4];
				temp[1] = line[(k*4) + 1];
				temp[2] = line[(k*4) + 2];
				sscanf (temp, "%d", &stagedata[i][j][k]);
			}
			fgets (line, 129, datafile);
		}
		fgets (line, 129, datafile);
	}

	/* Cerramos fichero */
	fclose (datafile);

	datafile = fopen("../data/enemies.txt", "r");
	fgets (line2, 61, datafile);
	fgets (line2, 61, datafile);

	/* Cargamos los datos del fichero en el array */
	for (i=0; i<=24; i++) {
		for (j=0; j<7; j++) {
			for (k=0; k<15; k++) {
				temp[0] = line2[k*4];
				temp[1] = line2[(k*4) + 1];
				temp[2] = line2[(k*4) + 2];
				sscanf (temp, "%d", &enemydata[i][j][k]);
			}
			fgets (line2, 61, datafile);
		}
		fgets (line2, 61, datafile);
	}

	fclose (datafile);

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
						if (changetiles == 1)
							srctiles.y = srctiles.y + 120;
						SDL_RenderCopy(renderer,tiles,&srctiles,&destiles);
					}
				}
				else {
					if (changetiles == 1)
						srctiles.y = srctiles.y + 120;
					SDL_RenderCopy(renderer,tiles,&srctiles,&destiles);
				}
			}
		}
	}

}