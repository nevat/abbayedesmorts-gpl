/* Abbaye des Morts */
/* Version 1.0 */
/* Port iniciado el 18/10/2012 */
/* Version 1.0 finalizada el 28/12/2012 */
/* Version 1.1 finalizada el 14/01/2013 */

/* (c) 2010 - Locomalito & Gryzor87 */
/* 2012 - Reprogramado por Nevat */

/* Licencia GPL 3 */
/* 2012 - Nevat
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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
#include <SDL_getenv.h>
#include "main.h"

main () {

  /* Variables */
  SDL_Surface *pantalla = NULL;
  int estadoj = 1; /* 1-Intro, 2-Intro2, 3-Juego, 4-Gameover, 5-Fin */
  int salir = 0;
	int cambiot = 0; /* 0-Graficos 8 bits, 1-16bits */

  /* Iniciar SDL */
  iniciar_sdl ();
	SDL_WM_SetCaption("Abbaye des Morts - v1.11", "Abbaye des Morts");
#ifdef _RENDER_320_240
  pantalla = SDL_SetVideoMode(320,240,8,SDL_HWSURFACE|SDL_DOUBLEBUF);
#else
  pantalla = SDL_SetVideoMode(640,480,8,SDL_HWSURFACE|SDL_DOUBLEBUF);
#endif

	/* Ocultando el raton en el área de juego */
	SDL_ShowCursor(SDL_DISABLE);

#ifdef _OPENPANDORA
	/* Sin filtro por defecto en la versión para OpenPandora */
	filtroPandora(0);
#endif

  /* Game Loop */
  while (salir != 1) {
		switch (estadoj) {
	  	case 1: intro1 (pantalla, &estadoj, &cambiot);
				  		estadoj = 2;
				  		break;
	  	case 2: intro2 (pantalla, &cambiot);
				  		estadoj = 3;
				  		break;
	  	case 3: juego (pantalla, &estadoj, &cambiot);
				  		break;
			case 4: gameover (pantalla);
							estadoj = 1;
							break;
			case 5: ending (pantalla);
							estadoj = 1;
							break;
		}
  }
  exit(0);
}

void iniciar_sdl () {

	/* Centrar ventana */
	putenv("SDL_VIDEO_CENTERED=1");

  if (SDL_Init(SDL_INIT_VIDEO)<0) {
	printf("No se puede inicializar SDL: %s\n",SDL_GetError());
	exit(1);
  }
  atexit(SDL_Quit);

  if (TTF_Init() < 0) {
	printf("No se puede inicializar SDL_ttf: %s\n",SDL_GetError());
	exit(1);
  }
  atexit(TTF_Quit);

  if (Mix_OpenAudio (44100, AUDIO_S16, 1, 4096)) {
	printf("No se pudo inicializar SDL_Mixer %s\n",Mix_GetError());
	exit(1);
  }
  Mix_AllocateChannels(3);
  atexit(Mix_CloseAudio);

}

int framerate (int i, int frate1) {
/* Control del framerate, 60 Hz */

	int now = SDL_GetTicks();

  if (i == 1) {
		return now;
		
	}

  if (i == 2) {
		int diff = now - frate1;
		if (diff<16)
			SDL_Delay(16-diff);
		return 0;
	}
}

