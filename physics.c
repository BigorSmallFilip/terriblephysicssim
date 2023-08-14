#include "physics.h"

#include "graphics.h"

Vector worldRigidbodies = { 0 };

double gravity = 200.0;

Vector collision_manifolds = { 0 };



static void clear_collision_manifolds()
{
	if (collision_manifolds.count == 0) return;

	for (int i = 0; i < collision_manifolds.count; i++)
	{
		free(collision_manifolds.items[i]);
	}
	free(collision_manifolds.items);
	collision_manifolds.items = NULL;
	collision_manifolds.count = 0;
	InitVector(&collision_manifolds, 4);
}



#define WALL_RADIUS 10000

static Rigidbody* create_wall()
{
	Rigidbody* rb = calloc(1, sizeof(Rigidbody));
	InitVector(&rb->colliders, 1);
	CircleCollider* col = CreateCollider(COLLIDERTYPE_CIRCLE);
	col->radius = WALL_RADIUS;
	PushBackVector(&rb->colliders, col);
	rb->mass = INFINITY;
	rb->invmass = 0;
	rb->is_static = TRUE;
	return rb;
}

static void add_walls()
{
	Rigidbody* rbt = create_wall();
	rbt->x = screen_width >> 1;
	rbt->y = -WALL_RADIUS + 10;
	PushBackVector(&worldRigidbodies, rbt);

	Rigidbody* rbb = create_wall();
	rbb->x = screen_width >> 1;
	rbb->y = screen_height + WALL_RADIUS - 10;
	PushBackVector(&worldRigidbodies, rbb);

	Rigidbody* rbl = create_wall();
	rbl->x = -WALL_RADIUS + 10;
	rbl->y = screen_height >> 1;
	PushBackVector(&worldRigidbodies, rbl);

	Rigidbody* rbr = create_wall();
	rbr->x = screen_width + WALL_RADIUS - 10;
	rbr->y = screen_height >> 1;
	PushBackVector(&worldRigidbodies, rbr);
}



Bool InitPhysics()
{
	if (!InitVector(&collision_manifolds, 4)) return FALSE;
	if (!InitVector(&worldRigidbodies, 4)) return FALSE;

	add_walls();
	
	for (int i = 0; i < 500; i++)
	{
		Rigidbody* ball = CreateBall(
			RandomRange(20, screen_width - 20),
			RandomRange(20, screen_height - 20),
			RandomRange(4, 12));
		ball->restitution = 0.8;
	}
	return TRUE;
}



Rigidbody* CreateRigidbody()
{
	Rigidbody* rb = NULL;
	Collider* col = NULL;
	rb = calloc(sizeof(Rigidbody), 1);
	if (!rb) return NULL;
	if (!InitVector(&rb->colliders, 4)) goto on_error;
	col = CreateCollider(COLLIDERTYPE_CIRCLE);
	if (!col) goto on_error;
	PushBackVector(&rb->colliders, col);
	return rb;
	
on_error:
	DestroyCollider(col);
	DestroyRigidbody(rb);
	return NULL;
}

void DestroyRigidbody(Rigidbody* rb)
{
	if (!rb) return;
	free(rb->colliders.items);
	free(rb);
}



#define BALL_DENSITY 0.005

Rigidbody* CreateBall(
	const double x,
	const double y,
	const double r
)
{
	Rigidbody* rb = NULL;
	CircleCollider* col = NULL;
	rb = calloc(sizeof(Rigidbody), 1);
	if (!rb) return NULL;
	if (!InitVector(&rb->colliders, 1)) goto on_error;
	col = CreateCollider(COLLIDERTYPE_CIRCLE);
	col->radius = r;
	if (!col) goto on_error;
	PushBackVector(&rb->colliders, col);
	
	rb->x = x;
	rb->y = y;
	rb->mass = M_PI * r * r * BALL_DENSITY;
	rb->invmass = 1.0 / rb->mass;
	PushBackVector(&worldRigidbodies, rb);
	return rb;

on_error:
	DestroyCollider(col);
	DestroyRigidbody(rb);
	return NULL;
}



