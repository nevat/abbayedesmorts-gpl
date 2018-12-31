/* enemies.c */

#include "enemies.h"

void searchenemies (uint room[], struct enem *enemies,uint *changeflag, int enemydata[][7][15]) {

	for (uint8_t y=0; y<7; y++) {
		enemies->type[y] = enemydata[room[0]][y][0];
		enemies->x[y] = enemydata[room[0]][y][1];
		enemies->y[y] = enemydata[room[0]][y][2];
		enemies->direction[y] = enemydata[room[0]][y][3];
		enemies->tilex[y] = enemydata[room[0]][y][4];
		enemies->tiley[y] = enemydata[room[0]][y][5];
		enemies->animation[y] = enemydata[room[0]][y][6];
		enemies->limleft[y] = enemydata[room[0]][y][7];
		enemies->limright[y] = enemydata[room[0]][y][8];
		enemies->speed[y] = enemydata[room[0]][y][9];
		enemies->fire[y] = enemydata[room[0]][y][10];
		enemies->adjustx1[y] = enemydata[room[0]][y][11];
		enemies->adjustx2[y] = enemydata[room[0]][y][12];
		enemies->adjusty1[y] = enemydata[room[0]][y][13];
		enemies->adjusty2[y] = enemydata[room[0]][y][14];
	}

	*changeflag -= 1;

}

void drawenemies (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,Mix_Chunk *fx[],uint changetiles) {

	SDL_Rect srctile = {0,0,16,16};
	SDL_Rect destile = {0,0,16,16};

	for (uint8_t i=0; i<7; i++) {
		if ((enemies->type[i] > 0) && (enemies->type[i] < 16)) {
			if ((enemies->type[i] == 3) || (enemies->type[i] == 5) || (enemies->type[i] == 15)) {
				srctile.h = 24;
				destile.h = 24;
			}
			else {
				srctile.h = 16;
				destile.h = 16;
			}
			if (enemies->type[i] == 6) {
				srctile.x = enemies->tilex[i] + (enemies->animation[i] * 24);
				srctile.w = 24;
				destile.w = 24;
			}
			else {
				if (enemies->type[i] == 15)
					srctile.x = enemies->tilex[i] + (enemies->animation[i] * 16);
				else
					srctile.x = enemies->tilex[i] + (enemies->animation[i] * 16);
				srctile.w = 16;
				destile.w = 16;
			}
			srctile.y = enemies->tiley[i] + (changetiles * 120);
			destile.x = enemies->x[i];
			destile.y = enemies->y[i];
			if (((enemies->type[i] != 13) && (enemies->type[i] != 14)) || (((enemies->type[i] == 13) || (enemies->type[i] == 14)) && (enemies->y[i] < enemies->limright[i] - 8)))
				SDL_RenderCopy(renderer,tiles,&srctile,&destile);

			if (enemies->type[i] == 13) { /* Water movement */
				if (((enemies->speed[i] > 30) && (enemies->speed[i] < 40)) || ((enemies->y[i] == enemies->limright[i] - 16) && (enemies->direction[i] == 1))) {
					srctile.x = 368;
					srctile.y = 32 + (changetiles * 120);
					srctile.h = 8;
					destile.h = 8;
					destile.y = enemies->limright[i];
					SDL_RenderCopy(renderer,tiles,&srctile,&destile);
					if ((enemies->speed[i] > 30) && (enemies->speed[i] < 40))
						Mix_PlayChannel(-1, fx[4], 0);;
				}
				if (((enemies->speed[i] > 39) && (enemies->speed[i] < 45)) || ((enemies->y[i] == enemies->limright[i] - 10) && (enemies->direction[i] == 1))) {
					srctile.x = 384;
					srctile.y = 32 + (changetiles * 120);
					srctile.h = 8;
					destile.h = 8;
					destile.y = enemies->limright[i];
					SDL_RenderCopy(renderer,tiles,&srctile,&destile);
				}
			}
		}
		/* Draw smoke */
		if (enemies->type[i] == 88) {
			if ((enemies->speed[i] < 10) || ((enemies->speed[i] > 19) && (enemies->speed[i] < 30)) || ((enemies->speed[i] > 39) && (enemies->speed[i] < 50)))
				srctile.x = 696;
			else
				srctile.x = 728;
			srctile.y = 0 + (changetiles * 120);
			srctile.w = 32;
			srctile.h = 48;
			destile.w = 32;
			destile.h = 48;
			destile.x = enemies->x[i];
			destile.y = enemies->y[i];
			SDL_RenderCopy(renderer,tiles,&srctile,&destile);
			if (enemies->speed[i] == 5)
				Mix_PlayChannel(-1, fx[6], 0);;
		}

	}

}

