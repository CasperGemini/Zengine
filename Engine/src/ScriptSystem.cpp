
#include "System.h"


uint8_t SystemWorld = 'G';
uint8_t SystemRoom  = 'J';



int          gVars   [VAR_SLOTS_MAX];

uint8_t      Flags   [VAR_SLOTS_MAX];

StateBoxEnt *StateBox[VAR_SLOTS_MAX];

puzzlenode  *StateBoxStk[STATEBOX_STACK_MAX];
uint32_t     StateBoxStkSz = 0;

bool BreakExecute = false;


pzllst   *uni    =NULL;
pzllst   *world  =NULL;
pzllst   *room   =NULL;
pzllst   *view   =NULL;

MList    *ctrl  =NULL;


char * ScrSys_ReturnListName(pzllst *lst)
{
    if (lst == world)
        return "world";

    if (lst == uni)
        return "universe";

    if (lst == room)
        return "room";

    if (lst == view)
        return "view";
}

pzllst *GetUni()
{
    return uni;
}

pzllst *Getworld()
{
    return world;
}

pzllst *Getroom()
{
    return room;
}

pzllst *Getview()
{
    return view;
}

MList *Getctrl()
{
    return ctrl;
}

void SetgVarInt(uint32_t indx, int var)
{
    gVars[indx]=var;

    ShakeStateBox(indx);
}

void SetDirectgVarInt(uint32_t indx, int var)
{
    gVars[indx]=var;
}

int GetgVarInt(uint32_t indx)
{
    return gVars[indx];
}

int *getdirectvar(uint32_t indx)
{
    return &gVars[indx];
}


uint8_t ScrSys_GetFlag(uint32_t indx)
{
    return Flags[indx];
}

void ScrSys_SetFlag(uint32_t indx, uint8_t newval)
{
    Flags[indx] = newval;
}

//Don't call it from loops for mylists!! it's cause error
bool ScrSys_SlotIsOwned(uint32_t i)
{
    MList *timers = tmr_GetTimerList();

    pushMList(timers);
    StartMList(timers);
    while (!eofMList(timers))
    {
        timernode *nod= (timernode *)DataMList(timers);

        if (nod->slot == i)
            return true;

        NextMList(timers);
    }
    popMList(timers);

    MList *wavs = snd_GetWavsList();

    pushMList(wavs);
    StartMList(wavs);
    while (!eofMList(wavs))
    {
        musicnode *nod= (musicnode *)DataMList(wavs);

        if (nod->slot == i)
            return true;

        NextMList(wavs);
    }
    popMList(wavs);

    return false;
}



void InitScriptsEngine()
{
    memset(gVars,0x0,VAR_SLOTS_MAX * sizeof(void *));

    view  = CreatePzlLst();
    room  = CreatePzlLst();
    world = CreatePzlLst();
    uni   = CreatePzlLst();

    ctrl  = CreateMList();

    snd_InitWavsList();
    tmr_InitTimerList();
    anim_InitAnimLists();

    memset(StateBox,0x0,VAR_SLOTS_MAX * sizeof(StateBoxEnt *));
    StateBoxStkSz = 0;

    memset(Flags,0x0,VAR_SLOTS_MAX * sizeof(uint8_t));
}





void LoadScriptFile(pzllst *lst, char *filename, bool control, MList *controlst)
{
#ifdef TRACE
    printf("Loading script file %s\n",filename);
#endif


    FILE *fl=fopen(filename,"rb");
    if (fl == NULL)
    {
        printf("Error opening file %s\n",filename);
        exit(1);
        return;
    }

    char buf[FILE_LN_BUF];

    while(!feof(fl))
    {
        fgets(buf,FILE_LN_BUF,fl);

        char *str=PrepareString(buf);


        if (strCMP(str,"puzzle")==0)
        {
            Parse_Puzzle(lst,fl,str);
        }
        else if (strCMP(str,"control")==0 && control )
        {
            Parse_Control(controlst,fl,str);
        }
    }

    fclose(fl);
}

