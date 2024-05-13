#include <string.h>
#include <conio.h>
#include "../header/cmd.h"
#include "../header/int256.h"
#include "../header/ui.h"
#include "../header/prgvar.h"
#include "../header/uilib.h"
#include "../header/outlog.h"

// Task: Ket hop flag kiem soat chuong trinh

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
    "clear-log",
    "encrypt",

    "decrypt"
};
int cmdListLen = 31;

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
    char  args[CMDARRLEN][CMDARRCLEN];
    char message[128];
    
    while (*c)
    {    
        c = phraseCmd(c, args);
        switch (searchCmd(args[0], 0, 1)){
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
                helpCmd(args + 1);
                break;
            case 9:
                resetDefaultPlaintext();
                break; 
            case 12:
                focus(&plainText );
                break;
            case 13:
                focus(&cipherText);
                break;
            case 14:
                focusOutText(&outText);
                break;
            case 17:
                return 0;
                break;
            case 18:
                hplt = 1;
                loadToText(&plainText , args[1], &pltBox);
                break;  
            case 19:
                hcpt = 1;
                loadToText(&cipherText, args[1], &cptBox);
                break;
            case 21:
                saveText(& plainText, args[1]);
                break;
            case 22:
                saveText(&cipherText, args[1]);
                break;
            case 25:
                strcpy(message, "Plaintext  path: ");
                strcat(strcat(message,  plainText.file), "\n");
                addText(&outText, message);
                break;
            case 26:
                strcpy(message, "Ciphertext path: ");
                strcat(strcat(message, cipherText.file), "\n");
                addText(&outText, message);
                break;
            case 28:
                *outText.text = '\0';
                outText.pos = outText.text;
                outText.end = outText.text;
                break;
            case 29: encryptCmd(args); break;
            case 30: decryptCmd(args); break;
            default:
                strcpy(message, "rsa: ");
                strcat(strcat(message, args[0]), ": command not found!");
                addError(&outText, message);
                break;
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

char* phraseCmd(char* c, char args[][CMDARRCLEN])
{
    char *cur = *args;
    int count = 0;
    
    while (*c == ' ' || *c == ';') ++c;

    while (*c != 0 && *c != ';'){
        if (*c == ' '){
            *cur = 0;
            ++count;
            cur = *(args + count);
            while (*c == ' ') ++c;
            continue;
        }
        *cur = *c;
        ++cur; ++c;
    }
    **(args + count + 1) = 0;
    *cur = 0;
    cur = *args;
    return c;
}

char* getParaVal(char* para, char args[][CMDARRCLEN])
{
    int onPara = 0;
    char (*arg)[CMDARRCLEN] = (args + 1);
    while (**arg != 0){
        if (strcmp(para, *arg) == 0)                          return *++arg;
        if (**arg != '-' && !onPara && strcmp(para, "") == 0) return *arg;
        if (**arg == '-') onPara = 1;
        else              onPara = 0;
        ++arg;
    }
    return NULL; 
}

void encryptCmd(char args[][CMDARRCLEN])
{
    char *input, *output;

    if (getParaVal("", args) != NULL) input = getParaVal("", args);
    else if (hplt || !splt)           input = plainText.file;
    else {
        addError(&outText, "Can't find plaintext");
        return;
    }

    if (getParaVal("-o", args) != NULL) output = getParaVal("-o", args);
    else if (hcpt || !scpt){
        if (getParaVal("-f", args) == NULL){
            addWarning(&outText, "This action can be change your current content of ciphertext file. Please add output file or put flag -f to force action.");
            return;
        } else {
            output = cipherText.file;
            ccpt = 0;
        }
    } else {
        output = cipherText.file;
        ccpt = 0;
    }

    encrypt(e.val, n.val, input, output);
    addSuccess(&outText, "Encryption finished successly.");
    readText(&cipherText, 0);
    resetText(&cptBox);
    showText(&cptBox);
}

void decryptCmd(char args[][CMDARRCLEN])
{
        char *input, *output;

    if (getParaVal("", args) != NULL) input = getParaVal("", args);
    else if (hcpt || !scpt)           input = cipherText.file;
    else {
        addError(&outText, "Can't find ciphertext");
        return;
    }

    if (getParaVal("-o", args) != NULL) output = getParaVal("-o", args);
    else if (hplt || !splt){
        if (getParaVal("-f", args) == NULL){
            addWarning(&outText, "This action can be change your current content of plaintext file. Please add output file or put flag -f to force action.");
            return;
        } else {
            output = plainText.file;
            cplt = 0;
        }
    } else {
        output = plainText.file;
        cplt = 0;
    }

    decrypt(d.val, p.val, q.val, input, output);
    addSuccess(&outText, "Decryption finished successly.");
    readText(&plainText, 0);
    resetText(&pltBox);
    showText(&pltBox);
}
