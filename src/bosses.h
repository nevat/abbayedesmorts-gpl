/* bosses.h */
/* Header para bosses.c */

#include "structs.h"

void muerte (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], float proyec[], int fase[][22][32], Mix_Chunk *fx[]);
void dragon (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], float proyec[], Mix_Chunk *fx[]);
void satan (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], float proyec[], Mix_Chunk *fx[]);
void bolafuego (struct enem *enemigos, SDL_Surface *ventana, SDL_Surface *tiles, int ciclo[], struct protagonista jean, int fase[][22][32]);
