#ifndef _PATHFINDER_H_
#define _PATHFINDER_H_

#include <raylib.h>

#include "map.h"
#include "util.h"

struct cell_t {
	int x;
	int y;
	int prio;
};

typedef struct queue_t
{	
	struct cell_t *cells;
	int len;
	int size;
} queue_t;

void queue_insert(queue_t *q, int x, int y, int prio);
struct cell_t queue_delete(queue_t *q);
void queue_dump(queue_t *q);

typedef struct
{
	map_t *map;
	queue_t queue;
	struct {
		int px;
		int py;
	} *cells;
} path_t;

void path_create(path_t *p);
int path_compute(path_t *p, int sx, int sy, int ex, int ey);
void path_destroy(path_t *p);

#endif
