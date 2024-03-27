#include "projectObj.h"
#include <stdio.h>

int _min(int a, int b) 
{
    return a > b? b: a;
}

void _reverse(unsigned char *str)
{
    int ri = 0;
    char temp;
    while (str[ri] != 0) ++ri;
    --ri;
    for (int i = 0; i < ri; ++i, --ri)
    {
        temp = str[i];
        str[i] = str[ri];
        str[ri] = temp;
    }
}

int _len(unsigned char *str)
{
    int result = 0;
    while (str[result] != 0) ++result;
    return result;
}

int _sub(struct Int256* result, struct Int256 a, struct Int256 b)
{
    int carry = 0;
    for (int i = 0; i < MAXBYTE; ++i)
    {
        result->value[i] = a.value[i] - b.value[i] - carry;
        if (a.value[i] < b.value[i] + carry) carry = 1;
        else                                 carry = 0;
    } 
    return carry;
} 

int _pls(struct Int256 *result, struct Int256 a, struct Int256 b)
{
    int carry = 0;
    for (int i = 0; i < MAXBYTE; ++i)
    {
        result->value[i] = a.value[i] + b.value[i] + carry;
        if (a.value[i] + b.value[i] + carry > 0xff) carry = 1;
        else                                        carry = 0;
    }
    return carry;
}

unsigned char tonum(unsigned char c)
{
    if (c == 0)            return 0;
    if (c > 47 && c <= 57) return c - 48;
    if (c > 64 && c <= 70) return c - 55;
}

unsigned char tohex(unsigned char c)
{
    if (c < 10 && c >=  0) return c + 48;
    if (c < 17 && c >= 10) return c + 55;
}

int index(struct Int256* num, int pos)
{
    if (MAXBIT > pos && pos >= 0)
        return (num->value[pos/8] >> (pos%8)) & 0x01;  
    else return -1; 
}

void set(struct Int256* num, int pos, int value)
{
    if (MAXBIT > pos && pos >= 0 && value == 1)
        num->value[pos/8] |= 1 << (pos%8);
    if (MAXBIT > pos && pos >= 0 && value == 0) 
        num->value[pos/8] &= ~(1 << (pos%8));
}

void assign(struct Int256* des, struct Int256* ref)
{
    for (int i = 0; i < MAXBYTE; ++i)
        des->value[i] = ref->value[i];
}

struct Int256 int256_c(unsigned char* str, int mode)
{
    struct Int256 num;
    int len = _len(str);


    // ASCII mode
    if (mode == ASCIIMODE)
        for (int i = 0; i < MAXBYTE; --i)
        {
            num.value[i] = str[_min(i, len)];     
        }      
    
    // HEX mode
    if (mode == HEXMODE)
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

void conv2char(unsigned char* str, struct Int256* num)
{
    for (int i = 1; i < MAXBYTE; ++i)
        str[i] = num->value[i];
    str[MAXBYTE] = 0;
}

void conv2hex(unsigned char* hex, struct Int256* num)
{
    for (int i = 0; i < MAXBYTE; ++i)
    {
        hex[2*i]     = tohex(num->value[i] & 0x0f);
        hex[2*i + 1] = tohex(num->value[i] >> 4 & 0x0f );
    }
    hex[2*MAXBYTE] = 0;
    _reverse(hex);
}

struct Int256 shiftleft(struct Int256 num, int times)
{
    struct Int256 temp = zero;
    for (int i = 0; i < MAXBIT; ++i)
        set(&temp, i + times, index(&num, i));
    return temp;
}

struct Int256 pls(struct Int256 a, struct Int256 b, struct Int256 n)
{
    struct Int256 result;
    int carry = 0, value;
    carry = _pls(&result, a, b);
    if (!eq(n, zero))
    {
        result = mod(result, n);
        if (carry == 1)
        {
            _pls(&result, result, mod(maxv, n));
            _pls(&result, result, int256_c("1", HEXMODE));
            result = mod(result, n);
        }
    }
    return result;
}

struct Int256 sub(struct Int256 a, struct Int256 b, struct Int256 n)
{
    struct Int256 result;
    _sub(&result, a, b);
    return mod(result, n);
}
struct Int256 pow(struct Int256, struct Int256, struct Int256);
struct Int256 mul(struct Int256, struct Int256, struct Int256);

struct Int256 mod(struct Int256 a, struct Int256 b)
{
    struct Int256 remainder;
    remainder = a;

    int mostbit = MAXBIT-1;

    while (index(&b, mostbit) == 0) --mostbit; 
    for (int i = MAXBIT - mostbit - 1; i >= 0; --i)
        if (!le(remainder, shiftleft(b, i)))
            _sub(&remainder ,remainder, shiftleft(b, i));
    
    return remainder;
}

struct Int256 div(struct Int256 a, struct Int256 b)
{
    struct Int256 temp, remainder;
    remainder = a;
    temp = zero;
    int mostbit = MAXBIT-1;

    // if (le(a, b)) return temp;

    while (index(&b, mostbit) == 0) --mostbit; 
    for (int i = MAXBIT - mostbit - 1; i >= 0; --i)
    {
        if (le(remainder, shiftleft(b, i)))
            set(&temp, i, 0);
        else 
        {
            set(&temp, i, 1);
            _sub(&remainder, remainder, shiftleft(b, i));
        }
    }
    return temp;
}

int le(struct Int256 a, struct Int256 b)
{
    int i = MAXBYTE - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0 || a.value[i] > b.value[i]) return 0;
    else                                  return 1;
}
int eq(struct Int256 a, struct Int256 b)
{
    int i = MAXBYTE - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0) return 1;
    else       return 0;
}
void show(struct Int256 n)
{
    char hex[MAXHEX];
    conv2hex(hex, &n);
    printf("%s", hex);
}

int main()
{
    // Init start
    zero = int256_c("0", HEXMODE);
    for (int i = 0; i < MAXBYTE; ++i) maxv.value[i] = 0xff;
    // Init end
    struct Int256 n = int256_c("99212311", HEXMODE), 
                  m = int256_c("96541111", HEXMODE), 
                  t = int256_c("2345678" , HEXMODE);
    // set(&t, 0, 1);
    show(n);
    // show(div(n, m));
    // printf("\n");
    // printf("%d", index(&t, 0));
    return 0;
}