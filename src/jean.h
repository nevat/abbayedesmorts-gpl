/* jean.h */

#pragma once

#include "base.h"
#include "structs.h"

// Directions as indexes for 'size 4' arrays
enum {
	J_UP = 0,
	J_DOWN,
	J_LEFT,
	J_RIGHT
};

void drawjean (SDL_Renderer *renderer,SDL_Texture *tiles,struct hero *jean,uint counter[],Mix_Chunk *fx[],uint changetiles);
void movejean (struct hero *jean, Mix_Chunk *fx[]);
void touchobj (struct hero *jean,uint stagedata[][22][32],uint *room,uint *lastroom,uint *parchment,uint *changeflag,struct enem *enemies,float proyec[],Mix_Chunk *fx[]);
void contact (struct hero *jean,struct enem enemies,float proyec[],uint room);
void collisions (struct hero *jean,uint stagedata[][22][32],int room);
