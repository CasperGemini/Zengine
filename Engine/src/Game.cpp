#include "System.h"


Locate Need_Locate;
bool NeedToLoadScript=false;


//////DEPRECATED
extern SDL_Surface *screen;
/////////////



int32_t View_start_Loops = 0;

int8_t  SaveSlot = 0;


void SetNeedLocate(uint8_t w, uint8_t r,uint8_t v1, uint8_t v2, int32_t X)
{
    NeedToLoadScript  = true;
    Need_Locate.World = tolower(w);
    Need_Locate.Room  = tolower(r);
    Need_Locate.Node  = tolower(v1);
    Need_Locate.View  = tolower(v2);
    Need_Locate.X     = X;

    if (Need_Locate.World == '0')
    {
        if (GetgVarInt(SLOT_WORLD) == tolower(SystemWorld) &&
            GetgVarInt(SLOT_ROOM) == tolower(SystemRoom))
        {
            Need_Locate.World = GetgVarInt(SLOT_MENU_LASTWORLD);
            Need_Locate.Room  = GetgVarInt(SLOT_MENU_LASTROOM);
            Need_Locate.Node  = GetgVarInt(SLOT_MENU_LASTNODE);
            Need_Locate.View  = GetgVarInt(SLOT_MENU_LASTVIEW);
            Need_Locate.X     = GetgVarInt(SLOT_MENU_LASTVIEW_POS);
        }
        else
        {
            Need_Locate.World = GetgVarInt(SLOT_LASTWORLD);
            Need_Locate.Room  = GetgVarInt(SLOT_LASTROOM);
            Need_Locate.Node  = GetgVarInt(SLOT_LASTNODE);
            Need_Locate.View  = GetgVarInt(SLOT_LASTVIEW);
            Need_Locate.X     = GetgVarInt(SLOT_LASTVIEW_POS);
        }
    }

}


void SetgVarInt(void **Vars, int indx, int var)
{
    Vars[indx]=NULL;
    Vars[indx]=(void *)var;
}

int GetgVarInt(void **Vars, int indx)
{
    return (int)Vars[indx];
}

//void SaveGame(char *file)
//{
//    void *tmpVars[VAR_SLOTS_MAX];
//    memcpy(tmpVars,DGetGVars(),VAR_SLOTS_MAX*sizeof(void *));
//
//    MList *allres = GetAction_res_List();
//    StartMList(allres);
//    while (!eofMList(allres))
//    {
//        struct_action_res *nod=(struct_action_res *)DataMList(allres);
//
//        if (nod->node_type == NODE_TYPE_MUSIC)
//            SetgVarInt(tmpVars,nod->slot,2);
//
//        if (nod->node_type == NODE_TYPE_TIMER)
//            SetgVarInt(tmpVars,nod->slot,2);
//
//        if (nod->node_type == NODE_TYPE_ANIM)
//            SetgVarInt(tmpVars,nod->slot,2);
//
//        NextMList(allres);
//    }
//
//
//    pzllst *room = Getroom();
//    StartMList(room->_list);
//    while (!eofMList(room->_list))
//    {
//        puzzlenode *nod=(puzzlenode *)DataMList(room->_list);
//        SetgVarInt(tmpVars,nod->slot,0);
//        NextMList(room->_list);
//    }
//
//    pzllst *view = Getview();
//    StartMList(view->_list);
//    while (!eofMList(view->_list))
//    {
//        puzzlenode *nod=(puzzlenode *)DataMList(view->_list);
//        SetgVarInt(tmpVars,nod->slot,0);
//        NextMList(view->_list);
//    }
//
//    pzllst *world = Getworld();
//    StartMList(world->_list);
//    while (!eofMList(world->_list))
//    {
//        puzzlenode *nod=(puzzlenode *)DataMList(world->_list);
//        SetgVarInt(tmpVars,nod->slot,0);
//        NextMList(world->_list);
//    }
//
//    pzllst *uni = GetUni();
//    StartMList(uni->_list);
//    while (!eofMList(uni->_list))
//    {
//        puzzlenode *nod=(puzzlenode *)DataMList(uni->_list);
//        SetgVarInt(tmpVars,nod->slot,0);
//        NextMList(uni->_list);
//    }
//
//    FILE *fil=fopen(file, "wb");
//
////    fwrite(Previos_Locate,PREV_STACK_MAX,sizeof(Locate),fil);
////    fwrite(&PrevCount,1,sizeof(PrevCount),fil);
////    fwrite(&Location,1,sizeof(Location),fil);
////    fwrite(&Current_Locate,1,sizeof(Locate),fil);
//
//
//    fwrite(tmpVars,VAR_SLOTS_MAX,sizeof(void *),fil);
//    fwrite(DGetFlags(),VAR_SLOTS_MAX,sizeof(uint8_t),fil);
//
//    fclose(fil);
//}
//
//
//void LoadGame(char *file)
//{
//
//    FILE *fil=fopen(file, "rb");
//    if (!fil)
//        return;
//
//    ScrSys_DeleteAllRes();
//
//    fread(DGetGVars(),VAR_SLOTS_MAX,sizeof(void *),fil);
//    fread(DGetFlags(),VAR_SLOTS_MAX,sizeof(uint8_t),fil);
//
//    fclose(fil);
//
//    ScrSys_ChangeLocation(GetgVarInt(3),GetgVarInt(4),GetgVarInt(5),GetgVarInt(6),GetgVarInt(7));
//}




































