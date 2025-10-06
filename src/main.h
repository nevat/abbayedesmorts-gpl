/* main.h */

#pragma once

#include "base.h"

typedef enum {
	OPT_NONE = 0,
	OPT_STARTGAME,
	OPT_SHOWINSTRUCT,
	OPT_FULLSCREEN,
	OPT_GRAPHICS,
	OPT_QUIT
} gameoption_t;

int check_data();
void update_title(SDL_Window *screen, const char* title);
void renderpresent(SDL_Renderer *renderer);

#define GAME_TITLE "Abbaye des Morts v2.0.5"
