/* drawing.h */

#pragma once

#include "base.h"
#include "structs.h"

void drawrope (struct enem enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint changetiles);
void drawshoots (float proyec[],SDL_Texture *tiles,SDL_Renderer *renderer,struct enem *enemies,uint changetiles);
void drawscreen (SDL_Renderer *renderer,uint stagedata[][22][32],SDL_Texture *tiles,uint room,uint counter[],uint changeflag,Mix_Chunk *fx[],uint changetiles);
void statusbar (SDL_Renderer *renderer,SDL_Texture *tiles,int room,int lifes,int crosses,SDL_Texture *fonts,uint changetiles);
void showparchment (SDL_Renderer *renderer,uint *parchment);
void blueparchment (SDL_Renderer *renderer,struct hero *jean);
void redparchment (SDL_Renderer *renderer,struct hero *jean);
