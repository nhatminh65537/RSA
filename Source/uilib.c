#include "../header/uilib.h"
#include <conio.h>
#include <string.h>
#include <math.h>

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
char *splitline[] = {
    "\u2534", "\u253b", "\u2569",
    "\u2524", "\u252b", "\u2563",
    "\u252c", "\u2533", "\u2566",
    "\u251c", "\u2523", "\u2560"
};
char *intersect[] = {
    "\u253c", "\u254b", "\u256c"
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
             int lineType, int cornerType, int titleColor, int titleBold)
{
    if (!box->enable) return;

    BOX* parent = box->parent;

    FG_256(lineColor);
    FILLXY(box->right, box->top   , boxcorner[0*4 + cornerType]);
    FILLXY(box->left , box->top   , boxcorner[1*4 + cornerType]);
    FILLXY(box->left , box->bottom, boxcorner[2*4 + cornerType]);
    FILLXY(box->right, box->bottom, boxcorner[3*4 + cornerType]);
    
    if (box->right == parent->right && box->top    != parent->top   ){
        FILLXY(box->right, box->top   , splitline[1*3 + lineType]);
    }
    if (box->right != parent->right && box->top    == parent->top   ){
        FILLXY(box->right, box->top   , splitline[2*3 + lineType]);
    }
    if (box->left  == parent->left  && box->top    != parent->top   ){
        FILLXY(box->left , box->top   , splitline[3*3 + lineType]);
    }
    if (box->left  != parent->left  && box->top    == parent->top   ){
        FILLXY(box->left , box->top   , splitline[2*3 + lineType]);
    }
    if (box->left  == parent->left  && box->bottom != parent->bottom){
        FILLXY(box->left , box->bottom, splitline[3*3 + lineType]);
    }
    if (box->left  != parent->left  && box->bottom == parent->bottom){
        FILLXY(box->left , box->bottom, splitline[0*3 + lineType]);
    }
    if (box->right == parent->right && box->bottom != parent->bottom){
        FILLXY(box->right, box->bottom, splitline[1*3 + lineType]);
    }
    if (box->right != parent->right && box->bottom == parent->bottom){
        FILLXY(box->right, box->bottom, splitline[0*3 + lineType]);
    }
    
    if (parent->layout == FLEXX){
        int pre = -1;
        for (int i = 0; box != parent->child[i]; ++i)
            if (parent->child[i]->enable) pre = i;
        
        if (pre != -1 && parent->child[pre]->right == box->left){
            BOX* preBox = parent->child[pre];
            if (box->top == preBox->top){
                FILLXY(box->left, box->top, splitline[2*3 + lineType]);
            }
            if (box->top != parent->top && box->top > preBox->top){
                FILLXY(box->left, box->top, splitline[3*3 + lineType]);
            }
            if (box->top != parent->top && box->top < preBox->top){
                FILLXY(preBox->right, preBox->top, splitline[1*3 + lineType]);
            }

            if (box->bottom == preBox->bottom){
                FILLXY(box->left    , box->bottom   , splitline[0*3 + lineType]);
            }
            if (box->bottom != parent->bottom && box->bottom < preBox->bottom){
                FILLXY(box->left    , box->bottom   , splitline[3*3 + lineType]);
            }
            if (box->bottom != parent->bottom && box->bottom > preBox->bottom){
                FILLXY(preBox->right, preBox->bottom, splitline[1*3 + lineType]);
            }
        }
    }

    if (parent->layout == GRID){
        if (parent->gridType == INNER){
            int b1 = 1, b2 = 1, b3 = 1, b4 = 1, b5 = 1, x = box->x, y = box->y;
            if (y == 1            || x == parent->xBox || parent->child[(y-2)*parent->xBox + (x  )] == NULL)
                b1 = 0;
            if (y == 1                                 || parent->child[(y-2)*parent->xBox + (x-1)] == NULL)
                b2 = 0;
            if (y == 1            || x == 1            || parent->child[(y-2)*parent->xBox + (x-2)] == NULL)
                b3 = 0;
            if (                     x == 1            || parent->child[(y-1)*parent->xBox + (x-2)] == NULL)
                b4 = 0;
            if (y == parent->yBox || x == 1            || parent->child[(y  )*parent->xBox + (x-2)] == NULL)
                b5 = 0;
            
            if (b1 == 1){
                FILLXY(box->right, box->top, intersect[lineType]);
            }
            else if (b2 == 1){
                FILLXY(box->right, box->top, splitline[1*3 + lineType]);
            }
            if (b3 == 1 || (b2 == 1 && b4 == 1)){
                FILLXY(box->left, box->top, intersect[lineType]);
            } 
            else{
                if (b2 == 1){
                    FILLXY(box->left, box->top, splitline[3*3 + lineType]);
                }
                if (b4 == 1){
                    FILLXY(box->left, box->top, splitline[2*3 + lineType]);
                }
            }
            if (b5 == 1){
                FILLXY(box->left, box->bottom, intersect[lineType]);
            }
            else if (b4 == 1){
                FILLXY(box->left, box->bottom, splitline[0*3 + lineType]);
            }
        }
    }
             
    for (int x = box->left + 1; x < box->right; ++x){
        FILLXY(x, box->top   , boxhline[lineType]);
        FILLXY(x, box->bottom, boxhline[lineType]);
    }
    for (int y = box->top + 1; y < box->bottom; ++y){
        FILLXY(box->left , y, boxvline[lineType]);
        FILLXY(box->right, y, boxvline[lineType]);
    }
    MOVEXY(box->left + 2, box->top);

    if (titleBold) CSI(NORMAL_INTENSITY), CSI(BOLD);
    FG_256(titleColor);
    printf(box->name);
    MOVEXY(box->left + 1, box->top + 1 );
}

