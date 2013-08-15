/* jean.c */
/* Gestion del protagonista */

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "jean.h"

void moverjean (struct protagonista *jean, Mix_Chunk *fx[]) {

	/* Salto */
	if (jean->salto == 1) {
		if (jean->altura == 0) /* Reproducir sonido de salto */
			Mix_PlayChannel(-1, fx[3], 0);
		if (jean->altura < 56) {
			jean->altura += 1.6;
			if ((jean->colision[0] == 0) && (jean->altura < 44))
				jean->y -= 1.5;
			jean->animacion = 0;
		}
		else {
			jean->salto = 2;
			jean->colision[0] = 0;
		}
	}


	/* Mover a la derecha */
	if (jean->pulsa[3] == 1) {
		jean->direccion = 1;
		if (jean->colision[3] == 0) {
			if (jean->salto == 0) {
			  if (jean->animacion < 13)
					jean->animacion ++;
			  else
					jean->animacion = 0;
			}
			if (jean->pulsa[1] == 1)
				jean->x += 0.30;
			else
				jean->x += 0.65;
		}
	}

	/* Mover a la izquierda */
	if (jean->pulsa[2] == 1) {
		jean->direccion = 0;
		if (jean->colision[2] == 0) {
			if (jean->salto == 0) {
			  if (jean->animacion < 13)
					jean->animacion ++;
			  else
					jean->animacion = 0;
			}
			if (jean->pulsa[1] == 1)
				jean->x -= 0.30;
			else
				jean->x -= 0.65;
		}

	}

}

void dibujarjean (SDL_Surface *ventana, SDL_Surface *tiles, struct protagonista *jean, int ciclo[], Mix_Chunk *fx[]) {

	SDL_Rect srctile = {320,88,16,24};
	SDL_Rect destile = {0,0,16,24};
	SDL_Rect agatile = {448,88,18,13};
	SDL_Rect desagatile = {0,0,18,13};
	int r = 0;

	if (jean->muerte == 0) {
		if (jean->salto > 0) {
			r = 1;
			jean->animacion = 0;
		}

 		if (jean->agachado == 0) {
			srctile.x += (64 * jean->direccion) + ((jean->animacion / 7) * 16) + (r * 32);
		 	destile.y = jean->y;
			destile.x = jean->x;
			if (jean->y > 152)
				srctile.h = (176 - jean->y);
		 	SDL_BlitSurface(tiles, &srctile, ventana, &destile);
		}
		else {
  		agatile.x += (jean->direccion * 36) + ((jean->animacion / 7) * 18);
  		desagatile.y = jean->y + 11;
			desagatile.x = jean->x;
  		SDL_BlitSurface(tiles, &agatile, ventana, &desagatile);
		}
	}

	/* Animacion de muerte */
	if (jean->muerte > 0) {
		jean->muerte += 1;
		destile.x = jean->x;
		destile.y = jean->y;
		Mix_PauseMusic ();
		if (jean->muerte == 2)
			Mix_PlayChannel(-1, fx[6], 0);
		if ((jean->muerte < 8) || ((jean->muerte > 23) && (jean->muerte < 32)) || ((jean->muerte > 47) && (jean->muerte < 56))) {
			srctile.x = 368 + (jean->direccion * 64);
			SDL_BlitSurface(tiles, &srctile, ventana, &destile);
		}
		if (((jean->muerte > 7) && (jean->muerte < 16)) || ((jean->muerte > 31) && (jean->muerte < 40)) || ((jean->muerte > 55) && (jean->muerte < 64))) {
			srctile.x = 536;
			srctile.y = 87;
			SDL_BlitSurface(tiles, &srctile, ventana, &destile);
		}
		if (((jean->muerte > 15) && (jean->muerte < 24)) || ((jean->muerte > 39) && (jean->muerte < 48)) || ((jean->muerte > 63) && (jean->muerte < 73))) {
			srctile.x = 520;
			srctile.y = 87;
			SDL_BlitSurface(tiles, &srctile, ventana, &destile);
		}
	}

	/* Animacion hab. 24 */
	if ((jean->flags[6] == 5) && (ciclo[1] == 45)) {
		switch (jean->direccion) {
			case 0: jean->direccion = 1;
							break;
			case 1: jean->direccion = 0;
							break;
		}
	}

}

