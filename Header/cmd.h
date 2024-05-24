#ifndef __CMD__
#define __CMD__

#include "pv.h"

#define CMDLEN        32
#define CMDARRLEN     16
#define CMDARRCLEN    64
#define MESSLEN       256

void inputCmd();

extern char cmdList[][FULLCMDLEN];
int searchCmd(char*, int, int);
int runCmd();
char* phraseCmd (char*, char[][CMDARRCLEN]);
char* getParaVal(char* para,  char args[][CMDARRCLEN]);

void unloadPltCmd(char [][CMDARRCLEN]);
void unloadCptCmd(char [][CMDARRCLEN]);
void unloadKeyCmd(char [][CMDARRCLEN]);
void loadPltCmd(char [][CMDARRCLEN]);
void loadCptCmd(char [][CMDARRCLEN]);
void helpCmd(char [][CMDARRCLEN]);
void loadKeyCmd(char [][CMDARRCLEN]);
void saveTextCmd(TEXT* , char [][CMDARRCLEN]);
void saveKeyCmd(char [][CMDARRCLEN]);
void focusCmd(TEXT*);
void focusLogCmd(OUTTEXT*);
void whereKeyCmd();
void wherePltCmd();
void whereCptCmd();
void clearLogCmd();
void editPltCmd();
void editCptCmd();
void genkeyCmd(char [][CMDARRCLEN]);
void encryptCmd(char[][CMDARRCLEN]);
void decryptCmd(char[][CMDARRCLEN]);
void infoPblCmd();

#endif