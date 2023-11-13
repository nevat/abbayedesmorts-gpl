/* game.h */

#pragma once

#include "base.h"
#include "structs.h"

void game(SDL_Window *screen,uint8_t *state,uint8_t *grapset,uint8_t *fullscreen);

void keybpause (uint *keyp);
void music (uint room,uint lastroom,Mix_Music *bso[],uint *changeflag,int flag);
void changescreen (struct hero *jean,uint *room,uint *lastroom,uint *changeflag);
void events (struct hero *jean,uint stagedata[][22][32],uint room,uint counter[],struct enem *enemies,Mix_Chunk *fx[]);
void control (struct hero *jean,uint *keyp);
void counters (uint counter[]);
void animation (uint stagedata[][22][32],uint room,uint counter[]);
