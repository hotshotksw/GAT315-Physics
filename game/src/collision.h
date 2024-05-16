#pragma once
#include "raylib.h"

typedef struct ncContact ncContact_t;
typedef struct kwBody kwBody;

void CreateContacts(kwBody* bodies, ncContact_t** contacts);
ncContact_t* GenerateContact(kwBody* body1, kwBody* body2);

void SeparateContacts(ncContact_t* contacts);
void ResolveContacts(ncContact_t* contacts);