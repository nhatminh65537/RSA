#include <string.h>
#include <conio.h>
#include "../header/cmd.h"
#include "../header/int256.h"
#include "../header/ui.h"
#include "../header/prgvar.h"
#include "../header/uilib.h"
#include "../header/outlog.h"

int runCmd()
{
    char *c = cmd.string;
    
    while (*c)
    {    
        c = phraseCmd(c);
        if (strcmp(cmdArr[0], "exit") == 0)
            return 0;

        if (strcmp(cmdArr[0], "focus") == 0){
            printf("\x1b[?25l");
            if (strcmp(cmdArr[1], "plt") == 0) focus(&plainText);
            if (strcmp(cmdArr[1], "log") == 0) focusOutText(&outText);
            printf("\x1b[?25h");
        }
        if (strcmp(cmdArr[0], "load") == 0)
            loadToText(&plainText, cmdArr[1], &pltBox);

        if (strcmp(cmdArr[0], "cls") == 0){
            clearBox(&optBox);
        }
        if (strcmp(cmdArr[0], "disable") == 0){
            if (strcmp(cmdArr[1], "plt") == 0){
                cptBox.sx = FULL;
                enableBox(&pltBox, FALSE);
            }
            if (strcmp(cmdArr[1], "cpt") == 0) 
                enableBox(&cptBox, FALSE);
            if (strcmp(cmdArr[1], "log") == 0) 
                enableBox(&logBox, FALSE);
            clearBox(&optBox);
            show(&optBox);
        }
        if (strcmp(cmdArr[0], "enable") == 0){
            if (strcmp(cmdArr[1], "plt") == 0){
                cptBox.sx = OVER;
                enableBox(&pltBox, TRUE);
            } 
            if (strcmp(cmdArr[1], "cpt") == 0) enableBox(&cptBox, TRUE);
            if (strcmp(cmdArr[1], "log") == 0) enableBox(&logBox, TRUE);
            clearBox(&optBox);
            show(&optBox);
        }
        if (strcmp(cmdArr[0], "help") == 0){
            helpCmd();
        }
    }

    return 1;
}

void helpCmd()
{
    addFile(&outText, "data/help.txt");
}

void focus(TEXT* text)
{
    while (1){
        BOX* box = text->box;
        int linesize = box->right - box->left - 1;
        int pagesize = box->top - box->bottom - 1; 
        char c = getch();
        if (c == EXT){
            c = getch();
            switch (c)
            {
            case UP:
                readText(text, -linesize);
                break;
            case DOWN:
                readText(text, linesize);
                break;
            case PGDN:
                readText(text, -linesize*pagesize/2);
                break;
            case PGUP:
                readText(text, linesize*pagesize/2);
                break;
            }
        }
        if (c == EXIT){
            break;
        }
        resetText(&pltBox);
        // clearBox(&pltBox);
        showText(&pltBox);
    }
}

void focusOutText(OUTTEXT* out)
{
    while (1){
        char c = getch();
        if (c == EXT){
            c = getch();
            switch (c)
            {
            case UP:
                reassignText(out, prevLine(out, out->pos));
                break;
            case DOWN:
                reassignText(out, nextLine(out, out->pos));
                break;
            case PGDN:
                break;
            case PGUP:
                break;
            }
        }
        if (c == EXIT){
            break;
        }
        resetText(out->box);
        showOutText(out);
    }
}