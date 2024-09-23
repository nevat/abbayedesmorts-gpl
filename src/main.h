/* main.h */

#pragma once

#include "base.h"

int check_data();
void update_title(SDL_Window *screen, const char* title);
void renderpresent(SDL_Renderer *renderer);

#define GAME_TITLE "Abbaye des Morts v2.0.4"
