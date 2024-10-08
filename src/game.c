/* game.c */

#include "game.h"
#include "loading.h"
#include "jean.h"
#include "enemies.h"
#include "drawing.h"
#define INSIDE_GAME_C
#include "rooms.h"
#include "main.h"

void game(SDL_Window *screen,uint8_t *state,uint8_t *grapset,uint8_t *fullscreen) {

	/* Sounds */
	Mix_Music *bso[8];
	Mix_Chunk *fx[7];

	uint stagedata[25][22][32];
	int enemydata[25][7][15];
	uint room = ROOM_ESCAPE; /* Room and previous room */
	uint lastroom = ROOM_ESCAPE;
	uint exit = 0;
	uint changeflag = 1; /* Screen change */
	uint counter[3] = {0,0,0}; /* Counters */
	uint changetiles=*grapset;
	uint i = 0;
	float proyec[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* Enemiess shoots */
	uint keyp = 0;
	uint parchment = 0;
	uint n = 0;
	uint winfull = *fullscreen;

	/* Loading PNG */
	SDL_Surface *tilesb = IMG_Load(DATADIR "/graphics/tiles.png");
	SDL_SetColorKey(tilesb, SDL_TRUE, SDL_MapRGB(tilesb->format, 0, 0, 0) );
	SDL_Texture *tiles = SDL_CreateTextureFromSurface(renderer, tilesb);
	SDL_FreeSurface(tilesb);
	SDL_Texture *fonts = loadtexture(renderer, "fonts");

	/* Loading musics */
	loadingmusic(bso,fx);

	/* Load data */
	loaddata(stagedata,enemydata);

	/* Init struct */
	struct enem enemies = {
		.type = {0,0,0,0,0,0,0},
		.x = {0,0,0,0,0,0,0},
		.y = {0,0,0,0,0,0,0},
		.direction = {0,0,0,0,0,0,0},
		.tilex = {0,0,0,0,0,0,0},
		.tiley = {0,0,0,0,0,0,0},
		.animation = {0,0,0,0,0,0,0},
		.limleft = {0,0,0,0,0,0,0},
		.limright = {0,0,0,0,0,0,0},
		.speed = {0,0,0,0,0,0,0},
		.fire = {0,0,0,0,0,0,0},
		.adjustx1 = {0,0,0,0,0,0,0},
		.adjustx2 = {0,0,0,0,0,0,0},
		.adjusty1 = {0,0,0,0,0,0,0},
		.adjusty2 = {0,0,0,0,0,0,0}
	};

	struct hero jean = {
		.x = 72,
		.y = 136,
		.direction = 1,
		.jump = 0,
		.height = 0,
		.animation = 0,
		.gravity = 1.9,
		.points = {0,0,0,0,0,0,0,0},
		.ground = 0,
		.collision = {0,0,0,0},
		.ducking = 0,
		.checkpoint = {5,72,136,5},
		.state = {9,0},
		.flags = {0,0,0,0,0,0,0},
		.death = 0,
		.push = {0,0,0,0},
		.temp = 0
	};

	/* Game loop */
	while (exit != 1) {

		/* Manage counters */
		counters(counter);

		/* Cleaning the renderer */
		SDL_RenderClear(renderer);

		/* Change graphic set requested */
		if (keyp == 9) {
			keyp = 0;
			if (changetiles == 0)
				changetiles = 1;
			else
				changetiles = 0;
			*grapset = changetiles;
		}

		/* Switch fullscreen/windowed requested */
		if (keyp == 6) {
			if (*fullscreen == 0) {
				SDL_SetWindowFullscreen(screen,SDL_WINDOW_FULLSCREEN_DESKTOP);
				*fullscreen = 1;
			}
			else {
				SDL_SetWindowFullscreen(screen,0);
				*fullscreen = 0;
			}
			keyp = 0;
		}

		/* Exit requested */
		if (keyp == 10) {
			exit = 1;
			*state = 6;
		}

		/* Animation of fire and water */
		animation(stagedata,room,counter);

		/* Draw screen */
		drawscreen(renderer,stagedata,tiles,room,counter,changeflag,fx,changetiles);

		/* Draw statusbar */
		if (room != ROOM_THEEND)
			statusbar(renderer,tiles,room,jean.state[0],jean.state[1],fonts,changetiles);

		/* Draw Jean */
		if (jean.flags[6] < 8)
			drawjean (renderer,tiles,&jean,counter,fx,changetiles);

		/* Enemies */
		if (enemies.type[0] > 0) {
			if (room != ROOM_THEEND)
				movenemies (&enemies,stagedata,counter,proyec,jean,fx);
			if ((room == ROOM_ESCAPE) || (room == ROOM_CLOSE))
				crusaders (&enemies,renderer,tiles,counter,room,changetiles);
			if (room == ROOM_BEAST)
				dragon (&enemies,renderer,tiles,counter,proyec,fx,changetiles);
			if (room == ROOM_CAVE)
				fireball (&enemies,renderer,tiles,counter,jean,stagedata,changetiles);
			if (room == ROOM_GARDEN)
				plants (&enemies,renderer,tiles,counter,proyec,fx,changetiles);
			if (room == ROOM_HANGMAN)
				drawrope (enemies,renderer,tiles,changetiles);
			if (room == ROOM_BANQUET)
				death (&enemies,renderer,tiles,counter,proyec,stagedata,fx,changetiles);
			if ((room == ROOM_SATAN) && (enemies.type[0] == 18))
				satan (&enemies,renderer,tiles,counter,proyec,fx,changetiles);
			if ((room == ROOM_SATAN) && (jean.flags[6] == 5))
				crusaders (&enemies,renderer,tiles,counter,room,changetiles);
			drawenemies (&enemies,renderer,tiles,fx,changetiles);
		}

		/* Shoots */
		if ((proyec[0] > 0) && ((room == ROOM_WHEEL) || (room == ROOM_GATE) || (room == ROOM_EVIL) || (room == ROOM_SOULS)))
			drawshoots (proyec,tiles,renderer,&enemies,changetiles);

		/* Jean management */
		if (jean.death == 0) {
			if (jean.flags[6] < 5) {
					if (jean.temp == 0)
						control(&jean,&keyp);
					if (jean.temp == 1)
						jean.temp = 0;
					collisions (&jean,stagedata,room);
					movejean (&jean,fx);
			}
			if (room != ROOM_THEEND) {
				touchobj (&jean,stagedata,&room,&lastroom,&parchment,&changeflag,&enemies,proyec,fx);
				contact (&jean,enemies,proyec,room);
			}
			events (&jean,stagedata,room,counter,&enemies,fx);
		}

		/* Jean death */
		if (jean.death == 98) {
			if (room != ROOM_THEEND) {
				room = jean.checkpoint[0];
				lastroom = jean.checkpoint[0];
				jean.x = jean.checkpoint[1];
				jean.y = jean.checkpoint[2];
				jean.jump = 0;
				jean.height = 0;
				jean.push[0] = 0;
				jean.push[1] = 0;
				jean.push[2] = 0;
				jean.push[3] = 0;
				changeflag = 2;
				jean.state[0] --;
				jean.death = 0;
				jean.temp = 1;
				music (room,lastroom,bso,&changeflag,jean.flags[6]);
				Mix_ResumeMusic();
			}
			else {
				jean.death = 0;
				jean.flags[6] = 8;
			}
		}
		/* Using flag 6 as counter, to make time */
		if (jean.flags[6] > 7)
			jean.flags[6] ++;
		/* Reaching 15, jump to ending sequence */
		if (jean.flags[6] == 15) {
			*state = 4;
			exit = 1;
		}
		changescreen (&jean,&room,&lastroom,&changeflag);

		if (changeflag > 0) {
			if ((jean.flags[6] < 4) || (jean.flags[6] > 5))
				searchenemies (room,&enemies,&changeflag,enemydata);
			music (room,lastroom,bso,&changeflag,jean.flags[6]);
			for (n=0; n<24; n++) { /* Reset enemyshoots */
				proyec[n] = 0;
			}
			counter[0] = 0;
			changeflag = 0;

			/* Update window title */
			update_title(screen, room_names[room]);
#if DEBUG
			if (room != lastroom)
				printf("Room change: \"%s\" (%d) -> \"%s\" (%d)\n",
					room_names[lastroom], lastroom, room_names[room], room);
#endif
		}
		/* Parchments */
		if (parchment > 0)
			showparchment (renderer,&parchment);
		if (jean.flags[6] == 3)
			redparchment (renderer,&jean);
		if (jean.flags[6] == 6)
			blueparchment (renderer,&jean);

		/* Flip ! */
		renderpresent(renderer);

		if (parchment > 0) {
			Mix_PlayChannel(-1, fx[2], 0);
			Mix_PauseMusic();
			/* Waiting a key */
			while (keyp == 0)
				keybpause (&keyp);
			jean.push[2] = 0;
			jean.push[3] = 0;
			keyp = 0;
			Mix_ResumeMusic();
			parchment = 0;
		}
		if (jean.flags[6] == 4) {
			Mix_PlayChannel(-1, fx[2], 0);
			SDL_Delay(5 * 1000);
			jean.flags[6] = 5;
			jean.direction = 0;
			music (room,lastroom,bso,&changeflag,jean.flags[6]);
		}
		if (jean.flags[6] == 6) {
			SDL_Delay(5 * 1000);
			jean.death = 0;
			changeflag = 1;
			room = ROOM_THEEND;
			jean.flags[6] = 7;
			jean.x = 125;
			jean.y = 115;
			jean.jump = 1;
		}

		if (jean.state[0] == 0) {
			Mix_HaltMusic();
			/* Mix_FreeMusic(sonido); */
			*state = 3;
			exit = 1;
		}

	}

	/* Cleaning */
	SDL_DestroyTexture(tiles);
	SDL_DestroyTexture(fonts);
	for (i=0;i<8;i++)
		Mix_FreeMusic(bso[i]);
	for (i=0;i<7;i++)
		Mix_FreeChunk(fx[i]);

	*fullscreen = winfull;

}

void animation (uint stagedata[][22][32],uint room,uint counter[]) {

	uint16_t data = 0;

	for (int8_t j=0; j<=21; j++) {
		for (int8_t i=0; i<=31; i++) {

			data = stagedata[room][j][i];

			/* Fire animation */
			if ((data == 59) || (data == 60)) {
				if ((counter[0] == 1) || (counter[0] == 11) || (counter[0] == 21)) {
					if (data == 59)
						data = 60;
					else
						data = 59;
				}
			}

			/* Water animation */
			if ((data > 500) && (data < 533)) {
				if (data < 532)
					data ++;
				else
					data = 501;
			}

			stagedata[room][j][i] = data;

		}
	}
}

void counters (uint counter[]) {

	if (counter[0] < 29)
		counter[0] ++;
	else
		counter[0] = 0;

	if (counter[1] < 90)
		counter[1] ++;
	else
		counter[1] = 0;

	if (counter[2] < 8)
		counter[2] ++;
	else
		counter[2] = 0;

}

void control (struct hero *jean,uint *keyp) {

	SDL_Event event;

	if (*keyp > 0)
		*keyp = 0;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			*keyp = 10;
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_UP) {
				if ((jean->push[0] == 0) && (jean->jump == 0) && (jean->ducking == 0))
					jean->jump = 1;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				if ((jean->push[1] == 0) && (jean->jump == 0)) {
					jean->push[1] = 1;
					jean->ducking = 1;
				}
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				if (jean->push[2] == 0) {
					jean->push[2] = 1;
					jean->push[3] = 0;
				}
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				if (jean->push[3] == 0) {
					jean->push[3] = 1;
					jean->push[2] = 0;
				}
			}
			if (event.key.keysym.sym == SDLK_f)
				*keyp = 6;
			if (event.key.keysym.sym == SDLK_c)
				*keyp = 9;
			if (event.key.keysym.sym == SDLK_ESCAPE)
				*keyp = 10;
			}

		if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_UP)
				jean->push[0] = 0;
			if (event.key.keysym.sym == SDLK_DOWN) {
				jean->push[1] = 0;
				jean->ducking = 0;
			}
			if (event.key.keysym.sym == SDLK_LEFT)
				jean->push[2] = 0;
			if (event.key.keysym.sym == SDLK_RIGHT)
				jean->push[3] = 0;
		}

		if (event.type == SDL_JOYAXISMOTION) {
			if (event.jaxis.axis == X_JOYAXIS) {
				if (event.jaxis.value < 0) { // BUTTONDOWN LEFT	
					jean->push[2] = 1;
					jean->push[3] = 0;
				}
				if (event.jaxis.value > 0) { // BUTTONDOWN RIGHT
					jean->push[3] = 1;
					jean->push[2] = 0;
				}
				if (event.jaxis.value == 0) { // BUTTONUP	
					jean->push[2] = 0;
					jean->push[3] = 0;
				}
			}
			if (event.jaxis.axis == Y_JOYAXIS) {
				if (event.jaxis.value > 0) { // BUTTONDOWN DUCK
					jean->push[1] = 1;
					jean->ducking = 1;
				}
				if (event.jaxis.value == 0) { // BUTTONUP
					jean->push[1] = 0;
					jean->ducking = 0;
				}
			}

		}

		if (event.type == SDL_JOYHATMOTION) {
			jean->push[1] = 0;
			jean->push[2] = 0;
			jean->push[3] = 0;
			jean->ducking = 0;
			if (event.jhat.value & SDL_HAT_LEFT) {
				jean->push[2] = 1;
				jean->push[3] = 0;
			} else if (event.jhat.value & SDL_HAT_RIGHT) {
				jean->push[3] = 1;
				jean->push[2] = 0;
			}
			if (event.jhat.value & SDL_HAT_DOWN) {
				jean->push[1] = 1;
				jean->ducking = 1;
			} else if (event.jhat.value & SDL_HAT_UP) {
				jean->push[1] = 0;
				jean->ducking = 0;
			}
		}

		if (event.type == SDL_JOYBUTTONDOWN) {
			if (event.jbutton.button == JUMP_JOYBUTTON)
				if ((jean->push[0] == 0) && (jean->jump == 0) && (jean->ducking == 0))
					jean->jump = 1;
		
			if (event.jbutton.button == SELECT_JOYBUTTON)
				*keyp = 10;
		}
		
		if (event.type == SDL_JOYBUTTONUP) {
				if (event.jbutton.button == JUMP_JOYBUTTON)
					jean->push[0] = 0;
		}
	}
}

