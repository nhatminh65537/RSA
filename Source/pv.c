#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "../header/pv.h"
#include "../header/ui.h"
// #include <conio.>

#define CHECKRANGE 16
#define PREPRINT   256

OUTTEXT logText;
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
    spu = 1;   
    hpu = 0;
    fulfillKey(&n);
    fulfillKey(&e);
}
void resetDefaultPrKey()
{
    p.val = zero; 
    q.val = zero;
    d.val = zero; 
    spr = 1;
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

void initPV()
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

int fstrlen(char* fstr, char end)
{
    int count = 0;
    while(*fstr != 0 && *fstr != end){
        if (0x20 <= *fstr <= 0x7f){
            ++fstr;
        } 
        else if (*fstr == '\x1b') while (*fstr++ != 'm') continue;
        ++count;
    }
}

char* nextLine(OUTTEXT* out, char* pos)
{
    char *newpos = pos;
    int width = out->box->right - out->box->left - 1;
    while (width > 0){
        if (0x20 <= *newpos && *newpos <= 0x7f){
            ++newpos;
            --width;
        } 
        else if (*newpos == '\x1b') while (*newpos++ != 'm');
        else if (*newpos == '\n' ){
            ++newpos;
            break;
        }
        else if (*newpos == '\0') break;
        else ++newpos;
    } 
    return newpos;
}

char* prevLine(OUTTEXT* out, char* pos)
{
    char* newpos = pos;
    if ( newpos - out->text > 1 && *(newpos - 1) == '\n') newpos -= 2;
    while (newpos - out->text > 0 && *newpos != '\n') --newpos;
    while (nextLine(out, newpos) < pos) newpos = nextLine(out, newpos);
    return newpos;

    // int width = out->box->right - out->box->left - 1;
    // while (width > 0){       
    //     if (*newpos == 'm'){
    //         for (int i = 1; i < CHECKRANGE; ++i){
    //             if (newpos - out->text < i || *(newpos - i) == 'm') break;
    //             else if (*(newpos - i) == '\x1b'){
    //                 newpos = newpos - i;
    //                 break;
    //             } 
    //         }
    //     }
    //     else if (0x20 <= *newpos && *newpos <= 0x7f){
    //         --width;
    //     }
    //     // else if (*newpos == '\x1b'){
    //     //     int count = 1;
    //     //     while (*(newpos + count) != 'm') ++count;
    //     //     width += count;
    //     // }
    //     else if (*newpos == '\n'){
    //         ++newpos;
    //         break;
    //     }
    //     if (newpos == out->text) break;
    //     --newpos;
    // }
    // // for (int i = CHECKRANGE; i > 0; --i){
    // //     if (newpos - out->text < i) continue;;
    // //     if (*(newpos - i) == '\x1b'){
    // //         int j = i;
    // //         while (*(newpos - j) != 'm') --j;
    // //         if (j < 0){
    // //             newpos = newpos - i;
    // //             break;
    // //         } 
    // //         i = j;
    // //     }
    // // }
    // return newpos;
}

void addText(OUTTEXT* out, char* str)
{
    while (strlen(str) + out->end - out->text > OUTLEN){
        char *newc = out->text, *c = out->text;
        for (int i = 0; i < 50; ++i) newc = nextLine(out, newc);
        while (*newc != 0){
            if (out->pos == newc) out->pos = c;
            *c = *newc;
            ++c;
            ++newc;
        }
        out->end  = c;
        *out->end = 0;
    }
    do {
        *out->end = *str;
        ++out->end;
        ++str;
    } while (*str != 0);
    *out->end = 0;
}

void addFile(OUTTEXT* out, char* name)
{
    char buff[128];
    int cnt = (out->box->bottom - out->box->top - 1)*(out->box->right - out->box->left - 1) - (out->end - out->pos);
    FILE *f = fopen(name, "r");
    int pos = 0;
    while (!feof(f)){
        pos = fread(buff, 1, 127, f);
        buff[pos] = 0;
        cnt -= pos;
        addText(out, buff);
    }
    if (cnt < 0){
        reassignText(out, startLine(out));
    }
    fclose(f);
}

char* startLine(OUTTEXT* out)
{
    char* s = out->end;
    int height = out->box->bottom - out->box->top - 1;
    while (height > 0){
        --height;
        s = prevLine(out, s);
        if (s == out->pos) break; 
    }
    return s;
}

void  reassignText(OUTTEXT* out, char* pos)
{
    out->pos = pos;
    out->box->text = pos;
}

void initOutText(OUTTEXT* out, BOX* box)
{
    out->box = box;
    *out->text = '\0';
    out->end = out->text;
    out->pos = out->text;
}

void  addError(OUTTEXT* out, char* str)
{
    addText(out, "\x1b[38;5;1mError: ");
    addText(out, str);
    addText(out, "\x1b[39m\n");
}

void  addWarning(OUTTEXT* out, char* str)
{
    addText(out, "\x1b[38;5;3mWarning: ");
    addText(out, str);
    addText(out, "\x1b[39m\n");    
}

void  addSuccess(OUTTEXT* out, char* str)
{
    addText(out, "\x1b[38;5;2mSuccess: ");
    addText(out, str);
    addText(out, "\x1b[39m\n");   
}

void  showOutText(OUTTEXT* out)
{
    BOX* box = out->box;
    if (box == NULL || !box->enable) return;

    char *c = out->pos;

    MOVEXY(box->xPoint, box->yPoint);
    CSI(RESET);

    for (int i = PREPRINT; i > 0; --i){
        if (c - i < out->text) continue;
        if (*(c - i) == '\x1b'){
            char esc[CHECKRANGE], *cc = esc;
            while (*(c - i) != 'm'){
                *cc = *(c - i);
                ++cc;
                --i;
            }
            *cc++ = 'm';
            *cc = 0;
            --i;
            printf(esc);
            continue;
        }
    }

    while (*c!=0) 
    {   
        if (*c == '\x1b'){
            char esc[CHECKRANGE], *i = esc;
            while (*c != 'm'){
                *i = *c;
                ++i;
                ++c;
            }
            *i++ = *c++;
            *i = 0;
            printf(esc);
            continue;
        }
        if (box->xPoint >= box->right || *c == '\r' || *c == '\n')
        {
            for (int i = box->xPoint; i < box->right; ++i) printf("%c", ' ');
            ++box->yPoint;
            box->xPoint = box->left + 1;
            if (*c == '\r' || *c == '\n') ++c; 
        }
        if (box->yPoint >= box->bottom) return;

        MOVEXY(box->xPoint, box->yPoint);
        if (32 <= *c & *c <= 126)
        {
            ++box->xPoint;
            printf("%c", *c);
            ++c;
        }
    }

    for (int i = box->xPoint; i < box->right; ++i) printf("%c", ' ');
    clearBlock(box->left + 1, box->yPoint + 1, box->right - 1, box->bottom - 1);
}