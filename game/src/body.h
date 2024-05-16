#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum 
{
	BT_DYNAMIC,
	BT_STATIC,
	BT_KINEMATIC
} kwBodyType;

typedef enum
{
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
} kwForceMode;

typedef struct kwBody
{
	kwBodyType type;
	// acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	float mass;
	float inverseMass; // 1 / mass (static = 0)
	float gravityScale;
	float damping;

	float restitution;

	Color color;

	struct kwBody* next;
	struct kwBody* prev;
} kwBody;

inline void ApplyForce(kwBody* body, Vector2 force, kwForceMode mode)
{
	if (body->type != BT_DYNAMIC) return;

	switch (mode)
	{
	case FM_FORCE:
		body->force = Vector2Add(body->force, force);
		break;
	case FM_IMPULSE:
		// applies a sudden change in momentum
		body->velocity = Vector2Add(body->velocity, Vector2Scale(force, body->inverseMass));
		break;
	case FM_VELOCITY:
		body->velocity = force;
		break;
	}
}

inline void ClearForce(kwBody* body)
{
	body->force = Vector2Zero();
}

void Step(kwBody* body, float timestep);