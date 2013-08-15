/* pantallas.c */
/* Mostrar ventanas adicionales */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>
#include <SDL_mixer.h>
#include <math.h>
#include <SDL_rotozoom.h>
#include "comun.h"
#include "pantallas.h"

#ifdef _OPENPANDORA
/* Filtro gráfico para la consola OpenPandora */
/* FALSE: Sin filtro (point sampling)         */
/* TRUE:  Con filtro (ligeramente borroso)    */
void filtroPandora(int filtro)
{
	if (filtro) system("sudo -n /usr/pandora/scripts/op_videofir.sh default");
	else system("sudo -n /usr/pandora/scripts/op_videofir.sh none");
}

/* Cambia el fitro gráfico para la versión Pandora, entre "none" y "default" */
void cambiarFiltroPandora(void)
{
	static int filtro = 0;

	filtroPandora(filtro=(!filtro));
}
#endif

void intro1 (SDL_Surface *pantalla, int *estadoj, int *cambiot) {

	SDL_Surface *imagen = NULL;
	SDL_Surface *titlemd = NULL;
	SDL_Surface *abbayemd = NULL;
#ifdef _RENDER_320_240
	SDL_Rect srctile = {0,0,256,192};
	SDL_Rect destile = {32,24,0,0};
	SDL_Rect srcmd = {0,0,178,86};
	SDL_Rect desmd = {66,95,178,86};
	SDL_Rect srcmd2 = {0,0,107,75};
	SDL_Rect desmd2 = {142,40,107,75};
#else
	SDL_Rect srctile = {0,0,512,384};
	SDL_Rect destile = {64,48,0,0};
	SDL_Rect srcmd = {0,0,356,172};
	SDL_Rect desmd = {132,190,356,172};
	SDL_Rect srcmd2 = {0,0,214,150};
	SDL_Rect desmd2 = {284,80,214,150};
#endif
	Mix_Chunk *sonido = NULL;

	int i = 0;
	int salir = 0;
	int frate1 = 0;
	int tecla = 0;
	int sonar = 1;

	imagen = IMG_Load(RUTA_GRA_INTRO);
	abbayemd = IMG_Load(RUTA_GRA_ABBEY_MD);
	titlemd = IMG_Load(RUTA_GRA_TITLE_MD);
	SDL_SetColorKey(titlemd, SDL_SRCCOLORKEY, SDL_MapRGB(titlemd->format, 0, 0, 0) );

#ifndef _RENDER_320_240
	imagen = zoomSurface (imagen, 2, 2, 0);
	abbayemd = zoomSurface (abbayemd, 2, 2, 0);
	titlemd = zoomSurface (titlemd, 2, 2, 0);
#endif

	sonido = Mix_LoadWAV(RUTA_MUS_TITLE);

	while ((salir != 1) && (*estadoj != 2)) {
		frate1 = framerate (1,0);
#ifdef _RENDER_320_240
		srctile.y = 192 * i;
#else
		srctile.y = 384 * i;
#endif
		SDL_FillRect(pantalla, NULL, SDL_MapRGB(pantalla->format, 0, 0, 0));
		SDL_BlitSurface(imagen, &srctile, pantalla, &destile);
		if ((*cambiot == 1) && (i == 0)) {
			SDL_BlitSurface(abbayemd, &srcmd, pantalla, &desmd);
			SDL_BlitSurface(titlemd, &srcmd2, pantalla, &desmd2);
		}
		tecla = 0;
		teclado(&tecla, 1);
		switch (tecla) {
			case 7: if (i == 0)
								i = 1;
					  	else {
								i = 0;
								sonar = 1;
					  	}
					  	break;
			case 5: *estadoj = 2;
					  	break;
			case 6:
#ifdef _OPENPANDORA
							cambiarFiltroPandora();
#else
							SDL_WM_ToggleFullScreen (pantalla);
#endif
							break;
			case 9: if (*cambiot == 0)
								*cambiot = 1;
							else
								*cambiot = 0;
							break;
		}
		if (sonar == 1) {
			Mix_PlayChannel(-1, sonido, 0);
			sonar = 0;
		}
		SDL_Flip (pantalla);
		SDL_FreeSurface (pantalla);
		framerate (2,frate1);
	}

	Mix_FreeChunk (sonido);
	SDL_FreeSurface (imagen);
}

