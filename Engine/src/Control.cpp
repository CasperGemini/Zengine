#include "System.h"

int FocusInput=0;
bool pushChangeMouse=false;

pushnode * CreatePushNode()
{
    pushnode *tmp = new(pushnode);
    tmp->cursor = CURSOR_IDLE;
    tmp->flat = false;
    tmp->x = 0;
    tmp->y = 0;
    tmp->w = 0;
    tmp->h = 0;
    return tmp;
}

inputnode * CreateInputNode()
{
    inputnode *tmp = new(inputnode);
    tmp->rect = NULL;
    __InitRect(&tmp->rectangle);
    __InitRect(&tmp->hotspot);
    tmp->cursor = NULL;
    tmp->frame = 0;
    tmp->period = 0;
    tmp->readonly = false;
    tmp->textwidth = 0;
    tmp->textchanged = true;
    tmp->enterkey  = false;
    tmp->focused   = false;
    memset(tmp->text,0,SAVE_NAME_MAX_LEN+1);
    txt_init_txt_struct(&tmp->string_init);
    txt_init_txt_struct(&tmp->string_chooser_init);
    return tmp;
}

slotnode * CreateSlotNode()
{
    slotnode *tmp = new(slotnode);
    tmp->cursor = CURSOR_IDLE;
    tmp->eligable_cnt = 0;
    tmp->eligible_objects = NULL;
    tmp->srf = NULL;
    __InitRect(&tmp->rectangle);
    __InitRect(&tmp->hotspot);
    tmp->flat = false;

    return tmp;
}

saveloadnode * CreateSaveNode()
{
    saveloadnode *tmp = new(saveloadnode);

    tmp->forsaving = false;

    for (int32_t i=0; i<MAX_SAVES; i++)
    {
        tmp->inputslot[i] = -1;
        tmp->input_nodes[i] = NULL;
        memset(tmp->Names[i],0,SAVE_NAME_MAX_LEN+1);
    }

    return tmp;
}

ctrlnode *Ctrl_CreateNode(int type)
{
    ctrlnode *tmp;
    tmp = new(ctrlnode);
    tmp->node.unknown = NULL;
    tmp->slot = 0;
    tmp->type = CTRL_UNKN;
    tmp->func = NULL;

    switch(type)
    {
    case CTRL_PUSH:
        tmp->type = CTRL_PUSH;
        tmp->node.push = CreatePushNode();
        tmp->func = control_push;
        break;

    case CTRL_INPUT:
        tmp->type = CTRL_INPUT;
        tmp->node.inp = CreateInputNode();
        tmp->func = control_input;
        break;

    case CTRL_SLOT:
        tmp->type = CTRL_SLOT;
        tmp->node.slot = CreateSlotNode();
        tmp->func = control_slot;
        break;

    case CTRL_SAVE:
        tmp->type = CTRL_SAVE;
        tmp->node.svld = CreateSaveNode();
        tmp->func = control_save;
        break;

    };
    return tmp;
}

void __InitRect(Rect *rct)
{
    rct->h = 0;
    rct->w = 0;
    rct->x = 0;
    rct->y = 0;
}


bool Ctrl_Eligeblity(int obj, slotnode *slut)
{
    bool eli = false;

    for (int i=0; i< slut->eligable_cnt; i++)
        if (obj == slut->eligible_objects[i])
        {
            eli = true;
            break;
        }
    return eli;
}

void control_slot_draw(ctrlnode *nod)
{
    slotnode *slut = nod->node.slot;

    //rectangleRGBA(screen,slut->hotspot.x,slut->hotspot.y+GAME_Y,slut->hotspot.w,slut->hotspot.h+GAME_Y,255,0,0,255);


    int tmp1 = GetgVarInt(nod->slot);

    //printf("%d %d\n",nod->slot,tmp1);
    //bool in = Ctrl_Eligeblity(tmp1,slut);

    if (tmp1 != 0)
    {
        if (slut->srf==NULL)
        {
            char bff[16];
            sprintf(bff,"G0ZYU%2.2x1.tga",tmp1);
            char *fil = GetFilePath(bff);
            if (fil)
                slut->srf=LoadConvertImg(fil,Rend_MapScreenRGB(0,0,0));
        }

        if (slut->srf)
            Rend_DrawImageUpGamescr(slut->srf,    slut->rectangle.x,  slut->rectangle.y);
    }
    else
    {
        if (slut->srf != NULL)
        {
            SDL_FreeSurface(slut->srf);
            slut->srf=NULL;
        }

    }

}