void movenemies (struct enem *enemies,uint stagedata[][22][32],uint counter[],float proyec[],struct hero jean,Mix_Chunk *fx[]) {

	for (uint8_t i=0; i<7; i++) {
		if ((enemies->type[i] > 0) && (enemies->type[i] < 10)) {
			if (enemies->direction[i] == 0) { /* Go right */
				if ((enemies->x[i] + 1) < enemies->limright[i])
					enemies->x[i] += enemies->speed[i] * 0.10;
				else {
					enemies->direction[i] = 1;
					if (enemies->type[i] != 2) {
						if (enemies->type[i] == 6)
							enemies->tilex[i] -= 48;
						else
							enemies->tilex[i] -= 32;
					}
				}
			}
			if (enemies->direction[i] == 1) { /* Go left */
				if ((enemies->x[i] - 1) > enemies->limleft[i])
					enemies->x[i] -= enemies->speed[i] * 0.10;
				else {
					enemies->direction[i] = 0;
					if (enemies->type[i] != 2) {
						if (enemies->type[i] == 6)
							enemies->tilex[i] += 48;
						else
							enemies->tilex[i] += 32;
					}
				}
			}
			if (enemies->direction[i] == 2) { /* Go up */
				if ((enemies->y[i] - 1) > enemies->limleft[i])
					enemies->y[i] -= enemies->speed[i] * 0.10;
				else {
					enemies->direction[i] = 3;
					if (enemies->type[i] == 4)
						enemies->tilex[i] += 32;
					if (enemies->type[i] == 5)
						enemies->tilex[i] += 16;
				}
			}
			if (enemies->direction[i] == 3) { /* Go down */
				if ((enemies->y[i] + 1) < enemies->limright[i])
					enemies->y[i] += enemies->speed[i] * 0.10;
				else {
					enemies->direction[i] = 2;
					if (enemies->type[i] == 4)
					enemies->tilex[i] -= 32;
					if (enemies->type[i] == 5)
					enemies->tilex[i] -= 16;
				}
			}

		}

		/* Animation */
		if (enemies->type[i] < 10) {
			if (((counter[0] == 1) || (counter[0] == 11) || (counter[0] == 21)) && (enemies->type[i] != 5)){
				if (enemies->animation[i] == 0)
					enemies->animation[i] = 1;
				else
					enemies->animation[i] = 0;
			}
		}

		/* Enemy's shoots */
		if ((enemies->type[i] == 15) || (enemies->type[i] == 11)) {
			if (enemies->type[i] == 15) { /* Move enemies looking at Jean */
				if ((jean.x + 23) < (enemies->x[i]) && (enemies->speed[i] == 0)) {
					enemies->tilex[i] = 256;
					enemies->direction[i] = 1;
				}
				if ((jean.x > (enemies->x[i] + 16)) && (enemies->speed[i] == 0)) {
					enemies->tilex[i] = 288;
					enemies->direction[i] = 0;
				}
			}
			if (i == 0) {
				if (enemies->speed[i] < 55) {
					enemies->speed[i] ++;
					enemies->animation[i] = 1;
				}
				else
					enemies->animation[i] = 0;
			}
			else {
				enemies->speed[i] = enemies->speed[0];
				enemies->animation[i] = enemies->animation[0];
			}
			/* Enable fire */
			if (enemies->speed[i] == 50) {
				for (uint8_t n=0; n<=4; n+=2) {
					if (proyec[n] == 0) {
						enemies->fire[i] = 1;
						proyec[n] = enemies->x[i] - 8;
						if (n > 0)
							proyec[n+1] = i;
						n = 5;
						Mix_PlayChannel(-1, fx[0], 0);;
					}
				}
			}
		}

		if (enemies->type[i] == 12) { /* Water drop */
			if (enemies->speed[i] < 60)
				enemies->speed[i] ++; /* Using speed like counter */
			enemies->tilex[i] = 640 + ((enemies->speed[i] / 30) * 16);
			if (enemies->tilex[i] == 672) {
				if (enemies->y[i] < (enemies->limright[i] -8))
					enemies->y[i] += 0.9;
				else { /* Reboot drop */
					enemies->y[i] = enemies->limleft[i]; /* Using limleft like drop start */
					enemies->speed[i] = 0;
					enemies->tilex[i] = 640;
				}
			}
		}

		if ((enemies->type[i] == 13) || (enemies->type[i] == 14)) { /* Fireballs & fishes */
			if ((counter[0] == 1) || (counter[0] == 11) || (counter[0] == 21)) {
				if (enemies->animation[i] == 0)
					enemies->animation[i] = 1;
				else
					enemies->animation[i] = 0;
			}
			if (enemies->direction[i] == 0) { /* Fireball going up */
				if (enemies->y[i] == (enemies->limright[i])) {
					if (enemies->speed[i] < 45) /* Temp */
						enemies->speed[i] ++;
					else
						enemies->y[i] --;
				}
				else {
					if (enemies->y[i] > (enemies->limleft[i] + 16))
						enemies->y[i] -= 2;
					else
						enemies->y[i] --;
				}
				if (enemies->y[i] == enemies->limleft[i] + 8)
					enemies->direction[i] = 1;
			}
			else {
				if (enemies->y[i] != enemies->limright[i])
					enemies->y[i] += 2;
				else { /* Reboot */
					enemies->speed[i] = 0;
					enemies->direction[i] = 0;
				}
			}
		}

	}
}

