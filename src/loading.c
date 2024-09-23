/* loading.c */

#include "loading.h"

void loaddata(uint stagedata[][22][32],int enemydata[][7][15]) {

	FILE *datafile = NULL;
	char line[129],temp[4],line2[61];
	temp[3] = 0;

	/* Loading stage data file */
	datafile = fopen(DATADIR "/data/map.txt", "r");
	fgets (line, 129, datafile); /* skip header */
	fgets (line, 129, datafile);

	/* Loading data into the array */
	for (int i=0; i<=24; i++) {
		for (int j=0; j<=21; j++) {
			for (int k=0; k<=31; k++) {
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
	fgets (line2, 61, datafile);  /* skip header */
	fgets (line2, 61, datafile);

	/* Loading data into the array */
	for (int i=0; i<=24; i++) {
		for (int j=0; j<7; j++) {
			for (int k=0; k<15; k++) {
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
	bso[0] = loadmus("PrayerofHopeN");
	bso[1] = loadmus("AreaIChurchN");
	bso[2] = loadmus("GameOverV2N");
	bso[3] = loadmus("HangmansTree");
	bso[4] = loadmus("AreaIICavesV2N");
	bso[5] = loadmus("EvilFightN");
	bso[6] = loadmus("AreaIIIHellN");
	bso[7] = loadmus("ManhuntwoodN");

	/* Fxs */
	fx[0] = loadwav("shoot");
	fx[1] = loadwav("doorfx");
	fx[2] = loadwav("Item");
	fx[3] = loadwav("jump");
	fx[4] = loadwav("slash");
	fx[5] = loadwav("mechanismn");
	fx[6] = loadwav("onedeathn");

}

static char* makepath(const char *folder, const char *file, const char *ext) {
	static int len1 = strlen(DATADIR) + 1; // sep
	int len2 = strlen(folder) + 1; // sep
	int len3 = strlen(file) + 1; // sep
	int len4 = strlen(ext) + 1; // null

	char *result = malloc(len1+len2+len3+len4);
	sprintf(result, "%s/%s/%s.%s", DATADIR, folder, file, ext);

	return result;
}

SDL_Texture *loadtexture(SDL_Renderer *renderer, const char *file) {
	char *path = makepath("graphics", file, "png");

	SDL_Texture *result = IMG_LoadTexture(renderer, path);

	free(path);
	return result;
}

Mix_Music *loadmus(const char *file) {
	char *path = makepath("sounds", file, "ogg");

	Mix_Music *result = Mix_LoadMUS(path);

	free(path);
	return result;
}

Mix_Chunk *loadwav(const char *file) {
	char *path = makepath("sounds", file, "ogg");

	Mix_Chunk *result = Mix_LoadWAV(path);

	free(path);
	return result;
}
