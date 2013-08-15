/* enemigos.h */
/* Header para enemigos.c */

#include "structs.h"

void buscarenemigos (int habitacion, struct enem *enemigos, int *cambio, int datosenem[][7][15]);
void dibujarenemigos (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, Mix_Chunk *fx[]);
void moverenemigos (struct enem *enemigos, int fase[][22][32], int ciclo[], float proyec[], struct protagonista jean, Mix_Chunk *fx[]);
void plantas (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], float proyec[], Mix_Chunk *fx[]);
void cruzados (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], int habitacion);
