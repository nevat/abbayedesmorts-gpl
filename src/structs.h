/* structs.h */

#pragma once

/* Structs */
struct enem {
	int type[7];
	float x[7];
	float y[7];
	int direction[7];
	int tilex[7];
	int tiley[7];
	int animation[7];
	int limleft[7];
	int limright[7];
	int speed[7];
	int fire[7];
	int adjustx1[7];
	int adjustx2[7];
	int adjusty1[7];
	int adjusty2[7];
};

struct hero {
	float x;
	float y;
	int direction;
	int jump; /* 1-Up, 2-Down */
	float height; /* Limit of jump */
	int animation;
	float gravity;
	int points[8]; /* Points of collision */
	int ground; /* Pixel where is ground */
	int collision[4]; /* Collisions, in 4 directions */
	int ducking;
	int checkpoint[4];
	int state[2]; /* Vidas y cruces */
	int flags[7];
	int death;
	int push[4]; /* Pulsaciones de teclas */
	int temp;
};
