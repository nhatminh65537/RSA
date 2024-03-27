#ifndef __PROJECTOBJ__
#define __PROJECTOBJ__

#define MAXBYTE 4
#define MAXHEX MAXBYTE*2
#define MAXBIT MAXBYTE*8
#define NON zero
// #define MAXCHAR 32

struct Int256
{
    unsigned char value[MAXBYTE];
};

enum Mode
{
    ASCIIMODE,
    HEXMODE,
    DECMODE,
};

void conv2hex(unsigned char*, struct Int256*);
void conv2char(unsigned char*, struct Int256*);

// void assign(struct Int256*, struct Int256*);
void load(struct Int256*, unsigned char*, int);
int index(struct Int256*, int);

struct Int256 pls(struct Int256, struct Int256, struct Int256);
struct Int256 sub(struct Int256, struct Int256, struct Int256);
struct Int256 mul(struct Int256, struct Int256, struct Int256);
struct Int256 pow(struct Int256, struct Int256, struct Int256);

struct Int256 mod(struct Int256, struct Int256);
struct Int256 div(struct Int256, struct Int256);

int le(struct Int256, struct Int256);
int eq(struct Int256, struct Int256);

struct Int256 zero;
struct Int256 maxv;

#endif