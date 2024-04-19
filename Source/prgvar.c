#include <stdio.h>
#include <string.h>
#include "prgvar.h"

TEXT cipherText, plainText, logText;

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

void initPrgVar()
{
    strcpy(logText.text, "");
}

