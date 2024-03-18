#ifndef __PROJECTOBJ__
#define __PROJECTOBJ__

struct Int256
{
    char value[32];
    char hex[64];

    Int256(char [], int);

    char* conv2hex();
    char* conv2char();

    void assign(Int256*);
    void index(int);
};

Int256 pls(Int256*, Int256*, Int256*);
Int256 sub(Int256*, Int256*, Int256*);
Int256 mul(Int256*, Int256*, Int256*);
Int256 pow(Int256*, Int256*, Int256*);

Int256 mod(Int256*, Int256*);
Int256 div(Int256*, Int256*);

#endif