void plants (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles) {

	SDL_Rect srctile = {0,40,16,16};
	SDL_Rect destile = {0,0,16,16};
	SDL_Rect srcfire = {660,32,4,4};
	SDL_Rect desfire = {0,0,4,4};

	/* Animation */
	for (uint8_t n=1; n<4; n++) {
		if (enemies->speed[n] < (140 + ((n-1) * 30)))
			enemies->speed[n] ++;
		else
			enemies->speed[n] = 0;

		if (enemies->speed[n] < (100 + ((n-1) * 30)))
			enemies->animation[n] = 0;
		else
			enemies->animation[n] = 1;

		srctile.x = 384 + (enemies->animation[n] * 16);
		srctile.y = 40 + (changetiles * 120);
		destile.x = enemies->x[n];
		destile.y = enemies->y[n];

		SDL_RenderCopy(renderer,tiles,&srctile,&destile);
}

	/* Init fire */
	for (uint8_t n=1; n<4; n++) {
		if (enemies->speed[n] == 135 + ((n-1) * 30)) {
			int16_t r = (n-1) * 4;
			proyec[r] = enemies->x[n] - 1;
			proyec[r+1] = enemies->y[n] - 1;
			proyec[r+2] = enemies->x[n] + 16;
			proyec[r+3] = enemies->y[n] - 1;
			Mix_PlayChannel(-1, fx[0], 0);
		}
	}

	/* Move fires */
	for (uint8_t n=1; n<4; n++) {
		uint16_t r = (n-1) * 4;
		if (proyec[r] > 0) {
			if (enemies->direction[n] == 0) {
				if (enemies->y[n] - proyec[r+1] < 24) {
					proyec[r+1] --;
					proyec[r+3] --;
				}
				else
					enemies->direction[n] = 1;
				if (counter[0] % 2 == 0) {
					proyec[r] --;
					proyec[r+2] ++;
				}
			}
			if ((enemies->direction[n]>0) && (enemies->direction[n] < 6)) {
				proyec[r] --;
				proyec[r+2] ++;
				enemies->direction[n] ++;
			}
			if (enemies->direction[n] == 6) {
				if (proyec[r+1] < enemies->limleft[n]) {
					proyec[r+1] += 2;
					if (counter[0] % 2 == 0)
						proyec[r] --;
				}
				else {
					proyec[r] = 0;
					proyec[r+1] = 0;
				}
				if ((proyec[r+3] < enemies->limright[n]) && (proyec[r+3] > 7) && (proyec[r+2] < 240)) {
					proyec[r+3] += 2;
					if (counter[0] % 2 == 0)
						proyec[r+2] ++;
				}
				else {
					proyec[r+2] = 0;
					proyec[r+3] = 0;
				}
			}
			if ((proyec[r] == 0) && (proyec[r+2] == 0))
				enemies->direction[n] = 0;
		}

	}

	/* Draw fire */
	for (uint8_t n=1; n<4; n++) {
		uint16_t r = (n-1) * 4;
		if (proyec[r] > 0) {
			srcfire.y = 32 + (changetiles * 120);
			desfire.x = proyec[r];
			desfire.y = proyec[r+1];
			SDL_RenderCopy(renderer,tiles,&srcfire,&desfire);
		}
		if (proyec[r+2] > 0) {
			srcfire.y = 32 + (changetiles * 120);
			desfire.x = proyec[r+2];
			desfire.y = proyec[r+3];
			SDL_RenderCopy(renderer,tiles,&srcfire,&desfire);
		}

	}

}

