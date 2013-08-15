/* jean-h */
/* Header para jean.c */

#include "structs.h"

void moverjean (struct protagonista *jean, Mix_Chunk *fx[]);
void dibujarjean (SDL_Surface *ventana, SDL_Surface *tiles, struct protagonista *jean, int ciclo[], Mix_Chunk *fx[]);
void colisiones (struct protagonista *jean, int fase[][22][32], int habitacion);