void intro2 (SDL_Surface *pantalla, int *cambiot) {

	SDL_Surface *temp = NULL;
	SDL_Surface *imagen = NULL;
	SDL_Surface *protag = NULL;
	SDL_Surface *temp2 = NULL;
	Mix_Chunk *sonido = NULL;
#ifdef _RENDER_320_240
	SDL_Rect desimagen = {32,24,0,0};
#else
	SDL_Surface *doble = NULL;
	SDL_Rect desimagen = {64,48,0,0};
#endif
	SDL_Rect srcprota = {0,88,16,24};
	SDL_Rect desprota = {0,100,0,0};
	SDL_Rect srcenemigo = {0,64,16,24};
	SDL_Rect desenemigo = {0,100,0,0};

	int salir = 0;
	int frate1 = 0;
	int tecla = 0;
	int sonar = 1;
	int animacion = 0;
	int ciclo = 0;
	int eneminicial = 0;

	temp = IMG_Load(RUTA_GRA_INTRO2);
	imagen = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if (*cambiot == 0)
		temp = IMG_Load(RUTA_GRA_TILES);
	else
		temp = IMG_Load(RUTA_GRA_TILES_MD);
	protag = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	temp = IMG_Load(RUTA_GRA_TEMP);
	temp2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	sonido = Mix_LoadWAV(RUTA_MUS_MANHUNT);

	while (salir != 1) {

		frate1 = framerate (1,0);
		SDL_FillRect(pantalla, NULL, SDL_MapRGB(pantalla->format, 0, 0, 0));
		SDL_FillRect(temp2, NULL, SDL_MapRGB(pantalla->format, 0, 0, 0));
		SDL_BlitSurface(imagen, NULL, temp2, NULL);
		if (ciclo == 5)
			ciclo = 0;
		else
			ciclo ++;
		if (animacion < 13)
			animacion ++;
		else
			animacion = 0;

		if (ciclo < 4) {
			desprota.x += 1;
			srcprota.x = ((animacion / 7) * 16) + 384;
		}

		if (desprota.x < 254)
			SDL_BlitSurface(protag, &srcprota, temp2, &desprota);

		if (desprota.x > 45) {
			if (ciclo < 3) {
				srcenemigo.x = ((animacion / 7) * 16) + 96;
				desenemigo.x ++;
			}
			SDL_BlitSurface(protag, &srcenemigo, temp2, &desenemigo);
			if (desenemigo.x > 30) {
				eneminicial = desenemigo.x;
				desenemigo.x -= 30;
				SDL_BlitSurface(protag, &srcenemigo, temp2, &desenemigo);
				if (desenemigo.x > 30) {
					desenemigo.x -= 30;
					SDL_BlitSurface(protag, &srcenemigo, temp2, &desenemigo);
				}
				desenemigo.x = eneminicial;
			}
		}

#ifdef _RENDER_320_240
		SDL_BlitSurface(temp2, NULL, pantalla, &desimagen);
#else
		doble = zoomSurface (temp2, 2, 2, 0);
		SDL_BlitSurface(doble, NULL, pantalla, &desimagen);
#endif

		if (sonar == 1) {
			Mix_PlayChannel(-1, sonido, 0);
			sonar = 0;
		}

		SDL_Flip (pantalla);
		SDL_FreeSurface (pantalla);
#ifndef _RENDER_320_240
		SDL_FreeSurface (doble);
#endif

		tecla = 0;
		teclado(&tecla, 2);
		switch (tecla) {
			case 5: salir = 1;
					  	break;
			case 6:
#ifdef _OPENPANDORA
							cambiarFiltroPandora();
#else
							SDL_WM_ToggleFullScreen (pantalla);
#endif
							break;
			case 9: if (*cambiot == 0) {
								*cambiot = 1;
								SDL_FreeSurface (protag);
								temp = IMG_Load(RUTA_GRA_TILES_MD);
								protag = SDL_DisplayFormat(temp);
								SDL_FreeSurface(temp);
								SDL_SetColorKey(protag, SDL_SRCCOLORKEY, SDL_MapRGB(protag->format, 0, 0, 0) );
							}
							else {
								*cambiot = 0;
								SDL_FreeSurface (protag);
								temp = IMG_Load(RUTA_GRA_TILES);
								protag = SDL_DisplayFormat(temp);
								SDL_FreeSurface(temp);
								SDL_SetColorKey(protag, SDL_SRCCOLORKEY, SDL_MapRGB(protag->format, 0, 0, 0) );
							}
							break;
		}

		framerate (2,frate1);

	}

	Mix_FreeChunk (sonido);
	SDL_FreeSurface (imagen);
	SDL_FreeSurface (protag);
	SDL_FreeSurface (temp2);

}

