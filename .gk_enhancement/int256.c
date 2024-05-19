#include "int256.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

INT256 zero;
INT256 maxval;
INT256 one;

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
    unsigned long carry = 0;
    for (int i = 0; i < MAXWORD; ++i)
    {
        result->value[i] = a.value[i] - b.value[i] - carry;
        if (a.value[i] < b.value[i] + carry) carry = 1;
        else                                 carry = 0;
    } 
    return carry;
} 
int _pls(INT256 *result, INT256 a, INT256 b)
{
    unsigned long carry = 0;
    for (int i = 0; i < MAXWORD; ++i)
    {
        result->value[i] = a.value[i] + b.value[i] + carry;
        if (a.value[i] + b.value[i] + carry > maxval.value[0]) carry = 1;
        else                                                   carry = 0;
    }
    return carry;
}
void _mul(INT256* result, INT256* over, INT256 a, INT256 b)
{
    *result = zero;
    *over = zero;
    unsigned long carry = 0;
    for (int i = 0; i < MAXWORD; ++i){
        for (int j = 0; j <= i; ++j){
            carry = carry + (a.value[i-j]*b.value[j]);
        }
        result->value[i] = carry;
        carry = carry >> WORD;
    }
    for (int i = 1; i < MAXWORD; ++i){
        for (int j = i; j < MAXWORD; ++j){
            carry = carry + (a.value[MAXWORD-1+i-j]*b.value[j]);
        }
        over->value[i-1] = carry;
        carry = carry >> WORD; 
    }
    over->value[MAXWORD-1] = carry;
}

unsigned char _tonum(unsigned char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}
unsigned char _tohex(unsigned char c)
{
    if (c <=  9 && c >=  0) return c + '0';
    if (c <= 16 && c >= 10) return c + 'A' - 10;
    return '0';
}

int _index(INT256* num, int pos)
{
    if (MAXBIT > pos && pos >= 0)
        return (num->value[pos/WORD] >> (pos%WORD)) & 1;  
    else return -1; 
}
void _set(INT256* num, int pos, int value)
{
    if (MAXBIT > pos && pos >= 0 && value == 1)
        num->value[pos/WORD] |= 1 << (pos%WORD);
    if (MAXBIT > pos && pos >= 0 && value == 0) 
        num->value[pos/WORD] &= ~(1 << (pos%WORD));
}

void assign(INT256* des, INT256* ref)
{
    for (int i = 0; i < MAXWORD; ++i)
        des->value[i] = ref->value[i];
}

INT256 int256_c(unsigned char* str, enum Mode mode)
{
    int len = _len(str);
    INT256 result;

    // ASCII mode
    if (mode == ASCIIMODE)
        for (int i = 0; i < MAXWORD; ++i)
        {
            result.value[i] = str[_min(i, len)];     
        }      
    
    // HEX mode
    if (mode == HEXMODE){
        sscanf(str, "%x", result.value);
        for (int i = len/(WORD/4) + 1; i < MAXWORD; ++i) result.value[i] = 0;
    }
        
    // DEC mode
    if (mode == DECMODE){
        result = zero;
        INT256 ten = int256_c("A", HEXMODE);
        char dig[2];
        dig[1] = '\0';
        for (int i = 0; i < len; ++i){
            dig[0] = str[i];
            result = ipls(imul(result, ten, NON), int256_c(dig, HEXMODE), NON);
        }
    }

    return result;
}

void conv2char(unsigned char* str, INT256* num)
{
    for (int i = 0; i < MAXWORD; ++i)
        str[i] = num->value[i];
    str[MAXWORD] = 0;
}

void conv2hex(unsigned char* hex, INT256* num)
{
    for (int i = MAXWORD - 1; i >=0; --i){
        sprintf(hex, "%04x", num->value[i]);
        hex += 4;
    }
    hex[MAXHEX] =  0;
}

void conv2dec(unsigned char* dec, INT256* num)
{
    int i = 0;
    INT256 ten, dig;
    dig = *num;
    ten = int256_c("A", HEXMODE);
    do{
        dec[i] = '0' + imod(dig, ten).value[0];
        dig = idiv(dig, ten);
        ++i;
    }while(igt(dig, zero));
    dec[i] = 0;
    _reverse(dec);
}

int ile(INT256 a, INT256 b)
{
    int i = MAXWORD - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0 || a.value[i] > b.value[i]) return 0;
    else                                  return 1;
}
int ieq(INT256 a, INT256 b)
{
    int i = MAXWORD - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0) return 1;
    else       return 0;
}
int igt(INT256 a, INT256 b)
{
    return !ieq(a, b) && !ile(a, b);
}

INT256 shiftleft(INT256 num, int times)
{
    INT256 result = zero;
    for (int i = times; i < MAXBIT; ++i)
        result.value[i/WORD] = result.value[i/WORD] | ((num.value[(i - times)/WORD] >> ((i - times)%WORD) & 1) << (i%WORD));
    return result;
}

