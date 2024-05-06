#pragma once
#include "raylib.h"

typedef struct kwBody kwBody;

extern kwBody* kwBodies;
extern int bodyCount;
extern Vector2 kwGravity;

kwBody* CreateBody();
void DestroyBody(kwBody* body);
void DestroyAllBodies();