void mostrarpergamino (SDL_Surface *ventana, int *pergamino) {

	SDL_Surface *pergamin = NULL;
	SDL_Surface *temp = NULL;
  TTF_Font *fuente = NULL;
	SDL_Surface *marcador = NULL;
  SDL_Color fgcolor = {0,0,0};
	SDL_Rect desmarcador = {0,0,0,0};
	int alto = 0;
	int ancho = 0;
  char renglon1[18];
	char renglon2[20];

  fuente = TTF_OpenFont(RUTA_FON_VENICE_CLASSIC, 18);
	temp = IMG_Load(RUTA_GRA_PERGAMINO);
	pergamin = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	SDL_BlitSurface (pergamin, NULL, ventana, NULL);
	SDL_FreeSurface(pergamin);

	switch (*pergamino) {
		case 3: sprintf (renglon1, "Twelve crosses");
						sprintf (renglon2, "against the devil");
						break;
		case 8:	sprintf (renglon1, "Twelve brothers");
						sprintf (renglon2, "hid and died here");
						break;
		case 12: sprintf (renglon1, "Four brothers");
						 sprintf (renglon2, "changed their faith");
						 break;
		case 14: sprintf (renglon1, "An invisible path");
						 sprintf (renglon2, "over a wood bridge");
						 break;
		case 16: sprintf (renglon1, "Jump to death");
						 sprintf (renglon2, "and prove your faith");
						 break;
		case 21: sprintf (renglon1, "Glide through");
						 sprintf (renglon2, "the beast cage");
						 break;
	}

	marcador = TTF_RenderText_Blended(fuente, renglon1, fgcolor);
	TTF_SizeText(fuente, renglon1, &ancho, &alto);
	desmarcador.x = 127 - (ancho / 2);
	desmarcador.y = 81 - alto;
	SDL_BlitSurface(marcador, NULL, ventana, &desmarcador);
	SDL_FreeSurface (marcador);
	marcador = TTF_RenderText_Blended(fuente, renglon2, fgcolor);
	TTF_SizeText(fuente, renglon2, &ancho, &alto);
	desmarcador.x = 127 - (ancho / 2);
	desmarcador.y = 85;
	SDL_BlitSurface(marcador, NULL, ventana, &desmarcador);
	SDL_FreeSurface (marcador);
  TTF_CloseFont(fuente);

}

void gameover (SDL_Surface *pantalla) {

	SDL_Surface *temp = NULL;
	SDL_Surface *ventana = NULL;
#ifdef _RENDER_320_240
	SDL_Rect srcventana = {0,0,256,192};
#else
	SDL_Surface *doble = NULL;
	SDL_Rect srcventana = {0,0,512,384};
#endif
  SDL_Color fgcolor = {255,255,255};
  TTF_Font *fuente = NULL;
	SDL_Surface *marcador = NULL;
	SDL_Rect desmarcador = {0,0,0,0};
	Mix_Chunk *sonido = NULL;
	int alto = 0;
	int ancho = 0;
  char frase[16];

	SDL_FillRect(pantalla, NULL, SDL_MapRGB(pantalla->format, 0, 0, 0));

  fuente = TTF_OpenFont(RUTA_FON_VENICE_CLASSIC, 18);

	temp = IMG_Load(RUTA_GRA_TEMP);
	ventana = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	sonido = Mix_LoadWAV(RUTA_MUS_GAMEOVER);

	sprintf (frase, "G a m e  O v e r");

	marcador = TTF_RenderText_Blended(fuente, frase, fgcolor);
	TTF_SizeText(fuente, frase, &ancho, &alto);
#ifdef _RENDER_320_240
	desmarcador.x = 125 - 16 - (ancho / 2);
	desmarcador.y = 80 - 12 - (alto / 2);
#else
	desmarcador.x = 125 - (ancho / 2);
	desmarcador.y = 80 - (alto / 2);
#endif
	SDL_BlitSurface(marcador, NULL, ventana, &desmarcador);
#ifdef _RENDER_320_240
	SDL_BlitSurface(ventana, &srcventana, pantalla, &desmarcador);
#else
	doble = zoomSurface (ventana, 2, 2, 0);
	SDL_BlitSurface(doble, &srcventana, pantalla, &desmarcador);
#endif
	SDL_Flip (pantalla);
	Mix_PlayChannel(-1, sonido, 0);

	sleep(12);

	SDL_FreeSurface(pantalla);
	Mix_FreeChunk (sonido);
	SDL_FreeSurface(marcador);
	TTF_CloseFont(fuente);
	SDL_FreeSurface(ventana);
#ifndef _RENDER_320_240
	SDL_FreeSurface(doble);
#endif

}

void pergaminorojo (SDL_Surface *ventana, struct protagonista *jean) {

	SDL_Surface *pergamin = NULL;
	SDL_Surface *temp = NULL;

	temp = IMG_Load(RUTA_GRA_PERGAMINO_ROJO);
	pergamin = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	SDL_BlitSurface (pergamin, NULL, ventana, NULL);
	SDL_FreeSurface(pergamin);

	jean->flags[6] = 4;

}

