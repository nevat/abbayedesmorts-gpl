/* fase.h */
/* Header para fase.c */

void dibujarfase (SDL_Surface *ventana, int fase[][22][32], SDL_Surface *tiles, int habitacion, int ciclo[], int cambiot, SDL_Surface *fondomd, Mix_Chunk *fx[]);
void animitems (int fase[][22][32], int habitacion, int ciclo[]);
