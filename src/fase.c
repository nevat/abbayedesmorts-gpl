/* fase.c */
/* Dibujo y animación de tiles */

#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "fase.h"

void dibujarfase (SDL_Surface *ventana, int fase[][22][32], SDL_Surface *tiles, int habitacion, int ciclo[], int cambiot, SDL_Surface *fondomd, Mix_Chunk *fx[]) {

  int coordx = 0;
  int coordy = 0;
  SDL_Rect srctiles = {0,0,8,8};
  SDL_Rect destiles = {0,0,0,0};
  SDL_Rect srcbarra = {91,24,13,12};
  SDL_Rect desbarra = {0,177,0,0};
	SDL_Rect srcfondomd = {0,0,256,176};
	SDL_Rect desfondomd = {0,0,256,176};

 	SDL_FillRect(ventana, NULL, SDL_MapRGB(ventana->format, 0, 0, 0));

	if ((cambiot == 1) && (habitacion != 4))
		SDL_BlitSurface(fondomd, &srcfondomd, ventana, &desfondomd);

	for (coordy=0; coordy<=21; coordy++) {
		for (coordx=0; coordx<=31; coordx++) {
			if ((fase[habitacion][coordy][coordx] > 0) && (fase[habitacion][coordy][coordx] != 99)) {
				destiles.x = coordx * 8;
				destiles.y = coordy * 8;
				if (fase[habitacion][coordy][coordx] < 200) {
					srctiles.w = 8;
					srctiles.h = 8;
					if (fase[habitacion][coordy][coordx] < 101) {
						srctiles.y = 0;
						if (fase[habitacion][coordy][coordx] == 84) /* Brillo de las cruces */
							srctiles.x = (fase[habitacion][coordy][coordx] - 1) * 8 + (ciclo[0]/8 * 8);
						else
							srctiles.x = (fase[habitacion][coordy][coordx] - 1) * 8;
					}
					else {
						if (fase[habitacion][coordy][coordx] == 154) {
						  srctiles.x=600 + ((ciclo[0] / 8) * 16);
						  srctiles.y=0;
						  srctiles.w=16;
						  srctiles.h=24;
						}
						else {
						  srctiles.y = 8;
						  srctiles.x = (fase[habitacion][coordy][coordx] - 101) * 8;
						}
					}
				}
				if ((fase[habitacion][coordy][coordx] > 199) && (fase[habitacion][coordy][coordx] < 300)) {
					srctiles.x = (fase[habitacion][coordy][coordx] - 201) * 48;
					srctiles.y = 16;
					srctiles.w = 48;
					srctiles.h = 48;
				}
				if ((fase[habitacion][coordy][coordx] > 299) && (fase[habitacion][coordy][coordx] < 399)) {
						srctiles.x = 96 + ((fase[habitacion][coordy][coordx] - 301) * 8);
						srctiles.y = 16;
						srctiles.w = 8;
						srctiles.h = 8;
						/* Golpeo de la puerta */
						if ((habitacion == 7) && ((ciclo[1] > 59) && (ciclo[1] < 71))) {
							if ((fase[habitacion][coordy][coordx] == 347) || (fase[habitacion][coordy][coordx] == 348) || (fase[habitacion][coordy][coordx] == 349) || (fase[habitacion][coordy][coordx] == 350)) {
								destiles.x += 2;
								if ((fase[habitacion][coordy][coordx] == 350) && (ciclo[1] == 70))
									Mix_PlayChannel(-1, fx[3], 0);
							}
						}
				}
				/* Corazones */
				if ((fase[habitacion][coordy][coordx] > 399) && (fase[habitacion][coordy][coordx] < 405)) {
					srctiles.x = 96 + ((fase[habitacion][coordy][coordx] - 401) * 8) + (32 * (ciclo[0] / 15));
					srctiles.y = 24;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				/* Cruces */
				if ((fase[habitacion][coordy][coordx] > 408) && (fase[habitacion][coordy][coordx] < 429)) {
					srctiles.x = 96 + ((fase[habitacion][coordy][coordx] - 401) * 8) + (32 * (ciclo[1] / 23));
					srctiles.y = 24;
					srctiles.w = 8;
					srctiles.h = 8;
				}

				if ((fase[habitacion][coordy][coordx] > 499) && (fase[habitacion][coordy][coordx] < 599)) {
					srctiles.x = 96 + ((fase[habitacion][coordy][coordx] - 501) * 8);
					srctiles.y = 32;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				if ((fase[habitacion][coordy][coordx] > 599) && (fase[habitacion][coordy][coordx] < 650)) {
					srctiles.x = 96 + ((fase[habitacion][coordy][coordx] - 601) * 8);
					srctiles.y = 56;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				if (fase[habitacion][coordy][coordx] == 650) { /* Copa */
					srctiles.x = 584;
					srctiles.y = 87;
					srctiles.w = 16;
					srctiles.h = 16;
				}
				if ((fase[habitacion][coordy][coordx] == 152) || (fase[habitacion][coordy][coordx] == 137) || (fase[habitacion][coordy][coordx] == 136)) {
					if (cambiot == 0)
						SDL_BlitSurface(tiles, &srctiles, ventana, &destiles);
				}
				else
					SDL_BlitSurface(tiles, &srctiles, ventana, &destiles);
			}
		}
	}

}

void animitems (int fase[][22][32], int habitacion, int ciclo[]) {

  int i = 0;
  int j = 0;

  for (j=0; j<=21; j++) {
		for (i=0; i<=31; i++) {

		  /* Fuego */
		  if ((fase[habitacion][j][i] == 59) || (fase[habitacion][j][i] == 60)) {
				if ((ciclo[0] == 1) || (ciclo[0] == 11) || (ciclo[0] == 21)) {
				  if (fase[habitacion][j][i] == 59)
						fase[habitacion][j][i] = 60;
				  else
						fase[habitacion][j][i] = 59;
				}
		  }

		  /* Agua */
		  if ((fase[habitacion][j][i] > 499) && (fase[habitacion][j][i] < 533)) {
				if (fase[habitacion][j][i] < 532)
				  fase[habitacion][j][i]++;
				else
				  fase[habitacion][j][i] = 501;
		  }

		}
  }
}
