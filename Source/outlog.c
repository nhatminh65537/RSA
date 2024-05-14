#include "../header/outlog.h"
#include <string.h>
// #include <conio.>

#define CHECKRANGE 16
#define PREPRINT   256

OUTTEXT logText;

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
    if (pos < 0){
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
    if (!box->enable) return;

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