void events (struct hero *jean,uint stagedata[][22][32],uint room,uint counter[],struct enem *enemies,Mix_Chunk *fx[]) {

	int8_t x = 0;
	int8_t y = 0;

	if (room == ROOM_THEEND) {
		if (jean->temp < 7) {
			/* Moving Jean */
			if (counter[1] == 45) {
				jean->direction ^= 1;
				jean->temp ++;
			}
		} else {
			jean->direction = 0;
			jean->death = 1;
		}
	}

	if (room == ROOM_CHURCH) {
		/* Close door */
		if ((jean->x > 24) && (jean->flags[0] == 0)) {
			stagedata[ROOM_CHURCH][14][0] = 347;
			stagedata[ROOM_CHURCH][15][0] = 348;
			stagedata[ROOM_CHURCH][16][0] = 349;
			stagedata[ROOM_CHURCH][17][0] = 350;
			jean->flags[0] = 1;
			Mix_PlayChannel(-1, fx[1], 0);
			SDL_Delay(1 * 1000);
		}
	}

	if (room == ROOM_ALTAR) {
		/* Open ground */
		if ((jean->x > 15) && (jean->flags[1] == 1) && (stagedata[ROOM_ALTAR][20][26] == 7)) {
			stagedata[ROOM_ALTAR][20][26] = 38;
			stagedata[ROOM_ALTAR][20][27] = 0;
			stagedata[ROOM_ALTAR][21][26] = 0;
			stagedata[ROOM_ALTAR][21][27] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			SDL_Delay(1 * 1000);
		}
		/* Open door */
		if ((jean->x > 211) && (jean->flags[2] == 1) && (stagedata[ROOM_ALTAR][14][31] == 343)) {
			stagedata[ROOM_ALTAR][14][31] = 0;
			stagedata[ROOM_ALTAR][15][31] = 0;
			stagedata[ROOM_ALTAR][16][31] = 0;
			stagedata[ROOM_ALTAR][17][31] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			SDL_Delay(1 * 1000);
		}
	}
	if (room == ROOM_BEAST) {
		/* Dragon fire kills Jean */
		if (((jean->x > 127) && (jean->x < 144)) && (jean->y < 89)) {
			if ((enemies->speed[0] > 109) && (enemies->speed[0] < 146))
				jean->death = 1;
		}
	}
	if (room == ROOM_RIVER) {
		/* Open door */
		if ((jean->y > 16) && (jean->flags[3] == 1) && (stagedata[ROOM_RIVER][16][0] == 347)) {
			stagedata[ROOM_RIVER][16][0] = 0;
			stagedata[ROOM_RIVER][17][0] = 0;
			stagedata[ROOM_RIVER][18][0] = 0;
			stagedata[ROOM_RIVER][19][0] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			SDL_Delay(1 * 1000);
		}
	}
	if (room == ROOM_GATE) {
		/* Open door */
		if ((jean->x > 208) && (jean->flags[4] == 1) && (stagedata[ROOM_GATE][14][31] == 343)) {
			stagedata[ROOM_GATE][14][31] = 0;
			stagedata[ROOM_GATE][15][31] = 0;
			stagedata[ROOM_GATE][16][31] = 0;
			stagedata[ROOM_GATE][17][31] = 0;
			Mix_PlayChannel(-1, fx[1], 0);
			SDL_Delay(1 * 1000);
		}
	}

	if (room == ROOM_SATAN) {
		if ((jean->state[1] == 12) && (jean->x > 8) && (jean->flags[6] == 0)) {
			/* Block entry */
			stagedata[ROOM_SATAN][16][0] = 99;
			stagedata[ROOM_SATAN][17][0] = 99;
			stagedata[ROOM_SATAN][18][0] = 99;
			stagedata[ROOM_SATAN][19][0] = 99;
			jean->flags[6] = 1;
			/* Mark checkpoint */
			jean->checkpoint[0] = 24;
			jean->checkpoint[1] = jean->x;
			jean->checkpoint[2] = jean->y;
		}
		if ((jean->flags[6] == 1) && (jean->state[1] > 0) && (counter[0] == 0)) {
			/* Putting crosses */
			switch (jean->state[1]) {
				case 1:
					x=11;
					y=5;
					break;
				case 2:
					x=10;
					y=5;
					break;
				case 3:
					x=8;
					y=8;
					break;
				case 4:
					x=5;
					y=8;
					break;
				case 5:
					x=4;
					y=8;
					break;
				case 6:
					x=2;
					y=12;
					break;
				case 7:
					x=5;
					y=14;
					break;
				case 8:
					x=6;
					y=14;
					break;
				case 9:
					x=9;
					y=14;
					break;
				case 10:
					x=10;
					y=14;
					break;
				case 11:
					x=13;
					y=13;
					break;
				case 12:
					x=15;
					y=16;
					break;
			}
			stagedata[ROOM_SATAN][y][x] = 84;
			jean->state[1] --;
			Mix_PlayChannel(-1, fx[1], 0);
		}
		if ((jean->flags[6] == 1) && (jean->state[1] == 0) && (counter[0] == 29)) {
			/* Draw cup */
			stagedata[ROOM_SATAN][3][15] = 650;
			jean->flags[6] = 2;
			Mix_PlayChannel(-1, fx[1], 0);
		}
		/* Killed Satan, Smoke appears */
		if (enemies->type[0] == 88) {
			if (enemies->speed[0] < 90)
				enemies->speed[0] ++;
			else {
				enemies->speed[0] = 0;
				enemies->type[0] = 0;
				enemies->x[0] = 0;
				enemies->y[0] = 0;
				enemies->type[0] = 17;
				/* Putting red parchment */
				stagedata[ROOM_SATAN][14][28] = 339;
				stagedata[ROOM_SATAN][14][29] = 340;
				stagedata[ROOM_SATAN][15][28] = 341;
				stagedata[ROOM_SATAN][15][29] = 342;
			}
		}
	}
}

