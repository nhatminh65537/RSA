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
char* phraseCmd (char*, char[][CMDARRCLEN]);
char* getParaVal(char* para,  char args[][CMDARRCLEN]);



void loadCmd();
void exportCmd();
void focusCmd();
void helpCmd();
void focus(TEXT*);
void focusOutText(OUTTEXT*);
void encryptCmd(char[][CMDARRCLEN]);
void decryptCmd(char[][CMDARRCLEN]);

// decrypt.c
void decrypt(INT256 d ,INT256 p , INT256 q, char* cpt, char* plt);
// encrypt.c
void encrypt(INT256 e, INT256 n, char *plt, char *cpt);

#endif