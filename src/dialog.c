#include <string.h>
#include <stdio.h>
#include "raylib.h"
#include "screens.h"
#include "scene.h"
#include "dialog.h"

int IsDialogOpen = 0;
int IsDialogComplete = 0;

static int dialogJustClosed = 0;
static float dialogLetterTimeout = 0.0f;
static int currentDialogLetter = 0;

DialogTextEntry dialogQueue[32];
static int dialogQueueSize = 0;
Color dialogBackground = BLACK;

void InitDialog()
{
    dialogLetterTimeout = 0;
    currentDialogLetter = 0;
    IsDialogComplete = false;
    IsDialogOpen = false;
    dialogQueueSize = 0;
    dialogBackground = ColorAlpha(BLACK, 0.75f);

    for (int i = 0; i < 32; i++)
    {
        dialogQueue[i].speaker = NPC_NULL;
    }
}

void AddDialog(DialogTextEntry entry)
{
    dialogQueue[dialogQueueSize].speaker = entry.speaker;
    dialogQueue[dialogQueueSize].portraitOnRight = entry.portraitOnRight;
    dialogQueue[dialogQueueSize].text = entry.text;
    dialogQueue[dialogQueueSize].texture = entry.texture;

    if (entry.text != NULL)
    {    
        dialogQueue[dialogQueueSize].contentSize = MeasureTextEx(font, entry.text, font.baseSize * DIALOG_FONT_SIZE_MULTIPLIER, 1);
        // the textsize seems to be off by 1 character width
        dialogQueue[dialogQueueSize].contentSize.x += 28;
    }
    else if (entry.texture)
    {
        
        dialogQueue[dialogQueueSize].contentSize.x = entry.texture->width;
        dialogQueue[dialogQueueSize].contentSize.y = entry.texture->height;
    }

    dialogQueueSize++;

    if (dialogQueueSize > 32)
    {
        printf("DIALOG QUEUE SIZE IS TOO BIG! %i\n", dialogQueueSize);
    }
}

void UpdateDialog(float delta)
{
    if (dialogQueue[0].speaker > NPC_NULL)
    {
        IsDialogOpen = true;
        if (!IsDialogComplete)
        {
            if (dialogQueue[0].text != NULL)
            {
                if (dialogLetterTimeout <= 0)
                {
                    dialogLetterTimeout = DIALOG_LETTER_TIMEOUT;
                    currentDialogLetter++;
                }

                if (currentDialogLetter > strlen(dialogQueue[0].text))
                {
                    IsDialogComplete = true;
                    currentDialogLetter = strlen(dialogQueue[0].text);
                }
                dialogLetterTimeout -= delta;
            }
        }
    }
    else
    {
        IsDialogOpen = false;
    }
    dialogJustClosed = false;
}

void SkipDialogAnimation()
{
    if (dialogQueue[0].text != NULL)
    {
        currentDialogLetter = strlen(dialogQueue[0].text);
    }
}

void CloseCurrentDialog()
{
    if (dialogJustClosed == 1)
    {
        return;
    }

    for (int i = 0; i < dialogQueueSize - 1; i++)
    {
        dialogQueue[i].portraitOnRight = dialogQueue[i + 1].portraitOnRight;
        dialogQueue[i].speaker = dialogQueue[i + 1].speaker;
        dialogQueue[i].text = dialogQueue[i + 1].text;
        dialogQueue[i].contentSize = dialogQueue[i + 1].contentSize;
    }
    dialogQueue[dialogQueueSize - 1].speaker = NPC_NULL;
    dialogQueueSize--;

    currentDialogLetter = 0;
    dialogLetterTimeout = DIALOG_LETTER_TIMEOUT;
    IsDialogComplete = false;

    dialogJustClosed = 1;
}

void DrawDialog()
{
    if (dialogQueue[0].speaker > NPC_NULL)
    {
        if (dialogQueue[0].text != NULL)
        {
            DrawRectangle(20, 20, screenWidth - 40, screenHeight - 40, dialogBackground);
            DrawRectangleLines(20, 20, screenWidth - 40, screenHeight - 40, GREEN);
            DrawRectangleLines(22, 22, screenWidth - 44, screenHeight - 44, LIME);
            DrawRectangleLines(18, 18, screenWidth - 36, screenHeight - 36, LIME);
            DrawTextEx(font, TextFormat("%.*s", currentDialogLetter, dialogQueue[0].text), (Vector2){screenWidth / 2.0f - dialogQueue[0].contentSize.x / 2.0f, (screenHeight) / 2.0f - dialogQueue[0].contentSize.y / 2.0f}, font.baseSize * DIALOG_FONT_SIZE_MULTIPLIER, DIALOG_FONT_SPACING, GREEN);
        } else if (dialogQueue[0].texture)
        {
            printf("TODO: MAKE IT SO DIALOG PICTURES SHOW\n");
        }
    }
}