void initBox(BOX* newBox, char* name, BOX* parent, enum LayoutType layout,
             int x, int y, int sx, int sy)
{
    newBox->enable = FALSE;
    strcpy(newBox->name, name);
    // for (int i = 0; i < MAXCHILD; ++i) 

    newBox->childCnt       = 0;
    newBox->childEnableCnt = 0;
    newBox->tPoint         = 0;
    newBox->text           = NULL;
    newBox->textEnable     = FALSE;
    newBox->layout         = layout;
    newBox->parent         = parent;
    newBox->sx             = sx;
    newBox->sy             = sy;
    
    if (parent == NULL){
        newBox->top    = y;
        newBox->left   = x;
        newBox->bottom = y + sy + 1;
        newBox->right  = x + sx + 1;
        return;
    }
    
    if (parent->layout != GRID)
        parent->child[parent->childCnt++] = newBox;
    else{
        parent->child[(y-1)*parent->xBox + (x-1)] = newBox;
        ++parent->childCnt;
    }

    switch (parent->layout){
        case FIX:
            newBox->top    = y;
            newBox->left   = x;
            
            BOX* root = newBox;
            while (root->parent != NULL) root = root->parent;
            
            newBox->bottom = y + sy - 1 >= root->bottom? root->bottom - 1: y + sy - 1;
            newBox->right  = x + sx - 1 >= root->right ? root->right  - 1: x + sy - 1;
            break;
        
        case ABS:
            newBox->top    = y + parent->top ;
            newBox->left   = x + parent->left;
            
            if (sx == FULL)
                newBox->right  = parent->right  - 1;
            else if (sx == OVER || x + parent->left + sx - 1>= parent->right  - 1)
                newBox->right  = parent->right;
            else
                newBox->right  = x + parent->left + sx - 1;


            if (sy == FULL)
                newBox->bottom  = parent->bottom  - 1;
            else if (sy == OVER || y + parent->top  + sy - 1 >= parent->bottom - 1)
                newBox->bottom  = parent->bottom;
            else
                newBox->bottom = y + parent->top  + sy - 1;
            break;

        case FLEXX:
            newBox->top    = 1;
            newBox->left   = 1;
            newBox->bottom = sy;
            newBox->right  = sx;
            break;

        case GRID:
            if (parent->gridType == INNER){
                newBox->x = x;
                newBox->y = y;
                // unsigned freeSpaceX = parent->right - parent->left - 1,
                //          freeSpaceY = parent->bottom - parent->top - 1;
                
                // for (int i = parent->xBox; i = parent->xBox + 1 - x; ++i) 
                //     freeSpaceX = freeSpaceX - round((float)freeSpaceX/i);
                newBox->left  = parent->left + 1 + round((float)(parent->right - parent->left - 2)*(x-1)  /parent->xBox);
                newBox->right = parent->left + 1 + round((float)(parent->right - parent->left - 2)*(x-1+sx)/parent->xBox);

                // for (int i = parent->yBox; i = parent->yBox + 1 - y; ++i) 
                //     freeSpaceY = freeSpaceY - round((float)freeSpaceY/i);
                // newBox->top    = parent->bottom - freeSpaceX + 1;
                // newBox->bottom = newBox->top    + round((float)freeSpaceY/(parent->yBox + 1 - y));
                newBox->top    = parent->top + 1 + round((float)(parent->bottom - parent->top - 2)*(y-1)   /parent->yBox);
                newBox->bottom = parent->top + 1 + round((float)(parent->bottom - parent->top - 2)*(y-1+sy)/parent->yBox);
            }        
            break;
    }

    newBox->xPoint   = newBox->left + 1;
    newBox->yPoint   = newBox->top  + 1;
}

