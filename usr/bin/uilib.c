#include "uilib.h"
#include <conio.h>
#include <string.h>

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
    char erase[ERASESIZE];
    for (int x = 0; x <= x2-x1; ++x) erase[x] = ' ';
    erase[x2-x1 + 1] = 0;

    CSI(DEFAULT_BG); CSI(DEFAULT_FG);
    for (int y = y1; y <= y2; ++y){
        MOVEXY(x1, y);
        printf(erase);
    }
    MOVEXY(x1, y1);
}

void drawBox(BOX* box, int lineColor,
             int lineType, int cornerType, int titleColor)
{
    FG_256(lineColor);
    MOVEXY(box->right, box->top   ); printf(boxcorner[0*4 + cornerType]);
    MOVEXY(box->left , box->top   ); printf(boxcorner[1*4 + cornerType]);
    MOVEXY(box->left , box->bottom); printf(boxcorner[2*4 + cornerType]);
    MOVEXY(box->right, box->bottom); printf(boxcorner[3*4 + cornerType]);
    for (int x = box->left + 1; x < box->right; ++x){
        FILLXY(x, box->top   , boxhline[lineType]);
        FILLXY(x, box->bottom, boxhline[lineType]);
    }
    for (int y = box->top + 1; y < box->bottom; ++y){
        FILLXY(box->left , y, boxvline[lineType]);
        FILLXY(box->right, y, boxvline[lineType]);
    }
    MOVEXY(box->left + 2, box->top);
    printf("\x1b[1m%s\x1b[0m", box->name);
    MOVEXY(box->left + 1, box->top + 1 );
}

void initBox(BOX* newBox, char* name, BOX* parent, enum LayoutType layout,
             int x, int y, int sx, int sy)
{
    newBox->state = FALSE;
    strcpy(newBox->name, name);

    newBox->childCnt = 0;
    newBox->tPoint   = 0;

    newBox->text     = NULL;
    
    newBox->layout = layout;
    
    newBox->parent = parent;
    
    if (parent == NULL){
        newBox->top    = y;
        newBox->left   = x;
        newBox->bottom = y + sy;
        newBox->right  = x + sx;
        return;
    }

    
    parent->child[parent->childCnt++] = newBox;
    switch (parent->layout)
    {
    case FIX:
        newBox->top    = y;
        newBox->left   = x;
        
        BOX* root = newBox;
        while (root->parent != NULL) root = root->parent;
        
        newBox->bottom = y + sy - 1 >= root->bottom? root->bottom: y + sy - 1;
        newBox->right  = x + sx - 1 >= root->right ? root->right : x + sy - 1;
        break;
    
    case ABS:
        newBox->top    = y + parent->top ;
        newBox->left   = x + parent->left;
        
        switch (sx)
        {
        case FULL:
            newBox->right  = parent->right  - 1;
            break;
        case OVER:
            newBox->right  = parent->right;
            break;
        default:
            newBox->right  = x + parent->left + sx - 1>= parent->right  - 1? 
                             parent->right: x + parent->left + sx - 1;
            break;
        }

        switch (sy)
        {
        case FULL:
            newBox->bottom  = parent->bottom  - 1;
            break;
        case OVER:
            newBox->bottom  = parent->bottom;
            break;
        default:
            newBox->bottom = y + parent->top  + sy - 1 >= parent->bottom - 1? 
                             parent->bottom: y + parent->top  + sy - 1;
            break;
        }
    }

    newBox->xPoint   = newBox->left + 1;
    newBox->yPoint   = newBox->top  + 1;
}

void showText(BOX* box)
{
    int breakWord;
    char *c = &box->text[box->tPoint];

    MOVEXY(box->xPoint, box->yPoint);
    printf("\u2596");
    ++box->xPoint;
    while (*c!=0) 
    {   
        if (box->xPoint > box->right - 1 || *c == '\r' || *c == '\n')
        {
            ++box->yPoint;
            box->xPoint = box->left + 1;
            if (!breakWord) ++c; 
        }
        if (box->xPoint == box->right - 1)
        {
            if (*c != ' ' & *(c + 1) != ' ') 
            {
                breakWord = TRUE;
                if (*(c - 1) != ' ')
                    putch('-');
                ++box->xPoint;
                continue;
            }
            else breakWord = FALSE;
        }
        if (box->yPoint > box->bottom - 1)
            return;
        MOVEXY(box->xPoint, box->yPoint);
        if (32 <= *c & *c <= 126)
        {
            ++box->xPoint;
            putch(*c);
            ++c;
        }
        breakWord = FALSE;
    }
    box->xPoint = box->left + 1;
    ++box->yPoint;
}