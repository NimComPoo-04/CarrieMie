#include "pathfinder.h"

void heap_recover(queue_t *q, int indx)
{
	if(indx <= 0)
		return;

	int parent = (indx-1)/2;

	if(q->cells[indx].prio < q->cells[parent].prio)
	{
		struct cell_t k = q->cells[indx];
		q->cells[indx] = q->cells[parent];
		q->cells[parent] = k;

		heap_recover(q, parent);
	}
}

void heap_extract(queue_t *q, int parent)
{
	if(parent * 2 >= q->len)
		return;

	int sib1 = parent * 2 + 1;
	int sib2 = parent * 2 + 2;

	if(sib1 < q->len && sib2 < q->len)
	{
		int which = q->cells[sib1].prio < q->cells[sib2].prio ? sib1 : sib2;

		if(q->cells[which].prio < q->cells[parent].prio)
		{
			struct cell_t k = q->cells[parent];
			q->cells[parent] = q->cells[which];
			q->cells[which] = k;

			heap_extract(q, which);
		}
	}
	else if(sib1 < q->len && q->cells[sib1].prio < q->cells[parent].prio)
	{
		struct cell_t k = q->cells[parent];
		q->cells[parent] = q->cells[sib1];
		q->cells[sib1] = k;

		heap_extract(q, sib1);
	}
	else if(sib2 < q->len && q->cells[sib2].prio < q->cells[parent].prio)
	{
		struct cell_t k = q->cells[parent];
		q->cells[parent] = q->cells[sib2];
		q->cells[sib2] = k;

		heap_extract(q, sib2);
	}
}

void queue_insert(queue_t *q, int x, int y, int prio)
{
	struct cell_t c = {x, y, prio};

	// insert to bottom, reassign
	if(q->len >= q->size)
	{
		q->size += 32;
		q->cells = MemRealloc(q->cells, sizeof(struct cell_t) * q->size);
	}
	q->cells[q->len++] = c;

	// heap insert dwag
	heap_recover(q, q->len-1);
}

struct cell_t queue_delete(queue_t *q)
{
	if(q->len <= 0)
		return (struct cell_t){-1, -1, 1000000};

	struct cell_t c = q->cells[0];

	q->cells[0] = q->cells[q->len - 1];
	q->len--;

	heap_extract(q, 0);

	return c;
}

void queue_dump(queue_t *q)
{
	for(int i = 0; i < q->len; i++)
	{
		TraceLog(LOG_INFO, "\t(%d %d %d)", q->cells[i].x, q->cells[i].y, q->cells[i].prio);
	}
}

void path_create(path_t *p)
{
	p->cells = MemAlloc(p->map->width * p->map->height * sizeof(*p->cells));
}

// FIXME: get a proper function for this dwag
int issolid(int a);
int path_compute(path_t *p, int sx, int sy, int ex, int ey)
{
	// reset the whole grid
	for(int i = 0; i < p->map->width * p->map->height; i++)
	{
		p->cells[i].px = -1;
		p->cells[i].py = -1;
	}

	// nuke the queue if its filled eyy
	p->queue.len = 0;

	int depth = 0;
	int huristic = (ex - sx) + (ey + sy);

	// lets do greedy bfs first then we can do a*
	queue_insert(&p->queue, sx, sy, depth);

	while(p->queue.len)
	{
		struct cell_t c = queue_delete(&p->queue);
		if(c.x == ex && c.y == ey)
			return 1;

		for(int i = -1; i <= 1; i++)
		{
			if(c.x + i < 0 || c.x + i >= p->map->width)
				continue;

			int m = get_tile(p->map, c.x + i, c.y).type;

			int indx = c.x + i + c.y * p->map->width;

			// if solid or if already visited gtfo
			if(!issolid(m) && p->cells[indx].px < 0)
			{
				p->cells[indx].px = c.x; 
				p->cells[indx].py = c.y; 

				int huristic = (ex - c.x - i ) + (ey - c.y);
				queue_insert(&p->queue, c.x + i, c.y, depth);
				depth++;
			}
		}

		for(int j = -1; j <= 1; j++)
		{
			if(c.y + j < 0 || c.y + j >= p->map->height)
				continue;

			int m = get_tile(p->map, c.x, c.y + j).type;

			int indx = c.x + (c.y + j) * p->map->width;

			// if solid or if already visited gtfo
			if(!issolid(m) && p->cells[indx].px < 0)
			{
				p->cells[indx].px = c.x; 
				p->cells[indx].py = c.y; 

				int huristic = (ex - c.x) + (ey - c.y - j);
				queue_insert(&p->queue, c.x, c.y + j, depth);
				depth++;
			}
		}
	}

	return 0;
}

void path_destroy(path_t *p)
{
	MemFree(p->cells);
	MemFree(p->queue.cells);
}
