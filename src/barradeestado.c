/* barradeestado.c */
/* Muestra vidas, cruces y fase actual */
/* en la parte inferior de la pantalla */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "comun.h"

void barradeestado (SDL_Surface *ventana, SDL_Surface *tiles, int habitacion, int vidas, int cruces, TTF_Font *fuente) {

  SDL_Rect srcbarra = {448,104,13,12};
  SDL_Rect desbarra = {0,177,0,0};
  SDL_Color fgcolor = {255,255,255}; /* Color de la fuente, blanco */
  SDL_Surface *marcador = NULL;
  SDL_Rect desmarcador = {0,0,0,0};
  int i = 0;

  char datos[1];
  char nompantalla[18];
  int ancho = 0;
  int alto = 0;

  SDL_BlitSurface(tiles, &srcbarra, ventana, &desbarra);
  srcbarra.x = 461;
  srcbarra.w = 12;
  desbarra.x = 32;
  SDL_BlitSurface(tiles, &srcbarra, ventana, &desbarra);

  desmarcador.y = 174;

  for (i=0; i<=2; i++) {
		switch (i) {
			case 0: sprintf(datos, "%d", vidas);
					marcador = TTF_RenderText_Solid(fuente, datos, fgcolor);
					desmarcador.x = 18;
					break;
			case 1: sprintf(datos, "%d", cruces);
					marcador = TTF_RenderText_Solid(fuente, datos, fgcolor);
					desmarcador.x = 50;
					break;
			case 2:
					if (habitacion == 0)
						sprintf (nompantalla, ":)");
					if (habitacion == 1)
						sprintf (nompantalla, "A prayer of Hope");
					if (habitacion == 2)
						sprintf (nompantalla, "Tower of the Bell");
					if (habitacion == 3)
						sprintf (nompantalla, "Wine supplies");
					if (habitacion == 4)
						sprintf (nompantalla, ":$");
					if (habitacion == 5)
						sprintf (nompantalla, "Escape !!!");
					if (habitacion == 6)
						sprintf (nompantalla, "Death is close");
					if (habitacion == 7)
						sprintf (nompantalla, "Abandoned church");
					if (habitacion == 8)
						sprintf (nompantalla, "The Altar");
					if (habitacion == 9)
						sprintf (nompantalla, "Hangman tree");
					if (habitacion == 10)
						sprintf (nompantalla, "Pestilent Beast");
					if (habitacion == 11)
						sprintf (nompantalla, "Cave of illusions");
					if (habitacion == 12)
						sprintf (nompantalla, "Plagued ruins");
					if (habitacion == 13)
						sprintf (nompantalla, "Catacombs");
					if (habitacion == 14)
						sprintf (nompantalla, "Hidden garden");
					if (habitacion == 15)
						sprintf (nompantalla, "Gloomy tunels");
					if (habitacion == 16)
						sprintf (nompantalla, "Lake of despair");
					if (habitacion == 17)
						sprintf (nompantalla, "The wheel of faith");
					if (habitacion == 18)
						sprintf (nompantalla, "Banquet of Death");
					if (habitacion == 19)
						sprintf (nompantalla, "Underground river");
					if (habitacion == 20)
						sprintf (nompantalla, "Unexpected gate");
					if (habitacion == 21)
						sprintf (nompantalla, "Evil church");
					if (habitacion == 22)
						sprintf (nompantalla, "Tortured souls");
					if (habitacion == 23)
						sprintf (nompantalla, "Ashes to ashes");
					if (habitacion == 24)
						sprintf (nompantalla, "Satan !!!");

					marcador = TTF_RenderText_Solid(fuente, nompantalla, fgcolor);
					TTF_SizeText(fuente, nompantalla, &ancho, &alto);
					desmarcador.x = 256 - ancho;
					break;
		}

		SDL_BlitSurface(marcador, NULL, ventana, &desmarcador);
		SDL_FreeSurface (marcador);

  }

}
