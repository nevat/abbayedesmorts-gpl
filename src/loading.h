/* loading.h */

#pragma once

#include "base.h"

void loadingmusic(Mix_Music *bso[],Mix_Chunk *fx[]);
void loaddata(uint stagedata[][22][32],int enemydata[][7][15]);
SDL_Texture *loadtexture(SDL_Renderer *renderer, const char *file);
Mix_Music *loadmus(const char *file);
Mix_Chunk *loadwav(const char *file);