void control_input_draw(ctrlnode *ct)
{
    inputnode *inp = ct->node.inp;

    if (strlen(inp->text))
    {
        if (inp->textchanged)
        {

            SDL_FillRect(inp->rect,NULL,0);

            if (!inp->readonly || !inp->focused)
                inp->textwidth = txt_DrawTxt(inp->text,&inp->string_init,inp->rect);
            else
                inp->textwidth = txt_DrawTxt(inp->text,&inp->string_chooser_init,inp->rect);

            inp->textchanged = false;
        }
        Rend_DrawImageUpGamescr(inp->rect,inp->rectangle.x,inp->rectangle.y);
    }
    else
        inp->textwidth = 0;


    if (FocusInput == ct->slot)
    {

        if (inp->readonly)
        {

        }
        else
        {

            if (inp->cursor != NULL)
            {

                Rend_DrawImageUpGamescr(inp->cursor,inp->rectangle.x + inp->textwidth,inp->rectangle.y,inp->frame);

                inp->period -= GetDTime();

                if (inp->period <= 0)
                {
                    inp->frame++;
                    inp->period = inp->cursor->info.time;
                }

                if (inp->frame >= inp->cursor->info.frames)
                    inp->frame = 0;
            }
        }

    }
}

void Ctrl_DrawControls()
{
    MList *ctrl = Getctrl();
    StartMList(ctrl);

    while (!eofMList(ctrl))
    {
        ctrlnode *nod=(ctrlnode *)DataMList(ctrl);
        if (nod->type == CTRL_SLOT)
            control_slot_draw(nod);
        else if (nod->type == CTRL_INPUT)
            control_input_draw(nod);


        NextMList(ctrl);
    }
}


void control_input(ctrlnode *ct)
{
    inputnode *inp = ct->node.inp;
    bool mousein = false;

    if (inp->readonly)
    {
        if ( inp->rectangle.x <= Rend_GetMouseGameX() &&\
                inp->rectangle.w >= Rend_GetMouseGameX() &&\
                inp->rectangle.y <= Rend_GetMouseGameY() &&\
                inp->rectangle.h >= Rend_GetMouseGameY() )
            mousein = true;
    }
    else
    {
        if ( inp->hotspot.x <= Rend_GetMouseGameX() &&\
                inp->hotspot.w >= Rend_GetMouseGameX() &&\
                inp->hotspot.y <= Rend_GetMouseGameY() &&\
                inp->hotspot.h >= Rend_GetMouseGameY() )
            mousein = true;
    }

    if (FocusInput == ct->slot)
        if (!inp->readonly)
        {
            if (KeyAnyHit())
            {
                SDLKey key = GetLastKey();
                int tmplen = strlen(inp->text);

                if (key>=SDLK_0 && key<=SDLK_9 ||
                        key>=SDLK_a && key<=SDLK_z ||
                        key==SDLK_SPACE)
                {
                    if (tmplen < SAVE_NAME_MAX_LEN)
                    {
                        inp->text[tmplen] = key;
                        inp->textchanged  = true;
                    }
                }
                else if(key == SDLK_BACKSPACE)
                {
                    if (tmplen > 0)
                    {
                        inp->text[tmplen-1] = 0x0;
                        inp->textchanged  = true;
                    }
                }
                else if(key == SDLK_RETURN)
                {
                    if (tmplen > 0)
                        inp->enterkey = true;
                }
                else if(key == SDLK_TAB)
                {
                    //TODO
                }

            }
        }

    if (inp->readonly)
    {
        if (FocusInput == ct->slot && !inp->focused)
        {
            inp->textchanged = true;
            inp->focused = true;
        }
        else if (FocusInput != ct->slot && inp->focused)
        {
            inp->textchanged = true;
            inp->focused = false;
        }
    }

    if (mousein)
    {
        if (!inp->readonly)
        {
            if (Mouse_IsCurrentCur(CURSOR_IDLE))
                Mouse_SetCursor(CURSOR_ACTIVE);

            if (MouseUp(SDL_BUTTON_LEFT))
            {
                FocusInput = ct->slot;
                FlushMouseBtn(SDL_BUTTON_LEFT);
            }
        }
        else if (inp->text[0]>=' ')
        {
            if (Mouse_IsCurrentCur(CURSOR_IDLE))
                Mouse_SetCursor(CURSOR_ACTIVE);

            FocusInput = ct->slot;

            if (MouseUp(SDL_BUTTON_LEFT))
            {
                FocusInput = 0;
                inp->enterkey = true;
            }

        }


    }
}

