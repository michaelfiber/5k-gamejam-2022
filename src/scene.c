#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "screens.h"
#include "scene.h"
#include "dialog.h"

void DrawDebug(void);
void UpdateWalkingUI(float delta);
void DrawWalkingUI(void);

char *modeText;

int CurrentScene = SCENE_NULL;
int CurrentDeck = 3;
float CurrentDeckFlashTimeout = 0.25f;
int CurrentDeckHighlighted = 1;

Color deckColors[] = {
    GRAY,
    BLUE,
    ORANGE,
    GREEN,
    YELLOW,
    RED,
    PURPLE,
    PINK,
    LIGHTGRAY,
    WHITE};

Color darkerGray = {
    50, 50, 50, 255
};

char *deckNames[] = {
    "ENGR",
    "DOCK",
    "SPLY",
    "LiVE",
    "TCOM",
    "SCOM",
    "OFCR",
    "MED+",
    "iNTL",
    "BRDG"};

Scene scene[22];
Texture2D tex_walking_ui_ship_deck_mid;
Texture2D tex_walking_ui_ship_deck_top;
Texture2D tex_common_ui_dpad;

// SOME UI DRAWING STUFF
void UIDrawInsetRectangle(int x, int y, int width, int height, Color bgColor)
{
    DrawRectangle(x, y, width, height, bgColor);
    DrawRectangle(x, y, 2, height, LIGHTGRAY);
    DrawRectangle(x, y + height - 2, width, 2, LIGHTGRAY);
    DrawRectangle(x, y, width, 2, DARKGRAY);
    DrawRectangle(x + width - 2, y, 2, height, DARKGRAY);
}

// MAIN SCENE FUNCTION
void InitSceneSystem(void)
{
    scene[SCENE_01_WALKING_INTRO].name = "Scene 01 - Intro";
    scene[SCENE_01_WALKING_INTRO].hasCutScene = true;

    scene[SCENE_01_WALKING_INTRO].cutSceneEntries[0].speaker = NPC_NARRATOR;
    scene[SCENE_01_WALKING_INTRO].cutSceneEntries[0].text = "Welcome to the SNS Ernest King.\nAs a member of Tactical Command\n(TACCOM) you will be providing\nDANGER CLOSE fire support from\norbit.";

    scene[SCENE_01_WALKING_INTRO].cutSceneEntries[1].speaker = NPC_NARRATOR;
    scene[SCENE_01_WALKING_INTRO].cutSceneEntries[1].text = "This is your first day on the ship.\nWalk around and get to know people.\nSim Practice will begin shortly.";

    scene[SCENE_01_WALKING_INTRO].playMode = MODE_WALKING;

    CurrentScene = SCENE_01_WALKING_INTRO;
    InitScene();
}

void InitScene(void)
{
    InitDialog();

    tex_walking_ui_ship_deck_mid = LoadTexture("resources/walking-ui-ship-deck-mid.png");
    tex_walking_ui_ship_deck_top = LoadTexture("resources/walking-ui-ship-deck-top.png");
    tex_common_ui_dpad = LoadTexture("resources/common-ui-dpad.png");

    // if the scene starts with a cutscene add it to the dialog queue.
    if (scene[CurrentScene].hasCutScene)
    {
        for (int i = 0; i < SCENE_CUTSCENE_DIALOG_MAX; i++)
        {
            if (scene[CurrentScene].cutSceneEntries[i].speaker > NPC_NULL)
            {
                AddDialog(scene[CurrentScene].cutSceneEntries[i]);
            }
        }
    }
}

void UpdateScene(void)
{
    float delta = GetFrameTime();
    UpdateDialog(delta);

    if (scene[CurrentScene].playMode == MODE_WALKING)
    {
        UpdateWalkingUI(delta);
    }

    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
    {
        if (IsDialogOpen)
        {
            if (IsDialogComplete)
            {
                CloseCurrentDialog();
            }
            else
            {
                SkipDialogAnimation();
            }
        }
        else
        {
            // do something in the scene.
            printf("button pressed in scene\n");
        }
    }
}

void DrawScene(void)
{
    ClearBackground(BLACK);

    // draw the scene.
    if (scene[CurrentScene].playMode == MODE_WALKING)
    {
        DrawWalkingUI();
    }

    DrawDialog();

#ifdef _DEBUG
    DrawDebug();
#endif
}

void UnloadScene(void)
{
    UnloadTexture(tex_walking_ui_ship_deck_mid);
    UnloadTexture(tex_walking_ui_ship_deck_top);
    UnloadTexture(tex_common_ui_dpad);
}

// WALKING UI
void DrawWalkingUI()
{
    DrawRectangle(0, screenHeight - 128, screenWidth, 128, GRAY);
    DrawRectangle(0, screenHeight - 132, screenWidth, 4, LIGHTGRAY);

    UIDrawInsetRectangle(64 - tex_common_ui_dpad.width / 2.0f - 4, screenHeight - 64 - tex_common_ui_dpad.height / 2.0f - 4, tex_common_ui_dpad.width + 8, tex_common_ui_dpad.height + 8, darkerGray);
    DrawTexture(tex_common_ui_dpad, 64 - tex_common_ui_dpad.width / 2.0f, screenHeight - 64 - tex_common_ui_dpad.height / 2.0f, WHITE);

    Texture2D *deckTex;
    Color deckColor;

    UIDrawInsetRectangle(208, 324, 300, 120, BLACK);

    for (int i = 0; i < 10; i++)
    {
        if (i == 9)
        {
            // draw top
            deckTex = &tex_walking_ui_ship_deck_top;
        }
        else
        {
            // draw mid
            deckTex = &tex_walking_ui_ship_deck_mid;
        }

        if (CurrentDeck == i)
        {
            DrawText(deckNames[i], 235 + i * 26, 360, font.baseSize, deckColors[i]);
        }

        if (CurrentDeck == i && CurrentDeckHighlighted == 1)
        {
            deckColor = deckColors[i];
        }
        else
        {
            deckColor = ColorAlpha(deckColors[i], 0.25f);
        }

        DrawTexture(*deckTex, 235 + i * 26, 375, deckColor);
    }
}

void UpdateWalkingUI(float delta)
{
    CurrentDeckFlashTimeout -= delta;
    if (CurrentDeckFlashTimeout <= 0)
    {
        CurrentDeckFlashTimeout = 0.25f;
        if (CurrentDeckHighlighted == 1)
            CurrentDeckHighlighted = 0;
        else
            CurrentDeckHighlighted = 1;
    }
}

// DEBUG STUFF
void DrawDebug()
{
    switch (scene[CurrentScene].playMode)
    {
    case MODE_WALKING:
        modeText = "WALKING";
        break;
    case MODE_BATTLE:
        modeText = "BATTLE";
        break;
    default:
        modeText = "UNKNOWN";
        break;
    }

    DrawText(scene[CurrentScene].name, 10, 10, 20, RED);
    DrawText(modeText, 10, 30, 20, RED);
}
