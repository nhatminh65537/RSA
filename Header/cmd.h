#ifndef __CMD__
#define __CMD__

#include "../header/prgvar.h"
#include "../header/outlog.h"

#define CMDLEN     32
#define CMDARRLEN     16
#define CMDARRCLEN    64

extern char cmdList[][FULLCMDLEN];
int searchCmd(char*, int, int);
int runCmd();
char* phraseCmd(char*,char[][CMDARRCLEN]);

void loadCmd();
void exportCmd();
void focusCmd();
void helpCmd();
void focus(TEXT*);
void focusOutText(OUTTEXT*);

#endif