void control_slot(ctrlnode *ct)
{
    slotnode *slut = ct->node.slot;
    bool mousein = false;

    if ( slut->hotspot.x <= Rend_GetMouseGameX() &&\
            slut->hotspot.w >= Rend_GetMouseGameX() &&\
            slut->hotspot.y <= Rend_GetMouseGameY() &&\
            slut->hotspot.h >= Rend_GetMouseGameY() )
        mousein = true;

    if (mousein)
    {

        if (GetgVarInt(ct->slot)!=0)
            if (Mouse_IsCurrentCur(CURSOR_IDLE))
                Mouse_SetCursor(slut->cursor);

        if (MouseUp(SDL_BUTTON_LEFT))
        {
            if (GetgVarInt(SLOT_INVENTORY_MOUSE)==0)
            {
                SetgVarInt(SLOT_INVENTORY_MOUSE,GetgVarInt(ct->slot));
                SetgVarInt(ct->slot,0);

            }
            else if (Ctrl_Eligeblity(GetgVarInt(SLOT_INVENTORY_MOUSE),slut))
            {
                int te=GetgVarInt(ct->slot);
                SetgVarInt(ct->slot,GetgVarInt(SLOT_INVENTORY_MOUSE));
                SetgVarInt(SLOT_INVENTORY_MOUSE,te);
                SDL_FreeSurface(slut->srf);
                slut->srf=NULL;
            }
#ifdef TRACE
            printf("Pushed\n");
            printf("Slot #%d to 1\n",ct->slot);
#endif
            FlushMouseBtn(SDL_BUTTON_LEFT);

        }

    }
}

void control_push(ctrlnode *ct)
{
    bool mousein=false;

#ifdef FULLTRACE
    printf("Push_toggle\n");
#endif

    if (!Rend_MouseInGamescr())
        return;
    pushnode *psh = ct->node.push;

    if ( psh->x          <= Rend_GetMouseGameX() &&\
            psh->x+psh->w   >= Rend_GetMouseGameX() &&\
            psh->y          <= Rend_GetMouseGameY() &&\
            psh->y+psh->h   >= Rend_GetMouseGameY() )
        mousein = true;

    if (mousein)
    {
        if (!pushChangeMouse)
            if (Mouse_IsCurrentCur(CURSOR_IDLE))
            {
                Mouse_SetCursor(psh->cursor);
                pushChangeMouse = true;
            }

        if (MouseUp(SDL_BUTTON_LEFT))
        {
#ifdef TRACE
            printf("Pushed #%d\n",ct->slot);
#endif
            SetgVarInt(ct->slot,1);

            FlushMouseBtn(SDL_BUTTON_LEFT);

        }

    }
}


void control_save(ctrlnode *ct)
{
    saveloadnode *sv = ct->node.svld;
    for (int i=0; i<MAX_SAVES; i++)
        if (sv->inputslot[i] != -1)
            if (sv->input_nodes[i]->node.inp->enterkey)
                if(sv->forsaving)
                {
                    FILE *f = fopen("inquis.sav","wb");

                    for (int j=0; j<MAX_SAVES; j++)
                        if (j!=i)
                            fprintf(f,"%s\r\n",sv->Names[j]);
                        else
                            fprintf(f,"%s\r\n",sv->input_nodes[i]->node.inp->text);

                    fclose(f);

                    char fln[32];

                    sprintf(fln,"inqsav%d.sav",i+1);

                    ScrSys_SaveGame(fln);
                    SetNeedLocate('0','0','0','0',0);
                    break;
                }
                else
                {
                    char fln[32];

                    sprintf(fln,"inqsav%d.sav",i+1);

                    ScrSys_LoadGame(fln);
                    break;
                }

}


