#ifndef SCRIPTSYSTEM_H_INCLUDED
#define SCRIPTSYSTEM_H_INCLUDED

#include "Puzzle.h"


#define VAR_SLOTS_MAX 0xFFFF
#define STATEBOX_STACK_MAX 1024
#define MaxStateBoxEnts 0x400 //0xFF is very small


#define SLOT_LOCATION_CUR_WO    3
#define SLOT_LOCATION_CUR_RO    4
#define SLOT_LOCATION_CUR_V1    5
#define SLOT_LOCATION_CUR_V2    6
#define SLOT_LOCATION_CUR_X     7
#define SLOT_KEY_PRESS          8 //keycode in slot
#define SLOT_INVENTORY_MOUSE    9 //code of item in slot
#define SLOT_MOUSE_DOWN        10 //1 if clicked
#define SLOT_MOUSE_RIGHT_CLICK 18 //1 if right button of mouse
#define SLOT_MENU_STATE        19
#define SLOT_JUST_RESTORED     20
#define SLOT_KLASTWORLD        45
#define SLOT_KLASTROOM         46
#define SLOT_KLASTNODE         47
#define SLOT_KLASTVIEW         48
#define SLOT_KLASTPOS          49
#define SLOT_SUBTITLE_FLAG     51
#define SLOT_PANAROTATE_SPEED  53
#define SLOT_QSOUND_ENABLE     57
#define SLOT_HIGH_QUIALITY     59
#define SLOT_LINE_SKIP_VIDEO   65
#define SLOT_INSTALL_LEVEL     67
#define SLOT_COUNTRY_CODE      68
#define SLOT_TURN_OFF_ANIM     71
#define SLOT_INV_STORAGE_1    102
#define SLOT_INV_STORAGE_2    103
#define SLOT_INV_STORAGE_3    104
#define SLOT_INV_STORAGE_4    105
#define SLOT_INV_STORAGE_5    106
#define SLOT_INV_STORAGE_6    107
#define SLOT_TOTAL_INV_AVAIL  150
#define SLOT_START_SLOT       151
#define SLOT_END_SLOT         170
#define SLOT_SPELL_1          191
#define SLOT_SPELL_2          192
#define SLOT_SPELL_3          193
#define SLOT_SPELL_4          194
#define SLOT_SPELL_5          195
#define SLOT_SPELL_6          196
#define SLOT_SPELL_7          197
#define SLOT_SPELL_8          198
#define SLOT_SPELL_9          199
#define SLOT_SPELL_10         200
#define SLOT_SPELL_11         201
#define SLOT_SPELL_12         202
#define SLOT_USER_CHOSE_THIS_SPELL 205
#define SLOT_REVERSED_SPELLBOOK    206

#define FLAG_ONCE_PER_I 1
#define FLAG_DISABLED   2
#define FLAG_DO_ME_NOW  4

#define CRIT_OP_EQU   0  //=
#define CRIT_OP_LEA   1  //<
#define CRIT_OP_GRE   2  //>
#define CRIT_OP_NOT   3  //!


struct StateBoxEnt{
    puzzlenode * nod[MaxStateBoxEnts];
    uint32_t     cnt;
};


pzllst *GetUni();
pzllst *Getworld();
pzllst *Getroom();
pzllst *Getview();
MList  *Getctrl();

char * ScrSys_ReturnListName(pzllst *lst);

void    ScrSys_SetSystemClass(char World, char Room);
uint8_t ScrSys_GetSystemWorld();
uint8_t ScrSys_GetSystemRoom();

uint8_t ScrSys_GetFlag(uint32_t indx);
void    ScrSys_SetFlag(uint32_t indx, uint8_t newval);

bool ScrSys_SlotIsOwned(uint32_t i);

void SetgVarInt(uint32_t indx, int var);
int  GetgVarInt(uint32_t indx);
void SetDirectgVarInt(uint32_t indx, int var);
int *getdirectvar(uint32_t indx);



void LoadScriptFile(pzllst *lst, char *filename, bool control, MList *controlst);
void InitScriptsEngine();

void FillStateBoxFromList(pzllst *lst);

void ScrSys_ChangeLocation(uint8_t w, uint8_t r, uint8_t v1, uint8_t v2, int32_t X);

void ScrSys_exec_puzzle_list(pzllst *lst);



void ShakeStateBox(uint32_t indx);

bool ScrSys_BreakExec();
void ScrSys_SetBreak();

/////  Depricated
int *DGetGVars();
uint8_t *DGetFlags();
/////  /Depricated


#endif // SCRIPTSYSTEM_H_INCLUDED

