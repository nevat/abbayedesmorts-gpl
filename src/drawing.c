/* drawing.c */

# include <stdio.h>
# include <stdlib.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include "SDL2/SDL_mixer.h"
# include "SDL2/SDL_ttf.h"

# include "structs.h"

void drawscreen (SDL_Renderer *renderer,uint stagedata[][22][32],SDL_Texture *tiles,uint room[],uint counter[],uint changeflag,Mix_Chunk *fx[],uint changetiles) {

	int coordx = 0;
	int coordy = 0;
	SDL_Rect srctiles = {0,0,8,8};
	SDL_Rect destiles = {0,0,0,0};
	uint data = 0;

	for (coordy=0; coordy<=21; coordy++) {
		for (coordx=0; coordx<=31; coordx++) {
			data = stagedata[room[0]][coordy][coordx];
			if ((data > 0) && (data != 99)) {
				destiles.x = coordx * 8;
				destiles.y = coordy * 8;
				if (data < 200) {
					srctiles.w = 8;
					srctiles.h = 8;
					if (data < 101) {
						srctiles.y = 0;
						if (data == 84) /* Cross brightness */
							srctiles.x = (data - 1) * 8 + (counter[0]/8 * 8);
						else
							srctiles.x = (data - 1) * 8;
					}
					else {
						if (data == 154) {
							srctiles.x=600 + ((counter[0] / 8) * 16);
							srctiles.y=0;
							srctiles.w=16;
							srctiles.h=24;
						}
						else {
							srctiles.y = 8;
							srctiles.x = (data - 101) * 8;
						}
					}
				}
				if ((data > 199) && (data < 300)) {
					srctiles.x = (data - 201) * 48;
					srctiles.y = 16;
					srctiles.w = 48;
					srctiles.h = 48;
				}
				if ((data > 299) && (data < 399)) {
					srctiles.x = 96 + ((data - 301) * 8);
					srctiles.y = 16;
					srctiles.w = 8;
					srctiles.h = 8;
					/* Door movement */
					if ((room[0] == 7) && ((counter[1] > 59) && (counter[1] < 71))) {
						if ((data == 347) || (data == 348) || (data == 349) || (data == 350)) {
							destiles.x += 2;
							if ((data == 350) && (counter[1] == 70))
								Mix_PlayChannel(-1, fx[3], 0); /* Sound of door */
						}
					}
				}
				/* Hearts */
				if ((data > 399) && (data < 405)) {
					srctiles.x = 96 + ((data - 401) * 8) + (32 * (counter[0] / 15));
					srctiles.y = 24;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				/* Crosses */
				if ((data > 408) && (data < 429)) {
					srctiles.x = 96 + ((data - 401) * 8) + (32 * (counter[1] / 23));
					srctiles.y = 24;
					srctiles.w = 8;
					srctiles.h = 8;
				}

				if ((data > 499) && (data < 599)) {
					srctiles.x = 96 + ((data - 501) * 8);
					srctiles.y = 32;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				if ((data > 599) && (data < 650)) {
					srctiles.x = 96 + ((data - 601) * 8);
					srctiles.y = 56;
					srctiles.w = 8;
					srctiles.h = 8;
				}
				if (data == 650) { /* Cup */
					srctiles.x = 584;
					srctiles.y = 87;
					srctiles.w = 16;
					srctiles.h = 16;
				}
				if ((data == 152) || (data == 137) || (data == 136)) {
					if (changeflag == 0) {
						srctiles.y = srctiles.y + (changetiles * 120);
						SDL_RenderCopy(renderer,tiles,&srctiles,&destiles);
					}
				}
				else {
					srctiles.y = srctiles.y + (changetiles * 120);
					SDL_RenderCopy(renderer,tiles,&srctiles,&destiles);
				}
			}
		}
	}

}

void statusbar (SDL_Renderer *renderer,SDL_Texture *tiles,int room[],int lifes,int crosses,TTF_Font *font,uint changetiles) {

	SDL_Rect srcbar = {448,104,13,12};
	SDL_Rect desbar = {0,177,0,0};
	SDL_Color fgcolor = {255,255,255}; /* Color de la fuente, blanco */
	SDL_Surface *score = NULL;
	SDL_Texture *scoretext = NULL;
	SDL_Rect desscore = {0,0,0,0};
	int i = 0;

	char data[1];
	char screenname[18];
	int width = 0;
	int height = 0;

	/* Show heart and crosses sprites */
	if (changetiles == 1)
		srcbar.y = 224;
	SDL_RenderCopy(renderer,tiles,&srcbar,&desbar);
	srcbar.x = 461;
	srcbar.w = 12;
	desbar.x = 32;
	SDL_RenderCopy(renderer,tiles,&srcbar,&desbar);

	desscore.y = 174;

	for (i=0; i<=2; i++) {
		switch (i) {
			case 0: sprintf(data, "%d", lifes);
							score = TTF_RenderText_Solid(font, data, fgcolor);
							scoretext = SDL_CreateTextureFromSurface(renderer,score);
							desscore.x = 18;
							break;
			case 1: sprintf(data, "%d", crosses);
							score = TTF_RenderText_Solid(font, data, fgcolor);
							scoretext = SDL_CreateTextureFromSurface(renderer,score);
							desscore.x = 50;
							break;
			case 2:
				if (room[0] == 1)
					sprintf (screenname, "A prayer of Hope");
				if (room[0] == 2)
					sprintf (screenname, "Tower of the Bell");
				if (room[0] == 3)
					sprintf (screenname, "Wine supplies");
				if (room[0] == 5)
					sprintf (screenname, "Escape !!!");
				if (room[0] == 6)
					sprintf (screenname, "Death is close");
				if (room[0] == 7)
					sprintf (screenname, "Abandoned church");
				if (room[0] == 8)
					sprintf (screenname, "The Altar");
				if (room[0] == 9)
					sprintf (screenname, "Hangman tree");
				if (room[0] == 10)
					sprintf (screenname, "Pestilent Beast");
				if (room[0] == 11)
					sprintf (screenname, "Cave of illusions");
				if (room[0] == 12)
					sprintf (screenname, "Plagued ruins");
				if (room[0] == 13)
					sprintf (screenname, "Catacombs");
				if (room[0] == 14)
					sprintf (screenname, "Hidden garden");
				if (room[0] == 15)
					sprintf (screenname, "Gloomy tunels");
				if (room[0] == 16)
					sprintf (screenname, "Lake of despair");
				if (room[0] == 17)
					sprintf (screenname, "The wheel of faith");
				if (room[0] == 18)
					sprintf (screenname, "Banquet of Death");
				if (room[0] == 19)
					sprintf (screenname, "Underground river");
				if (room[0] == 20)
					sprintf (screenname, "Unexpected gate");
				if (room[0] == 21)
					sprintf (screenname, "Evil church");
				if (room[0] == 22)
					sprintf (screenname, "Tortured souls");
				if (room[0] == 23)
					sprintf (screenname, "Ashes to ashes");
				if (room[0] == 24)
					sprintf (screenname, "Satan !!!");

				score = TTF_RenderText_Solid(font, screenname, fgcolor);
				scoretext = SDL_CreateTextureFromSurface(renderer,score);
				TTF_SizeText(font, screenname, &width, &height);
				desscore.x = 256 - width;
				break;
		}

		SDL_RenderCopy(renderer,scoretext,NULL,&desscore);
		SDL_DestroyTexture(scoretext);
		SDL_FreeSurface(score);

	}

}

void drawrope (struct enem enemies,SDL_Renderer *renderer,SDL_Texture *tiles,uint changetiles) {

	int i = 0;
	int blocks = 0;
	int j = 0;
	SDL_Rect srctile = {424,8,16,8};
	SDL_Rect destile = {0,0,16,8};

	for (i=2; i<6; i++) {
		blocks = (enemies.y[i] - (enemies.limleft[i] - 8)) / 8;
		for (j=0; j<=blocks; j++) {
			srctile.y = 8 + (changetiles * 120);
	  	destile.x = enemies.x[i];
	  	destile.y = (enemies.limleft[i] - 8) + (8 * j);
			SDL_RenderCopy(renderer,tiles,&srctile,&destile);
		}
	}

}

void drawshoots (float proyec[],SDL_Texture *tiles,SDL_Renderer *renderer,struct enem *enemies,uint changetiles) {
/* Shoots from skeletons & gargoyles */

	SDL_Rect srctile = {656,24,16,8};
	SDL_Rect destile = {0,0,0,0};
	int i = 0;
	int n = 0;

	srctile.y = 24 + (changetiles * 120);

  for (n=0; n<=4; n+=2) {
		if (proyec[n] > 0) {
	  	i = proyec[n+1];
	  	if (enemies->type[i] == 15) {
				srctile.h = 16;
				srctile.x = 640 - (16 * enemies->direction[i]);
	  	}

	  	/* Move shoot */
	  	if (enemies->direction[i] == 1) {
				if (proyec[n] > enemies->limleft[i])
				  proyec[n] -= 2.5;
				else {
				  enemies->fire[i] = 0;
				  enemies->speed[i] = 0;
				  proyec[n] = 0;
				}
	  	}
	  	else {
				if (proyec[n] < enemies->limright[i])
		  		proyec[n] += 2.5;
				else {
		  		enemies->fire[i] = 0;
				  enemies->speed[i] = 0;
				  proyec[n] = 0;
				}
	  	}

	  	/* Draw shoot */
	  	switch (enemies->direction[i]) {
				case 0: if ((proyec[n] < (enemies->limright[i] - 8)) && (proyec[n] != 0)) {
								  destile.x = proyec[n];
								  destile.y = enemies->y[i] + 8;
									SDL_RenderCopy(renderer,tiles,&srctile,&destile);
								}
								break;
				case 1: if (proyec[n] > (enemies->limleft[i] + 8)) {
								  destile.x = proyec[n];
								  destile.y = enemies->y[i] + 8;
									SDL_RenderCopy(renderer,tiles,&srctile,&destile);
								}
								break;
	  	}
		}

	}

}

void showparchment (SDL_Renderer *renderer,uint *parchment,SDL_Texture *yparchment) {

	TTF_Font *font = TTF_OpenFont("../fonts/VeniceClassic.ttf",18);
	SDL_Surface *text = NULL;
	SDL_Texture *textsurf = NULL;
	SDL_Color fgcolor = {0,0,0};
	SDL_Rect destext = {0,0,0,0};
	int height = 0;
	int width = 0;
	char line1[18];
	char line2[20];

	SDL_RenderCopy(renderer,yparchment,NULL,NULL);

	switch (*parchment) {
		case 3: sprintf (line1, "Twelve crosses");
						sprintf (line2, "against the devil");
						break;
		case 8:	sprintf (line1, "Twelve brothers");
						sprintf (line2, "hid and died here");
						break;
		case 12: sprintf (line1, "Four brothers");
						 sprintf (line2, "changed their faith");
						 break;
		case 14: sprintf (line1, "An invisible path");
						 sprintf (line2, "over a wood bridge");
						 break;
		case 16: sprintf (line1, "Jump to death");
						 sprintf (line2, "and prove your faith");
						 break;
		case 21: sprintf (line1, "Glide through");
						 sprintf (line2, "the beast cage");
						 break;
	}

	text = TTF_RenderText_Blended(font, line1, fgcolor);
	textsurf = SDL_CreateTextureFromSurface(renderer,text);
	TTF_SizeText(font, line1, &width, &height);
	destext.x = 127 - (width / 2);
	destext.y = 81 - height;
	SDL_RenderCopy(renderer,textsurf,NULL,&destext);
	text= TTF_RenderText_Blended(font, line2, fgcolor);
	textsurf = SDL_CreateTextureFromSurface(renderer,text);
	TTF_SizeText(font, line2, &width, &height);
	destext.x = 127 - (width / 2);
	destext.y = 85;
	SDL_RenderCopy(renderer,textsurf,NULL,&destext);
	SDL_DestroyTexture(textsurf);
	TTF_CloseFont(font);
	SDL_FreeSurface(text);

}

void redparchment (SDL_Renderer *renderer,struct hero *jean) {

	SDL_Texture *rparchment = IMG_LoadTexture(renderer,"../graphics/redparch.png");
	SDL_RenderCopy(renderer,rparchment,NULL,NULL);
	SDL_DestroyTexture(rparchment);

	jean->flags[6] = 4;

}

void blueparchment (SDL_Renderer *renderer,struct hero *jean) {

	SDL_Texture *bparchment = IMG_LoadTexture(renderer,"../graphics/blueparch.png");
	SDL_RenderCopy(renderer,bparchment,NULL,NULL);
	SDL_DestroyTexture(bparchment);

}