int Parse_Control_Flat()
{
    Rend_SetRenderer (RENDER_FLAT);
}



int Parse_Control_Panorama(FILE *fl)
{
    char  buf[FILE_LN_BUF];
    char *str;
    int  good = 0;

    double angle = 27.0;
    double     k = 0.55;
    int      tmp = 0;

    Rend_SetRenderer (RENDER_PANA);

    while (!feof(fl))
    {
        fgets(buf,FILE_LN_BUF,fl);
        str = PrepareString(buf);

        if (str[0] == '}')
        {
            good = 1;
            break;
        }
        else if (strCMP(str,"angle")==0)
        {
            str   = GetParams(str);
            angle = atof(str);
            Rend_pana_SetAngle(angle);
        }
        else if (strCMP(str,"linscale")==0)
        {
            str   = GetParams(str);
            k = atof(str);
            Rend_pana_SetLinscale(k);
        }
        else if (strCMP(str,"reversepana")==0)
        {
            str   = GetParams(str);
            tmp = atoi(str);
            if (tmp == 1)
                Rend_SetReversePana(true);
        }
        else if (strCMP(str,"zeropoint")==0)
        {

        }

    }

    //  printf("%f\n",angle);
    //  printf("%f\n",k);

    Rend_pana_SetTable();
}

int Parse_Control_Tilt(FILE *fl)
{
    char  buf[FILE_LN_BUF];
    char *str;
    int  good = 0;

    double angle = 27.0;
    double     k = 0.55;
    int      tmp = 0;

    Rend_SetRenderer (RENDER_TILT);

    while (!feof(fl))
    {
        fgets(buf,FILE_LN_BUF,fl);
        str = PrepareString(buf);

        if (str[0] == '}')
        {
            good = 1;
            break;
        }
        else if (strCMP(str,"angle")==0)
        {
            str   = GetParams(str);
            angle = atof(str);
            Rend_tilt_SetAngle(angle);
        }
        else if (strCMP(str,"linscale")==0)
        {
            str   = GetParams(str);
            k = atof(str);
            Rend_tilt_SetLinscale(k);
        }
        else if (strCMP(str,"reversepana")==0)
        {
            str   = GetParams(str);
            tmp = atoi(str);
            //if (tmp == 1)
            //Rend_SetReversePana(true);
        }
    }

    //  printf("%f\n",angle);
    //  printf("%f\n",k);

    Rend_tilt_SetTable();
}

