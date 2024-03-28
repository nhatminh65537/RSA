#ifndef __PROJECTOBJ__
#define __PROJECTOBJ__

#define MAXBYTE 2
#define MAXHEX MAXBYTE*2
#define MAXBIT MAXBYTE*8
#define NON zero
// #define MAXCHAR 32

struct Int256
{
    unsigned char value[MAXBYTE];
};
typedef struct Int256 INT256;

enum Mode
{
    ASCIIMODE,
    HEXMODE,
    DECMODE,
};

void conv2hex(unsigned char*, INT256*);
void conv2char(unsigned char*, INT256*);

// void assign(INT256*, INT256*);
INT256 int256_c(unsigned char*, int);

INT256 pls(INT256, INT256, INT256);
INT256 sub(INT256, INT256, INT256);
INT256 mul(INT256, INT256, INT256);
INT256 pow(INT256, INT256, INT256);

INT256 mod(INT256, INT256);
INT256 div(INT256, INT256);

int le(INT256, INT256);
int eq(INT256, INT256);

INT256 zero;
INT256 maxv;
INT256 one;

#endif