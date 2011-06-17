#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <inttypes.h>
#include "mylist.h"
#include "Graph.h"
#include "Sound.h"

#include <math.h>














struct Locate
{
    uint8_t World;
    uint8_t Room;
    uint8_t View1;
    uint8_t View2;
    int16_t X;
};



void ProcessTriggers(MList *lst);
void InitScriptsEngine();
void ProcessControls(MList *ctrlst);

bool ProcessCriteries(MList *lst);


void GameLoop();
void InitGameLoop();

void SetNeedLocate(uint8_t w, uint8_t r,uint8_t v1, uint8_t v2, int32_t X, bool menu);

bool examine_criterias(puzzlenode *nod);



#endif // GAME_H_INCLUDED
