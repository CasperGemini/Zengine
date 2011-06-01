#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <inttypes.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <errno.h>

#include "mylist.h"

#include "config.h"
#include "types.h"

#include "Graph.h"
#include "Render.h"
#include "Subtitles.h"
#include "Sound.h"
#include "Mouse.h"
#include "Manage.h"
#include "Puzzle.h"
#include "SoundFX.h"
#include "Timers.h"
#include "Control.h"
#include "ScriptSystem.h"
#include "actions.h"
#include "anims.h"
#include "menu.h"
#include "Game.h"


void __END();

//System time functions
//uint64_t millisec();
#define millisec SDL_GetTicks

//Game timer functions
void InitMTime(int fps);
void ProcMTime();
bool GetBeat();
uint64_t GetBeatCount();

//Keyboard functions
void FlushHits();
void UpdateKeyboard();
bool KeyDown(SDLKey key);
bool KeyAnyHit();
int MouseX();
int MouseY();
bool MouseDown(int btn);
bool MouseHit(int btn);
bool MouseUp(int btn);
void FlushMouseBtn(int btn);
void SetHit(SDLKey key);
bool KeyHit(SDLKey key);

bool MouseInRect(int x, int y, int w, int h);


void InitVkKeys();
uint8_t GetWinKey(SDLKey key);
SDLKey GetLastKey();

struct FManNode
{
    char *File;
    char *Path;
};

struct FManRepNode
{
    char *ext;
    char *ext2;
};

void InitFileManage();
void ListDir(char *dir);
char *GetFilePath(char *chr);
char *GetExactFilePath(char *chr);
void AddReplacer(char *ext, char *ext2);

bool FileExist(char * fil);




char * PrepareString(char *buf);
char * TrimLeft(char *buf);
char * GetParams(char *str);
int GetIntVal(char *chr);

#define strCMP(X,Y) strncasecmp(X,Y,strlen(Y))


#endif // SYSTEM_H_INCLUDED
