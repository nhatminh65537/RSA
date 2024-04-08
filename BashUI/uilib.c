#include "uilib.h"

char *boxhline [] = {
    "\u2500", "\u2501", "\u2550", "\u2508", "\u2509", "\u2504", "\u2505", "\u254c", "\u254d"
};
char *boxvline [] = {
    "\u2502", "\u2503", "\u2551", "\u250a", "\u250b", "\u2506", "\u2507", "\u254e", "\u254f"
};
char *boxcorner[] = {
    "\u2510", "\u2513", "\u2557", "\u256e",
    "\u250c", "\u250f", "\u2554", "\u256d",
    "\u2514", "\u2517", "\u255a", "\u2570",
    "\u2518", "\u251b", "\u255d", "\u256f"
};

void clearBlock(int x1, int y1, int x2, int y2)
{
    char erase[MAXX + 1];
    for (int x = 0; x <= x2-x1; ++x) erase[x] = ' ';
    erase[x2-x1 + 1] = 0;

    FG_256(15); BG_256(0);
    for (int y = y1; y <= y2; ++y)
    {
        MOVEXY(x1, y);
        printf(erase);
    }
    MOVEXY(x1, y1);
}

void drawBox(int x1, int y1, int x2, int y2,
             int line_type, int corner_type, char* name)
{
    MOVEXY(x2, y1); printf(boxcorner[0*4 + corner_type]);
    MOVEXY(x1, y1); printf(boxcorner[1*4 + corner_type]);
    MOVEXY(x1, y2); printf(boxcorner[2*4 + corner_type]);
    MOVEXY(x2, y2); printf(boxcorner[3*4 + corner_type]);
    for (int x = x1 + 1; x < x2; ++x)
    {
        FILLXY(x, y1, boxhline[line_type]);
        FILLXY(x, y2, boxhline[line_type]);
    }
    for (int y = y1 + 1; y < y2; ++y)
    {
        FILLXY(x1, y, boxvline[line_type]);
        FILLXY(x2, y, boxvline[line_type]);
    }
    MOVEXY(x1+2, y1);
    printf("\x1b[1m%s\x1b[0m", name);
    MOVEXY(x1+1, y1+1);
}