void ScrSys_ClearStateBox()
{
    for (int i=0; i<VAR_SLOTS_MAX; i++)
    {
        if (StateBox[i] != NULL)
            delete StateBox[i];
    }

    memset(StateBox,0,VAR_SLOTS_MAX * sizeof(StateBoxEnt *));
    StateBoxStkSz=0;
}

void ScrSys_ChangeLocation(uint8_t w, uint8_t r,uint8_t v1, uint8_t v2, int32_t X) // world / room / view
{
    //Needed reverse from 0x004246C7

    if (GetgVarInt(3) != SystemWorld &&
            GetgVarInt(4) != SystemRoom  )
    {
        if (w == SystemWorld &&
                r == SystemRoom  )
        {
            SetDirectgVarInt(45,GetgVarInt(3));
            SetDirectgVarInt(46,GetgVarInt(4));
            SetDirectgVarInt(47,GetgVarInt(5));
            SetDirectgVarInt(48,GetgVarInt(6));
            SetDirectgVarInt(49,GetgVarInt(7));
        }
        else
        {
            SetDirectgVarInt(40,GetgVarInt(3));
            SetDirectgVarInt(41,GetgVarInt(4));
            SetDirectgVarInt(42,GetgVarInt(5));
            SetDirectgVarInt(43,GetgVarInt(6));
            SetDirectgVarInt(44,GetgVarInt(7));
        }
    }


    Locate temp;
    temp.World =w;
    temp.Room  =r;
    temp.View1 =v1;
    temp.View2 =v2;
    temp.X     =X;


    ScrSys_ClearStateBox();

    char buf[32];
    char tm[5];


//    RenderDelay = 2;
//    View_start_Loops = 1;

    if (temp.View1 != GetgVarInt(6) || temp.View2 != GetgVarInt(5) || temp.Room != GetgVarInt(4) || temp.World != GetgVarInt(3) || view == NULL)
    {
        //if (view->_list->count > 0)
        //{
        tmr_DeleteTimerByOwner(view);
        snd_DeleteLoopedWavsByOwner(view);

        FlushPuzzleList(view);
        FlushControlList(ctrl);
        anim_FlushAnims();
        anim_FlushPreload();
        //}

        tm[0]=temp.World;
        tm[1]=temp.Room;
        tm[2]=temp.View1;
        tm[3]=temp.View2;
        tm[4]=0;
        sprintf(buf,"%s.scr",tm);

        LoadScriptFile(view,GetExactFilePath(buf),true,ctrl);

    }

    if (temp.Room != GetgVarInt(4) || temp.World != GetgVarInt(3) || room == NULL)
    {
        //if (room->_list->count > 0)
        //{
        snd_DeleteLoopedWavsByOwner(room);

        FlushPuzzleList(room);
        //}
        //room->exec_times = 0;

        tm[0]=temp.World;
        tm[1]=temp.Room;
        tm[2]=0;
        sprintf(buf,"%s.scr",tm);

        LoadScriptFile(room,GetExactFilePath(buf),false,NULL);
    }

    if (temp.World != GetgVarInt(3) || world == NULL)
    {
        //if (world->_list->count > 0)
        //{
        snd_DeleteLoopedWavsByOwner(world);

        FlushPuzzleList(world);
        //}
        //world->exec_times = 0;

        tm[0]=temp.World;
        tm[1]=0;
        sprintf(buf,"%s.scr",tm);

        LoadScriptFile(world,GetExactFilePath(buf),false,NULL);

        Mouse_ShowCursor();
    }

    FillStateBoxFromList(uni);
    FillStateBoxFromList(view);
    FillStateBoxFromList(room);
    FillStateBoxFromList(world);

    SetgVarInt(3,tolower(w));
    SetgVarInt(4,tolower(r));
    SetgVarInt(5,tolower(v1));
    SetgVarInt(6,tolower(v2));
    SetgVarInt(7,X);

    menu_SetMenuBarVal(0xFFFF);

    BreakExecute = false;
}


