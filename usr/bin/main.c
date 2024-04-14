#include <string.h>
#include <conio.h>
#include <stdio.h>
#include "ui.h"

void loadToText(char *);
void input(char*);
int  runCmd(char*);
char plaintext[2048], cmdOpt[2048];

int main()
{
    CLRSCR;
    char instr[256];
    
    initUI();
    
    do 
    {
        clearBlock(3, 3, MAXX - 2, 4);
        MOVEXY(3, 3); 
        printf("\x1b[38;5;10m $ \x1b[39m");
        input(instr);
    } while(runCmd(instr));
    return 0;
}

void input(char* instr)
{
    int count = 0;
    char c;
    for (char *temp = instr; *temp != 0; ++temp)
        *temp = 0;
    
    while (TRUE)
    {        
        c = getch();
        if (c == '\r')
        {
            instr[count] = 0;
            return;
        }
        if (c == '\b')
        {
            if (count > 0)
            {
                --count;
                instr[count] = c;
                putch(c); putch(' '); putch(c);
            }
            continue;  
        }
        if (32 <= c & c <= 126)
        {
            instr[count] = c;
            ++count;
            putch(c);
        }
        
    } 
}

int runCmd(char* cmd)
{
    char cmdArr[16][64], *cur = *cmdArr;
    int count = 0;
    for (char *c = cmd; *c != 0; ++c){
        if (*c == ' '){
            *cur = 0;
            ++count;
            cur = *(cmdArr + count);
            continue;
        }
        
        *cur = *c;
        ++cur;
    }
    *cur = 0;

    if (strcmp(cmd, "exit") == 0)
        return FALSE;
    
    if (strcmp(cmdArr[0], "load") == 0)
        loadToText(cmdArr[1]);

    if (strcmp(cmd, "cls") == 0){
        clearBox(&optBox);
    }
    if (strcmp(cmdArr[0], "disable") == 0){
        if (strcmp(cmdArr[1], "plt") == 0){
            cptBox.sx = FULL;
            enableBox(&pltBox, FALSE);
        }
        if (strcmp(cmdArr[1], "cpt") == 0) enableBox(&cptBox, FALSE);
        if (strcmp(cmdArr[1], "log") == 0) enableBox(&logBox, FALSE);
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
    return TRUE;
}

void loadToText(char * fileName)
{
    char buff[256];
    int cnt;
    char *c = plaintext;
    FILE *f = fopen(fileName, "r");
    if(f != NULL) 
        while(!feof(f)){
            cnt = fread(buff, 1, 256, f);
            strcpy(c, buff);
            c += cnt;
        }
    else
        strcat(cmdOpt, "File not Found!\n");
    fclose(f);
    *c = 0;

    enableText(&pltBox, TRUE, plaintext);
    // pltBox.text = plaintext;
    showText(&pltBox);
}
