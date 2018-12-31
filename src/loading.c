/* loading.c */

#include "base.h"

void loaddata(uint stagedata[][22][32],int enemydata[][7][15]) {

	FILE *datafile = NULL;
	uint8_t line[129],temp[4],line2[61];
	temp[3] = 0;

	/* Loading stage data file */
	datafile = fopen(DATADIR "/data/map.txt", "r");
	fgets (line, 129, datafile);
	fgets (line, 129, datafile);

	/* Loading data into the array */
	for (uint8_t i=0; i<=24; i++) {
		for (uint8_t j=0; j<=21; j++) {
			for (uint8_t k=0; k<=31; k++) {
				temp[0] = line[k*4];
				temp[1] = line[(k*4) + 1];
				temp[2] = line[(k*4) + 2];
				sscanf (temp, "%d", &stagedata[i][j][k]);
			}
			fgets (line, 129, datafile);
		}
		fgets (line, 129, datafile);
	}

	/* Closing file */
	fclose (datafile);

	/* Loading enemies data file */
	datafile = fopen(DATADIR "/data/enemies.txt", "r");
	fgets (line2, 61, datafile);
	fgets (line2, 61, datafile);

	/* Loading data into the array */
	for (uint8_t i=0; i<=24; i++) {
		for (uint8_t j=0; j<7; j++) {
			for (uint8_t k=0; k<15; k++) {
				temp[0] = line2[k*4];
				temp[1] = line2[(k*4) + 1];
				temp[2] = line2[(k*4) + 2];
				sscanf (temp, "%d", &enemydata[i][j][k]);
			}
			fgets (line2, 61, datafile);
		}
		fgets (line2, 61, datafile);
	}

	/* Closing file */
	fclose (datafile);

}

void loadingmusic(Mix_Music *bso[],Mix_Chunk *fx[]) {

	/* Musics */
	bso[0] = Mix_LoadMUS(DATADIR "/sounds/PrayerofHopeN.ogg");
	bso[1] = Mix_LoadMUS(DATADIR "/sounds/AreaIChurchN.ogg");
	bso[2] = Mix_LoadMUS(DATADIR "/sounds/GameOverV2N.ogg");
	bso[3] = Mix_LoadMUS(DATADIR "/sounds/HangmansTree.ogg");
	bso[4] = Mix_LoadMUS(DATADIR "/sounds/AreaIICavesV2N.ogg");
	bso[5] = Mix_LoadMUS(DATADIR "/sounds/EvilFightN.ogg");
	bso[6] = Mix_LoadMUS(DATADIR "/sounds/AreaIIIHellN.ogg");
	bso[7] = Mix_LoadMUS(DATADIR "/sounds/ManhuntwoodN.ogg");

	/* Fxs */
	fx[0] = Mix_LoadWAV(DATADIR "/sounds/shoot.ogg");
	fx[1] = Mix_LoadWAV(DATADIR "/sounds/doorfx.ogg");
	fx[2] = Mix_LoadWAV(DATADIR "/sounds/Item.ogg");
	fx[3] = Mix_LoadWAV(DATADIR "/sounds/jump.ogg");
	fx[4] = Mix_LoadWAV(DATADIR "/sounds/slash.ogg");
	fx[5] = Mix_LoadWAV(DATADIR "/sounds/mechanismn.ogg");
	fx[6] = Mix_LoadWAV(DATADIR "/sounds/onedeathn.ogg");

}
