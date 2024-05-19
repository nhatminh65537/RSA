#include "../header/int512.h"
#include <stdlib.h>
#include <time.h>

INT512 zero;
INT512 maxval;
INT512 one;

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

int _sub(INT512* result, INT512 a, INT512 b)
{
    unsigned long carry = 0;
    for (int i = 0; i < MAXBYTE; ++i)
    {
        result->value[i] = a.value[i] - b.value[i] - carry;
        if (a.value[i] < b.value[i] + carry) carry = 1;
        else                                 carry = 0;
    } 
    return carry;
} 
int _pls(INT512 *result, INT512 a, INT512 b)
{
    unsigned long carry = 0;
    for (int i = 0; i < MAXBYTE; ++i)
    {
        result->value[i] = a.value[i] + b.value[i] + carry;
        if (a.value[i] + b.value[i] + carry > 0xff) carry = 1;
        else                                        carry = 0;
    }
    return carry;
}
void _mul(INT512* result, INT512* over, INT512 a, INT512 b)
{
    *result = zero;
    *over = zero;
    unsigned long carry = 0;
    for (int i = 0; i < MAXBYTE; ++i){
        for (int j = 0; j <= i; ++j){
            carry = carry + (a.value[i-j]*b.value[j]);
        }
        result->value[i] = carry;
        carry = carry >> 8;
    }
    for (int i = 1; i < MAXBYTE; ++i){
        for (int j = i; j < MAXBYTE; ++j){
            carry = carry + (a.value[MAXBYTE-1+i-j]*b.value[j]);
        }
        over->value[i-1] = carry;
        carry = carry >> 8; 
    }
    over->value[MAXBYTE-1] = carry;
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

int _index(INT512* num, int pos)
{
    if (MAXBIT > pos && pos >= 0)
        return (num->value[pos/8] >> (pos%8)) & 0x1;  
    else return -1; 
}
void _set(INT512* num, int pos, int value)
{
    if (MAXBIT > pos && pos >= 0 && value == 1)
        num->value[pos/8] |= 1 << (pos%8);
    if (MAXBIT > pos && pos >= 0 && value == 0) 
        num->value[pos/8] &= ~(1 << (pos%8));
}

void assign(INT512* des, INT512* ref)
{
    for (int i = 0; i < MAXBYTE; ++i)
        des->value[i] = ref->value[i];
}

INT512 int256_c(unsigned char* str, enum Mode mode)
{
    int len = _len(str);
    INT512 result;

    // ASCII mode
    if (mode == ASCIIMODE)
        for (int i = 0; i < MAXBYTE; ++i)
        {
            result.value[i] = str[_min(i, len)];     
        }      
    
    // HEX mode
    if (mode == HEXMODE){
        int num_left, num_right;
        for (int i = 0; i < MAXBYTE; ++i)
        {
            num_right = --len >= 0? _tonum(str[len]): 0;
            num_left  = --len >= 0? _tonum(str[len]): 0;
            result.value[i] = num_right + (num_left << 4);
        }
    }
        
    // DEC mode
    if (mode == DECMODE){
        result = zero;
        INT512 ten = int256_c("A", HEXMODE);
        char dig[2];
        dig[1] = '\0';
        for (int i = 0; i < len; ++i){
            dig[0] = str[i];
            result = ipls(imul(result, ten, NON), int256_c(dig, HEXMODE), NON);
        }
    }

    return result;
}

void conv2char(unsigned char* str, INT512* num)
{
    for (int i = 0; i < MAXBYTE; ++i)
        str[i] = num->value[i];
    str[MAXBYTE] = 0;
}
void conv2hex(unsigned char* hex, INT512* num)
{
    for (int i = 0; i < MAXBYTE; ++i)
    {
        hex[2*i]     = _tohex(num->value[i] & 0x0f);
        hex[2*i + 1] = _tohex(num->value[i] >> 4 & 0x0f );
    }
    hex[MAXHEX] =  0 ;
    _reverse(hex);
}
void conv2dec(unsigned char* dec, INT512* num)
{
    int i = 0;
    INT512 ten, dig;
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

int ile(INT512 a, INT512 b)
{
    int i = MAXBYTE - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0 || a.value[i] > b.value[i]) return 0;
    else                                  return 1;
}
int ieq(INT512 a, INT512 b)
{
    int i = MAXBYTE - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0) return 1;
    else       return 0;
}
int igt(INT512 a, INT512 b)
{
    return !ieq(a, b) && !ile(a, b);
}

INT512 shiftleft(INT512 num, int times)
{
    INT512 result = zero;
    for (int i = times; i < MAXBIT; ++i)
        result.value[i/8] = result.value[i/8] | ((num.value[(i - times)/8] >> ((i - times)%8) & 1) << (i%8));
    return result;
}

INT512 imod(INT512 a, INT512 b)
{
    INT512 remainder;
    remainder = a;

    int mostbit = MAXBIT-1;

    while (_index(&b, mostbit) == 0) --mostbit; 
    for (int i = MAXBIT - mostbit - 1; i >= 0; --i)
        if (!ile(remainder, shiftleft(b, i)))
            _sub(&remainder ,remainder, shiftleft(b, i));
    
    return remainder;
}
INT512 idiv(INT512 a, INT512 b)
{
    INT512 result, remainder;
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

INT512 ipls(INT512 a, INT512 b, INT512 n)
{
    INT512 result;
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
INT512 isub(INT512 a, INT512 b, INT512 n)
{
    INT512 result;
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

INT512 imul(INT512 a, INT512 b, INT512 n)
{
    INT512 result, temp, over;
    _mul(&result, &over, a, b);
    if (!ieq(n, NON)){
        do {
            result = ipls(result, over, n);
            _mul(&temp, &over, imod(over, n), imod(maxval, n));
            result = ipls(result, temp, n);
        } while (igt(over, zero));
    }
    // result = zero;
    // temp =  a;
    // for (int i = 0; i < MAXBIT; ++i)
    // {  
    //     if (_index(&b, i) == 1)
    //         result = ipls(result, temp, n);
    //     if (_index(&temp, MAXBIT - 1) == 1) 
    //         temp = ipls(ipls(shiftleft(temp, 1), one, n), imod(maxval, n), n);
    //     else
    //         temp = imod(shiftleft(temp, 1), n); 
    // }
    return result;
}

INT512 ipow(INT512 a, INT512 b, INT512 n)
{
    INT512 result;
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

INT512 imulInverse(INT512 n, INT512 a)
{
    INT512 a0 = n, b0 = a;
    INT512 t0 = zero, t = one;
    INT512 q, r, temp;
 
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

INT512 irand(int minByte, int maxByte)
{
    INT512 result = zero;
    for (int i = 0; i < maxByte; i++) {
        result.value[i] = rand();
    }
    int check = 0;
    for (int i = minByte; i < maxByte; ++i) if (result.value[i] != 0) check = 1;
    if (!check) result = irand(minByte, maxByte);
    return result;
}

INT512 igcd(INT512 a, INT512 b) 
{
    while (!ieq(b, zero)) {
        INT512 r = imod(a, b);
        a = b;
        b = r;
    }
    return a;
}

void initInt()
{
    srand(time(NULL));
    zero = int256_c("0", HEXMODE);
    for (int i = 0; i < MAXBYTE; ++i) maxval.value[i] = 0xff;
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