void crusaders (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],uint room[],uint changetiles) {

	SDL_Rect srctile = {64,64,16,24};
	SDL_Rect destile = {0,0,16,24};

	for (uint8_t i=0; i<7; i++) {

		/* Init values */
		if (enemies->type[i] == 17) {
			enemies->type[i] = 16;
			enemies->x[i] = -52 - (24 * i);
			enemies->y[i] = 136;
		}

		/* Mover & animation */
		/* Jump when needed */
		if (room[0] == 5) {
			if ((enemies->x[i] == 146) || (enemies->x[i] == 160))
				(enemies->fire[i] = 1); /* Using fire as jump flag */
		}
		if (room[0] == 6) {
			if ((enemies->x[i] == 76) || (enemies->x[i] == 124.75) || (enemies->x[i] == 155))
				enemies->fire[i] = 2;
			if ((enemies->x[i] == 208.25) || (enemies->x[i] == 220.50))
				enemies->fire[i] = 1; /* Using fire as jump flag */
		}
		if (room[0] == 24) {
			if ((enemies->x[i] == 144) || (enemies->x[i] == 152) || (enemies->x[i] == 160) || (enemies->x[i] == 168))
				(enemies->fire[i] = 1);
		}

		/* Jump */
		if (enemies->fire[i] == 1) { /* Short jump */
			if (enemies->speed[i] < 23) {
				enemies->speed[i] += 2;
				enemies->y[i] -= 1;
				enemies->animation[i] = 0;
			}
			else {
				enemies->y[i] += 1;
				enemies->speed[i] += 2;
				enemies->animation[i] = 0;
			}
			if (enemies->speed[i] == 32) { /* Touching ground, reboot flags */
				enemies->speed[i] = 0;
				enemies->fire[i] = 0;
			}
		}
		if (enemies->fire[i] == 2) { /* Long jump */
			if (enemies->speed[i] < 8) {
				enemies->speed[i] += 1;
				enemies->y[i] -= 1;
				enemies->animation[i] = 0;
			}
			else {
				enemies->y[i] += 1;
				enemies->speed[i] += 1;
				enemies->animation[i] = 0;
			}
			if (enemies->speed[i] == 16) {
				enemies->fire[i] = 0;
				enemies->speed[i] = 0;
			}
		}

		if (enemies->animation[i] < 13)
			enemies->animation[i] ++;
		else
			enemies->animation[i] = 0;

		/* Fall */
		if (room[0] == 5) {
			if (enemies->x[i] > 206) {
				if (enemies->speed[i] < 8) {
					enemies->speed[i] ++;
					enemies->y[i]+= 2;
					enemies->animation[i] = 0;
				}
			}
		}

		/* Movement */
		if ((room[0] == 6) && (enemies->speed[i] > 0))
			enemies->x[i] += 0.75;
		else
			enemies->x[i] += 0.5;

		/* Draw */
		if ((enemies->x[i] > -8) && (enemies->x[i] < 257)) {
			srctile.x = 64 + (16 * (enemies->animation[i] / 7));
			srctile.y = 64 + (changetiles * 120);
			destile.x = enemies->x[i];
			destile.y = enemies->y[i];
			SDL_RenderCopyEx(renderer,tiles,&srctile,&destile,0,0,1);
		}

	}

}

