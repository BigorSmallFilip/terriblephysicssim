#pragma once

#include "utility.h"

#include "collider.h"



typedef struct
{
	double x, y, a;
	double velx, vely, vela;
	double mass;
	double invmass;
	
	double inertia;
	double restitution;
	Bool is_static;
	Vector colliders;
} Rigidbody;

extern Vector worldRigidbodies;

extern double gravity;

extern Vector collision_manifolds;

Bool InitPhysics();

Rigidbody* CreateRigidbody();
void DestroyRigidbody(Rigidbody* rb);

Rigidbody* CreateBall(
	const double x,
	const double y,
	const double r
);

void ApplyForce(
	Rigidbody* rb,
	const double rx,
	const double ry,
	const double fx,
	const double fy,
	const double dt
);

void TestRigidbodyCollisions(
	Rigidbody* a,
	Rigidbody* b
);




void UpdatePhysics(double dt);



void RenderCollider(
	const double x,
	const double y,
	const double a,
	const Collider* collider
);

void RenderRigidbody(const Rigidbody* rb);

void RenderPhysicsObjects();
