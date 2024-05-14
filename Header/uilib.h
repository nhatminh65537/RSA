#ifndef __UILIB__
#define __UILIB__

#include <stdio.h>

#define TRUE         1
#define FALSE        0
#define ERASESIZE    512
#define NAMELEN      128
#define NAN          0
#define FULL         -1
#define OVER         0
#define MAXCHILD     128
 
#define MOVEXY(x, y)       printf("\x1b[%d;%dH", y, x)
#define CUU(n)             printf("\x1b[%dA", n)
#define CUD(n)             printf("\x1b[%dB", n)
#define CUF(n)             printf("\x1b[%dC", n)
#define CUB(n)             printf("\x1b[%dD", n)
#define FILLXY(x, y, c)    MOVEXY(x, y); printf(c)
#define FG_256(n)          printf("\x1b[38;5;%dm", n)
#define FG_RGB(r, g, b)    printf("\x1b[38;2;%d;%d;%dm]", r, g, b)
#define BG_256(n)          printf("\x1b[48;5;%dm", n)
#define BG_RGB(r, g, b)    printf("\x1b[48;2;%d;%d;%dm]", r, g, b)
#define CSI(n)             printf("\x1b[%dm", n)
#define SAVECURSOR         printf("\x1b[s")
#define RESTORECURSOR      printf("\x1b[u")

extern char *boxhline [];
extern char *boxvline [];
extern char *boxcorner[];
extern char *splitline[];
extern char *intersect[];

enum Color{
    BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, GRAY,
    BRED, BGREEN, BYELLOW, BBLUE, BMAGENTA, BCYAN, BWHITE,
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
enum LayoutType{
    FLEXX,
    FLEXY,
    GRID,
    FIX,
    ABS,
};
enum GridType{
    INNER,
    CONNECT,
    SPLIT
};
enum Key{
    BACKSPACE = 8, RETURN = 13, TAB = 9,
    UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77,    
    HOME = 71, DEL = 83, PGUP = 73, PGDN = 81,
    EXIT = 27, EXT = -32
};

typedef struct Box
{
    int enable;
    enum LayoutType layout;
    char name[NAMELEN];
    
    unsigned top;
    unsigned left;
    unsigned bottom;
    unsigned right;
    int sx;
    int sy;
    
    struct Box *parent;
    struct Box *child[MAXCHILD];
    unsigned childCnt;
    unsigned childEnableCnt;
    
    int textEnable;
    char *text;
    unsigned xPoint;
    unsigned yPoint;
    unsigned tPoint;
    
    void (*show)();

    // For grid
    unsigned xBox;
    unsigned yBox;
    enum GridType gridType;
    unsigned x;
    unsigned y;
} BOX;


void initBox(BOX* cur, char* name, BOX* parent, enum LayoutType layout,
             int x, int y, int sx, int sy);
void clearBlock(int x1, int y1, int x2, int y2);
void drawBox(BOX* box, int lineColor,
             int lineType, int cornerType, int titleColor, int titleBold);
void resetText(BOX* box);
void showText(BOX* box);
void clearBox(BOX* box);
void enableBox(BOX* box, int val);
void enableText(BOX* box, int val, char* text);
void show(BOX* box);

#endif