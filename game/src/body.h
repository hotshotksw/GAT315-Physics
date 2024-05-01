#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum 
{
	STATIC,
	KINEMATIC,
	DYNAMIC
} kwBodyType;

typedef struct kwBody
{
	kwBodyType body;
	// force -> acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 force;

	float mass;
	float inverseMass; // 1 / mass (static = 0)

	struct kwBody* next;
	struct kwBody* prev;
} kwBody;

inline void ApplyForce(kwBody* body, Vector2 force)
{
	body->force = Vector2Add(body->force, force);
}

inline void ClearForce(kwBody* body)
{
	body->force = Vector2Zero();
}