#include "raylib.h"
#include "dialog.h"

#ifndef SCENE_H
#define SCENE_H

typedef enum PlayMode
{
    MODE_NULL = 0,
    MODE_WALKING,
    MODE_BATTLE
} PlayMode;

typedef enum BattleLocations
{
    LOC_NULL = 0,
    LOC_EARTH_EUROPE,
    LOC_EARTH_JUNGLE,
    LOC_ECHO7,
    LOC_OSCAR2
} BattleLocations;

typedef enum NPCs
{
    NPC_NULL = 0,
    NPC_NARRATOR,
    NPC_BUDDY,
    NPC_SHEEP_RUIZ,
    NPC_ENG_LEE,
    NPC_COMM_SMITH,
    NPC_CAP_LASSIC,
    NPC_INTEL_TARI
} NPCs;

typedef enum Touchables
{
    TOUCH_NULL = 0,
    TOUCH_BED,
    TOUCH_DOOR_BEDROOM,
    TOUCH_DOOR_TACCOM,
    TOUCH_DOOR_STRATCOM,
    TOUCH_DOOR_ELEVATOR_UP,
    TOUCH_DOOR_ELEVATOR_DOWN,
    TOUCH_CONSOLE_BRIDGE_SYSTEM,
    TOUCH_CONSOLE_BRIDGE_NAVIGATION
} Touchables;

typedef enum Scenes
{
    SCENE_NULL = 0,
    SCENE_01_WALKING_INTRO,
    SCENE_02_BATTLE_TACCOM_PRACTICE_1,
    SCENE_03_WALKING_AFTER_PRACTICE_1,
    SCENE_04_BATTLE_TACCOM_PRACTICE_2,
    SCENE_05_WALKING_AFTER_PRACTICE_2,
    SCENE_06_BATTLE_TACCOM_MISSION_1,
    SCENE_07_WALKING_AFTER_BATTLE_1,
    SCENE_08_WALKING_AFTER_JUMP,
    SCENE_09_BATTLE_TACCOM_MISSION_2,
    SCENE_10_WALKING_AFTER_BATTLE_2,
    SCENE_11_WALKING_BRIDGE_1,
    SCENE_12_WALKING_AFTER_BRIDGE_1,
    SCENE_13_WALKING_AFTER_JUMP_2,
    SCENE_14_BATTLE_TACCOM_MISSION_3,
    SCENE_15_WALKING_AFTER_BATTLE_3,
    SCENE_16_WALKING_AFTER_CONFINEMENT,
    SCENE_17_WALKING_AFTER_JUMP_3,
    SCENE_18_WALKING_BRIDGE_MUTINY,
    SCENE_19_WALKING_OVERLOAD_THE_REACTOR,
    SCENE_20_GOD_AMONGST_MEN
} Scenes;

typedef enum SceneEvents
{
    EVT_NULL = 0,
    EVT_SLEEP, // touch bed, go to sleep, optionally go to new scene after.
    EVT_TACCOM_ENTER

} SceneEvents;

typedef struct Scene
{
    char *name;
    short hasCutScene;
    DialogTextEntry cutSceneEntries[SCENE_CUTSCENE_DIALOG_MAX];
    int npcs[100];
    int playMode;
} Scene;

typedef struct SceneEvent
{
    int eventType;
    int touchable; // the touchable item that will trigger this event, 0 for nothing

} SceneEvent;

#endif // DIALOG_H