void death (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],uint stagedata[][22][32],Mix_Chunk *fx[],uint changetiles) {

	SDL_Rect srcaxe = {0,56,16,16};
	SDL_Rect desaxe = {0,0,16,16};
	SDL_Rect srctile = {0,88,32,32};
	SDL_Rect destile = {0,8,32,32};

	if (enemies->speed[0] < 60)
		enemies->speed[0]++;
	else
		enemies->speed[0] = 0;

	/* Movement */
	if (enemies->direction[0] == 0) { /* Left mov. */
		if (enemies->x[0] > enemies->limleft[0])
			enemies->x[0] --;
		else
			enemies->direction[0] = 1;
	}
	if (enemies->direction[0] == 1) { /* Right mov. */
		if (enemies->x[0] < enemies->limright[0])
			enemies->x[0] ++;
		else
			enemies->direction[0] = 0;
	}

	/* Init axes */
	if ((enemies->speed[0] == 45) && (enemies->x[0] > 48)) {
		for (uint8_t x=0; x<8; x+=2) {
			if (proyec[x] == 0) {
				proyec[x] = enemies->y[0] + 35;
				proyec[x+1] = enemies->x[0];
				x = 9;
				Mix_PlayChannel(-1, fx[0], 0);
			}
		}
	}

	/* Animation */
	if (enemies->speed[0] < 45) {
		if ((counter[0] == 1) || (counter[0] == 11) || (counter[0] == 21)) {
			if (enemies->animation[0] == 0)
				enemies->animation[0] = 1;
			else
				enemies->animation[0] = 0;
		}
	}
	else {
		if (enemies->x[0] > 48) {
			if (enemies->speed[0] < 55)
				enemies->animation[0] = 2;
			else
				enemies->animation[0] = 0;
		}
	}

	/* Draw */
	srctile.x = enemies->animation[0] * 32;
	srctile.y = 88 + (changetiles * 120);
	destile.x = enemies->x[0];
	SDL_RenderCopyEx(renderer,tiles,&srctile,&destile,0,0,enemies->direction[0]);

	/* Axes movement */
	for (uint8_t n=0; n<8; n+=2) {

		if (proyec[n] > 0) {
			/* Locating axe */
			int8_t x = proyec[n+1] / 8;
			int8_t y = proyec[n] / 8;

			/* Touching ground, take out & cleaning */
			if (y == 20) {
				proyec[n] = 0;
				proyec[n+1] = 0;
			}
			else {
				/* Touching a solid tile ? Move */
				if (((stagedata[18][y+2][x] != 73) && (stagedata[18][y+2][x] != 75)) && ((stagedata[18][y+2][x+1] != 73) && (stagedata[18][y+2][x+1] != 75)))
					proyec[n] ++;
				else
					proyec[n+1] --;
			}
			if (x == 0) {
				proyec[n] = 0;
				proyec[n+1] = 0;
			}
		}
	}

	/* Draw axe */
	for (uint8_t n=0; n<8; n+=2) {
		if (proyec[n] > 0) {
			desaxe.x = proyec[n+1];
			desaxe.y = proyec[n];
			/* Rotation */
			srcaxe.x = 576 + (16 * (counter[2] / 2));
			srcaxe.y = 56 + (changetiles * 120);
			SDL_RenderCopy(renderer,tiles,&srcaxe,&desaxe);
		}
	}

}

