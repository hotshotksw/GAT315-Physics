#include "editor.h"
#include "body.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
kwEditorData_t kwEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 925, 48 };
Texture2D cursorTexture;

bool EditorBoxActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

    Image image = LoadImage("resources/reticle 1.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    HideCursor();

    kwEditorData.anchor01 = (Vector2){ 925, 48 };
    kwEditorData.anchor02 = (Vector2){ 832, 96 };
    kwEditorData.anchor03 = (Vector2){ 832, 312 };
    kwEditorData.anchor04 = (Vector2){ 832, 312 };

    kwEditorData.EditorBoxActive = true;
    kwEditorData.MassMinValue = 0.0f;
    kwEditorData.MassMaxValue = 0.0f;
    kwEditorData.GravitationValue = 0.0f;
    kwEditorData.BodyTypeEditMode = false;
    kwEditorData.BodyTypeActive = 0;
    kwEditorData.BodyGravityValue = 0.0f;
    kwEditorData.DampingValue = 0.0f;
    kwEditorData.StiffnessValue = 20.0f;
    kwEditorData.RestitutionValue = 0.3f;
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor(Vector2 position)
{
    if (kwEditorData.BodyTypeEditMode) GuiLock();

    if (kwEditorData.EditorBoxActive)
    {
        kwEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { kwEditorData.anchor01.x + 0, kwEditorData.anchor01.y + 0, 280, 616 }, "Editor");
        GuiSliderBar((Rectangle) { kwEditorData.anchor01.x + 128, kwEditorData.anchor01.y + 104, 120, 16 }, "Gravity Scale", NULL, & kwEditorData.BodyGravityValue, 0, 100);
        GuiSliderBar((Rectangle) { kwEditorData.anchor01.x + 128, kwEditorData.anchor01.y + 136, 120, 16 }, "Body Mass", NULL, & kwEditorData.MassMinValue, 1, 10);
        GuiSliderBar((Rectangle) { kwEditorData.anchor01.x + 128, kwEditorData.anchor01.y + 168, 120, 16 }, "Body Damping", NULL, & kwEditorData.DampingValue, 0, 100);
        GuiSliderBar((Rectangle) { kwEditorData.anchor01.x + 128, kwEditorData.anchor01.y + 200, 120, 16 }, "Restitution", NULL, & kwEditorData.RestitutionValue, 0, 1);
        GuiSliderBar((Rectangle) { kwEditorData.anchor01.x + 128, kwEditorData.anchor01.y + 280, 120, 16 }, "Gravitation", NULL, & kwEditorData.GravitationValue, 0, 100);
        GuiSliderBar((Rectangle) { kwEditorData.anchor01.x + 128, kwEditorData.anchor01.y + 312, 120, 16 }, "Spring Stiffness (k)", NULL, & kwEditorData.StiffnessValue, 1, 100);
        if (GuiDropdownBox((Rectangle) { kwEditorData.anchor01.x + 40, kwEditorData.anchor01.y + 72, 208, 24 }, "DYNAMIC;STATIC;KINEMATIC", & kwEditorData.BodyTypeActive, kwEditorData.BodyTypeEditMode)) kwEditorData.BodyTypeEditMode = !kwEditorData.BodyTypeEditMode;
    }
    GuiGroupBox((Rectangle) { 832 + 110, 108, 240, 168 }, "BODY PROPERTIES");
    GuiGroupBox((Rectangle) { kwEditorData.anchor04.x + 110, kwEditorData.anchor04.y + 0, 240, 104 }, "WORLD PROPERTIES");

    DrawTexture(cursorTexture, position.x - cursorTexture.width / 2, position.y - cursorTexture.height / 2, WHITE);

    GuiUnlock();
}

kwBody* GetBodyIntersect(kwBody* bodies, Vector2 position)
{
    for (kwBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(kwBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
