#ifndef __PRGVAR__
#define __PRGVAR__

// #include "int256.h"
#include "ui.h"

#define NAMEMAX      128
#define TEXTMAX      1 << 12 //2^12 byte 
#define CMDLEN       256
#define MAXHIS       20

void initPrgVar();

typedef struct Text
{
    char file[NAMEMAX];
    char text[TEXTMAX];
    int  pos;
    BOX* box;
} TEXT;
extern TEXT cipherText, plainText, logText;

void initText(TEXT* text, char* fileName, BOX* box);
int  readText(TEXT* text, int offset);
void loadToText(TEXT* text, char * fileName, BOX* box);
void writeText(TEXT* text);

typedef struct CmdString
{
    char string[CMDLEN];
    int  count;
    int  pos;
} CMDSTR;
extern CMDSTR cmd;
extern char cmdArr[][64];

char* phraseCmd(char*);
void  insertChar(CMDSTR*, char);
void  deleteChar(CMDSTR*);
void  resetCmd(CMDSTR*);
void  retrievCmd(CMDSTR*, char*);
void  assignString(CMDSTR*, char*);

typedef struct CmdHistory
{
    char cmdHis[MAXHIS][CMDLEN];
    int  cur;
    int  pos;
} CMDHIS;
extern CMDHIS cmdHis;

char* preCmd(CMDHIS*);
char* sucCmd(CMDHIS*);
void  recordCmd(CMDHIS*, char*, int);
void  writeCmdHis(CMDHIS*, char*);
void  readCmdHis (CMDHIS*, char*);

void appendStringToOutput(char*);
void appendFileToOutput(char*);

#endif