#include <stdio.h>
#include <string.h>
#include "../header/prgvar.h"
#include "../header/ui.h"
#include "../header/outlog.h"

int spu, spr, splt, scpt,
    hpu, hpr, hplt, hcpt;
const char defaultPlaintextFile [] = "data/plaintext" ,
           defaultCiphertextFile[] = "data/ciphertext";
KEYELE p, q, n, e, d;
TEXT cipherText, plainText;
CMDSTR cmd;
CMDHIS cmdHis;

void fulfillKey(KEYELE* kel)
{
    conv2dec(kel->dec, &kel->val);
    strcpy(kel->hex, "0x");
    conv2hex(kel->hex + 2, &kel->val);

}

void resetDefaultPuKey()
{
    n.val = zero;
    e.val = zero; 
    spu = 0;   
    hpu = 0;
    fulfillKey(&n);
    fulfillKey(&e);
}
void resetDefaultPrKey()
{
    p.val = zero; 
    q.val = zero;
    d.val = zero; 
    spr = 0;
    hpr = 0;
    fulfillKey(&p);
    fulfillKey(&q);
    fulfillKey(&d);
}

void resetDefaultPlaintext ()
{
    FILE* f = fopen(defaultPlaintextFile, "w"); fclose(f);
    splt = 0;
    hplt = 0;
    loadToText(&plainText , defaultPlaintextFile , &pltBox);
}
void resetDefaultCiphertext()
{
    FILE* f = fopen(defaultCiphertextFile, "w"); fclose(f);
    scpt = 0;
    hcpt = 0;
    loadToText(&cipherText, defaultCiphertextFile, &cptBox);
}


void initPrgVar()
{
    resetCmd(&cmd);
    readCmdHis(&cmdHis, "data/cmdlog.txt");

    resetDefaultPlaintext ();
    resetDefaultCiphertext();

    resetDefaultPuKey();
    resetDefaultPrKey();
}

void initText(TEXT* text, const char* fileName, BOX* box)
{
    strcpy(text->file, fileName);
    text->pos      = 0;
    text->box      = box;
    *text->text    = '\0';
}

int readText(TEXT* text, int offset)
{
    FILE* f = fopen(text->file, "r");
    if(f != NULL) {
        fseek(f, 0, SEEK_END);
        if (text->pos + offset >= ftell(f)) return 0;
        if (text->pos + offset < 0){
            text->pos = 0;
        }
        else{
            text->pos += offset;
        }
        fseek(f, text->pos, SEEK_SET);
        text->text[fread(text->text, 1, TEXTMAX, f)] = 0;
    }
    else
        addError(&outText, "File not Found!\n");
    fclose(f);
}

void loadToText(TEXT* text, const char *fileName, BOX* box)
{
    initText(text, fileName, box);
    readText(text, 0);
    enableText(box, TRUE, text->text);
    showText(box);
}

void saveText(TEXT* text, const char * fileName)
{
    FILE *f = fopen(fileName, "w");
    fprintf(f, text->text);
    fclose(f);
}

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

char* preCmd(CMDHIS* chis)
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
    if (strcmp(chis->cmdHis[(chis->cur - 1 + MAXHIS)%MAXHIS], cmd) == 0) 
        return;
    strcpy(chis->cmdHis[chis->cur], cmd);
    if (permanent) chis->cur = (chis->cur + 1)%MAXHIS;
}

void readCmdHis(CMDHIS* chis, char* filename)
{
    FILE* f = fopen(filename, "r");
    for (int i = 0; i < MAXHIS; ++i){
        char *str = chis->cmdHis[i];
        if (feof(f)) break;
        fgets(str, FULLCMDLEN, f);
        str[strlen(str) - 1] = 0;
    }
    chis->cur = MAXHIS - 1;
    chis->pos = MAXHIS - 1;
    fclose(f);
}

void writeCmdHis(CMDHIS* chis, char* filename)
{
    FILE* f = fopen(filename, "w");
    for (int i = 0; i < MAXHIS; ++i){
        fputs(chis->cmdHis[(i + chis->cur)%MAXHIS], f);
        fputs("\n", f);
    }
    fclose(f);
}
