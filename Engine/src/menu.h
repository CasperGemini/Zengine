#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#define menu_BAR_EXIT      0x1
#define menu_BAR_SETTINGS  0x2
#define menu_BAR_RESTORE   0x4
#define menu_BAR_SAVE      0x8
#define menu_BAR_ITEM      0x100
#define menu_BAR_MAGIC     0x200


void menu_SetMenuBarVal(uint16_t val);
uint16_t menu_GetMenuBarVal();

void menu_LoadGraphics_zgi();
void menu_UpdateMenuBar_zgi();
void menu_DrawMenuBar_zgi();

void menu_LoadGraphics_znemesis();
void menu_UpdateMenuBar_znemesis();
void menu_DrawMenuBar_znemesis();

#endif // MENU_H_INCLUDED
