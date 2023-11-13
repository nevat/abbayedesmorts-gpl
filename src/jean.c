/* jean.c */

#include "jean.h"
#include "rooms.h"

void movejean (struct hero *jean, Mix_Chunk *fx[]) {

	/* Jump */
	if (jean->jump == 1) {
		if (jean->height == 0) /* Jump sound */
			Mix_PlayChannel(-1, fx[3], 0);
		if (jean->height < 56) {
			jean->height += 1.6;
			if ((jean->collision[0] == 0) && (jean->height < 44))
				jean->y -= 1.5;
			jean->animation = 0;
		}
		else {
			jean->jump = 2;
			jean->collision[0] = 0;
		}
	}


	/* Move to right */
	if (jean->push[3] == 1) {
		jean->direction = 1;
		if (jean->collision[3] == 0) {
			if (jean->jump == 0) {
				if (jean->animation < 13)
					jean->animation ++;
				else
					jean->animation = 0;
			}
			if (jean->push[1] == 1)
				jean->x += 0.30;
			else
				jean->x += 0.65;
		}
	}

	/* Move to left */
	if (jean->push[2] == 1) {
		jean->direction = 0;
		if (jean->collision[2] == 0) {
			if (jean->jump == 0) {
				if (jean->animation < 13)
					jean->animation ++;
				else
					jean->animation = 0;
			}
			if (jean->push[1] == 1)
				jean->x -= 0.30;
			else
				jean->x -= 0.65;
		}

	}

}

void drawjean (SDL_Renderer *renderer,SDL_Texture *tiles,struct hero *jean,uint counter[],Mix_Chunk *fx[],uint changetiles) {

	SDL_Rect srctile = {320,88,16,24};
	SDL_Rect destile = {0,0,16,24};
	SDL_Rect srcducktile = {448,88,18,13};
	SDL_Rect desducktile = {0,0,18,13};
	int r = 0;

	if (jean->death == 0) {
		if (jean->jump > 0) {
			r = 1;
			jean->animation = 0;
		}

		if (jean->ducking == 0) {
			srctile.x += ((jean->animation / 7) * 16) + (r * 32);
			destile.y = jean->y;
			destile.x = jean->x;
			if (jean->y > 152)
				srctile.h = (176 - jean->y);
			if (changetiles == 1)
				srctile.y = 208;
			SDL_RenderCopyEx(renderer,tiles,&srctile,&destile,0,0,jean->direction);
		}
		else {
			srcducktile.x += ((jean->animation / 7) * 18);
			desducktile.y = jean->y + 11;
			desducktile.x = jean->x;
			if (changetiles == 1)
				srcducktile.y = 208;
			SDL_RenderCopyEx(renderer,tiles,&srcducktile,&desducktile,0,0,jean->direction);
		}
	}

	/* Death animation */
	if (jean->death > 0) {
		jean->death += 1;
		destile.x = jean->x;
		destile.y = jean->y;
		Mix_PauseMusic();
		if (jean->death == 2)
			Mix_PlayChannel(-1, fx[6], 0);
		if ((jean->death < 8) || ((jean->death > 23) && (jean->death < 32)) || ((jean->death > 47) && (jean->death < 56))) {
			srctile.x = 368 + (jean->direction * 64);
			if (changetiles == 1)
				srctile.y = 208;
			SDL_RenderCopy(renderer,tiles,&srctile,&destile);
		}
		if (((jean->death > 7) && (jean->death < 16)) || ((jean->death > 31) && (jean->death < 40)) || ((jean->death > 55) && (jean->death < 64))) {
			srctile.x = 536;
			if (changetiles == 1)
				srctile.y = 207;
			else
				srctile.y = 87;
			SDL_RenderCopy(renderer,tiles,&srctile,&destile);
		}
		if (((jean->death > 15) && (jean->death < 24)) || ((jean->death > 39) && (jean->death < 48)) || ((jean->death > 63) && (jean->death < 73))) {
			srctile.x = 520;
			if (changetiles == 1)
				srctile.y = 207;
			else
				srctile.y = 87;
			SDL_RenderCopy(renderer,tiles,&srctile,&destile);
		}
	}

	/* Animation room 24 */
	if ((jean->flags[6] == 5) && (counter[1] == 45)) {
		jean->direction ^= 1;
	}

}