void juego (SDL_Surface *pantalla, int *estadoj, int *cambiot) {

	SDL_Surface *ventana = NULL;
	SDL_Surface *tiles = NULL;
	SDL_Surface *temp = NULL;
#ifdef _RENDER_320_240
	SDL_Rect desventana = {32,24,0,0};
	SDL_Rect srcventana = {0,0,256,192};
#else
	SDL_Surface *doble = NULL;
	SDL_Rect desventana = {64,48,0,0};
	SDL_Rect srcventana = {0,0,512,384};
#endif
	SDL_Surface *fondomd = NULL;
	TTF_Font *fuente = NULL;
	SDL_Event evento;

	Mix_Music *sonido[8];
	Mix_Chunk *fx[7];

	int fase[25][22][32];
	int datosenem[25][7][15];
	int habitacion = 5;
	int prehabitacion = 5;
	int coordx = 0;
	int coordy = 0;
	int salir = 0;
	int frate1 = 0;
	int tecla = 0;
	int cambio = 1;
	int ciclo[3] = {0,0,0};
	float proyec[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int n = 0;
	int pergamino = 0;
	int teclap = 0;
	int i = 0;

  struct enem enemigos = {
		.tipo = {0,0,0,0,0,0,0},
		.x = {0,0,0,0,0,0,0},
		.y = {0,0,0,0,0,0,0},
		.direccion = {0,0,0,0,0,0,0},
		.tilex = {0,0,0,0,0,0,0},
		.tiley = {0,0,0,0,0,0,0},
		.animacion = {0,0,0,0,0,0,0},
		.limizq = {0,0,0,0,0,0,0},
		.limder = {0,0,0,0,0,0,0},
	  .velocidad = {0,0,0,0,0,0,0},
		.disparo = {0,0,0,0,0,0,0},
		.ajustex1 = {0,0,0,0,0,0,0},
		.ajustex2 = {0,0,0,0,0,0,0},
		.ajustey1 = {0,0,0,0,0,0,0},
		.ajustey2 = {0,0,0,0,0,0,0}
	};

	struct protagonista jean = {
		.x = 72,
		.y = 136,
		.direccion = 1,
		.salto = 0,
		.altura = 0,
		.animacion = 0,
		.gravedad = 1.9,
		.puntos = {0,0,0,0,0,0,0,0},
		.suelo = 0,
		.colision = {0,0,0,0},
		.agachado = 0,
		.checkpoint = {5,72,136,5},
		.estado = {9,0},
		.flags = {0,0,0,0,0,0,0},
		.muerte = 0,
		.pulsa = {0,0,0,0},
		.temp = 0
	};

	temp = IMG_Load(RUTA_GRA_TEMP);
	ventana = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if (*cambiot == 0)
		temp = IMG_Load(RUTA_GRA_TILES);
	else
		temp = IMG_Load(RUTA_GRA_TILES_MD);
	tiles = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	SDL_SetColorKey(tiles, SDL_SRCCOLORKEY, SDL_MapRGB(tiles->format, 0, 0, 0) );

	fuente = TTF_OpenFont(RUTA_FON_VENICE_CLASSIC, 17);

	cargardatos (fase, datosenem);
	cargar_musica (sonido, fx);

	while (salir != 1) {

		frate1 = framerate (1,0);
		/* Pantalla completa o ventana */
		if (tecla == 6)
#ifdef _OPENPANDORA
			cambiarFiltroPandora();	// En OpenPandora cambiamos el filtro en lugar de ventana/pantalla
#else
			SDL_WM_ToggleFullScreen (pantalla);
#endif
		/* Cambio de graficos */
		if (tecla == 9) {
			switch (*cambiot) {
				case 0: *cambiot = 1;
								SDL_FreeSurface (tiles);
								temp = IMG_Load(RUTA_GRA_TILES_MD);
								tiles = SDL_DisplayFormat(temp);
								SDL_FreeSurface(temp);
								SDL_SetColorKey(tiles, SDL_SRCCOLORKEY, SDL_MapRGB(tiles->format, 0, 0, 0) );
								if (habitacion != 4) {
									SDL_FreeSurface(fondomd);
									if (habitacion == 1)
										temp = IMG_Load(RUTA_GRA_FONDO01_MD);
									if (habitacion == 2)
										temp = IMG_Load(RUTA_GRA_FONDO02_MD);
									if (habitacion == 3)
										temp = IMG_Load(RUTA_GRA_FONDO03_MD);
									if (habitacion == 5)
										temp = IMG_Load(RUTA_GRA_FONDO05_MD);
									if (habitacion == 6)
										temp = IMG_Load(RUTA_GRA_FONDO06_MD);
									if (habitacion == 7)
										temp = IMG_Load(RUTA_GRA_FONDO07_MD);
									if (habitacion == 8)
										temp = IMG_Load(RUTA_GRA_FONDO08_MD);
									if (habitacion == 9)
										temp = IMG_Load(RUTA_GRA_FONDO09_MD);
									if (habitacion == 10)
										temp = IMG_Load(RUTA_GRA_FONDO10_MD);
									if (habitacion == 11)
										temp = IMG_Load(RUTA_GRA_FONDO11_MD);
									if (habitacion == 12)
										temp = IMG_Load(RUTA_GRA_FONDO12_MD);
									if (habitacion == 13)
										temp = IMG_Load(RUTA_GRA_FONDO13_MD);
									if (habitacion == 14)
										temp = IMG_Load(RUTA_GRA_FONDO14_MD);
									if (habitacion == 15)
										temp = IMG_Load(RUTA_GRA_FONDO15_MD);
									if (habitacion == 16)
										temp = IMG_Load(RUTA_GRA_FONDO16_MD);
									if (habitacion == 17)
										temp = IMG_Load(RUTA_GRA_FONDO17_MD);
									if (habitacion == 18)
										temp = IMG_Load(RUTA_GRA_FONDO18_MD);
									if (habitacion == 19)
										temp = IMG_Load(RUTA_GRA_FONDO19_MD);
									if (habitacion == 20)
										temp = IMG_Load(RUTA_GRA_FONDO20_MD);
									if (habitacion == 21)
										temp = IMG_Load(RUTA_GRA_FONDO21_MD);
									if (habitacion == 22)
										temp = IMG_Load(RUTA_GRA_FONDO22_MD);
									if (habitacion == 23)
										temp = IMG_Load(RUTA_GRA_FONDO23_MD);
									if (habitacion == 24)
										temp = IMG_Load(RUTA_GRA_FONDO24_MD);
									fondomd = SDL_DisplayFormat(temp);
									SDL_FreeSurface(temp);
								}
								break;
				case 1: *cambiot = 0;
								SDL_FreeSurface (tiles);
								temp = IMG_Load(RUTA_GRA_TILES);
								tiles = SDL_DisplayFormat(temp);
								SDL_FreeSurface(temp);
								SDL_SetColorKey(tiles, SDL_SRCCOLORKEY, SDL_MapRGB(tiles->format, 0, 0, 0) );
								break;
			}
		}
		/* Contadores */
		if (ciclo[0] < 29)
			ciclo[0] ++;
		else
			ciclo[0] = 0;
		if (ciclo[1] < 90)
		  ciclo[1] ++;
		else
		  ciclo[1] = 0;
		if (ciclo[2] < 8)
		  ciclo[2] ++;
		else
		  ciclo[2] = 0;
		SDL_FillRect(pantalla, NULL, SDL_MapRGB(ventana->format, 0, 0, 0));
		animitems (fase, habitacion, ciclo);
		dibujarfase (ventana, fase, tiles, habitacion, ciclo, *cambiot, fondomd, fx);
		if (habitacion != 4)
			barradeestado (ventana, tiles, habitacion, jean.estado[0], jean.estado[1], fuente);
		if (jean.flags[6] < 8)
			dibujarjean (ventana, tiles, &jean, ciclo, fx);
		/* Gestion de enemigos */
		if (enemigos.tipo[0] > 0) {
			if (habitacion != 4)
				moverenemigos (&enemigos, fase, ciclo, proyec, jean, fx);
			if ((habitacion == 5) || (habitacion == 6))
				cruzados (&enemigos, ventana, tiles, ciclo, habitacion);
			if (habitacion == 10)
				dragon (&enemigos, ventana, tiles, ciclo, proyec, fx);
			if (habitacion == 11)
				bolafuego (&enemigos, ventana, tiles, ciclo, jean, fase);
			if (habitacion == 14)
				plantas (&enemigos, ventana, tiles, ciclo, proyec, fx);
			if (habitacion == 9)
			 	dibujarcuerda (enemigos, ventana, tiles);
			if (habitacion == 18)
				muerte (&enemigos, ventana, tiles, ciclo, proyec, fase, fx);
			if ((habitacion == 24) && (enemigos.tipo[0] == 18))
				satan (&enemigos, ventana, tiles, ciclo, proyec, fx);
			if ((habitacion == 24) && (jean.flags[6] == 5))
				cruzados (&enemigos, ventana, tiles, ciclo, habitacion);
			dibujarenemigos (&enemigos, ventana, tiles, fx);
		}
		/* Gestion de disparos */
		if ((proyec[0] > 0) && ((habitacion == 17) || (habitacion == 20) || (habitacion == 21) || (habitacion == 22)))
		  dibujardisparo (proyec, tiles, ventana, &enemigos);
		/* Gestion de Jean */
		if (jean.muerte == 0) {
			if (jean.flags[6] < 5) {
					if (jean.temp == 0)
						tecladoj(&jean, &tecla);
					if (jean.temp == 1)
						jean.temp = 0;
					colisiones (&jean, fase, habitacion);
					moverjean (&jean, fx);
			}
			if (habitacion != 4) {
				tocarobjetos (&jean, fase, &habitacion, &pergamino, &cambio, &enemigos, proyec, fx);
				contacto (&jean, enemigos, proyec, habitacion);
			}
			eventos (&jean, fase, habitacion, ciclo, &enemigos, fx);
		}
		if (jean.muerte == 98) { /* Pasos a realizar cuando se pierde una vida */
			if (habitacion != 4) {
				habitacion = jean.checkpoint[0];
				jean.x = jean.checkpoint[1];
				jean.y = jean.checkpoint[2];
				jean.salto = 0;
				jean.altura = 0;
				jean.pulsa[0] = 0;
				jean.pulsa[1] = 0;
				jean.pulsa[2] = 0;
				jean.pulsa[3] = 0;
				cambio = 2;
				jean.estado[0] --;
				jean.muerte = 0;
				jean.temp = 1;
				musica (habitacion, sonido, &cambio, prehabitacion, jean.flags[6]);
				Mix_ResumeMusic ();
			}
			else {
				jean.muerte = 0;
				jean.flags[6] = 8;
			}
		}
		/* Usar el flag 6 como contador, para hacer tiempo */
		if (jean.flags[6] > 7)
			jean.flags[6] ++;
		/* Al llegar a 15, que salte la pantalla final */
		if (jean.flags[6] == 15) {
			*estadoj = 5;
			salir = 1;
		}
		cambiopantalla (&jean, &habitacion, &cambio, &prehabitacion);
		if (cambio > 0) {
			buscarenemigos (habitacion, &enemigos, &cambio, datosenem);
			musica (habitacion, sonido, &cambio, prehabitacion, jean.flags[6]);
			for (n=0; n<24; n++) {
			  proyec[n] = 0;
			}
			if (habitacion != 4) {
				SDL_FreeSurface(fondomd);
				if (habitacion == 1)
					temp = IMG_Load(RUTA_GRA_FONDO01_MD);
				if (habitacion == 2)
					temp = IMG_Load(RUTA_GRA_FONDO02_MD);
				if (habitacion == 3)
					temp = IMG_Load(RUTA_GRA_FONDO03_MD);
				if (habitacion == 5)
					temp = IMG_Load(RUTA_GRA_FONDO05_MD);
				if (habitacion == 6)
					temp = IMG_Load(RUTA_GRA_FONDO06_MD);
				if (habitacion == 7)
					temp = IMG_Load(RUTA_GRA_FONDO07_MD);
				if (habitacion == 8)
					temp = IMG_Load(RUTA_GRA_FONDO08_MD);
				if (habitacion == 9)
					temp = IMG_Load(RUTA_GRA_FONDO09_MD);
				if (habitacion == 10)
					temp = IMG_Load(RUTA_GRA_FONDO10_MD);
				if (habitacion == 11)
					temp = IMG_Load(RUTA_GRA_FONDO11_MD);
				if (habitacion == 12)
					temp = IMG_Load(RUTA_GRA_FONDO12_MD);
				if (habitacion == 13)
					temp = IMG_Load(RUTA_GRA_FONDO13_MD);
				if (habitacion == 14)
					temp = IMG_Load(RUTA_GRA_FONDO14_MD);
				if (habitacion == 15)
					temp = IMG_Load(RUTA_GRA_FONDO15_MD);
				if (habitacion == 16)
					temp = IMG_Load(RUTA_GRA_FONDO16_MD);
				if (habitacion == 17)
					temp = IMG_Load(RUTA_GRA_FONDO17_MD);
				if (habitacion == 18)
					temp = IMG_Load(RUTA_GRA_FONDO18_MD);
				if (habitacion == 19)
					temp = IMG_Load(RUTA_GRA_FONDO19_MD);
				if (habitacion == 20)
					temp = IMG_Load(RUTA_GRA_FONDO20_MD);
				if (habitacion == 21)
					temp = IMG_Load(RUTA_GRA_FONDO21_MD);
				if (habitacion == 22)
					temp = IMG_Load(RUTA_GRA_FONDO22_MD);
				if (habitacion == 23)
					temp = IMG_Load(RUTA_GRA_FONDO23_MD);
				if (habitacion == 24)
					temp = IMG_Load(RUTA_GRA_FONDO24_MD);
				fondomd = SDL_DisplayFormat(temp);
				SDL_FreeSurface(temp);
			}
			ciclo[0] = 0;
			cambio = 0;
		}
		/* Pergaminos */
		if (pergamino > 0)
			mostrarpergamino (ventana, &pergamino);
		if (jean.flags[6] == 3)
			pergaminorojo (ventana, &jean);
		if (jean.flags[6] == 6)
			pergaminoazul (ventana, &jean);
#ifndef _RENDER_320_240
		/* Zoom 2x */
		doble = zoomSurface (ventana, 2, 2, 0);
		SDL_BlitSurface(doble, &srcventana, pantalla, &desventana);
#else
		SDL_BlitSurface(ventana, &srcventana, pantalla, &desventana);
#endif
		SDL_Flip (pantalla);
		if (pergamino > 0) {
			Mix_PlayChannel(-1, fx[2], 0);
			Mix_PauseMusic ();
			/* Esperamos la pulsacion de una tecla */
			while (teclap == 0)
				tecladop (&teclap);
			jean.pulsa[2] = 0;
			jean.pulsa[3] = 0;
			teclap = 0;
			Mix_ResumeMusic ();
			pergamino = 0;
		}
		if (jean.flags[6] == 4) {
			Mix_PlayChannel(-1, fx[2], 0);
			sleep(5);
			jean.flags[6] = 5;
			jean.direccion = 0;
			musica (habitacion, sonido, &cambio, prehabitacion, jean.flags[6]);
		}
		if (jean.flags[6] == 6) {
			sleep(5);
			jean.muerte = 0;
			cambio = 1;
			habitacion = 4;
			jean.flags[6] = 7;
			jean.x = 125;
			jean.y = 115;
			jean.salto = 1;
		}
		/* Cambio de fase */
		SDL_FreeSurface (pantalla);
#ifndef _RENDER_320_240
		SDL_FreeSurface (doble);
#endif
		framerate (2, frate1);

		if (jean.estado[0] == 0) {
			Mix_HaltMusic();
			/* Mix_FreeMusic(sonido); */
			*estadoj = 4;
			salir = 1;
		}

	}

	TTF_CloseFont(fuente);

	for (i=0;i<8;i++)
		Mix_FreeMusic(sonido[i]);

	for (i=0;i<7;i++)
		Mix_FreeChunk(fx[i]);

}

void cargardatos (int fase[][22][32], int datosenem[][7][15]) {

  FILE *fichero = NULL;
  int i = 0;
	int j = 0;
	int k = 0;
  char linea[129], temp[4];
  char linea2[61];
	temp[3] = 0;

  /* Cargar el fichero */
  fichero = fopen(RUTA_DAT_MAPA, "r");
  fgets (linea, 129, fichero);
  fgets (linea, 129, fichero);

  /* Cargamos los datos del fichero en el array */
 	for (i=0; i<=24; i++) {
	  for (j=0; j<=21; j++) {
			for (k=0; k<=31; k++) {
			  temp[0] = linea[k*4];
			  temp[1] = linea[(k*4) + 1];
			  temp[2] = linea[(k*4) + 2];
			  sscanf (temp, "%d", &fase[i][j][k]);
			}
			fgets (linea, 129, fichero);
	  }
	  fgets (linea, 129, fichero);
	}

  /* Cerramos fichero */
  fclose (fichero);

  fichero = fopen(RUTA_DAT_ENEMIGOS, "r");
  fgets (linea2, 61, fichero);
  fgets (linea2, 61, fichero);

  /* Cargamos los datos del fichero en el array */
	for (i=0; i<=24; i++) {
	  for (j=0; j<7; j++) {
			for (k=0; k<15; k++) {
			  temp[0] = linea2[k*4];
			  temp[1] = linea2[(k*4) + 1];
			  temp[2] = linea2[(k*4) + 2];
			  sscanf (temp, "%d", &datosenem[i][j][k]);
			}
			fgets (linea2, 61, fichero);
	  }
	  fgets (linea2, 61, fichero);
	}

  fclose (fichero);

}

void dibujarcuerda (struct enem enemigos, SDL_Surface *ventana, SDL_Surface *tiles) {

  int i = 0;
  int bloques = 0;
  int j = 0;
  SDL_Rect srctile = {424,8,16,8};
  SDL_Rect destile = {0,0,16,8};

  for (i=2; i<6; i++) {
		bloques = (enemigos.y[i] - (enemigos.limizq[i] - 8)) / 8;
		for (j=0; j<=bloques; j++) {
	  	destile.x = enemigos.x[i];
	  	destile.y = (enemigos.limizq[i] - 8) + (8 * j);
	  	SDL_BlitSurface(tiles, &srctile, ventana, &destile);
		}
  }

}

void dibujardisparo (float proyec[], SDL_Surface *tiles, SDL_Surface *ventana, struct enem *enemigos) {
/* Disparos de esqueletos y gargolas */

  SDL_Rect srctile = {656,24,16,8};
  SDL_Rect destile = {0,0,0,0};
  int i = 0;
  int n = 0;

  for (n=0; n<=4; n+=2) {
		if (proyec[n] > 0) {
	  	i = proyec[n+1];

	  	if (enemigos->tipo[i] == 15) {
				srctile.h = 16;
				srctile.x = 640 - (16 * enemigos->direccion[i]);
	  	}

	  	/* Mover el disparo */
	  	if (enemigos->direccion[i] == 1) {
				if (proyec[n] > enemigos->limizq[i])
				  proyec[n] -= 2.5;
				else {
				  enemigos->disparo[i] = 0;
				  enemigos->velocidad[i] = 0;
				  proyec[n] = 0;
				}
	  	}
	  	else {
				if (proyec[n] < enemigos->limder[i])
		  		proyec[n] += 2.5;
				else {
		  		enemigos->disparo[i] = 0;
				  enemigos->velocidad[i] = 0;
				  proyec[n] = 0;
				}
	  	}

	  	/* Dibujar el disparo */
	  	switch (enemigos->direccion[i]) {
				case 0: if ((proyec[n] < (enemigos->limder[i] - 8)) && (proyec[n] != 0)) {
								  destile.x = proyec[n];
								  destile.y = enemigos->y[i] + 8;
								  SDL_BlitSurface(tiles, &srctile, ventana, &destile);
								}
								break;
				case 1: if (proyec[n] > (enemigos->limizq[i] + 8)) {
								  destile.x = proyec[n];
								  destile.y = enemigos->y[i] + 8;
								  SDL_BlitSurface(tiles, &srctile, ventana, &destile);
								}
								break;
	  	}
		}

	}

}

void cambiopantalla (struct protagonista *jean, int *habitacion, int *cambio, int *prehabitacion) {
/* Cambiar de fase al tocar los limites */

	SDL_Surface *temp = NULL;

  if ((jean->x < 1) && (*habitacion != 5)) {
		*prehabitacion = *habitacion;
		*habitacion -= 1;
		jean->x = 240;
		*cambio = 1;
  }
  if ((jean->x + 8) > 256) {
		*prehabitacion = *habitacion;
		*habitacion += 1;
		jean->x = 1;
		*cambio = 1;
  }
	if ((jean->y + 12 < -16) && (jean->salto == 1)) {
		*prehabitacion = *habitacion;
		*habitacion -=5;
		jean->y = 152;
		*cambio = 1;
	}
	if ((jean->y > 175) && (jean->salto != 1)) {
		*prehabitacion = *habitacion;
		*habitacion +=5;
		jean->y = -16;
		*cambio = 1;
	}

}

void tocarobjetos (struct protagonista *jean, int fase[][22][32], int *habitacion, int *pergamino, int *cambio, struct enem *enemigos, float proyec[], Mix_Chunk *fx[]) {

	int x = 0;
	int y = 0;
	int h = 0;
	int v = 0;
	int r = 0;
	int flag = 0;

	x = (jean->x + 2) / 8;
	y = jean->y / 8;

	if (y > 0) {

		/* Tocar pinchos, agua o fuego (provisional) */
		if (((fase[*habitacion][y+3][x] == 5) || (fase[*habitacion][y+3][x+1] == 5)) || (((fase[*habitacion][y+3][x] > 500) && (fase[*habitacion][y+3][x] < 532))|| ((fase[*habitacion][y+3][x+1] > 500) && (fase[*habitacion][y+3][x+1] < 532))) || (((fase[*habitacion][y+3][x] == 59) || (fase[*habitacion][y+3][x] == 59)) || ((fase[*habitacion][y+3][x+1] == 60) && (fase[*habitacion][y+3][x+1] == 60)))) {
			if ((*habitacion == 11) && (y+3 == 20) && (x < 4))
				jean->muerte = 0;
			else {
				if (jean->muerte == 0)
					jean->muerte = 1;
			}
		}

		/* Tocar checkpoint */
		if (((fase[*habitacion][y][x] > 320) &&  (fase[*habitacion][y][x] < 325)) || ((fase[*habitacion][y][x+1] > 320) &&  (fase[*habitacion][y][x+1] < 325))) {
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((fase[*habitacion][v][h] > 320) && (fase[*habitacion][v][h] < 325))
						fase[*habitacion][v][h] += 6;
				}
			}
			jean->checkpoint[3] = jean->checkpoint[0];
			jean->checkpoint[0] = *habitacion;
			jean->checkpoint[1] = jean->x;
			jean->checkpoint[2] = jean->y;
			/* El antiguo checkpoint marcado vuelve a su estado original */
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((fase[jean->checkpoint[3]][v][h] > 326) && (fase[jean->checkpoint[3]][v][h] < 331))
						fase[jean->checkpoint[3]][v][h] -= 6;
				}
			}
			Mix_PlayChannel(-1, fx[2], 0);
		}

		/* Tocar campana */
		if (*habitacion == 2) {
			if (((fase[*habitacion][y+1][x] > 300) &&  (fase[*habitacion][y+1][x] < 305)) || ((fase[*habitacion][y+1][x+1] > 300) &&  (fase[*habitacion][y+1][x+1] < 305))) {
				for (v=1;v<3;v++) {
					for (h=5;h<7;h++) {
						if ((fase[*habitacion][v][h] > 300) && (fase[*habitacion][v][h] < 305))
							fase[*habitacion][v][h] += 4;
					}
				}
				jean->flags[1] = 1;
				Mix_PauseMusic ();
				Mix_PlayChannel(-1, fx[5], 0);
				sleep(2);
				Mix_ResumeMusic ();
			}
		}

		/* Tocar palanca */
		if (((fase[*habitacion][y+1][x] > 308) && (fase[*habitacion][y+1][x] < 313)) || ((fase[*habitacion][y+1][x+1] > 308) && (fase[*habitacion][y+1][x+1] < 313))) {
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((fase[*habitacion][v][h] > 308) && (fase[*habitacion][v][h] < 313))
						fase[*habitacion][v][h] += 4;
				}
			}
			if (*habitacion == 9)
				jean->flags[3] = 1;
			if (*habitacion == 10)
				jean->flags[2] = 1;
			if (*habitacion == 20)
				jean->flags[4] = 1;
			Mix_PauseMusic ();
			Mix_PlayChannel(-1, fx[5], 0);
			sleep(2);
			Mix_ResumeMusic ();
		}


		/* Tocar corazones */
		if (*habitacion == 23) {
			if (((fase[*habitacion][y+1][x] > 400) &&  (fase[*habitacion][y+1][x] < 405)) || ((fase[*habitacion][y+1][x+1] > 400) &&  (fase[*habitacion][y+1][x+1] < 405))) {
				if (jean->x > 160) {
					fase[23][7][23] = 0;
					fase[23][7][24] = 0;
					fase[23][8][23] = 0;
					fase[23][8][24] = 0;
				}
				else {
					fase[23][18][8] = 0;
					fase[23][18][9] = 0;
					fase[23][19][8] = 0;
					fase[23][19][9] = 0;
				}
				if (jean->estado[0] < 9)
					jean->estado[0] += 1;
				Mix_PlayChannel(-1, fx[2], 0);;
			}
		}
		else {
			if (((fase[*habitacion][y+1][x] > 400) &&  (fase[*habitacion][y+1][x] < 405)) || ((fase[*habitacion][y+1][x+1] > 400) &&  (fase[*habitacion][y+1][x+1] < 405))) {
				for (v=0;v<22;v++) {
					for (h=0;h<32;h++) {
						if ((fase[*habitacion][v][h] > 400) && (fase[*habitacion][v][h] < 405))
							fase[*habitacion][v][h] = 0;
					}
				}
				if (jean->estado[0] < 9)
					jean->estado[0] += 1;
				Mix_PlayChannel(-1, fx[2], 0);
			}
		}

		/* Tocar cruces */
		if (((fase[*habitacion][y+1][x] > 408) &&  (fase[*habitacion][y+1][x] < 413)) || ((fase[*habitacion][y+1][x+1] > 408) &&  (fase[*habitacion][y+1][x+1] < 413)) || ((fase[*habitacion][y+2][x] > 408) &&  (fase[*habitacion][y+2][x] < 413))) {
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((fase[*habitacion][v][h] > 408) && (fase[*habitacion][v][h] < 413))
						fase[*habitacion][v][h] = 0;
				}
			}
			jean->estado[1] += 1;
			Mix_PlayChannel(-1, fx[2], 0);;
		}

		/* Tocar pergamino amarillo */
		if (((fase[*habitacion][y+1][x] > 316) &&  (fase[*habitacion][y+1][x] < 321)) || ((fase[*habitacion][y+1][x+1] > 316) &&  (fase[*habitacion][y+1][x+1] < 321))) {
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((fase[*habitacion][v][h] > 316) && (fase[*habitacion][v][h] < 321))
						fase[*habitacion][v][h] = 0;
				}
			}
			*pergamino = *habitacion;
		}

		/* Tocar pergamino rojo */
		if (((fase[*habitacion][y+1][x] > 338) &&  (fase[*habitacion][y+1][x] < 343)) || ((fase[*habitacion][y+1][x+1] > 338) &&  (fase[*habitacion][y+1][x+1] < 343))) {
			jean->flags[6] = 3;
			/* Quitamos el pergamino */
			fase[24][14][28] = 0;
			fase[24][14][29] = 0;
			fase[24][15][28] = 0;
			fase[24][15][29] = 0;
		}

		/* Tocar puerta */
		if ((*habitacion == 10) || (*habitacion == 19)) {
			if (fase[*habitacion][y][x] == 154) {
				switch (*habitacion) {
					case 10: *habitacion = 19;
									 jean->x = 160;
									 jean->y = 120;
									 break;
				  case 19: *habitacion = 10;
									 jean->x = 176;
									 jean->y = 136;
								   break;
				}
				Mix_PlayChannel(-1, fx[1], 0);;
				*cambio = 1;
			}
		}

		/* Tocar switch */
		if (*habitacion == 17) {
			if ((((fase[*habitacion][y+1][x] > 330) && (fase[*habitacion][y+1][x] < 339)) || ((fase[*habitacion][y+1][x+1] > 330) && (fase[*habitacion][y+1][x+1] < 339))) && (jean->flags[5] == 0)) {
				for (v=2;v<4;v++) {
					for (h=15;h<17;h++) {
						if ((fase[*habitacion][v][h] > 330) && (fase[*habitacion][v][h] < 335)) {
							fase[*habitacion][v][h] += 4;
							jean->flags[5] = 1;
						}
						if (((fase[*habitacion][v][h] > 334) && (fase[*habitacion][v][h] < 339)) && (jean->flags[5] == 0))
							fase[*habitacion][v][h] -= 4;
					}
				}
				jean->flags[5] = 1;
				/* Activando y desactivando cruces de todo el mapa */
				for (r=1; r<25; r++) {
					for (v=0; v<22; v++) {
						for (h=0; h<32; h++) {
							flag = 0;
							/* Cruces activadas */
							if ((fase[r][v][h] > 408) && (fase[r][v][h] < 413)) {
								fase[r][v][h] += 16;
								flag = 1;
							}
							/* Cruces desactivadas */
							if ((fase[r][v][h] > 424) && (fase[r][v][h] < 429) && (flag == 0))
								fase[r][v][h] -= 16;
						}
					}
				}
			Mix_PauseMusic ();
			Mix_PlayChannel(-1, fx[5], 0);;
			sleep(2);
			Mix_ResumeMusic ();
			}
		}

		/* Tocar copa */
		if (*habitacion == 24) {
			if ((fase[*habitacion][y][x+1] == 650) || (fase[*habitacion][y+1][x+1] == 650) || (fase[*habitacion][y+2][x+1] == 650)) {
				Mix_HaltMusic ();
				Mix_PlayChannel(-1, fx[5], 0);
				sleep(2);
				fase[24][3][15] = 0; /* Quitamos la copa */
				/* Quitamos las cruces */
				for (v=0; v<22; v++) {
					for (h=0; h<32; h++) {
						if (fase[*habitacion][v][h] == 84)
							fase[*habitacion][v][h] = 0;
					}
				}
				/* Quitamos a Satan */
				enemigos->tipo[0] = 88;
				enemigos->velocidad[0] = 0; /* Usaremos velocidad como contador */
				enemigos->ajustex1[0] = 0;
				enemigos->ajustex2[0] = 0;
				enemigos->ajustey1[0] = 0;
				enemigos->ajustey2[0] = 0;
				/* Y tambien sus proyectiles */
				for (v=0;v<24;v++)
					proyec[v] = 0;
				/* Preparamos los cruzados */
				for (v=1;v<7;v++)
					enemigos->tipo[v] = 17;
				enemigos->ajustex2[0] = 15;
				enemigos->ajustey2[0] = 23;
			}
		}

	}

}