void music (uint room,uint lastroom,Mix_Music *bso[],uint *changeflag,int flag) {

	if (room == ROOM_PRAYER) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[0], 0);
	}
	if ((room == ROOM_TOWER) && (lastroom == ROOM_PRAYER)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (room == ROOM_THEEND) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[2], 0);
	}
	if ((room == ROOM_ESCAPE) && (lastroom != ROOM_CLOSE)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[7], -1);
	}
	if ((room == ROOM_CLOSE) && (lastroom == ROOM_CHURCH)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[7], -1);
	}
	if ((room == ROOM_CHURCH) && (lastroom == ROOM_CLOSE)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (((room == ROOM_ALTAR) && (lastroom == ROOM_HANGMAN)) || ((room == ROOM_ALTAR) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (room == ROOM_HANGMAN) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[3], 0);
	}
	if (((room == ROOM_CAVE) && (lastroom == ROOM_RUINS)) || ((room == ROOM_CAVE) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if (((room == ROOM_RUINS) && (lastroom == ROOM_CAVE)) || ((room == ROOM_RUINS) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if ((room == ROOM_CATACOMBS) && (lastroom == ROOM_GARDEN)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (((room == ROOM_GARDEN) && (lastroom == ROOM_CATACOMBS)) || ((room == ROOM_GARDEN) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room == ROOM_TUNELS) && (*changeflag == 2)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room == ROOM_LAKE) && (lastroom == ROOM_WHEEL)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room == ROOM_WHEEL) && (lastroom == ROOM_LAKE)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[1], -1);
	}
	if (room == ROOM_BANQUET) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[5], -1);
	}
	if (((room == ROOM_RIVER) && (lastroom == ROOM_BANQUET)) || ((room == ROOM_RIVER) && (*changeflag == 2))) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room == ROOM_GATE) && (lastroom == ROOM_EVIL)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[4], -1);
	}
	if ((room == ROOM_EVIL) && (lastroom == ROOM_GATE)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[6], -1);
	}
	if ((room == ROOM_ASHES) && (lastroom == ROOM_SATAN)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[6], -1);
	}
	if ((room == ROOM_SATAN) && (flag != 5)) {
		Mix_HaltMusic();
		Mix_PlayMusic(bso[5], -1);
	}
	if ((room == ROOM_SATAN) && (flag == 5))
		Mix_PlayMusic(bso[7], -1);

	*changeflag -= 1;

}

