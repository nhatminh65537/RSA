#ifndef __PROJECTOBJ__
#define __PROJECTOBJ__

struct Int256
{
    char value[32];
};

char* conv2hex(Int256*);
char* conv2char(Int256*);

void assign(Int256*, Int256*);
void assign(Int256*, char*, int);
void index(Int256*, int);

Int256 pls(Int256*, Int256*, Int256*);
Int256 sub(Int256*, Int256*, Int256*);
Int256 mul(Int256*, Int256*, Int256*);
Int256 pow(Int256*, Int256*, Int256*);

Int256 mod(Int256*, Int256*);
Int256 div(Int256*, Int256*);

#endif