bool ProcessCriteries2(MList *lst)
{
    bool tmp=true;

    StartMList(lst);
    while (!eofMList(lst))
    {
        crit_node *critnd=(crit_node *)DataMList(lst);
#ifdef TRACE
        printf("        [%d] %d [%d] %d\n",critnd->slot1,critnd->oper,critnd->slot2,critnd->var2);
#endif
        int tmp1=GetgVarInt(critnd->slot1);
        int tmp2;

        if (critnd->var2)
            tmp2=GetgVarInt(critnd->slot2);
        else
            tmp2=critnd->slot2;

        switch (critnd->oper)
        {
        case CRIT_OP_EQU:
            tmp &= (tmp1 == tmp2);
            break;
        case CRIT_OP_GRE:
            tmp &= (tmp1 > tmp2);
            break;
        case CRIT_OP_LEA:
            tmp &= (tmp1 < tmp2);
            break;
        case CRIT_OP_NOT:
            tmp &= (tmp1 != tmp2);
            break;
        }

        if (!tmp) break;

        NextMList(lst);
    }
    return tmp;
}

bool ProcessCriteries(MList *lst)
{
    bool tmp=true;

    StartMList(lst);
    while (!eofMList(lst))
    {
        crit_node *critnd=(crit_node *)DataMList(lst);
#ifdef FULLTRACE
        printf("        [%d] %d [%d] %d\n",critnd->slot1,critnd->oper,critnd->slot2,critnd->var2);
#endif
        int tmp1=GetgVarInt(critnd->slot1);
        int tmp2;

        if (critnd->var2)
            tmp2=GetgVarInt(critnd->slot2);
        else
            tmp2=critnd->slot2;

        switch (critnd->oper)
        {
        case CRIT_OP_EQU:
            tmp &= (tmp1 == tmp2);
            break;
        case CRIT_OP_GRE:
            tmp &= (tmp1 > tmp2);
            break;
        case CRIT_OP_LEA:
            tmp &= (tmp1 < tmp2);
            break;
        case CRIT_OP_NOT:
            tmp &= (tmp1 != tmp2);
            break;
        }

        if (!tmp) break;

        NextMList(lst);
    }
    return tmp;
}
































