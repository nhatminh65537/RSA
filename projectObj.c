#include "projectObj.h"
#include <string.h>
#include <stdio.h>

int _min(int a, int b) {return a > b? b: a;}

struct Int256 zero()
{
    struct Int256 num;
    for (int i = 0; i < MAXBYTE; ++i) num.value[i] = 0;
    return num;
}

char tonum(char c)
{
    if (c == 0)            return 0;
    if (c > 47 && c <= 57) return c - 48;
    if (c > 64 && c <= 70) return c - 55;
}

char tohex(char c)
{
    if (c == 0)            return 0;
    if (c < 10 && c >=  0) return c + 48;
    if (c < 17 && c >= 10) return c + 55;
}

int index(struct Int256* num, int pos)
{
    if (256 > pos && pos >= 0)
        return (num->value[pos/8] >> (pos%8)) & 1;  
    else return -1; 
}

void set(struct Int256* num, int pos, int value)
{
    if (256 > pos && pos >= 0 && value == 1)
        num->value[pos/8] |= 1 << (pos%8);
    if (256 > pos && pos >= 0 && value == 0) 
        num->value[pos/8] &= 0 << (pos%8);
}

void assign(struct Int256* des, struct Int256* ref)
{
    for (int i = 0; i < MAXBYTE; ++i)
        des->value[i] = ref->value[i];
}

struct Int256 int256_c(char* str, int mode)
{
    struct Int256 num;
    int len = strlen(str);


    // ASCII mode
    if (mode == 0)
        for (int i = 0; i < MAXBYTE; --i)
        {
            num.value[i] = str[_min(i, len)];     
        }      
    
    // HEX mode
    if (mode == 1)
    {
        int num_left, num_right;
        for (int i = 0; i < MAXBYTE; ++i)
        {
            num_right = --len >= 0? tonum(str[len]): 0;
            num_left  = --len >= 0? tonum(str[len]): 0;
            num.value[i] = num_right + (num_left << 4);
        }
    }
        
    // DEC mode
    // if (mode = 2)
    // {
    //     int exp = 0;
    // }
    return num;
}

void conv2char(char* str, struct Int256* num)
{
    for (int i = 1; i < MAXBYTE; ++i)
        str[i] = num->value[i];
    str[MAXBYTE] = 0;
}

void conv2hex(char* hex, struct Int256* num)
{
    for (int i = 0; i < MAXBYTE; ++i)
    {
        hex[2*i]     = tohex(num->value[i] & 0x0f);
        hex[2*i + 1] = tohex(num->value[i] >> 4 & 0x0f );
    }
    hex[2*MAXBYTE] = 0;
    strrev(hex);
}

struct Int256 shiftleft(struct Int256 num, int times)
{
    struct Int256 temp = zero();
    for (int i = 0; i < MAXBYTE; ++i)
        set(&temp, i + times, index(&num, i));
    return temp;
}

struct Int256 _sub(struct Int256 a, struct Int256 b)
{
    struct Int256 temp;
    int carry = 0, value;
    for (int i = 0; i < MAXBYTE; ++i)
    {
        set(&temp, i, index(&a, i) ^ index(&b, i) ^ carry);
        carry = index(&a, i) < index(&b, i) + carry? 1: 0; 
    } 
}

struct Int256 pls(struct Int256 a, struct Int256 b, struct Int256 n)
{
    struct Int256 temp;
    int carry = 0, value;
    for (int i = 0; i < MAXBYTE; ++i)
    {
        set(&temp, i, index(&a, i) ^ index(&b, i) ^ carry);
        carry = index(&a, i) + index(&b, i) + carry > 1? 1: 0; 
    } 
    
}
struct Int256 sub(struct Int256 a, struct Int256 b, struct Int256 n)
{
    return mod(_sub(a, b), n);
}
struct Int256 pow(struct Int256, struct Int256, struct Int256);
struct Int256 mul(struct Int256, struct Int256, struct Int256);

struct Int256 mod(struct Int256 a, struct Int256 b)
{
    struct Int256 temp, remainder = a;

    for (int i = MAXBYTE; i >= 0; --i)
        if (!less(remainder, shiftleft(b, i)))
            remainder = _sub(remainder, shiftleft(b, i));
    return remainder;
}

struct Int256 div(struct Int256 a, struct Int256 b)
{
    struct Int256 temp, remainder = a;
    
    if (less(a, b)) return zero();

    for (int i = MAXBYTE; i >= 0; --i)
    {
        if (less(remainder, shiftleft(b, i)))
            set(&temp, i, 0);
        else 
        {
            set(&temp, i, 1);
            remainder = _sub(remainder, shiftleft(b, i));
        }
    }
    return temp;
}

int less(struct Int256 a, struct Int256 b)
{
    int i = MAXBYTE - 1;
    while (i >= 0 && a.value[i] == 0 && b.value[i] == 0) 
        --i;
    if (i < 0 || a.value[i] >= b.value[i]) return 0;
    else                                   return 1;
}

void show(struct Int256 n)
{
    char hex[64];
    conv2hex(hex, &n);
    printf("%s", hex);
}

int main()
{
    struct Int256 n = int256_c("12" ,1);
    show(n);
    return 0;
}