void AddPuzzleToStateBox(int slot, puzzlenode *pzlnd)
{
    StateBoxEnt *ent = StateBox[slot];

    if (ent == NULL)
    {
        StateBox[slot] = ent = new (StateBoxEnt);
        ent->cnt = 0;
    }
    if (ent->cnt < MaxStateBoxEnts)
    {
        ent->nod[ent->cnt] = pzlnd;
        ent->cnt++;
    }
}

void FillStateBoxFromList(pzllst *lst)
{
    StartMList(lst->_list);
    while (!eofMList(lst->_list))
    {
        puzzlenode *pzlnod=(puzzlenode *)DataMList(lst->_list);

        if (pzlnod->flags & FLAG_ONCE_PER_I)
            AddPuzzleToStateBox(pzlnod->slot,pzlnod);

        StartMList(pzlnod->CritList);
        while (!eofMList(pzlnod->CritList))
        {
            MList *CriteriaLst= (MList *) DataMList(pzlnod->CritList);

            int prevslot=0;
            StartMList(CriteriaLst);
            while (!eofMList(CriteriaLst))
            {
                crit_node *crtnod = (crit_node *)DataMList(CriteriaLst);

                if (prevslot != crtnod->slot1)
                    AddPuzzleToStateBox(crtnod->slot1,pzlnod);

                prevslot = crtnod->slot1;

                NextMList(CriteriaLst);
            }

            NextMList(pzlnod->CritList);
        }
        NextMList(lst->_list);
    }
}

//Function clears trigger status for once_per_inst triggers
void ClearUsedOnOPIPuzz(MList *lst)
{
    if (!lst)
        return;
    StartMList(lst);
    while (!eofMList(lst))
    {
        puzzlenode *nod=(puzzlenode *)DataMList(lst);
        if (nod->flags & FLAG_ONCE_PER_I)
            SetgVarInt(nod->slot,0);
        NextMList(lst);
    }
}

void AddStateBoxToStk(puzzlenode *pzl)
{
    pzllst *owner = pzl->owner;
    if (owner->stksize < pzlSTACK)
    {
        if (owner->stksize > 0)
            if (owner->stack[owner->stksize - 1] == pzl)
                return;

        owner->stack[owner->stksize] = pzl;
        owner->stksize++;
    }
    else
    {
#ifdef TRACE
        printf("Can't add pzl# %d to Stack\n",pzl->slot);
#endif
    }
}

void ShakeStateBox(uint32_t indx)
{
    if (StateBox[indx] != NULL)
    {
        for (int i=StateBox[indx]->cnt-1; i >= 0; i--)
        {
            //if (examine_criterias(StateBox[indx]->nod[i])) //may cause bug's
            AddStateBoxToStk(StateBox[indx]->nod[i]);
        }
    }
}




void ScrSys_exec_puzzle_list(pzllst *lst)
{
    if (lst->exec_times<2)
    {
        StartMList(lst->_list);
        while (!eofMList(lst->_list))
        {
            if (Puzzle_try_exec( (puzzlenode *)DataMList(lst->_list) ) == ACTION_BREAK )
            {
                BreakExecute=true;
                break;
            }
            NextMList(lst->_list);
        }
        lst->exec_times++;
    }
    else
    {
        int i=0,j=lst->stksize;

        while ( i < j)
        {
            if ( Puzzle_try_exec( lst->stack[i] ) == ACTION_BREAK )
            {
                BreakExecute=true;
                break;
            }
            i++;
        }

        int z=0;
        for (i = j; i < lst->stksize; i++)
        {
            lst->stack[z] = lst->stack[i];
            z++;
        }
        lst->stksize = z;
    }
}

bool ScrSys_BreakExec()
{
    return BreakExecute;
}

void ScrSys_SetBreak()
{
    BreakExecute=true;
}







////Depricated

int *DGetGVars()
{
    return gVars;
}

uint8_t *DGetFlags()
{
    return Flags;
}
