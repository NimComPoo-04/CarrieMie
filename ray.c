#include "ray.h"
#include "map.h"
#include "player.h"

// FIXME: consolidate this function
int issolid(int a);
ray_hit_t ray_project(map_t *m, Vector2 pos, float a)
{
	ray_hit_t r = {0};
	r.origin = pos;
	r.angle = a;

	float dx = pos.x - (int)pos.x;
	float dy = pos.y - (int)pos.y;

	float dir_x = cos(a);
	float dir_y = sin(a);

	float mv  = dir_y/dir_x;
	float mi = dir_x/dir_y;

	// Vettical
	float stepx = dir_x < 0 ? -1 : 1;
	Vector2 vert = {pos.x, pos.y};

	vert.x += (stepx > 0 ? 1 - dx: -dx-0.00001);
	vert.y += (stepx > 0 ? 1 - dx: -dx-0.00001) * mv;

	int count = 0;
	while(!issolid(get_tile(m, vert.x, vert.y).type) && count < 10)
	{
		vert.x += stepx;
		vert.y += mv * stepx;
		count++;
	}

	float distv = Vector2Distance(vert, pos);

	// Horizontal
	float stepy = dir_y < 0 ? -1 : 1;
	Vector2 horiz = {pos.x, pos.y};

	horiz.x += (stepy > 0 ? 1 - dy: -dy-0.00001) * mi;
	horiz.y += (stepy > 0 ? 1 - dy: -dy-0.00001);

	count = 0;
	while(!issolid(get_tile(m, horiz.x, horiz.y).type) && count < 10)
	{
		horiz.x += mi * stepy;
		horiz.y += stepy;
		count++;
	}

	float disth = Vector2Distance(horiz, pos);

	// do someshit

	if(distv < disth)
	{
		r.hitpos = vert;
		r.hittile = get_tile(m, vert.x, vert.y);
		r.ix = vert.x;
		r.iy = vert.y;

		r.intrp = stepx >= 0 ? vert.y - r.iy : 1 - vert.y + r.iy;

		r.dist = distv;
		r.which = VERTICAL;
	}
	else
	{
		r.hitpos = horiz;
		r.hittile = get_tile(m, horiz.x, horiz.y);
		r.ix = horiz.x;
		r.iy = horiz.y;

		r.intrp = stepy >= 0 ? 1 - horiz.x + r.ix : horiz.x - r.ix;

		r.dist = disth;
		r.which = HORIZONTAL;
	}

	return r;
}