void ApplyForce(
	Rigidbody* rb,
	const double rx,
	const double ry,
	const double fx,
	const double fy,
	const double dt
)
{
	//SetColorI(0xff0000ff);
	//DrawLineF(rb->x + rx, rb->y + ry, rb->x + rx + fx, rb->y + ry + fy);
	rb->velx += (fx * rb->invmass) * dt;
	rb->vely += (fy * rb->invmass) * dt;
	//rb->vela += (CrossProduct(rx, ry, fx, fy) / rb->inertia) * dt;
}



void TestRigidbodyCollisions(
	Rigidbody* a,
	Rigidbody* b
)
{
	assert(a);
	assert(b);
	assert(a != b);
	for (int i = 0; i < a->colliders.count; i++)
	{
		Collider* a_collider = a->colliders.items[i];
		double a_x = a->x + a_collider->offsetx;
		double a_y = a->y + a_collider->offsety;
		double a_a = a->a + a_collider->offseta;

		for (int j = 0; j < b->colliders.count; j++)
		{
			Collider* b_collider = b->colliders.items[i];
			double b_x = b->x + b_collider->offsetx;
			double b_y = b->y + b_collider->offsety;
			double b_a = b->a + b_collider->offseta;

			Manifold* manifold = TestCollision(
				a_x, a_y, a_a, a_collider,
				b_x, b_y, b_a, b_collider);
			if (!manifold) continue;

			manifold->body_a = a;
			manifold->body_b = b;

			PushBackVector(&collision_manifolds, manifold);
		}
	}
}



static void move_rigidbody(
	Rigidbody* rb,
	const double dt
)
{
	rb->x += rb->velx * dt;
	rb->y += rb->vely * dt;
	rb->a += rb->vela * dt;
}



static void move_rigidbodies(double dt)
{
	for (int i = 0; i < worldRigidbodies.count; i++)
	{
		Rigidbody* rb = worldRigidbodies.items[i];
		if (!rb->is_static)
			rb->vely += gravity * dt;
		move_rigidbody(rb, dt);
	}
}



static void find_collisions()
{
	clear_collision_manifolds();

	for (int i = 0; i < worldRigidbodies.count; i++)
	{
		Rigidbody* a = worldRigidbodies.items[i];
		if (a->colliders.count == 0) continue; // Don't collide without colliders

		for (int j = 0;; j++)
		{
			if (i == j) break;
			Rigidbody* b = worldRigidbodies.items[j];

			if (a->is_static && b->is_static) continue;
			if (b->colliders.count == 0) continue; // Don't collide without colliders

			TestRigidbodyCollisions(a, b);
		}
	}
}





static void resolve_collision_no_rotation(
	const Manifold* manifold
)
{
	Rigidbody* body_a = manifold->body_a;
	Rigidbody* body_b = manifold->body_b;
	double nx = manifold->normal_x;
	double ny = manifold->normal_y;
	double depth = manifold->depth;

	double dvx = body_b->velx - body_a->velx;
	double dvy = body_b->vely - body_a->vely;

	double dot_dv_n = DotProduct(dvx, dvy, nx, ny);
	if (dot_dv_n > 0.0) return;

	double e = min(body_a->restitution, body_b->restitution);

	double j = -(1.0 + e) * dot_dv_n;
	j /= body_a->invmass + body_b->invmass;

	double impulsex = j * nx;
	double impulsey = j * ny;

	body_a->velx -= impulsex * body_a->invmass;
	body_a->vely -= impulsey * body_a->invmass;
	body_b->velx += impulsex * body_b->invmass;
	body_b->vely += impulsey * body_b->invmass;
}