void dragon (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles) {

	SDL_Rect srctile = {456,72,16,8};
	SDL_Rect destile = {120,40,16,8};

	/* Draw front paw */
	srctile.y = 72 + (changetiles * 120);
	SDL_RenderCopy(renderer,tiles,&srctile,&destile);

	/* Head position */
	if (enemies->speed[0] < 150) /* Using speed as temp */
		enemies->speed[0] ++;
	else
		enemies->speed[0] = 0;

	if (enemies->speed[0] < 110)
		enemies->animation[0] = 0;
	else
		enemies->animation[0] = 1;

	/* Draw head */
	srctile.w = 32;
	srctile.h = 24;
	srctile.x = 416;
	srctile.y = 56 + (changetiles * 120);;
	destile.w = 32;
	destile.h = 24;
	destile.x = 120;
	destile.y = 8 + (enemies->animation[0] * 3);
	SDL_RenderCopy(renderer,tiles,&srctile,&destile);

	/* Draw snout */
	srctile.h = 16;
	srctile.x = 448;
	destile.h = 16;
	destile.y += 24;
	SDL_RenderCopy(renderer,tiles,&srctile,&destile);

	/* Spit fire */
	if (enemies->animation[0] == 1) {
		if ((enemies->speed[0] == 115) || (enemies->speed[0] == 125) || (enemies->speed[0] == 135) || (enemies->speed[0] == 145))
			enemies->direction[0] = 1;
		if ((enemies->speed[0] == 110) || (enemies->speed[0] == 120) || (enemies->speed[0] == 130) || (enemies->speed[0] == 140))
			enemies->direction[0] = 0;
		srctile.x = 504 + (enemies->direction[0] * 48);
		srctile.y = 56 + (changetiles * 120);
		srctile.w = 24;
		srctile.h = 24;
		destile.x = 119 + (enemies->direction[0] * 16);
		destile.y = 51;
		destile.w = 24;
		destile.h = 24;
		SDL_RenderCopy(renderer,tiles,&srctile,&destile);
		srctile.x = 480 + (enemies->direction[0] * 48);
		destile.y = 75;
		destile.x = 127;
		SDL_RenderCopy(renderer,tiles,&srctile,&destile);
	}

	/* Make fire in ground */
	if (enemies->speed[0] == 150) {
		Mix_PlayChannel(-1, fx[0], 0);
		for (uint8_t n=0; n<16; n+=8) {
			if (proyec[n] == 0) {
				proyec[n] = 120;
				proyec[n+1] = 464;
				proyec[n+2] = 128;
				proyec[n+3] = 472;
				proyec[n+4] = 136;
				proyec[n+5] = 472;
				proyec[n+6] = 144;
				proyec[n+7] = 464;
				n = 16;
			}
		}
	}

	/* Fire movement & animation */
	for (uint8_t n=0; n<16; n+=2) {
		if (proyec[n] > 0) {
			if ((n < 3) || ((n>7) && (n<11))) {
				if (proyec[n] > 48) {
					proyec[n] -= 0.3;
					/* Animation */
					if (counter[0] % 8 == 0) {
						if (proyec[n+1] == 464)
							proyec[n+1] = 472;
						else
							proyec[n+1] = 464;
						}
				}
				else {
					proyec[n] = 0;
					proyec[n+1] = 0;
				}
			}
			else {
				if (proyec[n] < 248) {
					proyec[n] += 0.3;
					/* Animation */
					if (counter[0] % 8 == 0) {
						if (proyec[n+1] == 464)
							proyec[n+1] = 472;
						else
							proyec[n+1] = 464;
					}
				}
				else {
					proyec[n] = 0;
					proyec[n+1] = 0;
				}
			}
			srctile.x = proyec[n+1];
			srctile.y = 0 + (changetiles * 120);
			srctile.w = 8;
			srctile.h = 8;
			destile.x = proyec[n];
			destile.y = 88;
			destile.w = 8;
			destile.h = 8;
			SDL_RenderCopy(renderer,tiles,&srctile,&destile);
		}
	}
}

void satan (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles) {

	SDL_Rect srctile = {192,88,32,24};
	SDL_Rect destile = {192,0,32,24};
	SDL_Rect srcfire = {656,32,4,4};
	SDL_Rect desfire = {0,0,4,4};

	/* Movement */
	if (enemies->direction[0] == 0) { /* Go up */
		if (enemies->y[0] > enemies->limleft[0])
			enemies->y[0] -= 0.5;
		else
			enemies->direction[0] = 1;
	}
	if (enemies->direction[0] == 1) { /* Go down */
		if (enemies->y[0] < enemies->limright[0])
			enemies->y[0] += 0.5;
		else
			enemies->direction[0] = 0;
	}

	/* Animation */
	if ((counter[0] == 1) || (counter[0] == 11) || (counter[0] == 21)) {
		if (enemies->animation[0] == 0)
			enemies->animation[0] = 1;
		else
			enemies->animation[0] = 0;
	}

	/* Draw */
	srctile.x = 192 + (enemies->animation[0] * 64);
	srctile.y = 88 + (changetiles * 120);
	destile.y = enemies->y[0];
	SDL_RenderCopy(renderer,tiles,&srctile,&destile);
	srctile.x = 224 + (enemies->animation[0] * 64);
	destile.y = enemies->y[0] + 24;
	SDL_RenderCopy(renderer,tiles,&srctile,&destile);

	/* Init shoots */
	if (((enemies->y[0] == 37) && (enemies->direction[0] == 0)) || ((enemies->y[0] == 20) && (enemies->direction[0] == 1)) || ((enemies->y[0] == 62) && (enemies->direction[0] == 0))) {
		int8_t x = enemies->y[0];
		uint8_t r = 0;
		switch (x) {
			/*
			case 20:
				r = 0;
				break;
			*/
			case 37:
				r = 6;
				break;
			case 62:
				r = 12;
				break;
		}
		if ((proyec[r] == 0) && (proyec[r+2] == 0) && (proyec[r+4] == 0)) {
			proyec[r] = 190;
			proyec[r+1] = enemies->y[0] + 15;
			proyec[r+2] = 190;
			proyec[r+3] = enemies->y[0] + 10;
			proyec[r+4] = 190;
			proyec[r+5] = enemies->y[0] + 20;
			Mix_PlayChannel(-1, fx[0], 0);
		}
	}

	/* Move shoots */
	for (uint8_t n=0; n<17; n+=2) {
		if ((proyec[n] > 16) && ((proyec[n+1] > 8) && (proyec[n+1] < 160))) {
			proyec[n] -= 2;
			if ((n+1 == 3) || (n+1 == 9) || (n+1 == 15))
				proyec[n+1] -= 0.75;
			if ((n+1 == 5) || (n+1 == 11) || (n+1 == 17))
				proyec[n+1] += 0.75;
			srcfire.y = 32 + (changetiles * 120);
			desfire.x = proyec[n];
			desfire.y = proyec[n+1];
			SDL_RenderCopy(renderer,tiles,&srcfire,&desfire);
		}
		else {
			proyec[n] = 0;
			proyec[n+1] = 0;
		}
	}

}

