#include "player.h"
#include "util.h"
#include "ray.h"

static int issolid(int a)
{
	switch(a)
	{
		case TILE_FLOOR:
		case TILE_ROOM_FLOOR:
		case TILE_DOOR:
			return 0;
		default:
			return 1;
	}
}

void player_update(player_t *p, map_t *m)
{
	float dt = GetFrameTime();

	Vector2 vel = {0, 0};

	if(IsKeyDown(KEY_LEFT))  p->look -= 2 * dt;
	if(IsKeyDown(KEY_RIGHT)) p->look += 2 * dt;

	if(p->look >= 2 * PI) p->look = p->look - 2 * PI;
	if(p->look < 0) p->look = p->look + 2 * PI;

	Vector2 dir = { cos(p->look), sin(p->look) };

	if(IsKeyDown(KEY_UP))
	{
		vel.x = dir.x * p->vel.x;
		vel.y = dir.y * p->vel.y;
	}

	if(IsKeyDown(KEY_DOWN))
	{
		vel.x = -dir.x * p->vel.x;
		vel.y = -dir.y * p->vel.y;
	}

	Vector2 pos = Vector2Add(p->pos, Vector2Scale(vel, dt));

	int tx = clamp(min(p->pos.x, pos.x) - 1, 0, m->width);
	int ty = clamp(min(p->pos.y, pos.y) - 1, 0, m->height);

	int bx = clamp(max(p->pos.x, pos.x) + 1, 0, m->width);
	int by = clamp(max(p->pos.y, pos.y) + 1, 0, m->height);

	for(int x = tx; x <= bx; x++)
	{
		for(int y = ty; y <= by; y++)
		{
			// for now this is fine
			if(!issolid(get_tile(m, x, y).type))
				continue;

			float nx = clamp(pos.x, x, x + 1) - pos.x;
			float ny = clamp(pos.y, y, y + 1) - pos.y;

			float ov = p->size - hypot(nx, ny);

			if(isnan(ov)) ov = 0;

			if(ov > 0)
			{
				pos = Vector2Subtract(pos, Vector2Scale(Vector2Normalize((Vector2){nx, ny}), ov));
			}
		}
	}

	p->pos = pos;
}
