#include <raylib.h>
#include <raymath.h>

#include "map.h"
#include "util.h"

int bsp_split(bsp_t *cur, map_t *m)
{
	// If the room is too small just don't do anything, or just randomly discard
	if(cur->rect.width * cur->rect.height < 350)
	{
		cur->left = 0;
		cur->right = 0;

		if(m->rooms_count % 16 == 0)
			m->rooms = MemRealloc(m->rooms, (m->rooms_count + 16) * sizeof(Rectangle));

		Vector2 v1 = {GetRandomValue(cur->rect.x + 1, cur->rect.x + cur->rect.width/20),
			GetRandomValue(cur->rect.y + 1, cur->rect.y + cur->rect.height/20)};

		Vector2 v2 = {GetRandomValue(cur->rect.x + cur->rect.width * 19/20, cur->rect.x + cur->rect.width - 1),
			GetRandomValue(cur->rect.y + cur->rect.height * 19/20, cur->rect.y + cur->rect.height - 1)};

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

Vector2 gen_door(Rectangle r, int wall)
{
	float t = GetRandomValue(0, 100)/100.;

	Vector2 door = {0};
	switch(wall)
	{
		case 0:
			door.y = r.y;
			door.x = (r.x + 1) * t + (r.x + r.width - 2) * (1 - t);
			break;

		case 1:
			door.y = r.y + r.height - 1;
			door.x = (r.x + 1) * t + (r.x + r.width - 2) * (1 - t);
			break;

		case 2:
			door.x = r.x;
			door.y = (r.y + 1) * t + (r.y + r.height - 2) * (1 - t);
			break;

		case 3:
			door.x = r.x + r.width - 1;
			door.y = (r.y + 1) * t + (r.y + r.height - 2) * (1 - t);
			break;
	}

	return door;
}

void bsp_clearout(map_t *m)
{
	for(int i = 0; i < m->width * m->height; i++)
		m->tiles[i].type = TILE_FLOOR;

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
			Vector2 door = gen_door(r, k);

			m->tiles[(int)door.y * m->width + (int)door.x].type = TILE_DOOR;
		}
	}

	for(int i = 0; i < m->width; i++)
	{
		m->tiles[i].type = TILE_WALL;
		m->tiles[i + (m->height - 1) * m->width].type = TILE_WALL;
	}

	for(int i = 0; i < m->height; i++)
	{
		m->tiles[i * m->width].type = TILE_WALL;
		m->tiles[i * m->width + m->width - 1].type = TILE_WALL;
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

	bsp_clearout(&m);

	return m;
}

void map_destroy(map_t *m)
{
	MemFree(m->tiles);
}
