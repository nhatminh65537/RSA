#ifndef __INT512__
#define __INT512__

#define MAXBYTE  64

#define MAXHEX MAXBYTE*2
#define MAXBIT MAXBYTE*8
#define NON zero

enum Mode
{
    ASCIIMODE,
    HEXMODE,
    DECMODE,
};

struct Int512
{
    unsigned char value[MAXBYTE];
};
typedef struct Int512 INT512;

extern INT512 zero;
extern INT512 maxval;
extern INT512 one;

void conv2hex (unsigned char*, INT512*);
void conv2char(unsigned char*, INT512*);
void conv2dec (unsigned char*, INT512*);

void assign(INT512*, INT512*);

INT512 int256_c(unsigned char*, enum Mode);

int ile(INT512, INT512);
int ieq(INT512, INT512);
int igt(INT512, INT512);

INT512 shiftleft(INT512, int);

INT512 imod(INT512, INT512);
INT512 idiv(INT512, INT512);

INT512 ipls(INT512, INT512, INT512);
INT512 isub(INT512, INT512, INT512);

INT512 imul(INT512, INT512, INT512);

INT512 ipow(INT512, INT512, INT512);

INT512 imulInverse(INT512, INT512);

INT512 irand(int, int);

INT512 igcd(INT512 a, INT512 b); 

void initInt();

#endif