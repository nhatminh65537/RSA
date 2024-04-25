#ifndef __PRGVAR__
#define __PRGVAR__

// #include "int256.h"

#define NAMEMAX      128
#define TEXTMAX      16384 //2^14 byte 

typedef struct Text
{
    char file[NAMEMAX];
    char text[TEXTMAX];
    int  pos;
} TEXT;

extern TEXT cipherText, plainText, logText;
extern char instr[];

void setTextFile(TEXT* text, char* fileName);
int readText(TEXT* text, int offset);
void writeText(TEXT* text);

void initPrgVar();

#endif