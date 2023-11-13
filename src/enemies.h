/* enemies.h */

#pragma once

#include "base.h"
#include "structs.h"

void satan (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles);
void death (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],uint stagedata[][22][32],Mix_Chunk *fx[],uint changetiles);
void fireball (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],struct hero jean,uint stagedata[][22][32], uint changetiles);
void plants (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles);
void dragon (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles);
void crusaders (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],uint room,uint changetiles);
void movenemies (struct enem *enemies,uint stagedata[][22][32],uint counter[],float proyec[],struct hero jean,Mix_Chunk *fx[]);
void drawenemies (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,Mix_Chunk *fx[],uint changetiles);
void searchenemies (uint room, struct enem *enemies,uint *changeflag, int enemydata[][7][15]);
