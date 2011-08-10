#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "Subtitles.h"

#define RENDER_FLAT 0
#define RENDER_PANA 1
#define RENDER_TILT 2


struct struct_SubRect
{
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
    SDL_Surface *img;
    int32_t id;
    bool todelete;
};

void Rend_DrawImageToScr(SDL_Surface *scr,int x, int y);

void Rend_DrawImageToGamescr(SDL_Surface *scr,int x, int y);
void Rend_DrawImageToGamescr(anim_surf *scr,int x, int y, int frame);

void Rend_LoadGamescr(char *path);

void Rend_ProcessCursor();

int  Rend_GetMouseGameX();
int  Rend_GetMouseGameY();
bool Rend_MouseInGamescr();

int Rend_GetPanaWidth();
void Rend_SetReversePana(bool pana);

void Rend_SetRenderer(int meth);
void Rend_pana_SetTable();
void Rend_pana_SetAngle(float angle);
void Rend_pana_SetLinscale(float linscale);

void Rend_tilt_SetTable();
void Rend_tilt_SetAngle(float angle);
void Rend_tilt_SetLinscale(float linscale);
void Rend_DrawTilt();

void Rend_DrawImageUpGamescr(SDL_Surface *scr,int x, int y);
void Rend_DrawImageUpGamescr(anim_surf *scr,int x, int y, int frame);

int  Rend_GetRenderer();
void Rend_ProcessCursor();

void Rend_PanaMouseInteract();
void Rend_MouseInteractOfRender();
void Rend_tilt_MouseInteract();

void Rend_RenderFunc();
void Rend_InitGraphics(bool fullscreen,char *fontpath);
void Rend_SwitchFullscreen();

void Rend_SetDelay(int32_t delay);


struct_SubRect *Rend_CreateSubRect(int x, int y, int w, int h);
void Rend_DeleteSubRect(struct_SubRect *erect);
void Rend_ClearSubs();
void Rend_InitSubList();
void Rend_ProcessSubs();
struct_SubRect *Rend_GetSubById( int id);

SDL_Surface *Rend_GetGameScreen();

uint32_t Rend_MapScreenRGB(int r, int g, int b);
void Rend_ScreenFlip();

struct struct_distort
{


};

#endif // RENDER_H_INCLUDED
