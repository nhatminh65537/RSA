#include <string.h>
#include <conio.h>
#include "../header/cmd.h"
#include "../header/int256.h"
#include "../header/ui.h"
#include "../header/prgvar.h"
#include "../header/uilib.h"
#include "../header/outlog.h"

char cmdList[][FULLCMDLEN] = {
    "show-plt", // show plaintext
    "show-cpt", // show ciphertext
    "show-log", // show outputlog
    "unshow-plt", 
    "unshow-cpt", 

    "unshow-log",
    "info-program",
    "info-pbl",
    "help",
    "unload-plt",

    "unload-cpt",
    "unload-key",
    "focus-plt",
    "focus-cpt",
    "focus-log",

    "edit-plt",
    "edit-cpt",
    "exit",
    "load-plt",
    "load-cpt",

    "load-key",
    "save-plt",
    "save-cpt",
    "save-key",
    "where-key",

    "where-cpt",
    "where-plt",
    "gene-key",
    "clear-log"
};
int cmdListLen = 29;

int searchCmd(char* cmd, int s, int fullflag) 
{
    for (int i = 0; i < cmdListLen; ++i){
        int index = (s+i)%cmdListLen;
            if (strcmp(cmd, cmdList[index])           == 0 )             return index;
        if (strncmp(cmd, cmdList[index], strlen(cmd)) == 0 && !fullflag) return index;
    }
    return -1;
}

int runCmd()
{
    char *c = cmd.string;
    char  cmdArr[CMDARRLEN][CMDARRCLEN];
    
    while (*c)
    {    
        c = phraseCmd(c, cmdArr);
        switch (searchCmd(cmdArr[0], 0, 1)){
            case 0:
                cptBox.sx = OVER;
                enableBox(&pltBox, TRUE);
                break;
            case 1:
                enableBox(&cptBox, TRUE);
                break;
            case 2:
                enableBox(&logBox, TRUE);
                break;
            case 3:
                cptBox.sx = FULL;
                enableBox(&pltBox, FALSE);
                break;
            case 4:
                enableBox(&cptBox, FALSE);
                break;
            case 5:
                enableBox(&logBox, FALSE);
                break; 
            case 6:
                break;
            case 7:
                break;
            case 8:
                helpCmd(cmdArr + 1);
                break;
            case 9:
                resetDefaultPlaintext();
                break; 
            case 12:
                focus(&plainText);
                break;
            case 18:
                loadToText(&plainText, cmdArr[1], &pltBox);
                break;  
            case 19:
                break;
            default:
                char message[128] = "rsa: ";
                strcat(strcat(message, cmdArr[0]), ": command not found!\n");
                addError(&outText, message);
                 
        }

        if (strcmp(cmdArr[0], "exit") == 0)
            
            return 0;

        if (strcmp(cmdArr[0], "focus") == 0){
            printf("\x1b[?25l");
            if (strcmp(cmdArr[1], "plt") == 0) focus(&plainText);
            if (strcmp(cmdArr[1], "log") == 0) focusOutText(&outText);
            printf("\x1b[?25h");
        }
    }

    return 1;
}

void helpCmd(char para[][CMDARRCLEN])
{
    if (strcmp(para[0], "") == 0)
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
        resetText(text->box);
        // clearBox(&pltBox);
        showText(text->box);
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

char* phraseCmd(char* c,char cmdArr[][CMDARRCLEN])
{
    char *cur = *cmdArr;
    int count = 0;
    
    while (*c == ' ' || *c == ';') ++c;

    while (*c != 0 && *c != ';'){
        if (*c == ' '){
            *cur = 0;
            ++count;
            cur = *(cmdArr + count);
            while (*c == ' ') ++c;
            continue;
        }
        *cur = *c;
        ++cur; ++c;
    }
    **(cmdArr + count + 1) = 0;
    *cur = 0;
    cur = *cmdArr;
    return c;
}