void eventos (struct protagonista *jean, int fase[][22][32], int habitacion, int ciclo[], struct enem *enemigos, Mix_Chunk *fx[]) {

	int i = 0;
	int x = 0;
	int y = 0;

	if (habitacion == 4) {
		if (jean->temp < 7) {
			/* Mover a Jean */
			if (ciclo[1] == 45) {
				switch (jean->direccion) {
					case 0: jean->direccion = 1;
									break;
					case 1: jean->direccion = 0;
									break;
				}
				jean->temp ++;
			}
		}
		else {
			jean->direccion = 0;
			jean->muerte = 1;
		}
	}

	if (habitacion == 7) {
		/* Cierre de la puerta */
		if ((jean->x > 24) && (jean->flags[0] == 0)) {
			fase[7][14][0] = 347;
			fase[7][15][0] = 348;
			fase[7][16][0] = 349;
			fase[7][17][0] = 350;
			jean->flags[0] = 1;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
	}

	if (habitacion == 8) {
		/* Apertura suelo */
		if ((jean->x > 15) && (jean->flags[1] == 1) && (fase[8][20][26] == 7)) {
			fase[8][20][26] = 38;
			fase[8][20][27] = 0;
			fase[8][21][26] = 0;
			fase[8][21][27] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
		/* Apertura puerta */
		if ((jean->x > 211) && (jean->flags[2] == 1) && (fase[8][14][31] == 343)) {
			fase[8][14][31] = 0;
			fase[8][15][31] = 0;
			fase[8][16][31] = 0;
			fase[8][17][31] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
	}
	if (habitacion == 10) {
		/* El fuego del dragon mata a Jean */
		if (((jean->x > 127) && (jean->x < 144)) && (jean->y < 89)) {
			if ((enemigos->velocidad[0] > 109) && (enemigos->velocidad[0] < 146))
				jean->muerte = 1;
		}
	}
	if (habitacion == 19) {
		/* Apertura puerta */
		if ((jean->y > 16) && (jean->flags[3] == 1) && (fase[19][16][0] == 347)) {
			fase[19][16][0] = 0;
			fase[19][17][0] = 0;
			fase[19][18][0] = 0;
			fase[19][19][0] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
	}
	if (habitacion == 20) {
		/* Apertura puerta */
		if ((jean->x > 208) && (jean->flags[4] == 1) && (fase[20][14][31] == 343)) {
			fase[20][14][31] = 0;
			fase[20][15][31] = 0;
			fase[20][16][31] = 0;
			fase[20][17][31] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			sleep(1);
		}
	}

	if (habitacion == 24) {
		if ((jean->estado[1] == 12) && (jean->x > 8) && (jean->flags[6] == 0)) {
			/* Cerrando la entrada */
			fase[24][16][0] = 99;
			fase[24][17][0] = 99;
			fase[24][18][0] = 99;
			fase[24][19][0] = 99;
			jean->flags[6] = 1;
			/* Marcando checkpoint */
			jean->checkpoint[0] = 24;
			jean->checkpoint[1] = jean->x;
			jean->checkpoint[2] = jean->y;
		}
		if ((jean->flags[6] == 1) && (jean->estado[1] > 0) && (ciclo[0] == 0)) {
			/* Colocando cruces y restando */
			switch (jean->estado[1]) {
				case 1: x=11;
							  y=5;
							  break;
				case 2: x=10;
							  y=5;
							  break;
				case 3: x=8;
							  y=8;
							  break;
				case 4: x=5;
							  y=8;
							  break;
				case 5: x=4;
							  y=8;
							  break;
				case 6: x=2;
							  y=12;
							  break;
				case 7: x=5;
							  y=14;
							  break;
				case 8: x=6;
							  y=14;
							  break;
				case 9: x=9;
							  y=14;
							  break;
				case 10: x=10;
								 y=14;
								 break;
				case 11: x=13;
								 y=13;
								 break;
				case 12: x=15;
								 y=16;
								 break;
			}
			fase[24][y][x] = 84;
			jean->estado[1] --;
			Mix_PlayChannel(-1, fx[1], 0);
		}
		if ((jean->flags[6] == 1) && (jean->estado[1] == 0) && (ciclo[0] == 29)) {
			/* Dibujar copa */
			fase[24][3][15] = 650;
			jean->flags[6] = 2;
			Mix_PlayChannel(-1, fx[1], 0);
		}
		/* Humo que aparece tras desaparecer Satan */
		if (enemigos->tipo[0] == 88) {
			if (enemigos->velocidad[0] < 90)
				enemigos->velocidad[0] ++;
			else {
				enemigos->velocidad[0] = 0;
				enemigos->tipo[i] = 0;
				enemigos->x[0] = 0;
				enemigos->y[0] = 0;
				enemigos->tipo[0] = 17;
				/* Colocamos pergamino rojo */
				fase[24][14][28] = 339;
				fase[24][14][29] = 340;
				fase[24][15][28] = 341;
				fase[24][15][29] = 342;
			}
		}
	}
}

void contacto (struct protagonista *jean, struct enem enemigos, float proyec[], int habitacion) {

	int i = 0;
	int mitad = 0; /* Mitad del tamaño del sprite enemigo */
	int puntos[4] = {0,0,0,0}; /* Cuatro limites del sprite del enemigo */
	int x = 0;
	int y = 0;
	int n = 0;

	/* Comprobar contacto con enemigos */
	for (i=0;i<7;i++) {
		if (((enemigos.tipo[i] > 0) && (enemigos.tipo[i] != 12)) || ((enemigos.tipo[i] == 12) && (enemigos.y[i] > enemigos.limizq[i] + 8))) {
			/* Ajustando los bordes del sprite enemigo */
			puntos[0] = enemigos.x[i] + enemigos.ajustex1[i];
			puntos[1] = enemigos.x[i] + enemigos.ajustex2[i];
			puntos[2] = enemigos.y[i] + enemigos.ajustey1[i];
			puntos[3] = enemigos.y[i] + enemigos.ajustey2[i];
			/* Comprobando... */
			for (x=puntos[0];x<=puntos[1];x++) {
				if ((x>jean->x+1) && (x<jean->x+13)) {
					for (y=puntos[2];y<=puntos[3];y++) {
						if ((y>jean->y+(jean->agachado*8)) && (y<jean->y+22)) {
							if (jean->flags[6] < 5) {
								jean->muerte = 1;
								y=puntos[3] + 1;
								x=puntos[1] + 1;
							}
							else {
								/* Mix_HaltMusic (); */
								jean->flags[6] = 6;
							}
						}
					}
				}
			}

		}
	}

	/* Comprobar contacto con proyectiles */
	for (i=0;i<3;i++) {
		if (proyec[i*2] > 0) {
			/* Puntos de ajuste para los disparos */
			if (enemigos.tipo[i] == 11) { /* Gargola */
				puntos[0] = proyec[i*2];
				puntos[1] = proyec[i*2]+10;
				puntos[2] = enemigos.y[i] + 10;
				puntos[3] = enemigos.y[i] + 12;
			}

			if (enemigos.tipo[i] == 15) { /* Arqueros */
				puntos[0] = proyec[i*2] + 3;
				puntos[1] = proyec[i*2] + 7;
				puntos[2] = enemigos.y[i] + 10;
				puntos[3] = enemigos.y[i] + 17;
			}
			for (x=puntos[0];x<=puntos[1];x++) {
				if ((x>jean->x+3) && (x<jean->x+13)) {
					for (y=puntos[2];y<=puntos[3];y++) {
						if ((y>(jean->y+5+(jean->agachado*8))) && (y<jean->y+22)) {
							jean->muerte = 1;
							y=puntos[3] + 1;
							x=puntos[1] + 1;
						}
					}
				}
			}

		}

	}

	/* Comprobar contacto con disparos de flor, dragon, muerte y Satan*/
	if ((habitacion == 10) || (habitacion == 14) || (habitacion == 18) || (habitacion == 24)) {
		for (i=0;i<23;i+=2) {
			if (proyec[i] > 0) {
				if (habitacion == 18) {
					puntos[0] = proyec[i+1];
					puntos[1] = proyec[i+1]+15;
					puntos[2] = proyec[i];
					puntos[3] = proyec[i] + 15;
				}
				if ((habitacion == 14) || (habitacion == 24)) {
					puntos[0] = proyec[i];
					puntos[1] = proyec[i] + 3;
					puntos[2] = proyec[i+1];
					puntos[3] = proyec[i+1] + 3;
				}
				if (habitacion == 10) {
					puntos[0] = proyec[i];
					puntos[1] = proyec[i] + 8;
					puntos[2] = 88;
					puntos[3] = 96;
				}

				for (x=puntos[0];x<=puntos[1];x++) {
					if ((x>jean->x+1) && (x<jean->x+13)) {
						for (y=puntos[2];y<=puntos[3];y++) {
							if ((y>jean->y+(jean->agachado*8)) && (y<jean->y+22)) {
								jean->muerte = 1;
								y=puntos[3] + 1;
								x=puntos[1] + 1;
								i= 17;
							}
						}
					}
				}
			}
		}
	}

}

void tecladoj (struct protagonista *jean, int *tecla) {
/* Gestión del teclado durante el juego */

	SDL_Event evento;

	if (*tecla > 0)
		*tecla = 0;

   while (SDL_PollEvent(&evento)) {
		if (evento.type == SDL_QUIT)
	   	exit(0);
		if (evento.type == SDL_KEYDOWN) {
#ifdef _OPENPANDORA
			if (evento.key.keysym.sym == SDLK_PAGEDOWN) {
#else
			if (evento.key.keysym.sym == SDLK_UP) {
#endif
				if ((jean->pulsa[0] == 0) && (jean->salto == 0) && (jean->agachado == 0))
					jean->salto = 1;
			}
			if (evento.key.keysym.sym == SDLK_DOWN) {
				if ((jean->pulsa[1] == 0) && (jean->salto == 0)) {
					jean->pulsa[1] = 1;
					jean->agachado = 1;
				}
			}
			if (evento.key.keysym.sym == SDLK_LEFT) {
				if (jean->pulsa[2] == 0) {
					jean->pulsa[2] = 1;
					jean->pulsa[3] = 0;
				}
			}
			if (evento.key.keysym.sym == SDLK_RIGHT) {
				if (jean->pulsa[3] == 0) {
					jean->pulsa[3] = 1;
					jean->pulsa[2] = 0;
				}
			}
			if (evento.key.keysym.sym == SDLK_f)
				*tecla = 6;
			if (evento.key.keysym.sym == SDLK_c)
				*tecla = 9;
	   	if (evento.key.keysym.sym == SDLK_ESCAPE)
      		exit(0);
		}

		if (evento.type == SDL_KEYUP) {
#ifdef _OPENPANDORA
			if (evento.key.keysym.sym == SDLK_PAGEDOWN)
#else
			if (evento.key.keysym.sym == SDLK_UP)
#endif
					jean->pulsa[0] = 0;
			if (evento.key.keysym.sym == SDLK_DOWN) {
					jean->pulsa[1] = 0;
					jean->agachado = 0;
			}
			if (evento.key.keysym.sym == SDLK_LEFT)
					jean->pulsa[2] = 0;
			if (evento.key.keysym.sym == SDLK_RIGHT)
					jean->pulsa[3] = 0;
		}

	}

}

void teclado (int *tecla, int fase) {
/* Gestión de teclado durante las intros */

	SDL_Event evento;

   while (SDL_PollEvent(&evento)) {
		if (evento.type == SDL_QUIT)
	   	exit(0);
   	if (evento.type == SDL_KEYDOWN) {
	   	if (evento.key.keysym.sym == SDLK_i) {
      		if (fase == 1)
						*tecla = 7;
			}
	   	if (evento.key.keysym.sym == SDLK_ESCAPE)
      		exit(0);
			if (evento.key.keysym.sym == SDLK_f)
				*tecla = 6;
			if (evento.key.keysym.sym == SDLK_c)
				*tecla = 9;
#ifdef _OPENPANDORA
			if (evento.key.keysym.sym == SDLK_PAGEDOWN)
#else
			if (evento.key.keysym.sym == SDLK_SPACE)
#endif
				*tecla = 5;
		}
	}

}

void tecladop (int *teclap) {

	SDL_Event evento;

	while (SDL_PollEvent(&evento)) {
		if (evento.type == SDL_KEYDOWN) {
#ifdef _OPENPANDORA
			if ((evento.key.keysym.sym == SDLK_PAGEDOWN) ||
#else
			if ((evento.key.keysym.sym == SDLK_SPACE) ||
#endif
					(evento.key.keysym.sym == SDLK_LEFT) || (evento.key.keysym.sym == SDLK_RIGHT))
				*teclap = 1;
		}
	}

}
