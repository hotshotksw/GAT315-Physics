#pragma once
#include "body.h"

extern kwBody* kwBodies;
extern int bodyCount;

kwBody* CreateBody();
void DestroyBody(kwBody* body);