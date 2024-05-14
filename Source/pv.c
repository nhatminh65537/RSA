#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "../header/pv.h"
#include "../header/ui.h"
#include "../header/outlog.h"

int spu, spr, splt, scpt,
    hpu, hpr, hplt, hcpt,
              cplt, ccpt;
const char defaultPlaintextFile [] = "data/plaintext" ,
           defaultCiphertextFile[] = "data/ciphertext";
char publicKeyFile[NAMEMAX], privateKeyFile[NAMEMAX];
KEYELE p, q, n, e, d;
TEXT cipherText, plainText;
EDITTEXT pltEditText, cptEditText;
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
    cplt = 1;
    splt = 1;
    hplt = 0;
    loadToText(&plainText , defaultPlaintextFile , &pltBox);
    initEditText(&pltEditText, &pltBox);
}
void resetDefaultCiphertext()
{
    FILE* f = fopen(defaultCiphertextFile, "w"); fclose(f);
    ccpt = 1;
    scpt = 1;
    hcpt = 0;
    loadToText(&cipherText, defaultCiphertextFile, &cptBox);
    initEditText(&cptEditText, &cptBox);
}

void initPrgVar()
{
    resetCmd(&cmd);
    readCmdHis(&cmdHis, "data/cmdlog.txt");

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
        addError(&logText, "File not Found!");
    fclose(f);
}

void loadToText(TEXT* text, const char *fileName, BOX* box)
{
    initText(text, fileName, box);
    readText(text, 0);
    enableText(box, TRUE, text->text);
    showText(box);
}

// need update copy text to text
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

void initEditText(EDITTEXT* etext, BOX* box)
{
    etext->box = box;
    *etext->text = 0;
    etext->end = etext->text;
    etext->pos = etext->text;
}

void showEditText(EDITTEXT* etext)
{
    int top = etext->box->top + 1, bottom = etext->box->bottom - 1,
        left = etext->box->left + 1, right = etext->box->right - 1,
        cx = left, cy = top;

    char *c = etext->pos;
    while (*c != '\0' && cy <= bottom){
        MOVEXY(cx, cy);
        if (*c == '\n') {
            for (int i = cx; i <= right; ++i) putch(' '); 
            cx = left;
            ++cy;
            ++c;
            continue;
        }
        if (++cx > right) cx = left, ++cy;
        putch(*c);
        ++c;
    }  
    MOVEXY(cx, cy);
    for (int i = cx; i <= right && cy <= bottom; ++i) putch(' '); 
    clearBlock(left, cy + 1, right, bottom);
}

char* nextEditLine(EDITTEXT* etext, char* pos)
{
    int width = - etext->box->left +  etext->box->right - 1;
    char *c = pos;
    while (width-->0){
        if (*c == '\0') return pos;
        if (*c == '\n') return c + 1;
        ++c;
    }
    return c;
}

char* prevEditLine(EDITTEXT* etext, char* pos)
{
    char* newpos = pos;
    if ( newpos - etext->text > 1 && *(newpos - 1) == '\n') newpos -= 2;
    while (newpos - etext->text > 0 && *newpos != '\n') --newpos;
    while (newpos != nextEditLine(etext, newpos) && nextEditLine(etext, newpos) < pos) newpos = nextEditLine(etext, newpos);
    return newpos;
}

void _insertChar(char* str, char* pos, char c)
{
    while (c != '\0'){
        char t = *pos;
        *pos = c;
        c = t;
        ++pos;
    }
    *pos = '\0';
}

void _deleteChar(char* str, char* pos)
{
    if (pos == str) return;
    --pos;
    while (*pos != '\0'){
        *pos = *(pos + 1);
        ++pos;
    }
}

int _posChar(EDITTEXT* etext, char *c, int *x, int*y)
{
    *y = etext->box->top + 1;
    *x = etext->box->left + 1;
    char *pos = etext->pos;
    if (c <= pos) etext->pos = prevEditLine(etext, pos);
    while (pos != nextEditLine(etext, pos) && nextEditLine(etext, pos) <= c){

        pos = nextEditLine(etext, pos);
        ++*y;
        if (*y > etext->box->bottom - 1) {
            --*y;
            etext->pos = nextEditLine(etext, etext->pos);
        }
    }
    while (pos < c){
        ++pos;
        ++*x;
    }
    return 0;
}

char* _getChar(EDITTEXT* etext, int x, int y)
{

}

void edit(EDITTEXT* etext)
{   
    int top = etext->box->top + 1, bottom = etext->box->bottom - 1,
        left = etext->box->left + 1, right = etext->box->right - 1,
        x = etext->box->xPoint, y = etext->box->yPoint, cont = TRUE;
    char c, *cc = etext->end;

    while (cont){  
        int l = _posChar(etext, cc, &x, &y);
        if (etext->end - etext->text >= EDITSIZE) {
            addError(&logText, "Size reach Limit.");
        } 
        // if (y > bottom) {
        //     --y;
        //     etext->pos = nextEditLine(etext, etext->pos);
        // }   
        // if (y < top) {
        //     ++y;
        //     etext->pos = prevEditLine(etext);
        // }  
        showEditText(etext);

        MOVEXY(x, y);
        printf("\x1b[?25h");
        c = getch();
        printf("\x1b[?25l");
        
        switch (c){
            case RETURN:
                _insertChar(etext->text, cc, '\n');
                ++etext->end;
                ++cc;
                break;
            case BACKSPACE:
                _deleteChar(etext->text, cc);
                if (cc != etext->text) --cc, --etext->end;
                break;
            case EXIT:
                cont = FALSE;
                break;
            case EXT:
                c = getch();
                int t = left;
                switch (c)
                {
                    case   UP:
                        if (cc > etext->text && *cc == '\n') --cc;
                        cc = prevEditLine(etext, cc);
                        if (cc > etext->text && *(cc - 1) == '\n') cc = prevEditLine(etext, cc);  
                        while(*cc != '\n' && t != x) ++cc, ++t;
                        break;
                    case LEFT:
                        if (cc > etext->text) --cc;
                        break;
                    case RIGHT:
                        if (cc < etext->end) ++cc;
                        break;
                    case DOWN:
                        cc = nextEditLine(etext, cc);
                        while(*cc != '\n' && *cc != '\0' && t != x) ++cc, ++t;
                        break;
                }
                break;
            default:
                if (32 > c || c > 126) break;
                _insertChar(etext->text, cc, c);
                ++etext->end;
                ++cc;
                break;    
        }
    }
    // etext->box->tPoint = etext->pos;
}