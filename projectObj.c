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

int _sub(INT256* result, INT256 a, INT256 b)
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
int _pls(INT256 *result, INT256 a, INT256 b)
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

unsigned char _tonum(unsigned char c)
{
    if (c == 0)            return 0;
    if (c > 47 && c <= 57) return c - 48;
    if (c > 64 && c <= 70) return c - 55;
}
unsigned char _tohex(unsigned char c)
{
    if (c < 10 && c >=  0) return c + 48;
    if (c < 17 && c >= 10) return c + 55;
}

int _index(INT256* num, int pos)
{
    if (MAXBIT > pos && pos >= 0)
        return (num->value[pos/8] >> (pos%8)) & 0x01;  
    else return -1; 
}
void _set(INT256* num, int pos, int value)
{
    if (MAXBIT > pos && pos >= 0 && value == 1)
        num->value[pos/8] |= 1 << (pos%8);
    if (MAXBIT > pos && pos >= 0 && value == 0) 
        num->value[pos/8] &= ~(1 << (pos%8));
}

void assign(INT256* des, INT256* ref)
{
    for (int i = 0; i < MAXBYTE; ++i)
        des->value[i] = ref->value[i];
}

INT256 int256_c(unsigned char* str, int mode)
{
    INT256 num;
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
            num_right = --len >= 0? _tonum(str[len]): 0;
            num_left  = --len >= 0? _tonum(str[len]): 0;
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

void conv2char(unsigned char* str, INT256* num)
{
    for (int i = 1; i < MAXBYTE; ++i)
        str[i] = num->value[i];
    str[MAXBYTE] = 0;
}
void conv2hex(unsigned char* hex, INT256* num)
{
    for (int i = 0; i < MAXBYTE; ++i)
    {
        hex[2*i]     = _tohex(num->value[i] & 0x0f);
        hex[2*i + 1] = _tohex(num->value[i] >> 4 & 0x0f );
    }
    hex[2*MAXBYTE] = 0;
    _reverse(hex);
}

INT256 shiftleft(INT256 num, int times)
{
    INT256 temp = zero;
    for (int i = 0; i < MAXBIT; ++i)
        _set(&temp, i + times, _index(&num, i));
    return temp;
}

INT256 pls(INT256 a, INT256 b, INT256 n)
{
    INT256 result;
    int carry;

    carry = _pls(&result, a, b);
    if (!eq(n, zero))
    {
        result = mod(result, n);
        if (carry == 1)
        {
            _pls(&result, result, mod(maxv, n));
            _pls(&result, one, result);
            result = mod(result, n);
        }
    }
    return result;
}
INT256 sub(INT256 a, INT256 b, INT256 n)
{
    INT256 result;
    int carry;
    
    carry = _sub(&result, a, b);
    if (!eq(n, zero))
    {
        result = mod(result, n);
        if (carry == 1)
        {
            _sub(&result, maxv, result);
            _pls(&result, mod(result, n), one);
            result = mod(result, n);
        }
    }
    return result;
}

INT256 pow(INT256 a, INT256 b, INT256 n)
{
    INT256 result;
    result = one;
    for (int i = MAXBIT - 1; i>= 0; --i)
    {
        result = mul(result, result, n);
        if (_index(&b, i) == 1) 
            result = mul(result, a, n);
    }
    return result;
}
INT256 mul(INT256 a, INT256 b, INT256 n)
{
    INT256 result, temp;
    result = zero;
    temp =  a;
    for (int i = 0; i < MAXBIT; ++i)
    {  
        if (_index(&b, i) == 1)
            result = pls(result, temp, n);
        if (_index(&temp, MAXBIT - 1) == 1) 
            temp = pls(pls(shiftleft(temp, 1), one, n), mod(maxv, n), n);
        else
            temp = mod(shiftleft(temp, 1), n); 
    }
    return result;
}

INT256 mod(INT256 a, INT256 b)
{
    INT256 remainder;
    remainder = a;

    int mostbit = MAXBIT-1;

    while (_index(&b, mostbit) == 0) --mostbit; 
    for (int i = MAXBIT - mostbit - 1; i >= 0; --i)
        if (!le(remainder, shiftleft(b, i)))
            _sub(&remainder ,remainder, shiftleft(b, i));
    
    return remainder;
}
INT256 div(INT256 a, INT256 b)
{
    INT256 temp, remainder;
    remainder = a;
    temp = zero;
    int mostbit = MAXBIT-1;

    // if (le(a, b)) return temp;

    while (_index(&b, mostbit) == 0) --mostbit; 
    for (int i = MAXBIT - mostbit - 1; i >= 0; --i)
    {
        if (le(remainder, shiftleft(b, i)))
            _set(&temp, i, 0);
        else 
        {
            _set(&temp, i, 1);
            _sub(&remainder, remainder, shiftleft(b, i));
        }
    }
    return temp;
}

int le(INT256 a, INT256 b)
{
    int i = MAXBYTE - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0 || a.value[i] > b.value[i]) return 0;
    else                                  return 1;
}
int eq(INT256 a, INT256 b)
{
    int i = MAXBYTE - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0) return 1;
    else       return 0;
}

void show(INT256 n)
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
    one = int256_c("1", HEXMODE);

    // Init end
    INT256 n = int256_c("921", HEXMODE), 
                  m = int256_c("3", HEXMODE), 
                  t = int256_c("2345678" , HEXMODE);
    // _set(&t, 0, 1);
    show(pow(n, m, NON));
    // show(div(n, m));
    // printf("\n");
    // printf("%d", _index(&t, 0));
    return 0;
}