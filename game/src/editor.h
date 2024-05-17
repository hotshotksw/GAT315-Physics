#pragma once
#include "raylib.h"

typedef struct kwEditorData
{
    Vector2 anchor01;
    Vector2 anchor02;
    Vector2 anchor03;
    Vector2 anchor04;

    bool EditorBoxActive;
    float MassMinValue;
    float MassMaxValue;
    float GravitationValue;
    bool BodyTypeEditMode;
    int BodyTypeActive;
    float BodyGravityValue;
    float DampingValue;
    float StiffnessValue;
    float RestitutionValue;
} kwEditorData_t;

extern kwEditorData_t kwEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 mousePosition);
void DrawEditor(Vector2 position);

struct kwBody* GetBodyIntersect(struct kwBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct kwBody* body, Vector2 position);