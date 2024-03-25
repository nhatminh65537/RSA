#include "projectObj.h"

struct Int256 zero()
{
    struct Int256 num;
    num.mostbit == -1;
    for (int i = 0; i < MAXBYTE; ++i) num.value[i] = 0;
    return num;
}

char tonum(char c)
{
    if (c > 47 && c <= 57) return c - 48;
    if (c > 64 && c <= 70) return c - 55;
}

char tohex(char c)
{
    if (c < 10 && c >=  0) return c + 48;
    if (c < 17 && c >= 10) return c + 55;
}

int index(struct Int256* num, int pos)
{
    if (256 > num && num >= 0)
        return (num->value[pos/8] >> (pos%8)) & 1;  
    else return -1; 
}

void set(struct Int256* num, int pos, int value)
{
    if (256 > num && num >= 0 && value == 1)
        num->value[pos/8] |= 1 << (pos%8);
    if (256 > num && num >= 0 && value == 0) 
        num->value[pos/8] &= 0 << (pos%8);
}

void assign(struct Int256* des, struct Int256* ref)
{
    for (int i = 0; i < MAXBYTE; ++i)
    {
        des->value[i] = ref->value[i];
        des->mostbit  = des->mostbit;
    }       
}

void load(struct Int256* num, char* str, int mode)
{
    char first = 1;
    num->mostbit = -1;
    // ASCII mode
    if (mode = 0)
        for (int i = MAXBYTE - 1; i >= 0; --i)
        {        
            if (str[i] != 0 || first == 1)
            {
                char j = 7;
                while (str[i] >> j == 0) 
                    ;
                num->mostbit = 8*i + j;
                first = 0;
            }
            num->value[i] = str[i];     
        }
    
    // HEX mode
    if (mode = 1)
        for (int i = MAXBYTE - 1; i >= 0; --i)
        {
            num->value[i] = tonum(str[2*i]) << 4 + tonum(str[2*i + 1]);
            char first = 1;
            if (num->value[i] != 0 || first == 1)
            {
                char j = 7;
                while (num->value[i] >> j == 0) 
                    --j;
                num->mostbit = 8*i + j;
                first = 0;
            }    
        }

    // DEC mode
    if (mode = 2)
    {
        int exp = 0;
    }
}

void conv2char(char* str, struct Int256* num)
{
    for (int i = 1; i < MAXBYTE; ++i)
        str[i] = num->value[i];
}

void conv2hex(char* hex, struct Int256* num)
{
    for (int i = 0; i < MAXBYTE; ++i)
    {
        hex[2*i]     = tohex(num->value[i] >> 4);
        hex[2*i + 1] = tohex(num->value - (hex[2*i] >> 4));
    }
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
    temp.mostbit = -1;
    int carry = 0, value;
    for (int i = 0; i < MAXBYTE; ++i)
    {
        value = index(&a, i) ^ index(&b, i) ^ carry;
        if (value == 1) temp.mostbit = i;

        set(&temp, i, value);
        carry = index(&a, i) < index(&b, i) + carry? 1: 0; 
    } 
}

struct Int256 pls_mod(struct Int256 a, struct Int256 b, struct Int256 n)
{
    struct Int256 temp;
    temp.mostbit = -1;
    int carry = 0, value;
    for (int i = 0; i < MAXBYTE; ++i)
    {
        value = index(&a, i) ^ index(&b, i) ^ carry;
        if (value == 1) temp.mostbit = i;

        set(&temp, i, value);
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

    for (int i = a.mostbit-b.mostbit; i >= 0; --i)
        if (!less(remainder, shiftleft(b, i)))
            remainder = _sub(remainder, shiftleft(b, i));
    
    return remainder;
}

struct Int256 div(struct Int256 a, struct Int256 b)
{
    struct Int256 temp, remainder = a;
    
    if (less(a, b)) return zero();

    temp.mostbit = a.mostbit-b.mostbit;
    for (int i = temp.mostbit; i >= 0; --i)
    {
        if (less(remainder, shiftleft(b, i)))
            set(&temp, i, 0);
        else 
        {
            set(&temp, i, 1);
            remainder = _sub(remainder, shiftleft(b, i));
        }
    }
    if (index(&temp, temp.mostbit) == 0) 
        --temp.mostbit;
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

int main()
{
    return 0;
}