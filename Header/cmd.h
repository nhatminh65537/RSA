#ifndef __CMD__
#define __CMD__

#include "../header/prgvar.h"
#include "../header/outlog.h"

int runCmd();

void loadCmd();
void showCmd();
void unshowCmd();
void exportCmd();
void focusCmd();
void helpCmd();
void focus(TEXT*);
void focusOutText(OUTTEXT*);

#endif