void collisions (struct hero *jean,uint stagedata[][22][32],int room) {

	int blleft = 0;
	int blright = 0;
	int blground[4] = {0,0,0,0};
	int blroof[2] = {0,0};
	int points[8] = {0,0,0,0,0,0,0,0};
	int n = 0;
	int r = 0;

	points[0] = (jean->x + 1) / 8;
	points[1] = (jean->x + 7) / 8;
	points[2] = (jean->x + 8) / 8;
	points[3] = (jean->x + 13) / 8;
	points[4] = (jean->y + 1) / 8;
	points[5] = (jean->y + 8) / 8;
	points[6] = (jean->y + 15) / 8;
	points[7] = (jean->y + 23) / 8;

	jean->collision[0] = 0;
	jean->collision[1] = 0;
	jean->collision[2] = 0;
	jean->collision[3] = 0;

	/* Left & Right collisions */
	if (jean->ducking == 0) {
		for (n=4; n<8; n++) {
			if (((points[0] != 0) && (jean->direction ==0)) || ((points[3] != 31) && (jean->direction == 1))) {
				blleft = stagedata[room][points[n]][points[0] - 1];
				blright = stagedata[room][points[n]][points[3] + 1];
				if (((blleft > 0) && (blleft < 100) && (blleft != 16) && (blleft != 38) && (blleft != 37)) || ((stagedata[room][points[4]][points[0]] == 128) || (blleft == 348))) {
					if (jean->x - ((points[0] - 1) * 8 + 7) < 1.1)
						jean->collision[2] = 1;
				}
				if (((blright > 0) && (blright < 100) && (blright != 16) && (blright != 38) && (blright != 37)) || (blright == 344)) {
					if (((points[3] + 1) * 8) - (jean->x + 14) < 1.1)
						jean->collision[3] = 1;
				}
			}
		}
	}

	/* Collision with Jean ducking */
	if (jean->ducking == 1) {
		if (((points[0] != 0) && (jean->direction ==0)) || ((points[3] != 31) && (jean->direction == 1))) {
			r = (jean->y + 16) / 8;
			blleft = stagedata[room][r][points[0] - 1];
			blright = stagedata[room][r][points[3] + 1];
			if (((blleft > 0) && (blleft < 100) && (blleft != 37)) || ((stagedata[room][r][points[0]] == 128) || ((blleft > 346) && (blleft < 351)))) {
				if (jean->x - ((points[0] - 1) * 8 + 7) < 1.1)
					jean->collision[2] = 1;
			}
			if (((blright > 0) && (blright < 100) && (blright != 37)) || ((blright > 342) && (blright< 347))) {
				if (((points[3] + 1) * 8) - (jean->x + 14) < 1.1)
					jean->collision[3] = 1;
			}
		}
		/* Invisible wall */
		if ((room == ROOM_CAVE) && (r == 5)) {
			if ((points[0] - 1 == 0) || (points[0] - 1 == 1))
				jean->collision[2] = 0;
			if ((points[3] + 1 == 0) || (points[3] + 1 == 1))
				jean->collision[3] = 0;
		}
		if ((room == ROOM_BEAST) && (r == 5)) {
			if ((points[0] - 1 > 27) && (points[0] - 1 < 32))
				jean->collision[2] = 0;
			if ((points[3] + 1 > 27) && (points[3] + 1 < 32))
				jean->collision[3] = 0;
		}
	}

	/* Touch ground collision */
	blground[0] = stagedata[room][points[7]+1][points[0]];
	blground[1] = stagedata[room][points[7]+1][points[1]];
	blground[2] = stagedata[room][points[7]+1][points[2]];
	blground[3] = stagedata[room][points[7]+1][points[3]];

	if (jean->jump != 1) {
		/* Invisible ground */
		if (((room == ROOM_CAVE) && (points[7]+1 > 19) && (points[0] == 2)) || ((room == ROOM_LAKE) && ((jean->y / 8) < 4) && (points[0] == 2))) {
			jean->y += jean->gravity;
			jean->jump = 2;
		}
		else {
			if (((blground[0] > 0) && (blground[0] < 100)) || ((blground[1] > 0) && (blground[1] < 100)) || ((blground[2] > 0) && (blground[2] < 100)) || ((blground[3] > 0) && (blground[3] < 100))) {
				jean->ground = (points[7] + 1) * 8;
				if (points[7] + 1 > 21) /* Dirty trick to make Jean go bottom of the screen */
					jean->ground = 300;
				if ((jean->ground - 1) - (jean->y+23) > 1.2)
					jean->y += jean->gravity;
				else { /* Near ground */
					jean->y += (jean->ground - 1) - (jean->y + 23);
					jean->height = 0;
					jean->jump = 0;
					jean->flags[5] = 0;
				}
			}
			else {/* In air, ground near */
				jean->y += jean->gravity;
				jean->jump = 2;
			}
		}
	}

	/* Check small platforms */
	if (jean->direction == 0) {
		if ((blground[3] == 38) && ((jean->x + 13) < (points[3] * 8 + 5)) && (jean->push[2] == 1) && (jean->jump ==0)) {
			jean->y += jean->gravity;
			jean->jump = 2;
		}
	}
	if (jean->direction == 1) {
		if ((blground[0] == 38) && ((jean->x + 1) > (points[0] + 2)) && (jean->push[3] == 1) && (jean->jump == 0)) {
			jean->y += jean->gravity;
			jean->jump = 2;
		}
	}

	/* Touch roof collision */
	blroof[0] = stagedata[room][points[4]-1][points[0]];
	blroof[1] = stagedata[room][points[4]-1][points[3]];

	if ((jean->jump == 1) && (points[4] > 0)) {
		if (((blroof[0] > 0) && (blroof[0] < 100) && (blroof[0] != 16) && (blroof[0] != 38) && (blroof[0] != 37)) || ((blroof[1] > 0) && (blroof[1] < 100) && (blroof[1] != 16) && (blroof[1] != 38) && (blroof[1] != 37))) {
			if ((jean->y - 1) - ((points[4] - 1) * 8 + 7) < 1)
				jean->collision[0] = 1;
		}
	}

}

