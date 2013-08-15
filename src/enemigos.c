/* enemigos.c */
/* Gestionar, mover y dibujar enemigos */

#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "enemigos.h"

void buscarenemigos (int habitacion, struct enem *enemigos, int *cambio, int datosenem[][7][15]) {

  int y = 0;

  for (y=0; y<7; y++) {
		enemigos->tipo[y] = datosenem[habitacion][y][0];
		enemigos->x[y] = datosenem[habitacion][y][1];
		enemigos->y[y] = datosenem[habitacion][y][2];
		enemigos->direccion[y] = datosenem[habitacion][y][3];
		enemigos->tilex[y] = datosenem[habitacion][y][4];
		enemigos->tiley[y] = datosenem[habitacion][y][5];
		enemigos->animacion[y] = datosenem[habitacion][y][6];
		enemigos->limizq[y] = datosenem[habitacion][y][7];
		enemigos->limder[y] = datosenem[habitacion][y][8];
		enemigos->velocidad[y] = datosenem[habitacion][y][9];
		enemigos->disparo[y] = datosenem[habitacion][y][10];
		enemigos->ajustex1[y] = datosenem[habitacion][y][11];
		enemigos->ajustex2[y] = datosenem[habitacion][y][12];
		enemigos->ajustey1[y] = datosenem[habitacion][y][13];
		enemigos->ajustey2[y] = datosenem[habitacion][y][14];
  }
  *cambio = 0;

}

void dibujarenemigos (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, Mix_Chunk *fx[]) {

  SDL_Rect srctile = {0,0,16,16};
  SDL_Rect destile = {0,0,0,0};

  int i = 0;

  for (i=0; i<7; i++) {
		if ((enemigos->tipo[i] > 0) && (enemigos->tipo[i] < 16)) {
	  	if ((enemigos->tipo[i] == 3) || (enemigos->tipo[i] == 5) || (enemigos->tipo[i] == 15))
				srctile.h = 24;
	  	else
				srctile.h = 16;
	  	if (enemigos->tipo[i] == 6) {
				srctile.x = enemigos->tilex[i] + (enemigos->animacion[i] * 24);
				srctile.w = 24;
	  	}
	  	else {
				if (enemigos->tipo[i] == 15)
					srctile.x = enemigos->tilex[i] + (enemigos->animacion[i] * 16);
				else
					srctile.x = enemigos->tilex[i] + (enemigos->animacion[i] * 16);
				srctile.w = 16;
	  	}
	  	srctile.y = enemigos->tiley[i];
	  	destile.x = enemigos->x[i];
	  	destile.y = enemigos->y[i];
	  	if (((enemigos->tipo[i] != 13) && (enemigos->tipo[i] != 14)) || (((enemigos->tipo[i] == 13) || (enemigos->tipo[i] == 14)) && (enemigos->y[i] < enemigos->limder[i] - 8)))
				SDL_BlitSurface(tiles, &srctile, ventana, &destile);

	  	if (enemigos->tipo[i] == 13) { /* Mov. agua */
				if (((enemigos->velocidad[i] > 30) && (enemigos->velocidad[i] < 40)) || ((enemigos->y[i] == enemigos->limder[i] - 16) && (enemigos->direccion[i] == 1))) {
		  		srctile.x = 368;
		  		srctile.y = 32;
				  srctile.h = 8;
				  destile.y = enemigos->limder[i];
				  SDL_BlitSurface(tiles, &srctile, ventana, &destile);
					if ((enemigos->velocidad[i] > 30) && (enemigos->velocidad[i] < 40))
						Mix_PlayChannel(-1, fx[4], 0);;
				}
				if (((enemigos->velocidad[i] > 39) && (enemigos->velocidad[i] < 45)) || ((enemigos->y[i] == enemigos->limder[i] - 10) && (enemigos->direccion[i] == 1))) {
			  	srctile.x = 384;
				  srctile.y = 32;
				  srctile.h = 8;
				  destile.y = enemigos->limder[i];
				  SDL_BlitSurface(tiles, &srctile, ventana, &destile);
				}
	  	}
		}
		/* Dibujar humo */
		if (enemigos->tipo[i] == 88) {
			if ((enemigos->velocidad[i] < 10) || ((enemigos->velocidad[i] > 19) && (enemigos->velocidad[i] < 30)) || ((enemigos->velocidad[i] > 39) && (enemigos->velocidad[i] < 50)))
				srctile.x = 696;
			else
				srctile.x = 728;
			srctile.y = 0;
			srctile.w = 32;
			srctile.h = 48;
			destile.x = enemigos->x[i];
			destile.y = enemigos->y[i];
			SDL_BlitSurface(tiles, &srctile, ventana, &destile);
			if (enemigos->velocidad[i] == 5)
				Mix_PlayChannel(-1, fx[6], 0);;
		}

	}

}

