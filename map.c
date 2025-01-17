#include <raylib.h>
#include <raymath.h>

#include "map.h"
#include "util.h"

int bsp_split(bsp_t *cur, map_t *m)
{
	// If the room is too small just don't do anything, or just randomly discard
	if(cur->rect.width * cur->rect.height < 200)
	{
		cur->left = 0;
		cur->right = 0;

		// Discard 60 % of all rooms
		if(GetRandomValue(0, 100) < 45)
			return 0;

		if(m->rooms_count % 16 == 0)
			m->rooms = MemRealloc(m->rooms, (m->rooms_count + 16) * sizeof(Rectangle));

		Vector2 v1 = {cur->rect.x + 1, cur->rect.y + 1};
		Vector2 v2 = {cur->rect.x + cur->rect.width - 1, cur->rect.y + cur->rect.height - 1};

		m->rooms[m->rooms_count].x = v1.x;
		m->rooms[m->rooms_count].y = v1.y;
		m->rooms[m->rooms_count].width = v2.x - v1.x;
		m->rooms[m->rooms_count].height = v2.y - v1.y;

		m->rooms_count++;

		return 1;
	}

	bsp_t *lhs_bsp = MemAlloc(sizeof(bsp_t));
	bsp_t *rhs_bsp = MemAlloc(sizeof(bsp_t));

	Rectangle lhs, rhs;

	float t = GetRandomValue(40, 60)/100.;

	if(cur->rect.width > cur->rect.height)
	{
		// split horiz
		int lwid = cur->rect.width * t;
		int rwid = cur->rect.width - lwid;

		lhs.x = cur->rect.x;
		lhs.y = cur->rect.y;
		lhs.width = lwid;
		lhs.height = cur->rect.height;

		rhs.x = cur->rect.x + lwid;
		rhs.y = cur->rect.y;
		rhs.width = rwid;
		rhs.height = cur->rect.height;
	}
	else
	{
		// split vertical
		int lhei = cur->rect.height * t;
		int rhei = cur->rect.height - lhei;

		lhs.x = cur->rect.x;
		lhs.y = cur->rect.y;
		lhs.width = cur->rect.width;
		lhs.height = lhei;

		rhs.x = cur->rect.x;
		rhs.y = cur->rect.y + lhei;
		rhs.width = cur->rect.width;
		rhs.height = rhei;
	}

	lhs_bsp->rect = lhs;
	rhs_bsp->rect = rhs;

	cur->left = lhs_bsp;
	cur->right = rhs_bsp;

	int split = 0;

	split += bsp_split(lhs_bsp, m);
	split += bsp_split(rhs_bsp, m);

	return split;
}

Vector2 gen_door(map_t *m, Rectangle r, int wall, Vector2 *v)
{
	Vector2 door = {0};
	do
	{
		float t = GetRandomValue(0, 100)/100.;

		switch(wall)
		{
			case 0:
				door.y = r.y;
				door.x = (r.x + 1) * t + (r.x + r.width - 2) * (1 - t);

				v->y = door.y - 1;
				v->x = door.x;

				break;

			case 1:
				door.y = r.y + r.height - 1;
				door.x = (r.x + 1) * t + (r.x + r.width - 2) * (1 - t);

				v->y = door.y + 1;
				v->x = door.x;

				break;

			case 2:
				door.x = r.x;
				door.y = (r.y + 1) * t + (r.y + r.height - 2) * (1 - t);

				v->y = door.y;
				v->x = door.x - 1;

				break;

			case 3:
				door.x = r.x + r.width - 1;
				door.y = (r.y + 1) * t + (r.y + r.height - 2) * (1 - t);

				v->y = door.y;
				v->x = door.x + 1;

				break;
		}
	}
	while(m->tiles[(int)(v->y * m->width + v->x)].type != TILE_FLOOR);

	return door;
}

static tile_t get_tile(map_t *m, int x, int y)
{
	if(x < 0 || x >= m->width) return (tile_t){.type = TILE_COUNT};
	if(y < 0 || y >= m->height) return (tile_t){.type = TILE_COUNT};

	return m->tiles[y * m->width + x];
}

static tile_t set_tile(map_t *m, int x, int y, tile_t a)
{
	tile_t t = {0};

	if(x < 0 || x >= m->width) return (tile_t){.type = TILE_COUNT};
	if(y < 0 || y >= m->height) return (tile_t){.type = TILE_COUNT};

	t = m->tiles[y * m->width + x];
	m->tiles[y * m->width + x] = a;

	return t;
}

static void shuffle_rooms(Rectangle *rooms, int count)
{
	for(int i = 0; i < count - 1; i++)
	{
		int j = GetRandomValue(i, count-1);
		Rectangle a = rooms[i];
		rooms[i] = rooms[j];
		rooms[j] = a;
	}
}