void pergaminoazul (SDL_Surface *ventana, struct protagonista *jean) {

	SDL_Surface *pergamin = NULL;
	SDL_Surface *temp = NULL;

	temp = IMG_Load(RUTA_GRA_PERGAMINO_AZUL);
	pergamin = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	SDL_BlitSurface (pergamin, NULL, ventana, NULL);
	SDL_FreeSurface(pergamin);

}

void ending (SDL_Surface *pantalla) {

	SDL_Surface *fondo = NULL;
	SDL_Surface *tiles = NULL;
	SDL_Surface *temp = NULL;
	SDL_Surface *marcador = NULL;

	SDL_Color fgcolor = {255,255,255};
  TTF_Font *fuente = NULL;

#ifdef _RENDER_320_240
	SDL_Rect desventana = {32,24,0,0};
	SDL_Rect srcventana = {0,0,256,192};
#else
	SDL_Surface *doble = NULL;
	SDL_Rect desventana = {64,48,0,0};
	SDL_Rect srcventana = {0,0,512,384};
#endif
	SDL_Rect desmarcador = {0,0,0,0};

	SDL_Rect srcpuerta = {600,72,64,48};
	SDL_Rect despuerta = {96,72,64,48};

	Mix_Music *sonido = NULL;

	int i = 0;
	int x = 0;
	int alto = 0;
	int ancho = 0;
	char frase[25];
	int frate1 = 0;

	temp = IMG_Load(RUTA_GRA_TILES);
	tiles = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	temp = IMG_Load(RUTA_GRA_TEMP);
	fondo = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	sonido = Mix_LoadMUS(RUTA_MUS_PRAYER);
	fuente = TTF_OpenFont(RUTA_FON_VENICE_CLASSIC, 18);

	Mix_PlayMusic (sonido, 0);

	for (i=0;i<951;i++) {

		frate1 = framerate (1,0);

		SDL_FillRect(pantalla, NULL, SDL_MapRGB(pantalla->format, 0, 0, 0));
		SDL_FillRect(fondo, NULL, SDL_MapRGB(fondo->format, 0, 0, 0));

		if (i<360)
			x = i/60;
		else
			x = 5;

		srcpuerta.x = 600 + (64 * x);
		SDL_BlitSurface(tiles, &srcpuerta, fondo, &despuerta);
		if (i > 365) {
			sprintf (frase, "Your body has burned");
			marcador = TTF_RenderText_Blended(fuente, frase, fgcolor);
			TTF_SizeText(fuente, frase, &ancho, &alto);
			desmarcador.x = 120 - (ancho / 2);
			desmarcador.y = 20 - (alto / 2);
			SDL_BlitSurface(marcador, NULL, fondo, &desmarcador);
			sprintf (frase, "in the flames,");
			marcador = TTF_RenderText_Blended(fuente, frase, fgcolor);
			TTF_SizeText(fuente, frase, &ancho, &alto);
			desmarcador.x = 125 - (ancho / 2);
			desmarcador.y = 50 - (alto / 2);
			SDL_BlitSurface(marcador, NULL, fondo, &desmarcador);
			sprintf (frase, "but your soul has found");
			marcador = TTF_RenderText_Blended(fuente, frase, fgcolor);
			TTF_SizeText(fuente, frase, &ancho, &alto);
			desmarcador.x = 122 - (ancho / 2);
			desmarcador.y = 125 + (alto / 2);
			SDL_BlitSurface(marcador, NULL, fondo, &desmarcador);
			sprintf (frase, "a place in Heaven");
			marcador = TTF_RenderText_Blended(fuente, frase, fgcolor);
			TTF_SizeText(fuente, frase, &ancho, &alto);
			desmarcador.x = 121 - (ancho / 2);
			desmarcador.y = 155 + (alto / 2);
			SDL_BlitSurface(marcador, NULL, fondo, &desmarcador);
		}
#ifdef _RENDER_320_240
		SDL_BlitSurface(fondo, &srcventana, pantalla, &desventana);
#else
		doble = zoomSurface (fondo, 2, 2, 0);
		SDL_BlitSurface(doble, &srcventana, pantalla, &desventana);
#endif
		SDL_Flip (pantalla);
		SDL_FreeSurface (pantalla);

		framerate (2,frate1);

	}

	SDL_FreeSurface (tiles);
	SDL_FreeSurface (fondo);
#ifndef _RENDER_320_240
	SDL_FreeSurface (doble);
#endif
	Mix_FreeMusic (sonido);
	TTF_CloseFont (fuente);
	SDL_FreeSurface (marcador);

}

