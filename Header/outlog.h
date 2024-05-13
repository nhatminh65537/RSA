#ifndef __OUTLOG__
#define __OUTLOG__

#include "pv.h"
#include "uilib.h"

#define OUTLEN 1<<15 // 2^15 

typedef struct OutputText
{
    char text[OUTLEN + 1];
    BOX* box;
    char *end;
    char *pos;
} OUTTEXT;
extern OUTTEXT logText;

void  initOutText(OUTTEXT* ,BOX*);
int   fstrlen(char*, char);
char* nextLine(OUTTEXT*, char*);
char* prevLine(OUTTEXT*, char*);
void  addText(OUTTEXT*, char*);
void  addFile(OUTTEXT*, char*);
char* startLine(OUTTEXT*);
void  reassignText(OUTTEXT*, char*);
void  addError(OUTTEXT*, char*);
void  addWarning(OUTTEXT*, char*);
void  addSuccess(OUTTEXT*, char*);
void  showOutText(OUTTEXT*);

#endif