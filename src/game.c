/* game.c */

# include "game.h"

void game(SDL_Window *screen,SDL_Renderer *renderer,uint *state,uint *grapset) {

	uint stagedata[25][22][32];
	int enemydata[25][7][15];

	/* Loading PNG */
	SDL_Texture *tiles = IMG_LoadTexture(renderer,"../graphics/tiles.png");

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