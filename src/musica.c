/* musica.c */
/* Musica y sonidos */

#include <stdio.h>
#include <stdlib.h>
#include <SDL_mixer.h>
#include "comun.h"
#include "musica.h"

void musica (int habitacion, Mix_Music *sonido[], int *cambio, int prehabitacion, int flag) {

	if (habitacion == 1) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[0], 0);
	}
	if ((habitacion == 2) && (prehabitacion == 1)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[1], -1);
	}
	if (habitacion == 4) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[2], 0);
	}
	if ((habitacion == 5) && (prehabitacion != 6)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[7], -1);
	}
	if ((habitacion == 6) && (prehabitacion == 7)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[7], -1);
	}
	if ((habitacion == 7) && (prehabitacion == 6)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[1], -1);
	}
	if (((habitacion == 8) && (prehabitacion == 9)) || ((habitacion == 8) && (*cambio == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[1], -1);
	}
	if (habitacion == 9) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[3], 0);
	}
	if (((habitacion == 11) && (prehabitacion == 12)) || ((habitacion == 11) && (*cambio == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[4], -1);
	}
	if (((habitacion == 12) && (prehabitacion == 11)) || ((habitacion == 12) && (*cambio == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[1], -1);
	}
	if ((habitacion == 13) && (prehabitacion == 14)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[1], -1);
	}
	if (((habitacion == 14) && (prehabitacion == 13)) || ((habitacion == 14) && (*cambio == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[4], -1);
	}
	if ((habitacion == 15) && (*cambio == 2)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[4], -1);
	}
	if ((habitacion == 16) && (prehabitacion == 17)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[4], -1);
	}
	if ((habitacion == 17) && (prehabitacion == 16)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[1], -1);
	}
	if (habitacion == 18) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[5], -1);
	}
	if (((habitacion == 19) && (prehabitacion == 18)) || ((habitacion == 19) && (*cambio == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[4], -1);
	}
	if ((habitacion == 20) && (prehabitacion == 21)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[4], -1);
	}
	if ((habitacion == 21) && (prehabitacion == 20)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[6], -1);
	}
	if ((habitacion == 23) && (prehabitacion == 24)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[6], -1);
	}
	if ((habitacion == 24) && (flag != 5)) {
		Mix_HaltMusic();
		Mix_PlayMusic(sonido[5], -1);
	}
	if ((habitacion == 24) && (flag == 5))
		Mix_PlayMusic(sonido[7], -1);

 	*cambio -= 1;

}

void cargar_musica (Mix_Music *sonido[], Mix_Chunk *fx[]) {

	/* Musicas */
	sonido[0] = Mix_LoadMUS(RUTA_MUS_PRAYER);
	sonido[1] = Mix_LoadMUS(RUTA_MUS_CHURCH);
	sonido[2] = Mix_LoadMUS(RUTA_MUS_GAMEOVER);
	sonido[3] = Mix_LoadMUS(RUTA_MUS_HANGMAN);
	sonido[4] = Mix_LoadMUS(RUTA_MUS_CAVES);
	sonido[5] = Mix_LoadMUS(RUTA_MUS_EVIL_FIGHT);
	sonido[6] = Mix_LoadMUS(RUTA_MUS_HELL);
	sonido[7] = Mix_LoadMUS(RUTA_MUS_WOOD);

	/* Fxs */
	fx[0] = Mix_LoadWAV (RUTA_SFX_SHOOT);
	fx[1] = Mix_LoadWAV (RUTA_SFX_DOOR);
	fx[2] = Mix_LoadWAV (RUTA_SFX_ITEM);
	fx[3] = Mix_LoadWAV (RUTA_SFX_JUMP);
	fx[4] = Mix_LoadWAV (RUTA_SFX_SLASH);
	fx[5] = Mix_LoadWAV (RUTA_SFX_MECHANISM);
	fx[6] = Mix_LoadWAV (RUTA_SFX_DEATH);

}
