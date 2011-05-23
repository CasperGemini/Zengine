


#ifndef ANIMS_H_INCLUDED
#define ANIMS_H_INCLUDED

struct animnode
{
    int      x;
    int      y;
    int      w;
    int      h;
    int  start;
    int    end;
    int   loopcnt;
    int   unk1; //???
    int   unk2;
    int   mask;
    int   framerate;
    int  CurFr;
    int  nexttick;
    int  loops;
    union anim
    {
        anim_surf *rlf;
        anim_avi  *avi;
    } anim;
    bool   vid;
};

struct animprenode{
    char    *fil;
    int32_t   u1;
    int32_t   u2;
    int32_t   mask;
    int32_t   framerate;
};



struct_action_res *anim_CreateAnimNode();
struct_action_res *anim_CreateAnimPreNode();

int anim_ProcessAnim(struct_action_res *nod);
int anim_DeleteAnimNod(struct_action_res *nod);
int anim_DeleteAnimPreNod(struct_action_res *nod);

#endif // ANIMS_H_INCLUDED