int Parse_Control_Save(MList *controlst, FILE *fl, uint32_t slot)
{
    int good = 0;
    char buf[FILE_LN_BUF];
    char *str;

    ctrlnode *ctnode = Ctrl_CreateNode(CTRL_SAVE);
    saveloadnode *sv = ctnode->node.svld;

    AddToMList(controlst,ctnode);

    ctnode->slot      = slot;
    SetDirectgVarInt(slot,0);

    FILE *f = fopen("inquis.sav","rb");
    if (f == NULL)
    {
        f = fopen("inquis.sav","wb");
        for(int i=0; i< MAX_SAVES; i++)
            fprintf(f,"\r\n");
        fseek(f,0,SEEK_SET);
    }

    for (int i=0; i<MAX_SAVES; i++)
    {
        fgets(buf,FILE_LN_BUF,f);
        memset(sv->Names[i],0,SAVE_NAME_MAX_LEN+1);
        str = TrimRight(buf);
        if (strlen(str)>0)
        {
            char fln[32];
            sprintf(fln,"inqsav%d.sav",i+1);
            FILE *d = fopen(fln,"rb");
            if (d!= NULL)
            {
                strcpy(sv->Names[i],str);
                fclose(d);
            }
        }
    }

    fclose(f);



    while (!feof(fl))
    {
        fgets(buf,FILE_LN_BUF,fl);
        str = PrepareString(buf);

        if (str[0] == '}')
        {
            good = 1;
            break;
        }
        else if (strCMP(str,"savebox")==0)
        {
            str=GetParams(str);
            int ctrlslot, saveslot;
            sscanf(str,"%d %d",&saveslot,&ctrlslot);

            saveslot--;

            ctrlnode *nd = GetControlByID(ctrlslot);
            if (nd != NULL)
                if (nd->type == CTRL_INPUT)
                {
                    strcpy(nd->node.inp->text,sv->Names[saveslot]);
                    nd->node.inp->textchanged = true;
                    sv->inputslot[saveslot] = ctrlslot;
                    sv->input_nodes[saveslot] = nd;
                }


        }
        else if (strCMP(str,"control_type")==0)
        {
            str=GetParams(str);
            if (strCMP(str,"save") == 0)
                sv->forsaving = true;
            else if (strCMP(str,"restore") == 0)
                sv->forsaving = false;
            else
                printf("Unknown control_type: %s\n",str);

        }
        else
        {
            printf("Unknown parameter for save control: %s\n",str);
        }

    }//while (!feof(fl))

    for (int i=0; i< MAX_SAVES; i++)
        if (sv->inputslot[i] != -1)
        {
            ctrlnode *nd = sv->input_nodes[i];
            if (nd != NULL)
                if (nd->type == CTRL_INPUT)
                    nd->node.inp->readonly = !sv->forsaving;
        }

}

int Parse_Control_Input(MList *controlst, FILE *fl, uint32_t slot)
{
    int good = 0;
    char buf[FILE_LN_BUF];
    char *str;

    ctrlnode *ctnode = Ctrl_CreateNode(CTRL_INPUT);
    inputnode *inp = ctnode->node.inp;

    AddToMList(controlst,ctnode);

    ctnode->slot      = slot;
    SetDirectgVarInt(slot,0);

    while (!feof(fl))
    {
        fgets(buf,FILE_LN_BUF,fl);
        str = PrepareString(buf);

        if (str[0] == '}')
        {
            good = 1;
            break;
        }
        else if (strCMP(str,"rectangle")==0)
        {
            str=GetParams(str);
            sscanf(str,"%d %d %d %d",\
                   &inp->rectangle.x,\
                   &inp->rectangle.y,\
                   &inp->rectangle.w,\
                   &inp->rectangle.h);

            inp->rect = CreateSurface(inp->rectangle.w-inp->rectangle.x,inp->rectangle.h-inp->rectangle.y);
        }
        else if (strCMP(str,"aux_hotspot")==0)
        {
            str=GetParams(str);
            sscanf(str,"%d %d %d %d",\
                   &inp->hotspot.x,\
                   &inp->hotspot.y,\
                   &inp->hotspot.w,\
                   &inp->hotspot.h);
        }
        else if (strCMP(str,"cursor_animation_frames")==0)
        {
            // str=GetParams(str);
            //  inp->frame = atoi(str);
        }//if (str[0] == '}')
        else if (strCMP(str,"cursor_animation")==0)
        {
            str=GetParams(str);
            char file[16];

            sscanf(str,"%s",file);
            inp->cursor = LoadAnimImage(file,0);
        }
        else if (strCMP(str,"focus")==0)
        {
            str=GetParams(str);
            if (atoi(str) == 1)
                FocusInput = ctnode->slot;
        }
        else if (strCMP(str,"string_init")==0)
        {
            str=GetParams(str);
            char *tmp = GetSystemString(atoi(str));
            if (tmp != NULL)
                txt_readfontstyle(&inp->string_init,tmp);
        }
        else if (strCMP(str,"chooser_init_string")==0)
        {
            str=GetParams(str);
            char *tmp = GetSystemString(atoi(str));
            if (tmp != NULL)
                txt_readfontstyle(&inp->string_chooser_init,tmp);
        }

    }//while (!feof(fl))

}