// Weird depth first search algorithm.
// each path is 1 in width, we have a separate array to check if we visited
// some place or not.
static void depth_first_search(map_t *m)
{
	// Just keep track of the last point in thing
	struct stackdeez {
		int x; int y;
	} *stack = MemAlloc(sizeof(struct stackdeez) * m->width * m->height);
	int top = 0;

	int x = 0;
	int y = 0;

	do
	{
		x = GetRandomValue(0, m->width-1);
		y = GetRandomValue(0, m->height-1);

		stack[top].x = x;
		stack[top].y = y;

	} while(m->tiles[y * m->width + x].type != TILE_NONE);
	top++;

	int steps = 0;
	while(top > 0)
	{
		steps++;

		top--;
		x = stack[top].x;
		y = stack[top].y;

		tile_t flr = {TILE_FLOOR};
		tile_t wall = {TILE_WALL};

		set_tile(m, x, y, flr);

		if(get_tile(m, x + 1, y).type == TILE_NONE)
			set_tile(m, x + 1, y, wall);

		if(get_tile(m, x - 1, y).type == TILE_NONE)
			set_tile(m, x - 1, y, wall);

		if(get_tile(m, x, y + 1).type == TILE_NONE)
			set_tile(m, x, y + 1, wall);

		if(get_tile(m, x, y - 1).type == TILE_NONE)
			set_tile(m, x, y - 1, wall);

		static struct stackdeez deez[] = {
			{-1, 0}, {1, 0}, {0, -1}, {0, 1}
		};
		static int deezlen = sizeof(deez)/sizeof(deez[0]);

		if(steps % GetRandomValue(10, 25) == 0)
		{
			for(int i = 0; i < deezlen; i++)
			{
				struct stackdeez k = deez[i];
				int j = GetRandomValue(i, deezlen-1);
				deez[i] = deez[j];
				deez[j] = k;
			}
		}

		for(int i = 0; i < deezlen; i++)
		{
			tile_t t0 = get_tile(m, x + deez[i].x, y + deez[i].y);
			tile_t t1 = get_tile(m, x + 2 * deez[i].x, y + 2 * deez[i].y);

			if(t0.type == TILE_NONE || (t0.type == TILE_WALL && t1.type == TILE_NONE)
					|| (t0.type == TILE_WALL && t1.type == TILE_ROOM_WALL))
			{
				stack[top].x = x + deez[i].x;
				stack[top].y = y + deez[i].y;

				top++;
			}
		}
	}

	MemFree(stack);
}

void bsp_clearout(map_t *m)
{
	for(int i = 0; i < m->width * m->height; i++)
		m->tiles[i].type = TILE_NONE;

	/*
	drunkyards_walk(m->tiles, m->width, m->height, m->width/2, m->height/2,
			m->width * m->height * 5, TILE_COUNT);
			*/

	for(int i = 0; i < m->rooms_count; i++)
	{
		Rectangle r = m->rooms[i];

		// clear out the central part
		for(int i = r.y; i < r.y+r.height; i++)
		{
			for(int j = r.x; j < r.x+r.width; j++)
			{
				m->tiles[i * m->width + j].type = TILE_ROOM_FLOOR;
			}
		}

		for(int i = r.y; i < r.y+r.height; i++)
		{
			m->tiles[i * m->width + (int)r.x].type = TILE_ROOM_WALL;
			m->tiles[i * m->width + (int)(r.x+r.width-1)].type = TILE_ROOM_WALL;
		}

		for(int i = r.x; i < r.x+r.width; i++)
		{
			m->tiles[i + (int)r.y * m->width].type = TILE_ROOM_WALL;
			m->tiles[i + (int)(r.y+r.height-1) * m->width].type = TILE_ROOM_WALL;
		}
	}

	depth_first_search(m);

	// Generate a few doors on the dungeon
	for(int i = 0; i < m->rooms_count; i++)
	{
		int v = GetRandomValue(0, 99);
		int t = 0;

		if(v < 50) t = 1;
		else if(v < 70) t = 2;
		else if(v < 90) t = 3;
		else if(v < 100) t = 4;

		for(int k = 0; k < t; k++)
		{
			Rectangle r = m->rooms[i];
			Vector2 where = {0};
			Vector2 door = gen_door(m, r, k, &where);

			m->tiles[(int)door.y * m->width + (int)door.x].type = TILE_DOOR;
		}
	}
}

map_t map_create(int width, int height)
{
	map_t m = {0};

	m.width = width;
	m.height = height;
	m.tiles = MemAlloc(sizeof(tile_t) * width * height);

	for(int i = 0; i < width * height; i++)
		m.tiles[i].type = TILE_WALL;

	bsp_t *root = MemAlloc(sizeof(bsp_t));
	root->rect.x = 1;
	root->rect.y = 1;
	root->rect.width = width - 2;
	root->rect.height = height - 2;

	m.bsp = root;

	m.rooms_count = 0;
	m.rooms = 0;
	m.rooms_count = bsp_split(root, &m);

	shuffle_rooms(m.rooms, m.rooms_count);

	bsp_clearout(&m);

	return m;
}

void map_destroy(map_t *m)
{
	MemFree(m->tiles);
}
