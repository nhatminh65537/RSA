#include "../Header/outlog.h"

int fstrlen(char* fstr)
{
    int count = 0;
    while(*fstr != 0){
        if (*fstr == '\n' || *fstr == '\r'){
            ++fstr;
            continue;
        } 
        if (*fstr == '\x1b') while (*fstr++ == 'm') continue;
        ++count;

    }
}
char* nextline(char* fstr, int offset)
{
    int count = 0;
    while(*fstr != 0 || count == offset){
        if (*fstr == '\n' || *fstr)
        if (*fstr == '\x1b') while (*fstr++ == 'm') continue;
        ++count;
    }
    if (count < offset) return NULL;
    else 
}