#include <raylib.h>
#include <raymath.h>

#include "map.h"
#include "util.h"

void bsp_split(bsp_t *cur)
{
	// If the room is too small just don't do anything, or just randomly discard
	if(cur->rect.width * cur->rect.height < 50)
	{
		cur->left = 0;
		cur->right = 0;
		return;
	}

	bsp_t *lhs_bsp = MemAlloc(sizeof(bsp_t));
	bsp_t *rhs_bsp = MemAlloc(sizeof(bsp_t));

	Rectangle lhs, rhs;

	float t = GetRandomValue(0, 1) ? 0.45 : 0.55;

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

	bsp_split(lhs_bsp);
	bsp_split(rhs_bsp);
}

// generates the dugeon
void bsp_carveout(tile_t *til, int width, int height, bsp_t *bsp)
{
	if(!bsp)
		return;

	if(bsp->left != 0 && bsp->right != 0)
	{
		bsp_carveout(til, width, height, bsp->left);
		bsp_carveout(til, width, height, bsp->right);
	}
	else
	{
		Rectangle r = bsp->rect;

		for(int i = r.y+1; i < r.y + r.height; i++)
		{
			for(int j = r.x+1; j < r.x + r.width; j++)
			{
				til[j * width + i].type = TILE_FLOOR;
			}
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
	root->rect.x = 0;
	root->rect.y = 0;
	root->rect.width = width;
	root->rect.height = height;

	m.bsp = root;

	bsp_split(root);
	bsp_carveout(m.tiles, width, height, root);

	for(int i = 0; i < width; i++)
		m.tiles[i + width * (height - 1)].type = TILE_WALL;

	for(int i = 0; i < height ; i++)
		m.tiles[i * width + width - 1].type = TILE_WALL;

	return m;
}

void map_destroy(map_t *m)
{
	MemFree(m->tiles);
}