INT256 imod(INT256 a, INT256 b)
{
    INT256 remainder;
    remainder = a;

    int mostbit = MAXBIT-1;

    while (_index(&b, mostbit) == 0) --mostbit; 
    for (int i = MAXBIT - mostbit - 1; i >= 0; --i)
        if (!ile(remainder, shiftleft(b, i)))
            _sub(&remainder ,remainder, shiftleft(b, i));
    
    return remainder;
}
INT256 idiv(INT256 a, INT256 b)
{
    INT256 result, remainder;
    remainder = a;
    result = zero;
    int mostbit = MAXBIT - 1;

    while (_index(&b, mostbit) == 0) --mostbit; 
    for (int i = MAXBIT - 1 - mostbit; i >= 0; --i)
    {
        if (ile(remainder, shiftleft(b, i)))
            _set(&result, i, 0);
        else 
        {
            _set(&result, i, 1);
            _sub(&remainder, remainder, shiftleft(b, i));
        }
    }
    return result;
}

INT256 ipls(INT256 a, INT256 b, INT256 n)
{
    INT256 result;
    int carry;

    carry = _pls(&result, a, b);
    if (!ieq(n, zero))
    {
        result = imod(result, n);
        if (carry == 1)
        {
            _pls(&result, result, imod(maxval, n));
            _pls(&result, one, result);
            result = imod(result, n);
        }
    }
    return result;
}
INT256 isub(INT256 a, INT256 b, INT256 n)
{
    INT256 result;
    int carry;
    
    carry = _sub(&result, a, b);
    if (!ieq(n, zero))
    {  
        result = imod(result, n);
        if (carry == 1)
        {
            _sub(&result, b, a);
            result = imod(result, n);
            if (!ieq(result, zero))_sub(&result, n, result);
        }
    }
    return result;
}

INT256 imul(INT256 a, INT256 b, INT256 n)
{

    INT256 result, temp, over;
    result = zero;
    temp =  a;
    _mul(&result, &over, a, b);
    if (!ieq(n, NON)){
        do {
            result = ipls(result, over, n);
            _mul(&temp, &over, imod(over, n), imod(maxval, n));
            result = ipls(result, temp, n);
        } while (igt(over, zero));
    }
//     for (int i = 0; i < MAXBIT; ++i)
//     {  
//         if (_index(&b, i) == 1)
//             result = ipls(result, temp, n);
//         if (_index(&temp, MAXBIT - 1) == 1) 
//             temp = ipls(ipls(shiftleft(temp, 1), one, n), imod(maxval, n), n);
//         else
//             temp = imod(shiftleft(temp, 1), n); 
//     }
    return result;
}

INT256 ipow(INT256 a, INT256 b, INT256 n)
{
    INT256 result;
    result = one;
    int next = 1;
    for (int i = MAXBIT - 1; i>= 0; --i)
    {
        if (_index(&b, i) == 0 && next) continue;
        next = 0; 
        
        result = imul(result, result, n);
        if (_index(&b, i) == 1) 
            result = imul(result, a, n);
    }
    return result;
}

INT256 imulInverse(INT256 n, INT256 a)
{
    INT256 a0 = n, b0 = a;
    INT256 t0 = zero, t = one;
    INT256 q, r, temp;
 
    q = idiv(a0, b0);
    r = imod(a0, b0);

    while (igt(r, zero)) {
        
        temp = isub(t0, imul(q, t, n), n);
        t0 = t;
        t = temp;
        a0 = b0;
        b0 = r;
        q = idiv(a0, b0);
        r = imod(a0, b0);
    }

    if (!ieq(b0, one)) {
        return zero;
    }
    return t;
}

INT256 irand(int minByte, int maxByte)
{
    INT256 result = zero;
    for (int i = 0; i < maxByte; i++) {
        result.value[i] = rand();
    }
    int check = 0;
    for (int i = minByte; i < maxByte; ++i) if (result.value[i] != 0) check = 1;
    if (!check) result = irand(minByte, maxByte);
    return result;
}

INT256 igcd(INT256 a, INT256 b) 
{
    while (!ieq(b, zero)) {
        INT256 r = imod(a, b);
        a = b;
        b = r;
    }
    return a;
}

void initInt()
{
    srand(time(NULL));
    zero = int256_c("0", HEXMODE);
    for (int i = 0; i < MAXWORD; ++i) maxval.value[i] = -1;
    one = int256_c("1", HEXMODE);
}

// int main()
// {
//     initInt();
//     char ch[1024];
//     INT256 n,m,t,k,a,b,p;
//     n = int256_c("Re", ASCIIMODE), 
//     m = int256_c("6f"  , HEXMODE), 
//     t = int256_c("3", HEXMODE),
//     k = int256_c("123", HEXMODE),
//     a = int256_c("aaff541414514355145454514545fffaaff541414514355145454514545fff", HEXMODE),
//     b = int256_c("fffee13466756453bbbc92347767677aaff541414514355145454514545fff", HEXMODE),
//     p = int256_c("234134bcbcbbfbeba75346741326481aaff541414514355145454514545fff", HEXMODE);
//     // show(n, HEXMODE);
//     // conv2hex(&ch, &n);
//     // printf("%s", ch);
//     // printf("%d\n", &a);
//     // INT256 re;
//     // re = imul(a, b, p);
//     // show(ipow(a, b, p), HEXMODE); printf("\n");
//     // show(a, HEXMODE);
//     // printf("\n%d\n", &a);
//     // show(ipow(m, t, k), HEXMODE);
//     return 0;
// }