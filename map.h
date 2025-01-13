#ifndef _MAP_H_
#define _MAP_H_

typedef enum TileType
{
	TILE_FLOOR,
	TILE_WALL,
	TILE_ROOM_FLOOR,
	TILE_ROOM_WALL,
	TILE_DOOR,
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

#endif
