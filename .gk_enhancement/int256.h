#ifndef __INT256__
#define __INT256__

#define MAXWORD  128
#define WORD     8

#define MAXHEX MAXWORD*WORD/4
#define MAXBIT MAXWORD*WORD
#define NON zero

enum Mode
{
    ASCIIMODE,
    HEXMODE,
    DECMODE,
};

struct Int256
{
    unsigned char value[MAXWORD];
};
typedef struct Int256 INT256;

extern INT256 zero;
extern INT256 maxval;
extern INT256 one;

void conv2hex (unsigned char*, INT256*);
void conv2char(unsigned char*, INT256*);
void conv2dec (unsigned char*, INT256*);

void assign(INT256*, INT256*);

INT256 int256_c(unsigned char*, enum Mode);

int ile(INT256, INT256);
int ieq(INT256, INT256);
int igt(INT256, INT256);

INT256 shiftleft(INT256, int);

INT256 imod(INT256, INT256);
INT256 idiv(INT256, INT256);

INT256 ipls(INT256, INT256, INT256);
INT256 isub(INT256, INT256, INT256);

INT256 imul(INT256, INT256, INT256);

INT256 ipow(INT256, INT256, INT256);

INT256 imulInverse(INT256, INT256);

INT256 irand(int, int);

INT256 igcd(INT256 a, INT256 b); 

void initInt();

#endif