/* jean.c */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_mixer.h>
#include "struct.h"

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

void drawjean (SDL_Renderer *renderer,SDL_Textures *tiles,struct hero *jean,int counter[],Mix_Chunk *fx[],uint changetiles) {

	SDL_Rect srctile = {320,88,16,24};
	SDL_Rect destile = {0,0,16,24};
	SDL_Rect srcducktile = {448,88,18,13};
	SDL_Rect desducktile = {0,0,18,13};
	int r = 0;

	if (jean->deatch == 0) {
		if (jean->jump > 0) {
			r = 1;
			jean->animation = 0;
		}

 		if (jean->ducking == 0) {
			srctile.x += (64 * jean->direction) + ((jean->animation / 7) * 16) + (r * 32);
		 	destile.y = jean->y;
			destile.x = jean->x;
			if (jean->y > 152)
				srctile.h = (176 - jean->y);
			if (changetiles == 1)
				srctile.y = 208;
			SDL_RenderCopy(renderer,tiles,&srctile,&destile);
		}
		else {
  		srcducktile.x += (jean->direction * 36) + ((jean->animation / 7) * 18);
  		desducktile.y = jean->y + 11;
			desducktile.x = jean->x;
			if (changetiles == 1)
				srcducktile.y = 208;
			SDL_RenderCopy(renderer,tiles,&srcducktile,desducktile);
		}
	}

	/* Death animation */
	if (jean->death > 0) {
		jean->death += 1;
		destile.x = jean->x;
		destile.y = jean->y;
		Mix_PauseMusic ();
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

	/* Animation hab. 24 */
	if ((jean->flags[6] == 5) && (counter[1] == 45)) {
		switch (jean->direction) {
			case 0: jean->direction = 1;
							break;
			case 1: jean->direction = 0;
							break;
		}
	}

}

void collisions (struct hero *jean,uint stagedata[][22][32],int room[]) {

	int blleft = 0;
	int blright = 0;
	int blground[4] = {0,0,0,0};
	int blroof[2] = {0,0};
	int points[8] = {0,0,0,0,0,0,0,0};
	int n = 0;
	int pixel = 0;
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
				blleft = stagedata[room[0]][points[n]][points[0] - 1];
				blright = stagedata[room[0]][points[n]][points[3] + 1];
				if (((blleft > 0) && (blleft < 100) && (blleft != 16) && (blleft != 38) && (blleft != 37)) || ((stagedata[room[0]][points[4]][points[0]] == 128) || (blleft == 348))) {
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
			blleft = stagedata[room[0]][r][points[0] - 1];
			blright = stagedata[room[0]][r][points[3] + 1];
			if (((blleft > 0) && (blleft < 100) && (blleft != 37)) || ((stagedata[room[0]][r][points[0]] == 128) || ((blleft > 346) && (blleft < 351)))) {
				if (jean->x - ((points[0] - 1) * 8 + 7) < 1.1)
					jean->collision[2] = 1;
			}
			if (((blright > 0) && (blright < 100) && (blright != 37)) || ((blright > 342) && (blright< 347))) {
				if (((points[3] + 1) * 8) - (jean->x + 14) < 1.1)
					jean->collision[3] = 1;
			}
		}
		/* Invisible wall */
		if ((room[0] == 11) && (r == 5)) {
			if ((points[0] - 1 == 0) || (points[0] - 1 == 1))
				jean->collision[2] = 0;
			if ((points[3] + 1 == 0) || (points[3] + 1 == 1))
				jean->collision[3] = 0;
		}
		if ((room[0] == 10) && (r == 5)) {
			if ((points[0] - 1 > 27) && (points[0] - 1 < 32))
				jean->collision[2] = 0;
			if ((points[3] + 1 > 27) && (points[3] + 1 < 32))
				jean->collision[3] = 0;
		}
	}

	/* Touch ground collision */
	blground[0] = stagedata[room[0]][points[7]+1][points[0]];
	blground[1] = stagedata[room[0]][points[7]+1][points[1]];
	blground[2] = stagedata[room[0]][points[7]+1][points[2]];
	blground[3] = stagedata[room[0]][points[7]+1][points[3]];

	if ((jean->jump != 1) && (jean->ducking == 0)) {
		/* Invisible ground */
		if (((room[0] == 11) && (points[7]+1 > 19) && (points[0] == 2)) || ((room[0] == 16) && ((jean->y / 8) < 4) && (points[0] == 2))) {
			jean->y += jean->gravity;
			jean->jump = 2;
		}
		else {
			if (((blground[0] > 0) && (blground[0] < 100)) || ((blground[1] > 0) && (blground[1] < 100)) || ((blground[2] > 0) && (blground[2] < 100)) || ((blground[3] > 0) && (blground[3] < 100))) {
				jean->ground = (points[7] + 1) * 8;
				if (points[7] + 1 > 21) /* Chapuza para que Jean caiga por debajo de pantalla */
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
	blroof[0] = stagedata[room[0]][points[4]-1][points[0]];
	blroof[1] = stagedata[room[0]][points[4]-1][points[3]];

	if ((jean->jump == 1) && (points[4] > 0)) {
		if (((blroof[0] > 0) && (blroof[0] < 100) && (blroof[0] != 16) && (blroof[0] != 38) && (blroof[0] != 37)) || ((blroof[1] > 0) && (blroof[1] < 100) && (blroof[1] != 16) && (blroof[1] != 38) && (blroof[1] != 37))) {
			if ((jean->y - 1) - ((points[4] - 1) * 8 + 7) < 1)
				jean->collision[0] = 1;
		}
	}

}
