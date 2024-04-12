#include <string.h>
#include <conio.h>
#include "ui.h"
#include <stdio.h>

void putText(char *);
int textCursorX = OTPLEFT + 1, textCursorY = OPTTOP + 1;

void loadToText(char *);
void input(char*);
int  runCmd(char*);
char plaintext[2048];

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

    if (strcmp(cmd, "cls") == 0)
    {
        clearBlock(OTPLEFT + 1, OPTTOP + 1, OTPRIGHT - 1, OTPBOTTOM- 1);
        textCursorY = OPTTOP + 1;
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
            // putText(buff);
            strcpy(c, buff);
            c += cnt;
        }
    else
        putText("File not Found!\n");
    fclose(f);
    *c = 0;
    optBox.text = plaintext;
    showText(&optBox);
}

void putText(char* textString)
{
    int breakWord;
    MOVEXY(textCursorX, textCursorY);
    printf("\u2596");
    ++textCursorX;
    while (*textString!=0) 
    {   
        if (textCursorX > OTPRIGHT - 1 || *textString == '\r' || *textString == '\n')
        {
            ++textCursorY;
            textCursorX = OTPLEFT + 1;
            if (!breakWord) ++textString; 
        }
        if (textCursorX == OTPRIGHT - 1)
        {
            if (*textString != ' ' & *(textString + 1) != ' ') 
            {
                breakWord = TRUE;
                if (*(textString - 1) != ' ')
                    putch('-');
                ++textCursorX;
                continue;
            }
            else breakWord = FALSE;
        }
        if (textCursorY > OTPBOTTOM- 1)
            return;
        MOVEXY(textCursorX, textCursorY);
        if (32 <= *textString & *textString <= 126)
        {
            ++textCursorX;
            putch(*textString);
            ++textString;
        }
        breakWord = FALSE;
    }
    textCursorX = OTPLEFT + 1;
    ++textCursorY;
}