void touchobj (struct hero *jean,uint stagedata[][22][32],uint *room,uint *lastroom,uint *parchment,uint *changeflag,struct enem *enemies,float proyec[],Mix_Chunk *fx[]) {

	int x = 0;
	int y = 0;
	int h = 0;
	int v = 0;
	int r = 0;
	int flag = 0;

	x = (jean->x + 2) / 8;
	y = jean->y / 8;

	if (y > 0) {

		/* Touch spikes, water or fire */
		if (((stagedata[*room][y+3][x] == 5) || (stagedata[*room][y+3][x+1] == 5)) || (((stagedata[*room][y+3][x] > 500) && (stagedata[*room][y+3][x] < 532))|| ((stagedata[*room][y+3][x+1] > 500) && (stagedata[*room][y+3][x+1] < 532))) || (((stagedata[*room][y+3][x] == 59) || (stagedata[*room][y+3][x] == 59)) || ((stagedata[*room][y+3][x+1] == 60) && (stagedata[*room][y+3][x+1] == 60)))) {
			if ((*room == ROOM_CAVE) && (y+3 == 20) && (x < 4))
				jean->death = 0;
			else {
				if (jean->death == 0)
					jean->death = 1;
			}
		}

		/* Touch checkpoint */
		if (((stagedata[*room][y][x] > 320) && (stagedata[*room][y][x] < 325)) || ((stagedata[*room][y][x+1] > 320) && (stagedata[*room][y][x+1] < 325))) {
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((stagedata[*room][v][h] > 320) && (stagedata[*room][v][h] < 325))
						stagedata[*room][v][h] += 6;
				}
			}
			jean->checkpoint[3] = jean->checkpoint[0];
			jean->checkpoint[0] = *room;
			jean->checkpoint[1] = jean->x;
			jean->checkpoint[2] = jean->y;
			/* Old checkpoint returns to original state */
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((stagedata[jean->checkpoint[3]][v][h] > 326) && (stagedata[jean->checkpoint[3]][v][h] < 331))
						stagedata[jean->checkpoint[3]][v][h] -= 6;
				}
			}
			Mix_PlayChannel(-1, fx[2], 0);
		}

		/* Touch bell */
		if (*room == ROOM_TOWER) {
			if (((stagedata[*room][y+1][x] > 300) && (stagedata[*room][y+1][x] < 305)) || ((stagedata[*room][y+1][x+1] > 300) && (stagedata[*room][y+1][x+1] < 305))) {
				for (v=1;v<3;v++) {
					for (h=5;h<7;h++) {
						if ((stagedata[*room][v][h] > 300) && (stagedata[*room][v][h] < 305))
							stagedata[*room][v][h] += 4;
					}
				}
				jean->flags[1] = 1;
				Mix_PauseMusic();
				Mix_PlayChannel(-1, fx[5], 0);
				SDL_Delay(2 * 1000);
				Mix_ResumeMusic();
			}
		}

		/* Touch lever */
		if (((stagedata[*room][y+1][x] > 308) && (stagedata[*room][y+1][x] < 313)) || ((stagedata[*room][y+1][x+1] > 308) && (stagedata[*room][y+1][x+1] < 313))) {
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((stagedata[*room][v][h] > 308) && (stagedata[*room][v][h] < 313))
						stagedata[*room][v][h] += 4;
				}
			}
			if (*room == ROOM_HANGMAN)
				jean->flags[3] = 1;
			if (*room == ROOM_BEAST)
				jean->flags[2] = 1;
			if (*room == ROOM_GATE)
				jean->flags[4] = 1;
			Mix_PauseMusic();
			Mix_PlayChannel(-1, fx[5], 0);
			SDL_Delay(2 * 1000);
			Mix_ResumeMusic();
		}


		/* Touch hearts */
		if (*room == ROOM_ASHES) {
			if (((stagedata[*room][y+1][x] > 400) && (stagedata[*room][y+1][x] < 405)) || ((stagedata[*room][y+1][x+1] > 400) && (stagedata[*room][y+1][x+1] < 405))) {
				if (jean->x > 160) {
					stagedata[ROOM_ASHES][7][23] = 0;
					stagedata[ROOM_ASHES][7][24] = 0;
					stagedata[ROOM_ASHES][8][23] = 0;
					stagedata[ROOM_ASHES][8][24] = 0;
				}
				else {
					stagedata[ROOM_ASHES][18][8] = 0;
					stagedata[ROOM_ASHES][18][9] = 0;
					stagedata[ROOM_ASHES][19][8] = 0;
					stagedata[ROOM_ASHES][19][9] = 0;
				}
				if (jean->state[0] < 9)
					jean->state[0] += 1;
				Mix_PlayChannel(-1, fx[2], 0);;
			}
		}
		else {
			if (((stagedata[*room][y+1][x] > 400) && (stagedata[*room][y+1][x] < 405)) || ((stagedata[*room][y+1][x+1] > 400) && (stagedata[*room][y+1][x+1] < 405))) {
				for (v=0;v<22;v++) {
					for (h=0;h<32;h++) {
						if ((stagedata[*room][v][h] > 400) && (stagedata[*room][v][h] < 405))
							stagedata[*room][v][h] = 0;
					}
				}
				if (jean->state[0] < 9)
					jean->state[0] += 1;
				Mix_PlayChannel(-1, fx[2], 0);
			}
		}

		/* Touch crosses */
		if (((stagedata[*room][y+1][x] > 408) && (stagedata[*room][y+1][x] < 413)) || ((stagedata[*room][y+1][x+1] > 408) && (stagedata[*room][y+1][x+1] < 413)) || ((stagedata[*room][y+2][x] > 408) && (stagedata[*room][y+2][x] < 413))) {
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((stagedata[*room][v][h] > 408) && (stagedata[*room][v][h] < 413))
						stagedata[*room][v][h] = 0;
				}
			}
			jean->state[1] += 1;
			Mix_PlayChannel(-1, fx[2], 0);;
		}

		/* Touch yellow parchment */
		if (((stagedata[*room][y+1][x] > 316) && (stagedata[*room][y+1][x] < 321)) || ((stagedata[*room][y+1][x+1] > 316) && (stagedata[*room][y+1][x+1] < 321))) {
			for (v=0;v<22;v++) {
				for (h=0;h<32;h++) {
					if ((stagedata[*room][v][h] > 316) && (stagedata[*room][v][h] < 321))
						stagedata[*room][v][h] = 0;
				}
			}
			*parchment = *room;
		}

		/* Touch red parchment */
		if (((stagedata[*room][y+1][x] > 338) && (stagedata[*room][y+1][x] < 343)) || ((stagedata[*room][y+1][x+1] > 338) && (stagedata[*room][y+1][x+1] < 343))) {
			jean->flags[6] = 3;
			/* Delete parchment */
			stagedata[ROOM_SATAN][14][28] = 0;
			stagedata[ROOM_SATAN][14][29] = 0;
			stagedata[ROOM_SATAN][15][28] = 0;
			stagedata[ROOM_SATAN][15][29] = 0;
		}

		/* Touch door */
		if ((*room == ROOM_BEAST) || (*room == ROOM_RIVER)) {
			if (stagedata[*room][y][x] == 154) {
				switch (*room) {
					case ROOM_BEAST:
						*room = ROOM_RIVER;
						*lastroom = ROOM_BEAST;
						jean->x = 160;
						jean->y = 120;
						break;
					case ROOM_RIVER:
						*room = ROOM_BEAST;
						*lastroom = ROOM_RIVER;
						jean->x = 176;
						jean->y = 136;
						break;
				}
				Mix_PlayChannel(-1, fx[1], 0);;
				*changeflag = 1;
#ifdef DEBUG
				printf("Door used.\n");
#endif
			}
		}

		/* Touch switch */
		if (*room == ROOM_WHEEL) {
			if ((((stagedata[*room][y+1][x] > 330) && (stagedata[*room][y+1][x] < 339)) || ((stagedata[*room][y+1][x+1] > 330) && (stagedata[*room][y+1][x+1] < 339))) && (jean->flags[5] == 0)) {
				for (v=2;v<4;v++) {
					for (h=15;h<17;h++) {
						if ((stagedata[*room][v][h] > 330) && (stagedata[*room][v][h] < 335)) {
							stagedata[*room][v][h] += 4;
							jean->flags[5] = 1;
						}
						if (((stagedata[*room][v][h] > 334) && (stagedata[*room][v][h] < 339)) && (jean->flags[5] == 0))
							stagedata[*room][v][h] -= 4;
					}
				}
				jean->flags[5] = 1;
				/* Flapping all crosses */
				for (r=1; r<25; r++) {
					for (v=0; v<22; v++) {
						for (h=0; h<32; h++) {
							flag = 0;
							/* Crosses enabled */
							if ((stagedata[r][v][h] > 408) && (stagedata[r][v][h] < 413)) {
								stagedata[r][v][h] += 16;
								flag = 1;
							}
							/* Crosses disabled */
							if ((stagedata[r][v][h] > 424) && (stagedata[r][v][h] < 429) && (flag == 0))
								stagedata[r][v][h] -= 16;
						}
					}
				}
				Mix_PauseMusic();
				Mix_PlayChannel(-1, fx[5], 0);;
				SDL_Delay(2 * 1000);
				Mix_ResumeMusic();
#ifdef DEBUG
				printf("Crosses inverted.\n");
#endif
			}
		}

		/* Touch cup */
		if (*room == ROOM_SATAN) {
			if ((stagedata[*room][y][x+1] == 650) || (stagedata[*room][y+1][x+1] == 650) || (stagedata[*room][y+2][x+1] == 650)) {
				Mix_HaltMusic();
				Mix_PlayChannel(-1, fx[5], 0);
				SDL_Delay(2 * 1000);
				stagedata[ROOM_SATAN][3][15] = 0; /* Delete cup */
				/* Delete crosses */
				for (v=0; v<22; v++) {
					for (h=0; h<32; h++) {
						if (stagedata[*room][v][h] == 84)
							stagedata[*room][v][h] = 0;
					}
				}
				/* Delete Satan */
				enemies->type[0] = 88;
				enemies->speed[0] = 0; /* Using speed as counter */
				enemies->adjustx1[0] = 0;
				enemies->adjustx2[0] = 0;
				enemies->adjusty1[0] = 0;
				enemies->adjusty2[0] = 0;
				/* Deleting shoots */
				for (v=0;v<24;v++)
					proyec[v] = 0;
				/* Init crusaders */
				for (v=1;v<7;v++)
					enemies->type[v] = 17;
				enemies->adjustx2[0] = 15;
				enemies->adjusty2[0] = 23;
			}
		}

	}

}

