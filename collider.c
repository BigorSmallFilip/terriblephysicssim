#include "collider.h"



Collider* CreateCollider(const ColliderType type)
{
	Collider* col = NULL;
	switch (type)
	{
	case COLLIDERTYPE_CIRCLE:
		col = calloc(sizeof(CircleCollider), 1);
		CircleCollider* circlecol = (CircleCollider*)col;
		circlecol->radius = 40;
		break;

	case COLLIDERTYPE_RECT:
		col = calloc(sizeof(RectCollider), 1);
		RectCollider* rectcol = (RectCollider*)col;
		rectcol->width = 40;
		rectcol->height = 30;
		break;

	default:
		break;
	}
	if (!col) return NULL;
	col->type = type;
	return col;
}

void DestroyCollider(Collider* collider)
{
	if (!collider) return;
	free(collider);
}



static Manifold* test_collision_rect_rect(
	const double a_x,
	const double a_y,
	const double a_a,
	const Collider* a_collider,
	const double b_x,
	const double b_y,
	const double b_a,
	const Collider* b_collider
)
{
	return NULL;
}



static Manifold* test_collision_circle_rect(
	const double a_x,
	const double a_y,
	const double a_a,
	const CircleCollider* a_collider,
	const double b_x,
	const double b_y,
	const double b_a,
	const RectCollider* b_collider
)
{


	return NULL;
}



static Manifold* test_collision_circle_circle(
	const double a_x,
	const double a_y,
	const double a_a,
	const CircleCollider* a_collider,
	const double b_x,
	const double b_y,
	const double b_a,
	const CircleCollider* b_collider
)
{
	double dx = b_x - a_x;
	double dy = b_y - a_y;
	double rsq = dx * dx + dy * dy;
	double radius = a_collider->radius + b_collider->radius;
	if (rsq > radius * radius) return NULL;
	double dl = sqrt(rsq);
	double nx = dx / dl;
	double ny = dy / dl;

	Manifold* manifold = malloc(sizeof(Manifold));
	manifold->normal_x = nx;
	manifold->normal_y = ny;
	manifold->depth = radius - dl;
	manifold->a_x = a_x + nx * a_collider->radius;
	manifold->a_y = a_y + ny * a_collider->radius;
	manifold->point_count = 1;
	return manifold;
}



Manifold* TestCollision(
	const double a_x,
	const double a_y,
	const double a_a,
	const Collider* a_collider,
	const double b_x,
	const double b_y,
	const double b_a,
	const Collider* b_collider
)
{
	switch (a_collider->type)
	{
	case COLLIDERTYPE_CIRCLE:
		switch (b_collider->type)
		{
		case COLLIDERTYPE_CIRCLE: 
			return test_collision_circle_circle(
				a_x, a_y, a_a, a_collider,
				b_x, b_y, b_a, b_collider);

		default:
			break;
		}
		break;

	case COLLIDERTYPE_RECT:
		switch (b_collider->type)
		{
		case COLLIDERTYPE_RECT:
			return test_collision_rect_rect(
				a_x, a_y, a_a, a_collider,
				b_x, b_y, b_a, b_collider);

		default:
			break;
		}
		break;

	default:
		break;
	}
	return NULL;
}