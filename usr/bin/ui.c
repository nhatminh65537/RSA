#include "ui.h"
#include "uilib.h"

BOX screen, rsaBox, cmdBox, keyBox, optBox;

void initUI()
{
    initBox(&screen, "SCREEN"     , NULL   , FIX  , 0 , 0, MAXX, MAXY);
    initBox(&rsaBox, "RSA-PROJECT", &screen, ABS  , 1 , 1, FULL, FULL);
    initBox(&cmdBox, "COMMAND"    , &rsaBox, ABS  , 1 , 1, FULL, 4   );
    initBox(&keyBox, "KEY"        , &rsaBox, GRID , 1 , 5, 48  , FULL);
    initBox(&optBox, "OUTPUT"     , &rsaBox, FLEXX, 49, 5, FULL, FULL);

    drawBox(&rsaBox, BCYAN, LIGHT , CURVE , BCYAN);
    drawBox(&cmdBox, WHITE, DOUBLE, DOUBLE, WHITE);
    drawBox(&keyBox, WHITE, HEAVY , HEAVY , WHITE);
    drawBox(&optBox, WHITE, HEAVY , HEAVY , WHITE);
}