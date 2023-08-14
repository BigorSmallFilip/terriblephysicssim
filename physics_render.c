#include "physics.h"
#include "graphics.h"



static void render_rotate_rect(
	const double x,
	const double y,
	const double a,
	const double w,
	const double h
)
{
	const double w2 = w / 2;
	const double h2 = h / 2;
	const double x0 = cos(-a) * w2 - sin(-a) * h2;
	const double y0 = sin(-a) * w2 + cos(-a) * h2;
	const double x1 = cos(a) * w2 - sin(a) * h2;
	const double y1 = sin(a) * w2 + cos(a) * h2;
	DrawLineF(x - x0, y - y0, x + x1, y - y1);
	DrawLineF(x + x1, y - y1, x + x0, y + y0);
	DrawLineF(x + x0, y + y0, x - x1, y + y1);
	DrawLineF(x - x1, y + y1, x - x0, y - y0);
}



#define CIRCLE_QUADRANT_VERTECIES 16
#define CIRCLE_VERTEX_ANGLE (M_PI / 2.0 / CIRCLE_QUADRANT_VERTECIES)

static void render_rotate_circle(
	const double x,
	const double y,
	const double a,
	const double radius
)
{
	for (int i = 0; i < CIRCLE_QUADRANT_VERTECIES; i++)
	{
		double angle = (double)i * CIRCLE_VERTEX_ANGLE;
		double x0 = cos(angle) * radius;
		double y0 = sin(angle) * radius;
		double x1 = cos(angle + CIRCLE_VERTEX_ANGLE) * radius;
		double y1 = sin(angle + CIRCLE_VERTEX_ANGLE) * radius;
		DrawLineF(x - x0, y - y0, x - x1, y - y1);
		DrawLineF(x + x0, y - y0, x + x1, y - y1);
		DrawLineF(x - x0, y + y0, x - x1, y + y1);
		DrawLineF(x + x0, y + y0, x + x1, y + y1);
	}
	DrawLineF(x, y, x + cos(-a) * radius, y + sin(-a) * radius);
}



void RenderCollider(
	const double x,
	const double y,
	const double a,
	const Collider* collider
)
{
	assert(collider);
	switch (collider->type)
	{
	case COLLIDERTYPE_CIRCLE:
	{
		CircleCollider* circlecol = (CircleCollider*)collider;
		render_rotate_circle(
			x + circlecol->base.offsetx,
			y + circlecol->base.offsety,
			a + circlecol->base.offseta,
			circlecol->radius
		);
	}
		break;

	case COLLIDERTYPE_RECT:
	{
		RectCollider* rectcol = (RectCollider*)collider;
		render_rotate_rect(
			x + rectcol->base.offsetx,
			y + rectcol->base.offsety,
			a + rectcol->base.offseta,
			rectcol->width,
			rectcol->height
		);
	}
		break;

	default:
		break;
	}
}

void RenderRigidbody(const Rigidbody* rigidbody)
{
	SetColorI(0xffffffff);
	SDL_RenderDrawPointF(main_renderer, rigidbody->x, rigidbody->y);
	RenderCollider(rigidbody->x, rigidbody->y, rigidbody->a, rigidbody->colliders.items[0]);
}



static void render_collision_point(
	const double x,
	const double y
)
{
	DrawRect(x - 2, y - 2, 5, 5);
}

static void render_collision_points()
{
	SetColorI(0xff00ffff);
	for (int i = 0; i < collision_manifolds.count; i++)
	{
		Manifold* m = collision_manifolds.items[i];
		if (m->point_count == 0) continue;
		if (m->point_count == 1)
			render_collision_point(m->a_x, m->a_y);
		if (m->point_count == 2)
		{
			render_collision_point(m->a_x, m->a_y);
			render_collision_point(m->b_x, m->b_y);
		}
	}
}



void RenderPhysicsObjects()
{
	for (int i = 0; i < worldRigidbodies.count; i++)
	{
		Rigidbody* rb = worldRigidbodies.items[i];
		RenderRigidbody(rb);
	}

	render_collision_points();
}
