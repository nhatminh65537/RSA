#include <stdio.h>
#include <string.h>
#include "../header/prgvar.h"

TEXT cipherText, plainText, logText, hisText;
CMDSTR cmd;
CMDHIS cmdHis;

void initPrgVar()
{
    strcpy(logText.text, "");
    resetCmd(&cmd);
    readCmdHis(&cmdHis, "data/cmdlog.txt");
}

void setTextFile(TEXT* text, char* fileName)
{
    strcpy(text->file, fileName);
    text->pos      = 0;
}

int readText(TEXT* text, int offset)
{
    FILE* f = fopen(text->file, "r");
    if(f != NULL) {
        fseek(f, 0, SEEK_END);
        if (text->pos + offset >= ftell(f)) return 0;
        text->pos += offset;
        fseek(f, text->pos, SEEK_SET);
        fread(text->text, 1, TEXTMAX, f);
    }
    else
        strcat(logText.text, "File not Found!\n");
    fclose(f);
}

void writeText(TEXT* text);

void insertChar(CMDSTR* cstr, char c)
{
    ++cstr->count;
    for (int i = cstr->pos; i <= cstr->count; ++i){
        char tmp = cstr->string[i];
        cstr->string[i] = c;
        c = tmp;
    }
    ++cstr->pos;  
}

void deleteChar(CMDSTR* cstr)
{
    for (int i = cstr->pos; i < cstr->count; ++i){
        cstr->string[i] = cstr->string[i + 1];
    }
    --cstr->count;
}

void resetCmd(CMDSTR* cstr)
{
    cmd.count     = 0;
    cmd.pos       = 0;
    cmd.string[0] = 0;
}

void retrievCmd(CMDSTR* cstr, char* cmd)
{
    strcpy(cstr->string, cmd);
    cstr->pos   = strlen(cmd);
    cstr->count = strlen(cmd);
}

char *preCmd(CMDHIS* chis)
{
    int pre = (chis->pos - 1 + MAXHIS)%MAXHIS;
    if (pre != chis->cur) 
        chis->pos = pre;
    return chis->cmdHis[chis->pos];
}

char* sucCmd(CMDHIS* chis) 
{
    if (chis->pos != chis->cur) 
        chis->pos = (chis->pos + 1)%MAXHIS;
    return chis->cmdHis[chis->pos]; 
}

void recordCmd(CMDHIS* chis, char* cmd, int permanent)
{
    strcpy(chis->cmdHis[chis->cur], cmd);
    if (permanent) chis->cur = (chis->cur + 1)%MAXHIS;
}

void readCmdHis(CMDHIS* chis, char* filename)
{
    FILE* f = fopen(filename, "r");
    for (int i = 0; i < 20; ++i){
        char *str = chis->cmdHis[i];
        if (feof(f)) break;
        fgets(str, CMDLEN, f);
        str[strlen(str) - 1] = 0;
    }
    chis->cur = 0;
    chis->pos = 0;
    fclose(f);
}

void  writeCmdHis(CMDHIS* chis, char* filename)
{
    FILE* f = fopen(filename, "w");
    for (int i = 0; i < 20; ++i){
        fputs(chis->cmdHis[(i + chis->cur)%MAXHIS], f);
        fputs("\n", f);
    }
    fclose(f);
}