static void resolve_collision(
	const Manifold* manifold
)
{
	Rigidbody* body_a = manifold->body_a;
	Rigidbody* body_b = manifold->body_b;
	double nx = manifold->normal_x;
	double ny = manifold->normal_y;

	if (body_a->is_static)
	{
		body_b->x += nx * manifold->depth;
		body_b->y += ny * manifold->depth;
	} else if (body_b->is_static)
	{
		body_a->x -= nx * manifold->depth;
		body_a->y -= ny * manifold->depth;
	} else
	{
		body_a->x -= nx * manifold->depth / 2.0;
		body_a->y -= ny * manifold->depth / 2.0;
		body_b->x += nx * manifold->depth / 2.0;
		body_b->y += ny * manifold->depth / 2.0;
	}

	int contact_count = manifold->point_count;

	double e = min(body_a->restitution, body_b->restitution);

	static double l_contactx[2]; l_contactx[0] = manifold->a_x; l_contactx[1] = manifold->b_x;
	static double l_contacty[2]; l_contacty[0] = manifold->a_y; l_contacty[1] = manifold->b_y;

	static double l_impulsex[2]; l_impulsex[0] = 0; l_impulsex[1] = 0;
	static double l_impulsey[2]; l_impulsey[0] = 0; l_impulsey[1] = 0;
	static double l_rax[2]; l_rax[0] = 0; l_rax[1] = 0;
	static double l_ray[2]; l_ray[0] = 0; l_ray[1] = 0;
	static double l_rbx[2]; l_rbx[0] = 0; l_rbx[1] = 0;
	static double l_rby[2]; l_rby[0] = 0; l_rby[1] = 0;

	for (int i = 0; i < contact_count; i++)
	{
		double rax = l_contactx[i] - body_a->x;
		double ray = l_contacty[i] - body_a->y;
		l_rax[i] = rax;
		l_ray[i] = ray;
		double rbx = l_contactx[i] - body_b->x;
		double rby = l_contacty[i] - body_b->y;
		l_rbx[i] = rbx;
		l_rby[i] = rby;

		double alin_vel_a_x = -ray * body_a->vela;
		double alin_vel_a_y = rax * body_a->vela;
		double alin_vel_b_x = -rby * body_b->vela;
		double alin_vel_b_y = rbx * body_b->vela;

		double relative_vel_x = (body_b->velx + alin_vel_b_x) - (body_a->velx + alin_vel_a_x);
		double relative_vel_y = (body_b->vely + alin_vel_b_y) - (body_a->vely + alin_vel_a_y);
		
		double contact_vel_mag = DotProduct(relative_vel_x, relative_vel_y, nx, ny);

		if (contact_vel_mag > 0) continue;

		double ra_perp_dot_n = DotProduct(-ray, rax, nx, ny);
		double rb_perp_dot_n = DotProduct(-rby, rbx, nx, ny);

		double denom = body_a->invmass + body_b->invmass +
			(ra_perp_dot_n * ra_perp_dot_n) * body_a->inertia +
			(rb_perp_dot_n * rb_perp_dot_n) * body_b->inertia;

		double j = -(1.0 + e) * contact_vel_mag;
		j /= denom;
		j /= (double)contact_count;

		l_impulsex[i] = j * nx;
		l_impulsey[i] = j * ny;
	}

	for (int i = 0; i < contact_count; i++)
	{
		double impulsex = l_impulsex[i];
		double impulsey = l_impulsey[i];
		
		body_a->velx -= impulsex * body_a->invmass;
		body_a->vely -= impulsey * body_a->invmass;
		body_a->vela -= CrossProduct(l_rax[i], l_ray[i], impulsex, impulsey) * body_a->inertia;
		body_b->velx += impulsex * body_b->invmass;
		body_b->vely += impulsey * body_b->invmass;
		body_b->vela += CrossProduct(l_rbx[i], l_rby[i], impulsex, impulsey) * body_b->inertia;
	}
}



static void resolve_collisions(double dt)
{
	for (int i = 0; i < collision_manifolds.count; i++)
	{
		resolve_collision(collision_manifolds.items[i]);
	}
}

void UpdatePhysics(double dt)
{
	move_rigidbodies(dt);
	find_collisions();
	resolve_collisions(dt);
}

