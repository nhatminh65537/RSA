#ifndef __INT256__
#define __INT256__

#define MAXBYTE 2
#define MAXHEX MAXBYTE*2
#define MAXBIT MAXBYTE*8
#define NON zero

enum Mode
{
    ASCIIMODE,
    HEXMODE,
    DECMODE,
};

struct Int256
{
    unsigned char value[MAXBYTE];
};
typedef struct Int256 INT256;

extern INT256 zero;
extern INT256 maxval;
extern INT256 one;

void conv2hex(unsigned char*, INT256*);
void conv2char(unsigned char*, INT256*);

void assign(INT256*, INT256*);

INT256 int256_c(unsigned char*, enum Mode);

int le(INT256, INT256);
int eq(INT256, INT256);

INT256 shiftleft(INT256, int);

INT256 mod(INT256, INT256);
INT256 div(INT256, INT256);

INT256 pls(INT256, INT256, INT256);
INT256 sub(INT256, INT256, INT256);

INT256 mul(INT256, INT256, INT256);

INT256 pow(INT256, INT256, INT256);

void show(INT256, enum Mode);

void int256_init();

#endif