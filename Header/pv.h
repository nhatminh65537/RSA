#ifndef __PRGVAR__
#define __PRGVAR__

#include "int256.h"
#include "ui.h"

#define NAMEMAX       128
#define TEXTMAX       1 << 12 //2^12 byte 
#define FULLCMDLEN    256
#define MAXHIS        30
#define EDITSIZE      1 << 15

void initPrgVar();

// program flag
extern int spu, spr, splt, scpt,
           hpu, hpr, hplt, hcpt,
                     cplt, ccpt;
extern const char defaultPlaintextFile [],
                  defaultCiphertextFile[];
extern char publicKeyFile[], privateKeyFile[];

typedef struct KeyElement 
{
    INT256 val;
    char hex[MAXHEX+3];
    char dec[2*MAXHEX];

} KEYELE;
extern KEYELE p, q, n, e, d;

void fulfillKey(KEYELE*);

void resetDefaultPuKey();
void resetDefaultPrKey();

void resetDefaultPlaintext ();
void resetDefaultCiphertext();

typedef struct Text
{
    char file[NAMEMAX];
    char text[TEXTMAX];
    int  pos;
    BOX* box;
} TEXT;
extern TEXT cipherText, plainText;

void initText(TEXT* text, const char* fileName, BOX* box);
int  readText(TEXT* text, int offset);
void loadToText(TEXT* text, const char * fileName, BOX* box);
void saveText(TEXT* text, const char * fileName);

typedef struct CmdString
{
    char string[FULLCMDLEN];
    int  count;
    int  pos;
} CMDSTR;
extern CMDSTR cmd;

void  insertChar(CMDSTR*, char);
void  deleteChar(CMDSTR*);
void  resetCmd(CMDSTR*);
void  retrievCmd(CMDSTR*, char*);
void  assignString(CMDSTR*, char*);

typedef struct CmdHistory
{
    char cmdHis[MAXHIS][FULLCMDLEN];
    int  cur;
    int  pos;
} CMDHIS;
extern CMDHIS cmdHis;

char* preCmd(CMDHIS*);
char* sucCmd(CMDHIS*);
void  recordCmd(CMDHIS*, char*, int);
void  writeCmdHis(CMDHIS*, char*);
void  readCmdHis (CMDHIS*, char*);

typedef struct EditText
{
    char text[EDITSIZE];
    char *pos;
    char *end;
    BOX* box;
} EDITTEXT;
extern EDITTEXT pltEditText, cptEditText;

void  edit(EDITTEXT*);
void  initEditText(EDITTEXT*, BOX*);
void  showEditText(EDITTEXT*);
char* nextEditLine(EDITTEXT* etext, char* pos);
char* prevEditLine(EDITTEXT* etext, char* pos);

#endif