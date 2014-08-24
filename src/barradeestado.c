/* barradeestado.c */
/* Muestra vidas, cruces y fase actual */
/* en la parte inferior de la pantalla */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "comun.h"

void barradeestado (SDL_Surface *ventana, SDL_Surface *tiles, int habitacion, int vidas, int cruces, TTF_Font *fuente, SDL_Surface *fuentes) {

  SDL_Rect srcbarra = {448,104,13,12};
  SDL_Rect desbarra = {0,177,0,0};
	SDL_Rect srcnumbers = {0,460,10,10};
	SDL_Rect desnumbers = {18,178,10,10};
	SDL_Rect srctext = {0,0,140,20};
	SDL_Rect destext = {115,176,136,18};
  int i = 0;

  SDL_BlitSurface(tiles, &srcbarra, ventana, &desbarra);
  srcbarra.x = 461;
  srcbarra.w = 12;
  desbarra.x = 32;
  SDL_BlitSurface(tiles, &srcbarra, ventana, &desbarra);

	for (i=0; i<=2; i++) {
		switch (i) {
			case 0: srcnumbers.x = vidas * 10;
							SDL_BlitSurface(fuentes,&srcnumbers,ventana,&desnumbers);
							break;
			case 1: if (cruces < 10) {
								desnumbers.x = 50;
								srcnumbers.x = cruces * 10;
								SDL_BlitSurface(fuentes,&srcnumbers,ventana,&desnumbers);
							}
							else {
								desnumbers.x = 50;
								srcnumbers.x = 10;
								SDL_BlitSurface(fuentes,&srcnumbers,ventana,&desnumbers);
								desnumbers.x = 55;
								srcnumbers.x = (cruces - 10) * 10;
								SDL_BlitSurface(fuentes,&srcnumbers,ventana,&desnumbers);
							}
							break;
			case 2: if ((habitacion > 0) && (habitacion < 4)) {
								srctext.y = (habitacion - 1) * 20;
								SDL_BlitSurface(fuentes,&srctext,ventana,&destext);
							}
							if (habitacion > 4) {
								srctext.y = (habitacion - 2) * 20;
								SDL_BlitSurface(fuentes,&srctext,ventana,&destext);
							}
							break;
		}

	}

}