void fireball (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],struct hero jean, int stagedata[][22][32], uint changetiles) {

	SDL_Rect srctile = {576,40,16,16};
	SDL_Rect destile = {0,0,16,16};

	/* Current tile X and Y */
	int16_t x = enemies->x[0] / 8;
	int16_t y = enemies->y[0] / 8;

	/* Follow Jean */
	if (enemies->x[0] < jean.x + 1) {
		enemies->direction[0] = 1;
		if (((stagedata[11][y][x+2] == 0) || (stagedata[11][y][x+2] > 99) || (stagedata[11][y][x+2] == 37)) && ((stagedata[11][y+1][x+2] == 0) || (stagedata[11][y+1][x+2] > 99) || (stagedata[11][y+1][x+2] == 37))) {
			if (enemies->fire[0] == 0)
				enemies->x[0] += 0.3;
		}
	}
	if (enemies->x[0] > jean.x - 1) {
		enemies->direction[0] = 0;
		if (((stagedata[11][y][x-1] == 0) || (stagedata[11][y][x-1] > 99) || (stagedata[11][y][x-1] == 37)) && ((stagedata[11][y+1][x-1] == 0) || (stagedata[11][y+1][x-1] > 99) || (stagedata[11][y+1][x-1] == 37))) {
			if (enemies->fire[0] == 0)
				enemies->x[0] -= 0.3;
		}
	}
	if (enemies->y[0] < jean.y + 1) {
		if (((stagedata[11][y+2][x] == 0) || (stagedata[11][y+2][x] > 99)) && ((stagedata[11][y+2][x+1] == 0) || (stagedata[11][y+2][x+1] > 99))) {
			if (enemies->fire[0] == 0)
				enemies->y[0] +=0.3;
			enemies->fire[0] = 0;
		}
		else {
			enemies->fire[0] = 1; /* Using fire like flag */
			if (enemies->x[0] > jean.x -1)
				enemies->x[0] -=0.3;
			else
				enemies->x[0] +=0.3;
		}
	}
	if (enemies->y[0] > jean.y + 1) {
		if (((stagedata[11][y-1][x] == 0) || (stagedata[11][y-1][x] > 99) || (stagedata[11][y-1][x] == 37)) && ((stagedata[11][y-1][x+1] == 0) || (stagedata[11][y-1][x+1] > 99) || (stagedata[11][y-1][x+1] == 37))) {
			enemies->y[0] -=0.3;
			enemies->fire[0] = 0;
		}
		else {
			enemies->fire[0] = 1; /* Usando fire como flag */
			if (enemies->x[0] > jean.x - 1)
				enemies->x[0] -=0.3;
			else
				enemies->x[0] +=0.3;
		}
	}

	/* Animation */
	enemies->animation[0] = counter[0] / 15;

	/* Draw */
	srctile.x = 576 + (enemies->animation[0] * 16);
	srctile.y = 40 + (changetiles * 120);
	destile.x = enemies->x[0];
	destile.y = enemies->y[0];
	SDL_RenderCopyEx(renderer,tiles,&srctile,&destile,0,0,enemies->direction[0]);

}
