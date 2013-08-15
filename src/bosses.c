/* bosses.c */
/* Gestion de jefes: muerte, dragon y satan */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "bosses.h"

void muerte (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], float proyec[], int fase[][22][32], Mix_Chunk *fx[]) {

  SDL_Rect srchacha = {0,56,16,16};
  SDL_Rect deshacha = {0,0,16,16};
	SDL_Rect srctile = {0,88,32,32};
	SDL_Rect destile = {0,8,32,32};
	int x = 0;
  int y = 0;
  int n = 0;

	if (enemigos->velocidad[0] < 60)
	  enemigos->velocidad[0]++;
	else
	  enemigos->velocidad[0] = 0;

	/* Movimiento */
	if (enemigos->direccion[0] == 0) { /* Mov. izquierda */
		if (enemigos->x[0] > enemigos->limizq[0])
			enemigos->x[0] --;
		else
			enemigos->direccion[0] = 1;
	}
	if (enemigos->direccion[0] == 1) { /* Mov. derecha */
		if (enemigos->x[0] < enemigos->limder[0])
			enemigos->x[0] ++;
		else
			enemigos->direccion[0] = 0;
	}

	/* Disparar hachas */
	if ((enemigos->velocidad[0] == 45) && (enemigos->x[0] > 48)) {
	  for (x=0; x<8; x+=2) {
			if (proyec[x] == 0) {
			  proyec[x] = enemigos->y[0] + 35;
			  proyec[x+1] = enemigos->x[0];
			  x = 9;
			  Mix_PlayChannel(-1, fx[0], 0);
			}
	  }
	}

	/* Animacion */
	if (enemigos->velocidad[0] < 45) {
	  if ((ciclo[0] == 1) || (ciclo[0] == 11) || (ciclo[0] == 21)) {
			if (enemigos->animacion[0] == 0)
			  enemigos->animacion[0] = 1;
			else
				enemigos->animacion[0] = 0;
		  }
	}
	else {
	  if (enemigos->x[0] > 48) {
			if (enemigos->velocidad[0] < 55)
				enemigos->animacion[0] = 2;
			else
				enemigos->animacion[0] = 0;
		}
	}

	/* Dibujo */
	srctile.x = 0 + (enemigos->animacion[0] * 32) + (enemigos->direccion[0] * 96);
	destile.x = enemigos->x[0];
	SDL_BlitSurface(tiles, &srctile, ventana, &destile);

	/* Antiguo hachas */
  for (n=0; n<8; n+=2) {

		if (proyec[n] > 0) {
	  	/* Situar el hacha en el mapa de tiles */
		  x = proyec[n+1] / 8;
		  y = proyec[n] / 8;

		  /* Al llegar abajo, desaparecer y liberar */
		  if (y == 20) {
				proyec[n] = 0;
				proyec[n+1] = 0;
		  }
		  else {
				/* Comprobar si ha chocado con un tile solido */
				/* y mover en consecuencia */
				if (((fase[18][y+2][x] != 73) && (fase[18][y+2][x] != 75)) && ((fase[18][y+2][x+1] != 73) && (fase[18][y+2][x+1] != 75)))
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

  for (n=0; n<8; n+=2) {
		if (proyec[n] > 0) {
	  	deshacha.x = proyec[n+1];
		  deshacha.y = proyec[n];
		  /* Rotacion */
		  srchacha.x = 576 + (16 * (ciclo[2] / 2));
		  SDL_BlitSurface(tiles, &srchacha, ventana, &deshacha);
		}
  }

}

void dragon (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], float proyec[], Mix_Chunk *fx[]) {

	SDL_Rect srctile = {456,72,16,8};
	SDL_Rect destile = {120,40,16,8};
	int n = 0;

	/* Dibujando la pata delantera */
	SDL_BlitSurface(tiles, &srctile, ventana, &destile);

	/* Posicion de la cabeza */
	if (enemigos->velocidad[0] < 150) /* Usando velocidad como temporizador */
		enemigos->velocidad[0] ++;
	else
		enemigos->velocidad[0] = 0;

	if (enemigos->velocidad[0] < 110)
		enemigos->animacion[0] = 0;
	else
		enemigos->animacion[0] = 1;

	/* Dibujar la cabeza */
	srctile.w = 32;
	srctile.h = 24;
	srctile.x = 416;
	srctile.y = 56;
	destile.w = 32;
	destile.h = 24;
	destile.x = 120;
	destile.y = 8 + (enemigos->animacion[0] * 3);
	SDL_BlitSurface(tiles, &srctile, ventana, &destile);
	/* Hocico */
	srctile.h = 16;
	srctile.x = 448;
	destile.h = 16;
	destile.y += 24;
	SDL_BlitSurface(tiles, &srctile, ventana, &destile);

	/* Fuego escupido */
	if (enemigos->animacion[0] == 1) {
		if ((enemigos->velocidad[0] == 115) || (enemigos->velocidad[0] == 125) || (enemigos->velocidad[0] == 135) || (enemigos->velocidad[0] == 145))
			enemigos->direccion[0] = 1;
		if ((enemigos->velocidad[0] == 110) || (enemigos->velocidad[0] == 120) || (enemigos->velocidad[0] == 130) || (enemigos->velocidad[0] == 140))
			enemigos->direccion[0] = 0;
		srctile.x = 504 + (enemigos->direccion[0] * 48);
		srctile.y = 56;
		srctile.w = 24;
		srctile.h = 24;
		destile.x = 119 + (enemigos->direccion[0] * 16);
		destile.y = 51;
		destile.w = 24;
		destile.h = 24;
		SDL_BlitSurface(tiles, &srctile, ventana, &destile);
		srctile.x = 480 + (enemigos->direccion[0] * 48);
		destile.y = 75;
		destile.x = 127;
		SDL_BlitSurface(tiles, &srctile, ventana, &destile);
	}

	/* Generando fuego en el suelo */
	if (enemigos->velocidad[0] == 150) {
	  Mix_PlayChannel(-1, fx[0], 0);;
	  for (n=0; n<16; n+=8) {
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

	/* Mover y animar el fuego */
	for (n=0; n<16; n+=2) {
	  if (proyec[n] > 0) {
			if ((n < 3) || ((n>7) && (n<11))) {
		  	if (proyec[n] > 48) {
					proyec[n] -= 0.3;
					/* Animacion del fuego */
					if (ciclo[0] % 8 == 0) {
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
					/* Animacion del fuego */
					if (ciclo[0] % 8 == 0) {
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
			srctile.y = 0;
			srctile.w = 8;
			srctile.h = 8;
			destile.x = proyec[n];
			destile.y = 88;
			SDL_BlitSurface(tiles, &srctile, ventana, &destile);
	  }
	}
}

void satan (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], float proyec[], Mix_Chunk *fx[]) {

	SDL_Rect srctile = {192,88,32,24};
	SDL_Rect destile = {192,0,32,24};
	SDL_Rect srcdisparo = {656,32,4,4};
	SDL_Rect desdisparo = {0,0,4,4};
	int n = 0;
	int r = 0;
	int x = 0;

	/* Movimiento */
	if (enemigos->direccion[0] == 0) { /* Subiendo */
		if (enemigos->y[0] > enemigos->limizq[0])
			enemigos->y[0] -= 0.5;
		else
			enemigos->direccion[0] = 1;
	}
	if (enemigos->direccion[0] == 1) { /* Bajando */
		if (enemigos->y[0] < enemigos->limder[0])
			enemigos->y[0] += 0.5;
		else
			enemigos->direccion[0] = 0;
	}

	/* Animacion */
	if ((ciclo[0] == 1) || (ciclo[0] == 11) || (ciclo[0] == 21)) {
		if (enemigos->animacion[0] == 0)
			enemigos->animacion[0] = 1;
		else
			enemigos->animacion[0] = 0;
	}

	/* Dibujo */
	srctile.x = 192 + (enemigos->animacion[0] * 64);
	destile.y = enemigos->y[0];
	SDL_BlitSurface(tiles, &srctile, ventana, &destile);
	srctile.x = 224 + (enemigos->animacion[0] * 64);
	destile.y = enemigos->y[0] + 24;
	SDL_BlitSurface(tiles, &srctile, ventana, &destile);

	/* Activar disparo */
	if (((enemigos->y[0] == 37) && (enemigos->direccion[0] == 0)) || ((enemigos->y[0] == 20) && (enemigos->direccion[0] == 1)) || ((enemigos->y[0] == 62) && (enemigos->direccion[0] == 0))) {
		x = enemigos->y[0];
		switch (x) {
		  case 20: r = 0;
				   		 break;
		  case 37: r = 6;
				   	 	 break;
		  case 62: r = 12;
							 break;
		}
		if ((proyec[r] == 0) && (proyec[r+2] == 0) && (proyec[r+4] == 0)) {
			proyec[r] = 190;
			proyec[r+1] = enemigos->y[0] + 15;
			proyec[r+2] = 190;
			proyec[r+3] = enemigos->y[0] + 10;
			proyec[r+4] = 190;
			proyec[r+5] = enemigos->y[0] + 20;
			Mix_PlayChannel(-1, fx[0], 0);
		}
	}

	/* Mover disparo */
	for (n=0; n<17; n+=2) {
	  if ((proyec[n] > 16) && ((proyec[n+1] > 8) && (proyec[n+1] < 160))) {
			proyec[n] -= 2;
			if ((n+1 == 3) || (n+1 == 9) || (n+1 == 15))
			  proyec[n+1] -= 0.75;
			if ((n+1 == 5) || (n+1 == 11) || (n+1 == 17))
			  proyec[n+1] += 0.75;
			desdisparo.x = proyec[n];
			desdisparo.y = proyec[n+1];
			SDL_BlitSurface(tiles, &srcdisparo, ventana, &desdisparo);
	  }
	  else {
			proyec[n] = 0;
			proyec[n+1] = 0;
	  }
	}

}

void bolafuego (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], struct protagonista jean, int fase[][22][32]) {
/* A falta de saber el nombre de este enemigo, se queda en bolafuego */

	SDL_Rect srctile = {576,40,16,16};
	SDL_Rect destile = {0,0,16,16};
	int x = 0;
	int y = 0;

	/* Calcular tile actual de este bicho */
	x = enemigos->x[0] / 8;
	y = enemigos->y[0] / 8;

	/* Perseguir a Jean */
	if (enemigos->x[0] < jean.x + 1) {
		enemigos->direccion[0] = 1;
		if (((fase[11][y][x+2] == 0) || (fase[11][y][x+2] > 99) || (fase[11][y][x+2] == 37)) && ((fase[11][y+1][x+2] == 0) || (fase[11][y+1][x+2] > 99) || (fase[11][y+1][x+2] == 37))) {
			if (enemigos->disparo[0] == 0)
				enemigos->x[0] += 0.3;
		}
	}
	if (enemigos->x[0] > jean.x - 1) {
		enemigos->direccion[0] = 0;
		if (((fase[11][y][x-1] == 0) || (fase[11][y][x-1] > 99) || (fase[11][y][x-1] == 37)) && ((fase[11][y+1][x-1] == 0) || (fase[11][y+1][x-1] > 99) || (fase[11][y+1][x-1] == 37))) {
			if (enemigos->disparo[0] == 0)
				enemigos->x[0] -= 0.3;
		}
	}
	if (enemigos->y[0] < jean.y + 1) {
		if (((fase[11][y+2][x] == 0) || (fase[11][y+2][x] > 99)) && ((fase[11][y+2][x+1] == 0) || (fase[11][y+2][x+1] > 99))) {
			if (enemigos->disparo[0] == 0)
				enemigos->y[0] +=0.3;
			enemigos->disparo[0] = 0;
		}
		else {
			enemigos->disparo[0] = 1; /* Usando disparo como flag */
			if (enemigos->x[0] > jean.x -1)
				enemigos->x[0] -=0.3;
			else
				enemigos->x[0] +=0.3;
		}
	}
	if (enemigos->y[0] > jean.y + 1) {
		if (((fase[11][y-1][x] == 0) || (fase[11][y-1][x] > 99) || (fase[11][y-1][x] == 37)) && ((fase[11][y-1][x+1] == 0) || (fase[11][y-1][x+1] > 99) || (fase[11][y-1][x+1] == 37))) {
			enemigos->y[0] -=0.3;
			enemigos->disparo[0] = 0;
		}
		else {
			enemigos->disparo[0] = 1; /* Usando disparo como flag */
			if (enemigos->x[0] > jean.x - 1)
				enemigos->x[0] -=0.3;
			else
				enemigos->x[0] +=0.3;
		}
	}

	/* Animacion */
	enemigos->animacion[0] = ciclo[0] / 15;

	/* Dibujado */
	srctile.x = 576 + (enemigos->animacion[0] * 16) + (enemigos->direccion[0] * 32);
	destile.x = enemigos->x[0];
	destile.y = enemigos->y[0];
	SDL_BlitSurface(tiles, &srctile, ventana, &destile);

}