void contact (struct hero *jean,struct enem enemies,float proyec[],uint room) {

	int i = 0;
	int points[4] = {0,0,0,0}; /* 4 points of collision of enemy sprite */
	int x = 0;
	int y = 0;

	/* Collisions with enemies */
	for (i=0;i<7;i++) {
		if (((enemies.type[i] > 0) && (enemies.type[i] != 12)) || ((enemies.type[i] == 12) && (enemies.y[i] > enemies.limleft[i] + 8))) {
			/* Setting points of collision... */
			points[0] = enemies.x[i] + enemies.adjustx1[i];
			points[1] = enemies.x[i] + enemies.adjustx2[i];
			points[2] = enemies.y[i] + enemies.adjusty1[i];
			points[3] = enemies.y[i] + enemies.adjusty2[i];
			/* Checking... */
			for (x=points[0];x<=points[1];x++) {
				if ((x>jean->x+1) && (x<jean->x+13)) {
					for (y=points[2];y<=points[3];y++) {
						if ((y>jean->y+(jean->ducking*8)) && (y<jean->y+22)) {
							if (jean->flags[6] < 5) {
								jean->death = 1;
								y=points[3] + 1;
								x=points[1] + 1;
							}
							else {
								/* Mix_HaltMusic(); */
								jean->flags[6] = 6;
							}
						}
					}
				}
			}

		}
	}

	/* Collision with shoots */
	for (i=0;i<3;i++) {
		if (proyec[i*2] > 0) {
			/* Setting points of collision */
			if (enemies.type[i] == 11) { /* Gargoyle */
				points[0] = proyec[i*2];
				points[1] = proyec[i*2]+10;
				points[2] = enemies.y[i] + 10;
				points[3] = enemies.y[i] + 12;
			}

			if (enemies.type[i] == 15) { /* Archers */
				points[0] = proyec[i*2] + 3;
				points[1] = proyec[i*2] + 7;
				points[2] = enemies.y[i] + 10;
				points[3] = enemies.y[i] + 17;
			}
			for (x=points[0];x<=points[1];x++) {
				if ((x>jean->x+3) && (x<jean->x+13)) {
					for (y=points[2];y<=points[3];y++) {
						if ((y>(jean->y+5+(jean->ducking*8))) && (y<jean->y+22)) {
							jean->death = 1;
							y=points[3] + 1;
							x=points[1] + 1;
						}
					}
				}
			}

		}

	}

	/* Check collision with plants shoots, dragon, death and Satan */
	if ((room == ROOM_BEAST) || (room == ROOM_GARDEN) || (room == ROOM_BANQUET) || (room == ROOM_SATAN)) {
		for (i=0;i<23;i+=2) {
			if (proyec[i] > 0) {
				if (room == ROOM_BANQUET) {
					points[0] = proyec[i+1];
					points[1] = proyec[i+1]+15;
					points[2] = proyec[i];
					points[3] = proyec[i] + 15;
				}
				if ((room == ROOM_GARDEN) || (room == ROOM_SATAN)) {
					points[0] = proyec[i];
					points[1] = proyec[i] + 3;
					points[2] = proyec[i+1];
					points[3] = proyec[i+1] + 3;
				}
				if (room == ROOM_BEAST) {
					points[0] = proyec[i];
					points[1] = proyec[i] + 8;
					points[2] = 88;
					points[3] = 96;
				}

				for (x=points[0];x<=points[1];x++) {
					if ((x>jean->x+1) && (x<jean->x+13)) {
						for (y=points[2];y<=points[3];y++) {
							if ((y>jean->y+(jean->ducking*8)) && (y<jean->y+22)) {
								jean->death = 1;
								y=points[3] + 1;
								x=points[1] + 1;
								i= 17;
							}
						}
					}
				}
			}
		}
	}

}
