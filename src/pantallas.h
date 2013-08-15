/* pantallas.h */
/* Header for pantallas.c */

#include "structs.h"

void intro1 (SDL_Surface *pantalla, int *estadoj, int *cambiot);
void intro2 (SDL_Surface *pantalla, int *cambiot);
void mostrarpergamino (SDL_Surface *ventana, int *pergamino);
void gameover (SDL_Surface *pantalla);
void pergaminorojo (SDL_Surface *ventana, struct protagonista *jean);
void pergaminoazul (SDL_Surface *ventana, struct protagonista *jean);
void ending (SDL_Surface *pantalla);