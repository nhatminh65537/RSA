#include <string.h>
#include "../header/cmd.h"
#include "../header/int256.h"
#include "../header/ui.h"
#include "../header/prgvar.h"
#include "../header/uilib.h"

int runCmd()
{
    char *c = cmd.string;
    
    while (*c)
    {    
        c = phraseCmd(c);
        if (strcmp(cmdArr[0], "exit") == 0)
            return 0;

        if (strcmp(cmdArr[0], "load") == 0)
            loadToText(cmdArr[1]);

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
    }
    return 1;
}
