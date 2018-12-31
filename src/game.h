/* game.h */

#pragma once

#include <unistd.h>
#include "base.h"

#include "structs.h"

void keybpause (uint *keyp);
void music (uint room[],Mix_Music *bso[],uint *changeflag,int flag);
void changescreen (struct hero *jean,uint room[],uint *changeflag);
void events (struct hero *jean,uint stagedata[][22][32],uint room[],uint counter[],struct enem *enemies,Mix_Chunk *fx[]);
void control (struct hero *jean,uint *keyp);
void counters (uint counter[]);
void animation (uint stagedata[][22][32],int room[],int counter[]);

/* enemies */
void blueparchment (SDL_Renderer *renderer,struct hero *jean);
void redparchment (SDL_Renderer *renderer,struct hero *jean);
void satan (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles);
void death (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],uint stagedata[][22][32],Mix_Chunk *fx[],uint changetiles);
void fireball (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],struct hero jean,uint stagedata[][22][32], uint changetiles);
void plants (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles);
void dragon (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],float proyec[],Mix_Chunk *fx[],uint changetiles);
void crusaders (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint counter[],uint room[],uint changetiles);
void movenemies (struct enem *enemies,uint stagedata[][22][32],uint counter[],float proyec[],struct hero jean,Mix_Chunk *fx[]);
void drawenemies (struct enem *enemies,SDL_Renderer *renderer,SDL_Texture *tiles,Mix_Chunk *fx[],uint changetiles);
void searchenemies (uint room[], struct enem *enemies,uint *changeflag, int enemydata[][7][15]);

/* drawing */
void drawrope (struct enem enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint changetiles);
void drawshoots (float proyec[],SDL_Texture *tiles,SDL_Renderer *renderer,struct enem *enemies,uint changetiles);
void drawscreen (SDL_Renderer *renderer,uint stagedata[][22][32],SDL_Texture *tiles,uint room[],uint counter[],uint changeflag,Mix_Chunk *fx[],uint changetiles);
void statusbar (SDL_Renderer *renderer,SDL_Texture *tiles,int room[],int lifes,int crosses,SDL_Texture *fonts,uint changetiles);
void showparchment (SDL_Renderer *renderer,uint *parchment);

/* loading */
void loadingmusic(Mix_Music *bso[],Mix_Chunk *fx[]);
void loaddata(uint stagedata[][22][32],int enemydata[][7][15]);

/* jean */
void drawjean (SDL_Renderer *renderer,SDL_Texture *tiles,struct hero *jean,int counter[],Mix_Chunk *fx[],uint changetiles);
void movejean (struct hero *jean, Mix_Chunk *fx[]);
void touchobj (struct hero *jean,uint stagedata[][22][32],uint room[],uint *parchment,uint *changeflag,struct enem *enemies,float proyec[],Mix_Chunk *fx[]);
void contact (struct hero *jean,struct enem enemies,float proyec[],uint room[]);
void collisions (struct hero *jean,uint stagedata[][22][32],int room[]);