void moverenemigos (struct enem *enemigos, int fase[][22][32], int ciclo[], float proyec[], struct protagonista jean, Mix_Chunk *fx[]) {

  int i = 0;
  int tilex = 0;
  int tiley = 0;
  int n = 0;

  for (i=0; i<7; i++) {
	if ((enemigos->tipo[i] > 0) && (enemigos->tipo[i] < 10)) {
	  if (enemigos->direccion[i] == 0) { /* Andar derecha */
			if ((enemigos->x[i] + 1) < enemigos->limder[i])
				enemigos->x[i] += enemigos->velocidad[i] * 0.10;
			else {
				enemigos->direccion[i] = 1;
				if (enemigos->tipo[i] != 2) {
					if (enemigos->tipo[i] == 6)
						enemigos->tilex[i] -= 48;
					else
						enemigos->tilex[i] -= 32;
				}
			}
	  }
	  if (enemigos->direccion[i] == 1) { /* Andar izquierda */
			if ((enemigos->x[i] - 1) > enemigos->limizq[i])
				enemigos->x[i] -= enemigos->velocidad[i] * 0.10;
			else {
				enemigos->direccion[i] = 0;
				if (enemigos->tipo[i] != 2) {
					if (enemigos->tipo[i] == 6)
						enemigos->tilex[i] += 48;
					else
						enemigos->tilex[i] += 32;
				}
			}
	  }
	  if (enemigos->direccion[i] == 2) { /* Subir */
			if ((enemigos->y[i] - 1) > enemigos->limizq[i])
				enemigos->y[i] -= enemigos->velocidad[i] * 0.10;
			else {
				enemigos->direccion[i] = 3;
				if (enemigos->tipo[i] == 4)
					enemigos->tilex[i] += 32;
				if (enemigos->tipo[i] == 5)
					enemigos->tilex[i] += 16;
			}
	  }
	  if (enemigos->direccion[i] == 3) { /* Bajar */
			if ((enemigos->y[i] + 1) < enemigos->limder[i])
				enemigos->y[i] += enemigos->velocidad[i] * 0.10;
			else {
				enemigos->direccion[i] = 2;
				if (enemigos->tipo[i] == 4)
				enemigos->tilex[i] -= 32;
				if (enemigos->tipo[i] == 5)
				enemigos->tilex[i] -= 16;
			}
	  }

	}

	/* Animacion del personaje */
	if (enemigos->tipo[i] < 10) {
	  if (((ciclo[0] == 1) || (ciclo[0] == 11) || (ciclo[0] == 21)) && (enemigos->tipo[i] != 5)){
			if (enemigos->animacion[i] == 0)
				enemigos->animacion[i] = 1;
			else
				enemigos->animacion[i] = 0;
	  }
	}

	/* Enemigos que disparan */
	if ((enemigos->tipo[i] == 15) || (enemigos->tipo[i] == 11)) {
		if (enemigos->tipo[i] == 15) { /* Mover enemigos segun pos. de Jean */
			if ((jean.x + 23) < (enemigos->x[i]) && (enemigos->velocidad[i] == 0)) {
				enemigos->tilex[i] = 256;
				enemigos->direccion[i] = 1;
			}
			if ((jean.x > (enemigos->x[i] + 16)) && (enemigos->velocidad[i] == 0)) {
				enemigos->tilex[i] = 288;
				enemigos->direccion[i] = 0;
			}
		}
	  if (i == 0) {
			if (enemigos->velocidad[i] < 55) {
		  	enemigos->velocidad[i] ++;
			  enemigos->animacion[i] = 1;
			}
			else
				enemigos->animacion[i] = 0;
		}
	  else {
			enemigos->velocidad[i] = enemigos->velocidad[0];
			enemigos->animacion[i] = enemigos->animacion[0];
	  }
	  /* Habilitar disparo */
	  if (enemigos->velocidad[i] == 50) {
			for (n=0; n<=4; n+=2) {
				if (proyec[n] == 0) {
					enemigos->disparo[i] = 1;
					proyec[n] = enemigos->x[i] - 8;
					if (n > 0)
						proyec[n+1] = i;
					n = 5;
					Mix_PlayChannel(-1, fx[0], 0);;
				}
			}
	  }
	}

	if (enemigos->tipo[i] == 12) { /*Gotas de agua */
	  if (enemigos->velocidad[i] < 60)
			enemigos->velocidad[i] ++; /* Usando velocidad como temporizador */
	  enemigos->tilex[i] = 640 + ((enemigos->velocidad[i] / 30) * 16);
	  if (enemigos->tilex[i] == 672) {
			if (enemigos->y[i] < (enemigos->limder[i] -8))
				enemigos->y[i] += 0.9;
			else { /* Reiniciar gota */
				enemigos->y[i] = enemigos->limizq[i]; /* Usando limizq como inicio de la gota */
				enemigos->velocidad[i] = 0;
				enemigos->tilex[i] = 640;
			}
	  }
	}

	if ((enemigos->tipo[i] == 13) || (enemigos->tipo[i] == 14)) { /* Bolas de fuego y peces*/
	  if ((ciclo[0] == 1) || (ciclo[0] == 11) || (ciclo[0] == 21)) {
			if (enemigos->animacion[i] == 0)
				enemigos->animacion[i] = 1;
			else
				enemigos->animacion[i] = 0;
	  }
	  if (enemigos->direccion[i] == 0) { /* Bola subiendo */
			if (enemigos->y[i] == (enemigos->limder[i])) {
				if (enemigos->velocidad[i] < 45) /* Temporizador */
					enemigos->velocidad[i] ++;
				else
					enemigos->y[i] --;
			}
			else {
		  	if (enemigos->y[i] > (enemigos->limizq[i] + 16))
					enemigos->y[i] -= 2;
				else
					enemigos->y[i] --;
			}
			if (enemigos->y[i] == enemigos->limizq[i] + 8)
		  	enemigos->direccion[i] = 1;
	  }
	  else {
			if (enemigos->y[i] != enemigos->limder[i])
		  	enemigos->y[i] += 2;
			else { /* Reiniciar */
		  	enemigos->velocidad[i] = 0;
		  	enemigos->direccion[i] = 0;
			}
	  }
	}

  }
}