void colisiones (struct protagonista *jean, int fase[][22][32], int habitacion) {

	int blizq = 0;
	int blder = 0;
	int blsuel[4] = {0,0,0,0};
	int bltech[2] = {0,0};
	int puntos[8] = {0,0,0,0,0,0,0,0};
	int n = 0;
	int pixel = 0;
	int r = 0;

	puntos[0] = (jean->x + 1) / 8;
	puntos[1] = (jean->x + 7) / 8;
	puntos[2] = (jean->x + 8) / 8;
	puntos[3] = (jean->x + 13) / 8;
	puntos[4] = (jean->y + 1) / 8;
	puntos[5] = (jean->y + 8) / 8;
	puntos[6] = (jean->y + 15) / 8;
	puntos[7] = (jean->y + 23) / 8;

	jean->colision[0] = 0;
	jean->colision[1] = 0;
	jean->colision[2] = 0;
	jean->colision[3] = 0;

	/* Colisiones horizontales: izquierda y derecha */
	if (jean->agachado == 0) {
		for (n=4; n<8; n++) {
			if (((puntos[0] != 0) && (jean->direccion ==0)) || ((puntos[3] != 31) && (jean->direccion == 1))) {
				blizq = fase[habitacion][puntos[n]][puntos[0] - 1];
				blder = fase[habitacion][puntos[n]][puntos[3] + 1];
				if (((blizq > 0) && (blizq < 100) && (blizq != 16) && (blizq != 38) && (blizq != 37)) || ((fase[habitacion][puntos[4]][puntos[0]] == 128) || (blizq == 348))) {
					if (jean->x - ((puntos[0] - 1) * 8 + 7) < 1.1)
						jean->colision[2] = 1;
				}
				if (((blder > 0) && (blder < 100) && (blder != 16) && (blder != 38) && (blder != 37)) || (blder == 344)) {
					if (((puntos[3] + 1) * 8) - (jean->x + 14) < 1.1)
						jean->colision[3] = 1;
				}
			}
		}
	}

	/* Colision con Jean agachado */
	if (jean->agachado == 1) {
		if (((puntos[0] != 0) && (jean->direccion ==0)) || ((puntos[3] != 31) && (jean->direccion == 1))) {
			r = (jean->y + 16) / 8;
			blizq = fase[habitacion][r][puntos[0] - 1];
			blder = fase[habitacion][r][puntos[3] + 1];
			if (((blizq > 0) && (blizq < 100) && (blizq != 37)) || ((fase[habitacion][r][puntos[0]] == 128) || ((blizq > 346) && (blizq < 351)))) {
				if (jean->x - ((puntos[0] - 1) * 8 + 7) < 1.1)
					jean->colision[2] = 1;
			}
			if (((blder > 0) && (blder < 100) && (blder != 37)) || ((blder > 342) && (blder< 347))) {
				if (((puntos[3] + 1) * 8) - (jean->x + 14) < 1.1)
					jean->colision[3] = 1;
			}
		}
		/* Pared invisible */
		if ((habitacion == 11) && (r == 5)) {
			if ((puntos[0] - 1 == 0) || (puntos[0] - 1 == 1))
				jean->colision[2] = 0;
			if ((puntos[3] + 1 == 0) || (puntos[3] + 1 == 1))
				jean->colision[3] = 0;
		}
		if ((habitacion == 10) && (r == 5)) {
			if ((puntos[0] - 1 > 27) && (puntos[0] - 1 < 32))
				jean->colision[2] = 0;
			if ((puntos[3] + 1 > 27) && (puntos[3] + 1 < 32))
				jean->colision[3] = 0;
		}
	}

	/* Colision vertical: tocar suelo */
	blsuel[0] = fase[habitacion][puntos[7]+1][puntos[0]];
	blsuel[1] = fase[habitacion][puntos[7]+1][puntos[1]];
	blsuel[2] = fase[habitacion][puntos[7]+1][puntos[2]];
	blsuel[3] = fase[habitacion][puntos[7]+1][puntos[3]];

	if ((jean->salto != 1) && (jean->agachado == 0)) {
		/* Suelo invisible */
		if (((habitacion == 11) && (puntos[7]+1 > 19) && (puntos[0] == 2)) || ((habitacion == 16) && ((jean->y / 8) < 4) && (puntos[0] == 2))) {
			jean->y += jean->gravedad;
			jean->salto = 2;
		}
		else {
			if (((blsuel[0] > 0) && (blsuel[0] < 100)) || ((blsuel[1] > 0) && (blsuel[1] < 100)) || ((blsuel[2] > 0) && (blsuel[2] < 100)) || ((blsuel[3] > 0) && (blsuel[3] < 100))) {
				jean->suelo = (puntos[7] + 1) * 8;
				if (puntos[7] + 1 > 21) /* Chapuza para que Jean caiga por debajo de pantalla */
					jean->suelo = 300;
				if ((jean->suelo - 1) - (jean->y+23) > 1.2)
					jean->y += jean->gravedad;
				else { /* Cerca del suelo */
					jean->y += (jean->suelo - 1) - (jean->y + 23);
					jean->altura = 0;
					jean->salto = 0;
					jean->flags[5] = 0;
				}
			}
			else {/* Esta en el aire y sin el suelo cerca */
				jean->y += jean->gravedad;
				jean->salto = 2;
			}
		}
	}

	/* Comprobacion plataformas pequenyas */
	if (jean->direccion == 0) {
		if ((blsuel[3] == 38) && ((jean->x + 13) < (puntos[3] * 8 + 5)) && (jean->pulsa[2] == 1) && (jean->salto ==0)) {
			jean->y += jean->gravedad;
			jean->salto = 2;
		}
	}
	if (jean->direccion == 1) {
		if ((blsuel[0] == 38) && ((jean->x + 1) > (puntos[0] + 2)) && (jean->pulsa[3] == 1) && (jean->salto == 0)) {
			jean->y += jean->gravedad;
			jean->salto = 2;
		}
	}

	/* Colision vertical: tocar techo */
	bltech[0] = fase[habitacion][puntos[4]-1][puntos[0]];
	bltech[1] = fase[habitacion][puntos[4]-1][puntos[3]];

	if ((jean->salto == 1) && (puntos[4] > 0)) {
		if (((bltech[0] > 0) && (bltech[0] < 100) && (bltech[0] != 16) && (bltech[0] != 38) && (bltech[0] != 37)) || ((bltech[1] > 0) && (bltech[1] < 100) && (bltech[1] != 16) && (bltech[1] != 38) && (bltech[1] != 37))) {
			if ((jean->y - 1) - ((puntos[4] - 1) * 8 + 7) < 1)
				jean->colision[0] = 1;
		}
	}

}
