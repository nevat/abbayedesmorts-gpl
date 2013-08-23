/* barradeestado.c */
/* Muestra vidas, cruces y fase actual */
/* en la parte inferior de la pantalla */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "comun.h"

static const char *nome_habitacion[] = {
	":)",
	"A prayer of Hope",
	"Tower of the Bell",
	"Wine supplies"
	":$",
	"Escape !!!",
	"Death is close",
	"Abandoned church",
	"The Altar",
	"Hangman tree",
	"Pestilent Beast",
	"Cave of illusions",
	"Plagued ruins",
	"Catacombs",
	"Hidden garden",
	"Gloomy tunels",
	"Lake of despair",
	"The wheel of faith",
	"Banquet of Death",
	"Underground river",
	"Unexpected gate",
	"Evil church",
	"Tortured souls",
	"Ashes to ashes",
	"Satan !!!"};

void barradeestado (SDL_Surface *ventana, SDL_Surface *tiles, int habitacion, int vidas, int cruces, TTF_Font *fuente) {

  SDL_Rect srcbarra = {448,104,13,12};
  SDL_Rect desbarra = {0,177,0,0};
  SDL_Color fgcolor = {255,255,255}; /* Color de la fuente, blanco */
  SDL_Surface *marcador = NULL;
  SDL_Rect desmarcador = {0,0,0,0};
  int i = 0;

  char datos[1];
  const char *nompantalla;
  int ancho = 0;
  int alto = 0;

  SDL_BlitSurface(tiles, &srcbarra, ventana, &desbarra);
  srcbarra.x = 461;
  srcbarra.w = 12;
  desbarra.x = 32;
  SDL_BlitSurface(tiles, &srcbarra, ventana, &desbarra);

  desmarcador.y = 174;

	sprintf(datos, "%d", vidas);
	marcador = TTF_RenderText_Solid(fuente, datos, fgcolor);
	desmarcador.x = 18;
	SDL_BlitSurface(marcador, NULL, ventana, &desmarcador);
	
	sprintf(datos, "%d", cruces);
	marcador = TTF_RenderText_Solid(fuente, datos, fgcolor);
	desmarcador.x = 50;
	SDL_BlitSurface(marcador, NULL, ventana, &desmarcador);

	nompantalla = nome_habitacion[habitacion];
	marcador = TTF_RenderText_Solid(fuente, nompantalla, fgcolor);
	TTF_SizeText(fuente, nompantalla, &ancho, &alto);
	desmarcador.x = 256 - ancho;
	
	SDL_BlitSurface(marcador, NULL, ventana, &desmarcador);
	SDL_FreeSurface (marcador);
	
}
