#include "player.h"
#include "ray.h"

void render_player_view(player_t *p, map_t *m)
{
	int width = GetScreenWidth();
	float ch = 1. * GetScreenHeight() / m->height;

	float l = p->look > PI ? p->look - 2 * PI : p->look;

	for(int i = 0; i < width; i += 50)
	{
		float angle = atan2(i - width/2, width / 2) + l;

		ray_hit_t r = ray_project(m, p->pos, angle);

		Vector2 v1 = Vector2Scale(p->pos, ch);
		Vector2 v2 = Vector2Scale(r.hitpos, ch);

		DrawLineV(v1, v2, RED);
	}
}