int Parse_Control_Slot(MList *controlst, FILE *fl, uint32_t slot)
{
    int good = 0;
    char buf[FILE_LN_BUF];
    char *str;


    ctrlnode *ctnode = Ctrl_CreateNode(CTRL_SLOT);
    slotnode *slut = ctnode->node.slot;

    AddToMList(controlst,ctnode);
    ctnode->slot      = slot;
    slut->srf         = NULL;

    while (!feof(fl))
    {
        fgets(buf,FILE_LN_BUF,fl);
        str = PrepareString(buf);

        if (str[0] == '}')
        {
            good = 1;
            break;
        }
        else if (strCMP(str,"rectangle")==0)
        {
            str=GetParams(str);
            sscanf(str,"%d %d %d %d",\
                   &slut->rectangle.x,\
                   &slut->rectangle.y,\
                   &slut->rectangle.w,\
                   &slut->rectangle.h);
        }
        else if (strCMP(str,"hotspot")==0)
        {
            str=GetParams(str);
            sscanf(str,"%d %d %d %d",\
                   &slut->hotspot.x,\
                   &slut->hotspot.y,\
                   &slut->hotspot.w,\
                   &slut->hotspot.h);
        }
        else if (strCMP(str,"cursor")==0)
        {
            str=GetParams(str);
            for (int i=0; i<NUM_CURSORS; i++)
                if (strcasecmp(str,Mouse_GetName(i)) == 0)
                {
                    slut->cursor = i;
                    break;
                }
        }
        else if (strCMP(str,"eligible_objects")==0)
        {
            str=GetParams(str);
            int tmpobj=0;
            int strl=strlen(str);
            for (int i=0; i < strl; i++)
                if (str[i] == ' ')
                    tmpobj++;

            tmpobj++;

            slut->eligable_cnt = tmpobj;
            slut->eligible_objects = (int32_t *)malloc (tmpobj * sizeof(int32_t));
            int i=0;
            tmpobj=0;

            for (;;)
            {
                if (i>=strl)
                    break;
                if (str[i] != ' ')
                {
                    slut->eligible_objects[tmpobj] = atoi(str + i);
                    tmpobj++;

                    while (i<strl && str[i] != ' ')
                        i++;
                }
                i++;
            }//for (;;)
        }//if (str[0] == '}')
    }//while (!feof(fl))

}

int Parse_Control_PushTgl(MList *controlst, FILE *fl, uint32_t slot)
{
    int good = 0;
    char buf[FILE_LN_BUF];
    char *str;

    SetgVarInt(slot,0);



    ctrlnode *ctnode  = Ctrl_CreateNode(CTRL_PUSH);
    pushnode *psh     = ctnode->node.push;
    ctnode->slot      = slot;

    psh->cursor       = CURSOR_IDLE;

    while (!feof(fl))
    {
        fgets(buf,FILE_LN_BUF,fl);
        str = PrepareString(buf);

        if (str[0] == '}')
        {
            good = 1;
            break;
        }
        else if (strCMP(str,"flat_hotspot") == 0)
        {
            psh->flat=true;
            str = GetParams(str);
            sscanf(str,"%d, %d, %d, %d",&psh->x,&psh->y,&psh->w,&psh->h);
#ifdef FULLTRACE
            printf("    Flat %d %d %d %d %d\n",psh->x,psh->y,psh->w,psh->h,psh->flat);
#endif
        }
        else if (strCMP(str,"warp_hotspot") == 0)
        {
            psh->flat=true;
            str = GetParams(str);
            sscanf(str,"%d, %d, %d, %d",&psh->x,&psh->y,&psh->w,&psh->h);
#ifdef FULLTRACE
            printf("    Warp %d %d %d %d %d\n",psh->x,psh->y,psh->w,psh->h,psh->flat);
#endif
        }
        else if (strCMP(str,"cursor") == 0)
        {
            str = GetParams(str); //cursor
            psh->cursor = Mouse_GetCursorIndex(str);
        }
    }

    if (good == 1)
        AddToMList(controlst,ctnode);

    return good;
}

