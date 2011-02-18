#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <inttypes.h>
#include <SDL/SDL.h>
#include <errno.h>

#include "mylist.h"

#include "Graph.h"
#include "Render.h"
#include "Sound.h"
#include "Mouse.h"
#include "Manage.h"
#include "Puzzle.h"
#include "SoundFX.h"
#include "Timers.h"
#include "Control.h"
#include "ScriptSystem.h"
#include "actions.h"
#include "Game.h"




//System time functions
//uint64_t millisec();
#define millisec SDL_GetTicks

//Game timer functions
void InitMTime(int fps);
void ProcMTime();
bool GetTick();
uint64_t GetTickCount();

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



#define FILE_LN_BUF 0x400
char * PrepareString(char *buf);
char * GetParams(char *str);
int GetIntVal(char *chr);

#define strCMP(X,Y) strncasecmp(X,Y,strlen(Y))


#endif // SYSTEM_H_INCLUDED
