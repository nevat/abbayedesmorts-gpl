/* loading.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_mixer.h"

void loaddata(uint stagedata[][22][32],int enemydata[][7][15]) {

	FILE *datafile = NULL;
	int i = 0;
	int j = 0;
	int k = 0;
	char line[129],temp[4],line2[61];
	temp[3] = 0;

	/* Loading file */
	datafile = fopen("/usr/share/abbayev2/data/map.txt", "r");
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

	datafile = fopen("/usr/share/abbayev2/data/enemies.txt", "r");
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

void loadingmusic(Mix_Music *bso[],Mix_Chunk *fx[]) {

	/* Musics */
	bso[0] = Mix_LoadMUS("/usr/share/abbayev2/sounds/PrayerofHopeN.ogg");
	bso[1] = Mix_LoadMUS("/usr/share/abbayev2/sounds/AreaIChurchN.ogg");
	bso[2] = Mix_LoadMUS("/usr/share/abbayev2/sounds/GameOverV2N.ogg");
	bso[3] = Mix_LoadMUS("/usr/share/abbayev2/sounds/HangmansTree.ogg");
	bso[4] = Mix_LoadMUS("/usr/share/abbayev2/sounds/AreaIICavesV2N.ogg");
	bso[5] = Mix_LoadMUS("/usr/share/abbayev2/sounds/EvilFightN.ogg");
	bso[6] = Mix_LoadMUS("/usr/share/abbayev2/sounds/AreaIIIHellN.ogg");
	bso[7] = Mix_LoadMUS("/usr/share/abbayev2/sounds/ManhuntwoodN.ogg");

	/* Fxs */
	fx[0] = Mix_LoadWAV ("/usr/share/abbayev2/sounds/shoot.ogg");
	fx[1] = Mix_LoadWAV ("/usr/share/abbayev2/sounds/doorfx.ogg");
	fx[2] = Mix_LoadWAV ("/usr/share/abbayev2/sounds/Item.ogg");
	fx[3] = Mix_LoadWAV ("/usr/share/abbayev2/sounds/jump.ogg");
	fx[4] = Mix_LoadWAV ("/usr/share/abbayev2/sounds/slash.ogg");
	fx[5] = Mix_LoadWAV ("/usr/share/abbayev2/sounds/mechanismn.ogg");
	fx[6] = Mix_LoadWAV ("/usr/share/abbayev2/sounds/onedeathn.ogg");

}