int Parse_Control(MList *controlst,FILE *fl,char *ctstr)
{
    int  good = 0;

    char buf[FILE_LN_BUF];
    char *str;

    uint32_t    slot;
    char      ctrltp[100];
    memset(ctrltp,0,100);

    sscanf(ctstr,"control:%d %s",&slot,ctrltp); //read slot number;


#ifdef FULLTRACE
    printf("Creating control:%d %s Creating object\n",slot,ctrltp);
#endif


    if (strCMP(ctrltp,"flat")==0)
    {
#ifdef FULLTRACE
        printf("    Flat Renderer\n");
#endif

        Parse_Control_Flat();
    }
    else if (strCMP(ctrltp,"pana")==0)
    {
#ifdef FULLTRACE
        printf("    Panorama Renderer\n");
#endif

        Parse_Control_Panorama(fl);
    }
    else if (strCMP(ctrltp,"tilt")==0)
    {
#ifdef FULLTRACE
        printf("    Tilt Renderer\n");
#endif

        Parse_Control_Tilt(fl);
    }
    else if (strCMP(ctrltp,"push_toggle")==0)
    {
        Parse_Control_PushTgl(controlst,fl,slot);
    }
    else if (strCMP(ctrltp,"input")==0)
    {
        Parse_Control_Input(controlst,fl,slot);
    }
    else if (strCMP(ctrltp,"save")==0)
    {
        Parse_Control_Save(controlst,fl,slot);
    }
    else if (strCMP(ctrltp,"slot")==0)
    {
        Parse_Control_Slot(controlst,fl,slot);
    }

    return good;
}



void ProcessControls(MList *ctrlst)
{

    pushChangeMouse = false;

    LastMList(ctrlst);

    while (!eofMList(ctrlst))
    {
        ctrlnode *nod=(ctrlnode *)DataMList(ctrlst);

#ifdef FULLTRACE
        printf("Control, slot:%d \n",nod->slot);
#endif

        if (!(ScrSys_GetFlag(nod->slot) & FLAG_DISABLED))  //(nod->enable)
            if (nod->func != NULL)
                nod->func(nod);

        PrevMList(ctrlst);
    }
}


void ctrl_Delete_PushNode(ctrlnode *nod)
{
    delete nod->node.push;
}

void ctrl_Delete_SlotNode(ctrlnode *nod)
{
    if (nod->node.slot->srf)
        SDL_FreeSurface(nod->node.slot->srf);
    if (nod->node.slot->eligible_objects)
        free(nod->node.slot->eligible_objects);
    delete nod->node.slot;
}

void ctrl_Delete_InputNode(ctrlnode *nod)
{
    if (nod->node.inp->cursor)
        FreeAnimImage(nod->node.inp->cursor);
    if (nod->node.inp->rect)
        SDL_FreeSurface(nod->node.inp->rect);
    delete nod->node.inp;
}

void ctrl_Delete_SaveNode(ctrlnode *nod)
{
    delete nod->node.svld;
}

void DeleteSelControl(ctrlnode *nod)
{
    switch (nod->type)
    {
    case CTRL_PUSH:
        ctrl_Delete_PushNode(nod);
        break;
    case CTRL_SLOT:
        ctrl_Delete_SlotNode(nod);
        break;
    case CTRL_INPUT:
        ctrl_Delete_InputNode(nod);
        break;
    case CTRL_SAVE:
        ctrl_Delete_SaveNode(nod);
        break;
    }

    delete nod;
}

void DeleteControlList(MList *lst)
{

    StartMList(lst);
    while (!eofMList(lst))
    {
        ctrlnode *nod=(ctrlnode *)DataMList(lst);

        DeleteSelControl(nod);

        NextMList(lst);
    }

    DeleteMList(lst);
}

void FlushControlList(MList *lst)
{
    StartMList(lst);
    while (!eofMList(lst))
    {
        ctrlnode *nod=(ctrlnode *)DataMList(lst);


        DeleteSelControl(nod);

        NextMList(lst);
    }

    FlushMList(lst);
}

ctrlnode *GetControlByID(int32_t id)
{
    MList *lst = Getctrl();
    StartMList(lst);
    while (!eofMList(lst))
    {
        ctrlnode *nod=(ctrlnode *)DataMList(lst);

        if (nod->slot == id)
            return nod;

        NextMList(lst);
    }

    return NULL;

}
