#ifndef _MAP_H_
#define _MAP_H_

typedef enum TileType
{
	TILE_NONE,
	TILE_FLOOR,
	TILE_WALL,
	TILE_ROOM_FLOOR,
	TILE_ROOM_WALL,
	TILE_DOOR,
	TILE_ENTRY,
	TILE_EXIT,
	TILE_SPAWNER,
	TILE_TERMINAL,
	TILE_ARMORY,		// A aromory tile, recharges the gun of human
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

struct map_t;
int bsp_split(bsp_t *cur, struct map_t *m);
void bsp_clearout(struct map_t *m);

typedef struct map_t
{
	int width;
	int height;
	tile_t *tiles;

	bsp_t *bsp;
	Rectangle *rooms;
	int rooms_count;
} map_t;

map_t map_create(int width, int height);
void map_destroy(map_t *m);

tile_t set_tile(map_t *m, int x, int y, tile_t a);
tile_t get_tile(map_t *m, int x, int y);

#endif
