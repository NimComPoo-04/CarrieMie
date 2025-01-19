#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <raylib.h>
#include <raymath.h>

#include "map.h"

typedef struct
{
	Vector2 pos;	// position of the thing in the grid
	Vector2 vel;	// velocity of the thing,
	float look;	// look direction

	float eye;	// eye height
	float size;	// something idk < 1
} player_t;

void player_update(player_t *p, map_t *m);

#endif