void changescreen (struct hero *jean,uint *room, uint *lastroom, uint *changeflag) {

	if ((jean->x < -8) && (*room != ROOM_ESCAPE)) { /* Cannot go left to end room */
		*lastroom = *room;
		*room -= 1;
		jean->x = 240;
		*changeflag = 1;
	}
	if ((jean->x + 8) > SCREEN_W) {
		*lastroom = *room;
		*room += 1;
		jean->x = 1;
		*changeflag = 1;
	}
	if ((jean->y + 12 < -16) && (jean->jump == 1)) {
		*lastroom = *room;
		*room -= 5;
		jean->y = 152;
		*changeflag = 1;
	}
	if ((jean->y > 175) && (jean->jump != 1)) {
		*lastroom = *room;
		*room += 5;
		jean->y = -16;
		*changeflag = 1;
	}
}

void keybpause (uint *keyp) {

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if ((event.key.keysym.sym == SDLK_SPACE) || (event.key.keysym.sym == SDLK_LEFT) || (event.key.keysym.sym == SDLK_RIGHT))
				*keyp = 1;
		}
		if (event.type == SDL_JOYBUTTONDOWN) {
			if (event.jbutton.button == JUMP_JOYBUTTON || event.jbutton.button == START_JOYBUTTON) {
				*keyp = 1;
			}
		}
		if (event.type == SDL_QUIT) {
			*keyp = 10;
		}
	}

}
