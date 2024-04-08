#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "uilib.h"


enum TextArea
{
    TEXTTOP    = 7,
    TEXTBOTTOM = MAXY - 2,
    TEXTLEFT   = 48,
    TEXTRIGHT  = MAXX - 2
};

void putText(char *);
int textCursorX = TEXTLEFT, textCursorY = TEXTTOP;

void loadToText(char *);
void input(char*);
int  runCmd(char*);

int main()
{
    CLRSCR;
    char instr[256];
    int i = 40;
    
    // cputs("\x1b[20mHello");
    // while (1);
    printf("\x1b[38;2;121;144;169m");
    drawBox(1, 1, MAXX, MAXY, 0, 3, "RSA-PROJECT");
    drawBox(2, 2, MAXX - 1, 5, 2, 2, "COMMAND");
    drawBox(2, 6, 46, MAXY - 1, 1, 1, "KEY");
    drawBox(47, 6, MAXX - 1, MAXY - 1, 1, 1, "TEXT");
    do 
    {
        clearBlock(3, 3, MAXX - 2, 4);
        MOVEXY(3, 3); 
        printf("\x1b[38;5;10m $ \x1b[39m");
        input(instr);
        // CSI(RESET);
        printf("\x1b[%sm", instr);
        putText("Hello, this is demo for ANSI CSI");
        CSI(RESET);
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
        clearBlock(TEXTLEFT, TEXTTOP, TEXTRIGHT, TEXTBOTTOM);
        textCursorY = TEXTTOP;
    }
    
    return TRUE;
}

void loadToText(char * fileName)
{
    char buff[256];
    FILE *f = fopen(fileName, "r");
    if(f != NULL) 
        while(fgets(buff, 256, f)){
            putText(buff);
        }
    else
        putText("File not Found!\n");
    fclose(f);
}

void putText(char* textString)
{
    int breakWord;
    MOVEXY(textCursorX, textCursorY);
    printf("\u2596");
    ++textCursorX;
    while (*textString!=0) 
    {   
        if (textCursorX > TEXTRIGHT || *textString == '\r' || *textString == '\n')
        {
            ++textCursorY;
            textCursorX = TEXTLEFT;
            if (!breakWord) ++textString; 
        }
        if (textCursorX == TEXTRIGHT)
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
        if (textCursorY > TEXTBOTTOM)
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
    textCursorX = TEXTLEFT;
    ++textCursorY;
}