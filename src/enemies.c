/* enemies.c */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_mixer.h>
#include "structs.h"

void searchenemies (uint room[], struct enem *enemies, int *changeflag, int enemydata[][7][15]) {

  int y = 0;

  for (y=0; y<7; y++) {
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
  *changeflag = 0;

}

void drawenemies (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,Mix_Chunk *fx[],uint changetiles) {

  SDL_Rect srctile = {0,0,16,16};
  SDL_Rect destile = {0,0,0,0};

  int i = 0;

  for (i=0; i<7; i++) {
		if ((enemies->type[i] > 0) && (enemies->type[i] < 16)) {
	  	if ((enemies->type[i] == 3) || (enemies->type[i] == 5) || (enemies->type[i] == 15))
				srctile.h = 24;
	  	else
				srctile.h = 16;
	  	if (enemies->type[i] == 6) {
				srctile.x = enemies->tilex[i] + (enemies->animation[i] * 24);
				srctile.w = 24;
	  	}
	  	else {
				if (enemies->type[i] == 15)
					srctile.x = enemies->tilex[i] + (enemies->animation[i] * 16);
				else
					srctile.x = enemies->tilex[i] + (enemies->animation[i] * 16);
				srctile.w = 16;
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
				  destile.y = enemies->limright[i];
					SDL_RenderCopy(renderer,tiles,&srctile,&destile);
					if ((enemies->speed[i] > 30) && (enemies->speed[i] < 40))
						Mix_PlayChannel(-1, fx[4], 0);;
				}
				if (((enemies->speed[i] > 39) && (enemies->speed[i] < 45)) || ((enemies->y[i] == enemies->limright[i] - 10) && (enemies->direction[i] == 1))) {
			  	srctile.x = 384;
					srctile.y = 32 + (changetiles * 120);
				  srctile.h = 8;
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
			destile.x = enemies->x[i];
			destile.y = enemies->y[i];
			SDL_RenderCopy(renderer,tiles,&srctile,&destile);
			if (enemies->speed[i] == 5)
				Mix_PlayChannel(-1, fx[6], 0);;
		}

	}

}

void movenemies (struct enem *enemies,uint stagedata[][22][32],uint counter[],float proyec[],struct hero jean,Mix_Chunk *fx[]) {

  int i = 0;
  int tilex = 0;
  int tiley = 0;
  int n = 0;

  for (i=0; i<7; i++) {
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
			for (n=0; n<=4; n+=2) {
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

void plantas (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int counter[], float proyec[], Mix_Chunk *fx[]) {

  SDL_Rect srctile = {0,40,16,16};
  SDL_Rect destile = {0,0,16,16};
  SDL_Rect srcfire = {660,32,4,4};
  SDL_Rect desfire = {0,0,4,4};
  int n = 0;
  int r = 0;

  /* Animacion */

  for (n=1; n<4; n++) {
	if (enemies->speed[n] < (140 + ((n-1) * 30)))
	  enemies->speed[n] ++;
	else
	  enemies->speed[n] = 0;

	if (enemies->speed[n] < (100 + ((n-1) * 30)))
	  enemies->animation[n] = 0;
	else
	  enemies->animation[n] = 1;

	srctile.x = 384 + (enemies->animation[n] * 16);
	destile.x = enemies->x[n];
	destile.y = enemies->y[n];

	SDL_BlitSurface(tiles, &srctile, ventana, &destile);
  }

  /* Iniciar fire */
  for (n=1; n<4; n++) {
		if (enemies->speed[n] == 135 + ((n-1) * 30)) {
	 	 r = (n-1) * 4;
	  	proyec[r] = enemies->x[n] - 1;
	  	proyec[r+1] = enemies->y[n] - 1;
	  	proyec[r+2] = enemies->x[n] + 16;
	  	proyec[r+3] = enemies->y[n] - 1;
	  	Mix_PlayChannel(-1, fx[0], 0);
		}
  }

  /* Mover fires */
  for (n=1; n<4; n++) {
	r = (n-1) * 4;
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

 /* Dibujar fire */
 for (n=1; n<4; n++) {
	r = (n-1) * 4;
	if (proyec[r] > 0) {
	  desfire.x = proyec[r];
	  desfire.y = proyec[r+1];
	  SDL_BlitSurface(tiles, &srcfire, ventana, &desfire);
	}
	if (proyec[r+2] > 0) {
	  desfire.x = proyec[r+2];
	  desfire.y = proyec[r+3];
	  SDL_BlitSurface(tiles, &srcfire, ventana, &desfire);
	}

  }

}

void cruzados (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int counter[], int habitacion) {

	SDL_Rect srctile = {96,64,16,24};
	SDL_Rect destile = {0,0,16,24};
	int i = 0;

	for (i=0; i<7; i++) {

		/* Iniciar valor */
		if (enemies->type[i] == 17) {
			enemies->type[i] = 16;
			enemies->x[i] = -52 - (24 * i);
			enemies->y[i] = 136;
		}

		/* Mover y animar personaje */
		/* Saltar al llegar a ciertos puntos */
		if (habitacion == 5) {
			if ((enemies->x[i] == 146) || (enemies->x[i] == 160))
				(enemies->fire[i] = 1); /* Usando el fire como flag de salto */
		}
		if (habitacion == 6) {
			if ((enemies->x[i] == 76) || (enemies->x[i] == 124.75) || (enemies->x[i] == 155))
				enemies->fire[i] = 2;
			if ((enemies->x[i] == 208.25) || (enemies->x[i] == 220.50))
				enemies->fire[i] = 1; /* Usando el fire como flag de salto */
		}
		if (habitacion == 24) {
			if ((enemies->x[i] == 144) || (enemies->x[i] == 152) || (enemies->x[i] == 160) || (enemies->x[i] == 168))
				(enemies->fire[i] = 1);
		}

		/* Salto */
		if (enemies->fire[i] == 1) { /* Salto corto */
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
			if (enemies->speed[i] == 32) { /* Al tocar el suelo, reiniciar flags */
				enemies->speed[i] = 0;
				enemies->fire[i] = 0;
			}
		}
		if (enemies->fire[i] == 2) { /* Salto largo */
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

		/* Caida */
		if (habitacion == 5) {
			if (enemies->x[i] > 206) {
				if (enemies->speed[i] < 8) {
					enemies->speed[i] ++;
					enemies->y[i]+= 2;
					enemies->animation[i] = 0;
				}
			}
		}

		/* Movimiento horizontal */
		if ((habitacion == 6) && (enemies->speed[i] > 0))
			enemies->x[i] += 0.75;
		else
			enemies->x[i] += 0.5;

		/* Dibujar personaje */
		if ((enemies->x[i] > -8) && (enemies->x[i] < 257)) {
			srctile.x = 96 + (16 * (enemies->animation[i] / 7));
			destile.x = enemies->x[i];
			destile.y = enemies->y[i];
			SDL_BlitSurface (tiles, &srctile, ventana, &destile);
		}

	}

}
