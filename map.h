#ifndef _MAP_H_
#define _MAP_H_

typedef enum TileType
{
	TILE_WALL,
	TILE_FLOOR,
	TILE_COUNT
} TileType;

typedef struct tile_t
{
	TileType type;
} tile_t;


// Rectangular bsp tree to simplyfy things lol
typedef struct bsp_t
{
	Rectangle rect;
	struct bsp_t *left;
	struct bsp_t *right;
} bsp_t;

void bsp_split(bsp_t *cur);
void bsp_carveout(tile_t *til, int width, int height, bsp_t *bsp);

typedef struct map_t
{
	int width;
	int height;
	tile_t *tiles;

	bsp_t *bsp;
} map_t;

map_t map_create(int width, int height);
void map_destroy(map_t *m);

#endif
