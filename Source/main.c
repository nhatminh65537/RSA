#include <string.h>
#include <conio.h>
#include <stdio.h>
#include "../header/ui.h"
#include "../header/prgvar.h"

void loadToText(char *);
int  runCmd(char*);
char plaintext[2048], cmdOpt[2048];

int main()
{
    CLRSCR;
    
    initUI();
    initPrgVar();

    // enableText(&optBox, TRUE, logText.text);
    			
    do 
    {
        show(&logBox);
        clsInput();
        inputCmd();
        strcat(logText.text, cmd.string);
        strcat(logText.text, "\n");
    } while(runCmd(cmd.string));

    writeCmdHis(&cmdHis, "data/cmdlog.txt");
    return 0;
}

int runCmd(char* cmd)
{
    char cmdArr[16][64], *cur = *cmdArr;
    while (1)
    {    
        int count = 0;
        
        if (*cmd ==  0 ) return 1;
        while (*cmd == ' ' || *cmd == ';') ++cmd;

        while (*cmd != 0 && *cmd != ';'){
            if (*cmd == ' '){
                *cur = 0;
                ++count;
                cur = *(cmdArr + count);
                while (*cmd == ' ') ++cmd;
                continue;
            }
            *cur = *cmd;
            ++cur; ++cmd;
        }
        *cur = 0;
        cur = *cmdArr;

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

void loadToText(char * fileName)
{
    setTextFile(&plainText, fileName);
    readText(&plainText, 0);
    enableText(&pltBox, TRUE, plainText.text);
    showText(&pltBox);
}
