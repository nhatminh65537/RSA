#ifndef __UILIB__
#define __UILIB__

#include <stdio.h>

#define MAXX     150
#define MAXY     50
#define TRUE     1
#define FALSE    0

#define MOVEXY(x, y)       printf("\x1b[%d;%dH", y, x)
#define FILLXY(x, y, c)    MOVEXY(x, y); printf(c)
#define FG_256(n)          printf("\x1b[38;5;%dm", n)
#define FG_RGB(r, g, b)    printf("\x1b[38;2;%d;%d;%dm]", r, g, b)
#define BG_256(n)          printf("\x1b[48;5;%dm", n)
#define BG_RGB(r, g, b)    printf("\x1b[48;2;%d;%d;%dm]", r, g, b)
#define CSI(n)             printf("\x1b[%dm", n)
#define CLRSCR             clearBlock(1, 1, MAXX, MAXY)
#define SAVECURSOR         printf("\x1b[s")
#define RESTORECURSOR      printf("\x1b[u")

extern char *boxhline [];
extern char *boxvline [];
extern char *boxcorner[];

enum Color{
    BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, GRAY,
    BRED, BGREEN, BYELLOW, BBLUE, BMAGENTA, BCRYAN, BWHITE,
};

enum BoxStyle{
    LIGHT , HEAVY , DOUBLE, CURVE , L4DASH = 3, 
    H4DASH, L3DASH, H3DASH, L2DASH, H2DASH
};

enum BoxCorner{
    TOPRIGHT, TOPLEFT, BOTTOMLEFT, BOTTOMRIGHT
};

enum CSI{
    RESET, BOLD, DIM, ITALIC, UNDERLINE, BLINK, INVERSE = 7,
    INVISIBLE, STRIKEOUT, DOUBLE_UNDERLINE = 21, OVERLINE = 53,
    NORMAL_INTENSITY = 22, NOT_ITALIC, NOT_UNDERLINE, NOT_BLINK,
    NOT_INVERSE = 27, NOT_INVISIBLE, NOT_STRIKEOUT, NOT_OVERLINE = 55,
    DEFAULT_FG = 39, DEFAULT_BG = 49
};

void clearBlock(int x1, int y1, int x2, int y2);
void drawBox(int x1, int y1, int x2, int y2, int line_type, int corner_type, char* name);

#endif