void InitGameLoop()
{
    pzllst *uni = GetUni();
    LoadScriptFile(uni,GetExactFilePath("universe.scr"),false,NULL);

    ScrSys_ChangeLocation(InitWorld,InitRoom,InitNode,InitView,0,true);

    //Hack
    SetDirectgVarInt(SLOT_LASTWORLD, InitWorld);
    SetDirectgVarInt(SLOT_LASTROOM , InitRoom);
    SetDirectgVarInt(SLOT_LASTNODE , InitNode);
    SetDirectgVarInt(SLOT_LASTVIEW , InitView);
    SetDirectgVarInt(SLOT_MENU_LASTWORLD, InitWorld);
    SetDirectgVarInt(SLOT_MENU_LASTROOM , InitRoom);
    SetDirectgVarInt(SLOT_MENU_LASTNODE , InitNode);
    SetDirectgVarInt(SLOT_MENU_LASTVIEW , InitView);
    SetDirectgVarInt(53,250);
    //\Hack
}









void GameLoop()
{


    Mouse_SetCursor(CURSOR_IDLE);

    if (GetgVarInt(18) != 0)
        SetgVarInt(18,0);

    if (GetgVarInt(10) != 0)
        SetgVarInt(10,0);

    if (KeyAnyHit())
        if (GetLastKey() != SDLK_FIRST && GetLastKey() != SDLK_F5 &&\
                GetLastKey() != SDLK_F6    && GetLastKey() != SDLK_F7 &&\
                GetLastKey() != SDLK_F8)
            SetgVarInt(8,GetWinKey(GetLastKey()));

    if (Rend_MouseInGamescr())
    {
        if (MouseUp(SDL_BUTTON_RIGHT))
            SetgVarInt(18,1);
        if (MouseHit(SDL_BUTTON_LEFT))
            SetgVarInt(10,1);
    }

//    tmr_ProcessTimers();
//    snd_ProcessWavs();
    ScrSys_ProcessAllRes();

    pzllst *room  = Getroom();
    pzllst *view  = Getview();
    pzllst *world = Getworld();
    pzllst *uni   =  GetUni();

    if (!ScrSys_BreakExec())
        ScrSys_exec_puzzle_list(world);

    if (!ScrSys_BreakExec())
        ScrSys_exec_puzzle_list(room);

    if (!ScrSys_BreakExec())
        ScrSys_exec_puzzle_list(view);

    if (!ScrSys_BreakExec())
        ScrSys_exec_puzzle_list(uni);


    MList *ctrl = Getctrl();

    if (!ScrSys_BreakExec())
        ProcessControls(ctrl);

    if (!ScrSys_BreakExec())
        Rend_RenderFunc();

    if (NeedToLoadScript)
    {
        NeedToLoadScript=false;
        ScrSys_ChangeLocation(Need_Locate.World,Need_Locate.Room,Need_Locate.Node,Need_Locate.View,Need_Locate.X, false);
    }


    char savefile[16];

    sprintf(savefile,"Save%d.sav",SaveSlot);

    if (KeyHit(SDLK_F6))
        SaveSlot--;
    if (KeyHit(SDLK_F7))
        SaveSlot++;

    if (SaveSlot > 40)
        SaveSlot = 0;
    if (SaveSlot < 0)
        SaveSlot = 40;

    if (KeyHit(SDLK_F5))
        {
            ScrSys_PrepareSaveBuffer();
            ScrSys_SaveGame(SaveSlot);
        }
    if (KeyHit(SDLK_F8))
        {
            ScrSys_LoadGame(SaveSlot);
        }


    if (GetgVarInt(SLOT_WORLD) == InitWorld &&
        GetgVarInt(SLOT_ROOM)  == InitRoom &&
        GetgVarInt(SLOT_NODE)  == InitNode &&
        GetgVarInt(SLOT_VIEW)  == InitView &&
        KeyDown(SDLK_RCTRL) && KeyDown(SDLK_F12))
        {
            NeedToLoadScript  = true;
            SetNeedLocate('g', 'j', 't', 'm', 0);
        }

    stringColor(screen,0,470,savefile,0xFFFFFFFF);

    SDL_Flip(screen);
}





