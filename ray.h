#ifndef _RAY_H_
#define _RAY_H_

#include <raylib.h>
#include <raymath.h>
#include "map.h"

typedef struct ray_hit_t
{
	Vector2 origin;
	Vector2 hitpos;
	float angle;
	float dist;		// distance ray travelled
	
	enum {VERTICAL, HORIZONTAL} which;

	float intrp;		// interpolation value

	tile_t hittile;
	int ix, iy;		// index
} ray_hit_t;

ray_hit_t ray_project(map_t *m, Vector2 pos, float a);

#endif
