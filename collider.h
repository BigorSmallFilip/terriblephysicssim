#pragma once

#include "utility.h"



typedef enum
{
	COLLIDERTYPE_CIRCLE,
	COLLIDERTYPE_RECT,
} ColliderType;

typedef struct
{
	double offsetx, offsety, offseta;
	ColliderType type;
} Collider;



typedef struct
{
	Collider base;
	double width;
	double height;
} RectCollider;

typedef struct
{
	Collider base;
	double radius;
} CircleCollider;



Collider* CreateCollider(const ColliderType type);

void DestroyCollider(Collider* collider);



typedef struct
{
	struct Rigidbody* body_a;
	struct Rigidbody* body_b;
	double normal_x, normal_y;
	double depth;

	double a_x, a_y;
	double b_x, b_y;
	int point_count;
} Manifold;

Manifold* TestCollision(
	const double a_x,
	const double a_y,
	const double a_a,
	const Collider* a_collider,
	const double b_x,
	const double b_y,
	const double b_a,
	const Collider* b_collider
);