void enableBox(BOX* box, int val)
{
    if (box->enable == val) return;

    BOX* parent = box->parent;
    switch (parent->layout)
    {
    case FIX:
    case ABS:
        box->enable = val;
        if (box->enable)
            ++parent->childEnableCnt;
        else --parent->childEnableCnt;
        break;
    
    case FLEXX:
        box->enable = val;
        if (box->enable)
            ++parent->childEnableCnt;
        else --parent->childEnableCnt;

        int freeSpace         = parent->right - parent->left - 1,
            remainEnableChild = parent->childEnableCnt,
            maxX;
        for (BOX** child = parent->child; child - parent->child < parent->childCnt; ++child){
            if ((*child)->enable == TRUE){
                if ((*child)->sy == FULL){
                    (*child)->top    = parent->top    + 1;
                    (*child)->bottom = parent->bottom - 1;
                }
                else if ((*child)->sy == OVER || (*child)->sy > parent->bottom - parent->top + 1){
                    (*child)->top    = parent->top   ;
                    (*child)->bottom = parent->bottom;
                }
                else{
                    (*child)->top    = parent->top + (parent->bottom - parent->top + 1 - box->sy)/2;
                    (*child)->bottom = (*child)->top + (*child)->sy - 1;
                }
                
                (*child)->left = parent->right - freeSpace;
                maxX = round((float) freeSpace / remainEnableChild);
                --remainEnableChild;
                freeSpace -= maxX;
                if ((*child)->sx == FULL){
                    (*child)->right = (*child)->left + maxX - 1;
                }
                else if ((*child)->sx == OVER || (*child)->sx > maxX){
                    (*child)->right = (*child)->left + maxX;
                }
                else{
                    (*child)->right = (*child)->left + (*child)->sy - 1;
                    freeSpace += maxX - (*child)->sy;
                }
                enableText(*child, (*child)->textEnable, (*child)->text);
            }
        }
        break;
    
    case GRID:
        box->enable = val;
        if (box->enable)
            ++parent->childEnableCnt;
        else --parent->childEnableCnt;
        break;
    //         if (parent->gridType == INNER){
    //             unsigned freeSpaceX = parent->right - parent->left - 1,
    //                      freeSpaceY = parent->bottom - parent->top - 1;
                
    //             for (int i = parent->xBox; i = parent->xBox + 1 - x; ++i) 
    //                 freeSpaceX = freeSpaceX - round((float)freeSpaceX/i);
    //             box->left  = parent->right - freeSpaceX + 1;
    //             box->right = box->left  + round((float)freeSpaceX/(parent->xBox + 1 - x));

    //             for (int i = parent->yBox; i = parent->yBox + 1 - y; ++i) 
    //                 freeSpaceY = freeSpaceY - round((float)freeSpaceY/i);
    //             box->top    = parent->bottom - freeSpaceX + 1;
    //             box->bottom = box->top    + round((float)freeSpaceY/(parent->yBox + 1 - y));
    //         }     
        
    }
    clearBox(parent);
    show(parent);
}

void resetText(BOX* box)
{
    box->tPoint  = 0;
    box->xPoint  = box->left + 1;
    box->yPoint  = box->top  + 1;
}

void showText(BOX* box)
{
    if (!box->textEnable || !box->enable) return;

    int breakWord;
    char *c = &box->text[box->tPoint];

    MOVEXY(box->xPoint, box->yPoint);
    CSI(RESET);
    while (*c!=0) 
    {   
        if (*c == '\x1b'){
            char esc[16], *i = esc;
            while (*c != 'm'){
                *i = *c;
                ++i;
                ++c;
            }
            *i++ = *c++;
            *i = 0;
            printf(esc);
            continue;
        }
        if (box->xPoint > box->right - 1 || *c == '\r' || *c == '\n')
        {
            for (int i = box->xPoint; i < box->right; ++i) putch(' ');
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
    box->tPoint = c - box->text;

    for (int i = box->xPoint; i < box->right; ++i) putch(' ');
    clearBlock(box->left + 1, box->yPoint + 1, box->right - 1, box->bottom - 1);
}

void clearBox(BOX* box)
{
    clearBlock(box->left + 1, box->top + 1, box->right - 1, box->bottom - 1);
    box->xPoint = box->left + 1;
    box->yPoint = box->top  + 1;
    box->tPoint = 0;
}

void show(BOX* box)
{
    if (box->show == NULL) return;
    box->show();
    for (BOX** child = box->child; child - box->child < MAXCHILD; ++child){
        if (*child == NULL) continue;
        show(*child);
    }
}

void enableText(BOX* box, int val, char* text)
{
    box->textEnable = val;
    box->text       = text;
    box->xPoint     = box->left + 1;
    box->yPoint     = box->top  + 1;
    box->tPoint     = 0;
}