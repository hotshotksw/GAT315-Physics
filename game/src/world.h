#pragma once
#include "raylib.h"
#include "body.h"

typedef struct kwBody kwBody;

extern kwBody* kwBodies;
extern int bodyCount;
extern Vector2 kwGravity;

kwBody* CreateBody(Vector2 position, float mass, kwBodyType bodyType);
void AddBody(kwBody* body);
void DestroyBody(kwBody* body);
void DestroyAllBodies();