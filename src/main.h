/* main.h */
/* Header para main.c */

#include "structs.h"

/* Prototipos de funciones */
void iniciar_sdl ();
int framerate (int i, int frate1);
void juego (SDL_Surface *pantalla, int *estadoj, int *cambiot);
void teclado (int *tecla, int fase);
void cargardatos (int fase[][22][32], int datosenem[][7][15]);
void dibujarcuerda (struct enem enemigos, SDL_Surface *ventana, SDL_Surface *tiles);
void dibujardisparo (float proyec[], SDL_Surface *tiles, SDL_Surface *ventana, struct enem *enemigos);
void cambiopantalla (struct protagonista *jean, int *habitacion, int *cambio, int *prehabitacion);
void tocarobjetos (struct protagonista *jean, int fase[][22][32], int *habitacion, int *pergamino, int *cambio, struct enem *enemigos, float proyec[], Mix_Chunk *fx[]);
void eventos (struct protagonista *jean, int fase[][22][32], int habitacion, int ciclo[], struct enem *enemigos, Mix_Chunk *fx[]);
void contacto (struct protagonista *jean, struct enem enemigos, float proyec[], int habitacion);
void tecladoj (struct protagonista *jean, int *tecla);
void tecladop (int *teclap);
#ifdef _OPENPANDORA
void filtroPandora(int filtro);
void cambiarFiltroPandora(void);
#endif