void plantas (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], float proyec[], Mix_Chunk *fx[]) {

  SDL_Rect srctile = {0,40,16,16};
  SDL_Rect destile = {0,0,16,16};
  SDL_Rect srcdisparo = {660,32,4,4};
  SDL_Rect desdisparo = {0,0,4,4};
  int n = 0;
  int r = 0;

  /* Animacion */

  for (n=1; n<4; n++) {
	if (enemigos->velocidad[n] < (140 + ((n-1) * 30)))
	  enemigos->velocidad[n] ++;
	else
	  enemigos->velocidad[n] = 0;

	if (enemigos->velocidad[n] < (100 + ((n-1) * 30)))
	  enemigos->animacion[n] = 0;
	else
	  enemigos->animacion[n] = 1;

	srctile.x = 384 + (enemigos->animacion[n] * 16);
	destile.x = enemigos->x[n];
	destile.y = enemigos->y[n];

	SDL_BlitSurface(tiles, &srctile, ventana, &destile);
  }

  /* Iniciar disparo */
  for (n=1; n<4; n++) {
		if (enemigos->velocidad[n] == 135 + ((n-1) * 30)) {
	 	 r = (n-1) * 4;
	  	proyec[r] = enemigos->x[n] - 1;
	  	proyec[r+1] = enemigos->y[n] - 1;
	  	proyec[r+2] = enemigos->x[n] + 16;
	  	proyec[r+3] = enemigos->y[n] - 1;
	  	Mix_PlayChannel(-1, fx[0], 0);
		}
  }

  /* Mover disparos */
  for (n=1; n<4; n++) {
	r = (n-1) * 4;
	if (proyec[r] > 0) {
	  if (enemigos->direccion[n] == 0) {
		if (enemigos->y[n] - proyec[r+1] < 24) {
		  proyec[r+1] --;
		  proyec[r+3] --;
		}
		else
		  enemigos->direccion[n] = 1;
		if (ciclo[0] % 2 == 0) {
		  proyec[r] --;
		  proyec[r+2] ++;
		}
	  }
	  if ((enemigos->direccion[n]>0) && (enemigos->direccion[n] < 6)) {
		proyec[r] --;
		proyec[r+2] ++;
		enemigos->direccion[n] ++;
	  }
	  if (enemigos->direccion[n] == 6) {
		if (proyec[r+1] < enemigos->limizq[n]) {
		  proyec[r+1] += 2;
		  if (ciclo[0] % 2 == 0)
			proyec[r] --;
		}
		else {
		  proyec[r] = 0;
		  proyec[r+1] = 0;
		}
		if ((proyec[r+3] < enemigos->limder[n]) && (proyec[r+3] > 7) && (proyec[r+2] < 240)) {
		  proyec[r+3] += 2;
		  if (ciclo[0] % 2 == 0)
			proyec[r+2] ++;
		}
		else {
		  proyec[r+2] = 0;
		  proyec[r+3] = 0;
		}
	  }
	  if ((proyec[r] == 0) && (proyec[r+2] == 0))
		enemigos->direccion[n] = 0;
	}

  }

 /* Dibujar disparo */
 for (n=1; n<4; n++) {
	r = (n-1) * 4;
	if (proyec[r] > 0) {
	  desdisparo.x = proyec[r];
	  desdisparo.y = proyec[r+1];
	  SDL_BlitSurface(tiles, &srcdisparo, ventana, &desdisparo);
	}
	if (proyec[r+2] > 0) {
	  desdisparo.x = proyec[r+2];
	  desdisparo.y = proyec[r+3];
	  SDL_BlitSurface(tiles, &srcdisparo, ventana, &desdisparo);
	}

  }

}

