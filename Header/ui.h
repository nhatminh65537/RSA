#ifndef __UIRSA__
#define __UIRSA__

#include "../header/uilib.h"

#define MAXX       160
#define MAXY       50
// #define MAXX       155
// #define MAXY       44
#define CLRSCR     clearBlock(1, 1, MAXX, MAXY)
#define KEYSIZE    42    

extern BOX screen, rsaBox, cmdBox, keyBox, wrkBox, logBox, cptBox, pltBox,
           eBox  , dBox  , qBox  , pBox  , nBox;
void initUI();
void inputCmd();
void clsInput();

#endif