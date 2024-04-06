#include <stdio.h>
#include <signal.h>
#include <conio.h>

#define MOVEXY(x, y) printf("\x1b[%d;%dH", y, x)
#define FILLXY(x, y, c) MOVEXY(x, y); printf(c)
#define MAXX 150
#define MAXY 50
#define TRUE 1
#define FALSE 0
#define CLSSCR clrblk(1, 1, MAXX, MAXY);
enum TextArea
{
    TEXTTOP = 7,
    TEXTBOTTOM = MAXY - 2,
    TEXTLEFT = 48,
    TEXTRIGHT = MAXX - 2
};

void putText(char *);
int textCursorX = TEXTLEFT, textCursorY = TEXTTOP;

void loadToText(char *);

void nullfunc(int n)
{
    signal(SIGINT, nullfunc);
}

enum BoxStyle
{
    LIGHT, HEAVY, DOUBLE, L4DASH, H4DASH, L3DASH, H3DASH, L2DASH, H2DASH, 
};

void input(char*);

enum BoxCorner
{
    TOPRIGHT, TOPLEFT, BOTTOMLEFT, BOTTOMRIGHT
};

void clrblk(int x1, int y1, int x2, int y2)
{
    char erase[MAXX + 1];
    for (int x = 0; x <= x2-x1; ++x) erase[x] = ' ';
    erase[x2-x1 + 1] = 0;

    for (int y = y1; y <= y2; ++y)
    {
        MOVEXY(x1, y);
        printf(erase);
    }
    MOVEXY(x1+1, y2+1);
}

void drawBox(int x1, int y1, int x2, int y2, 
             int line_type, int corner_type, char* name);
int runCmd(char*);

char *boxhline[] = 
{
    "\u2500", "\u2501", "\u2550", "\u2508", "\u2509", "\u2504", "\u2505", "\u254c", "\u254d"
};

char *boxvline[] =
{
    "\u2502", "\u2503", "\u2551", "\u250a", "\u250b", "\u2506", "\u2507", "\u254e", "\u254f"
};

char *boxcorner[] =
{
    "\u2510", "\u2513", "\u2557", "\u256e",
    "\u250c", "\u250f", "\u2554", "\u256d",
    "\u2514", "\u2517", "\u255a", "\u2570",
    "\u2518", "\u251b", "\u255d", "\u256f"
};

int main()
{
    signal(SIGINT, nullfunc);
    char instr[256];
    MOVEXY(1,1);
    printf("\x1b[38;2;121;144;169m");
    drawBox(1, 1, MAXX, MAXY, 0, 3, "RSA-PROJECT");
    drawBox(2, 2, MAXX - 1, 5, 2, 2, "COMMAND");
    drawBox(2, 6, 46, MAXY - 1, 1, 1, "KEY");
    drawBox(47, 6, MAXX - 1, MAXY - 1, 1, 1, "TEXT");
    // CLSSCR;
    do 
    {
        clrblk(3, 3, MAXX - 2, 4);
        MOVEXY(3, 3); 
        printf("\x1b[38;5;10m$ \x1b[0m\x1b[38;5;12m");
        input(instr);
        // putText(instr);
    } while(runCmd(instr));
    return 0;
}

void drawBox(int x1, int y1, int x2, int y2,
             int line_type, int corner_type, char* name)
{
    MOVEXY(x2, y1); printf(boxcorner[0*4 + corner_type]);
    MOVEXY(x1, y1); printf(boxcorner[1*4 + corner_type]);
    MOVEXY(x1, y2); printf(boxcorner[2*4 + corner_type]);
    MOVEXY(x2, y2); printf(boxcorner[3*4 + corner_type]);
    for (int x = x1 + 1; x < x2; ++x)
    {
        FILLXY(x, y1, boxhline[line_type]);
        FILLXY(x, y2, boxhline[line_type]);
    }
    for (int y = y1 + 1; y < y2; ++y)
    {
        FILLXY(x1, y, boxvline[line_type]);
        FILLXY(x2, y, boxvline[line_type]);
    }
    MOVEXY(x1+2, y1);
    printf("\x1b[1m%s\x1b[0m", name);
    MOVEXY(x1+1, y1+1);
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

int _strcmp(char* a, char* b)
{
    while (*a != 0 & *b != 0 & *a == *b) ++a, ++b;
    if (*a == *b) return 1;
    else          return 0;
}

int runCmd(char* cmd)
{
    char cmdArr[16][64], *cur = *cmdArr;
    int count = 0;
    for (char *c = cmd; *c != 0; ++c)
    {
        if (*c == ' ')
        {
            *cur = 0;
            ++count;
            cur = *(cmdArr + count);
            continue;
        }
        
        *cur = *c;
        ++cur;
    }
    *cur = 0;

    if (_strcmp(cmd, "exit"))
        return FALSE;
    
    if (_strcmp(cmdArr[0], "load"))
        loadToText(cmdArr[1]);
    
    return TRUE;
}

void loadToText(char * fileName)
{
    char buff[256];
    FILE *f = fopen(fileName, "r");
    if(f != NULL) 
        while(fgets(buff, 256, f)) 
        {
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
    // printf("\u2596");
    // ++textCursorX;
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
    // textCursorX = TEXTLEFT;
    // ++textCursorY;
}