void cruzados (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], int habitacion) {

	SDL_Rect srctile = {96,64,16,24};
	SDL_Rect destile = {0,0,16,24};
	int i = 0;

	for (i=0; i<7; i++) {

		/* Iniciar valor */
		if (enemigos->tipo[i] == 17) {
			enemigos->tipo[i] = 16;
			enemigos->x[i] = -52 - (24 * i);
			enemigos->y[i] = 136;
		}

		/* Mover y animar personaje */
		/* Saltar al llegar a ciertos puntos */
		if (habitacion == 5) {
			if ((enemigos->x[i] == 146) || (enemigos->x[i] == 160))
				(enemigos->disparo[i] = 1); /* Usando el disparo como flag de salto */
		}
		if (habitacion == 6) {
			if ((enemigos->x[i] == 76) || (enemigos->x[i] == 124.75) || (enemigos->x[i] == 155))
				enemigos->disparo[i] = 2;
			if ((enemigos->x[i] == 208.25) || (enemigos->x[i] == 220.50))
				enemigos->disparo[i] = 1; /* Usando el disparo como flag de salto */
		}
		if (habitacion == 24) {
			if ((enemigos->x[i] == 144) || (enemigos->x[i] == 152) || (enemigos->x[i] == 160) || (enemigos->x[i] == 168))
				(enemigos->disparo[i] = 1);
		}

		/* Salto */
		if (enemigos->disparo[i] == 1) { /* Salto corto */
			if (enemigos->velocidad[i] < 23) {
				enemigos->velocidad[i] += 2;
				enemigos->y[i] -= 1;
				enemigos->animacion[i] = 0;
			}
			else {
				enemigos->y[i] += 1;
				enemigos->velocidad[i] += 2;
				enemigos->animacion[i] = 0;
			}
			if (enemigos->velocidad[i] == 32) { /* Al tocar el suelo, reiniciar flags */
				enemigos->velocidad[i] = 0;
				enemigos->disparo[i] = 0;
			}
		}
		if (enemigos->disparo[i] == 2) { /* Salto largo */
			if (enemigos->velocidad[i] < 8) {
				enemigos->velocidad[i] += 1;
				enemigos->y[i] -= 1;
				enemigos->animacion[i] = 0;
			}
			else {
				enemigos->y[i] += 1;
				enemigos->velocidad[i] += 1;
				enemigos->animacion[i] = 0;
			}
			if (enemigos->velocidad[i] == 16) {
				enemigos->disparo[i] = 0;
				enemigos->velocidad[i] = 0;
			}
		}

		if (enemigos->animacion[i] < 13)
			enemigos->animacion[i] ++;
		else
			enemigos->animacion[i] = 0;

		/* Caida */
		if (habitacion == 5) {
			if (enemigos->x[i] > 206) {
				if (enemigos->velocidad[i] < 8) {
					enemigos->velocidad[i] ++;
					enemigos->y[i]+= 2;
					enemigos->animacion[i] = 0;
				}
			}
		}

		/* Movimiento horizontal */
		if ((habitacion == 6) && (enemigos->velocidad[i] > 0))
			enemigos->x[i] += 0.75;
		else
			enemigos->x[i] += 0.5;

		/* Dibujar personaje */
		if ((enemigos->x[i] > -8) && (enemigos->x[i] < 257)) {
			srctile.x = 96 + (16 * (enemigos->animacion[i] / 7));
			destile.x = enemigos->x[i];
			destile.y = enemigos->y[i];
			SDL_BlitSurface (tiles, &srctile, ventana, &destile);
		}

	}

}
