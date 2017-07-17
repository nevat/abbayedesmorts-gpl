/* main.h */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"

void startscreen(SDL_Window *screen,uint *state,uint *grapset,uint *fullscreen);
void history(SDL_Window *screen,uint *state,uint *grapset,uint *fullscreen);
void game(SDL_Window *screen,uint *state,uint *grapset,uint *fullscreen);
void gameover(SDL_Window *screen,uint *state);
void ending (SDL_Window *screen,uint *state);
