#include "../header/int256.h"
// #include <stdio.h>

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

INT256 int256_c(unsigned char* str, enum Mode mode)
{
    int len = _len(str);
    INT256 result;

    // ASCII mode
    if (mode == ASCIIMODE)
        for (int i = 0; i < MAXBYTE; ++i)
        {
            result.value[i] = str[_min(i, len)];     
        }      
    
    // HEX mode
    if (mode == HEXMODE)
    {
        int num_left, num_right;
        for (int i = 0; i < MAXBYTE; ++i)
        {
            num_right = --len >= 0? _tonum(str[len]): 0;
            num_left  = --len >= 0? _tonum(str[len]): 0;
            result.value[i] = num_right + (num_left << 4);
        }
    }
        
    // DEC mode
    // Later update

    return result;
}

void conv2char(unsigned char* str, INT256* num)
{
    for (int i = 0; i < MAXBYTE; ++i)
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
    hex[MAXHEX] =  0 ;
    _reverse(hex);
}
void conv2dec(unsigned char* dec, INT256* num)
{
    int i = 0;
    INT256 ten, re, dig;
    dig = one;
    ten = int256_c("A", HEXMODE);
    do{
        re = imod(idiv(*num, dig), imul(ten, dig, NON));
        dec[i] = '0' + re.value[0];
        dig = imul(dig, ten, NON);
        ++i;
    }while(igt(*num, dig));
    dec[i] = 0;
    _reverse(dec);
}

int ile(INT256 a, INT256 b)
{
    int i = MAXBYTE - 1, result = 0;
    while (i >= 0 && a.value[i] == b.value[i])
        --i;
    if (i < 0 || a.value[i] > b.value[i]) return 0;
    else                                  return 1;
}
int ieq(INT256 a, INT256 b)
{
    int i = MAXBYTE - 1, result = 0;
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
    for (int i = 0; i < MAXBIT; ++i)
        _set(&result, i + times, _index(&num, i));
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
            _sub(&result, maxval, result);
            _pls(&result, imod(result, n), one);
            result = imod(result, n);
            if (!ieq(result, zero)) _sub(&result, n, result);
        }
    }
    return result;
}

INT256 imul(INT256 a, INT256 b, INT256 n)
{
    INT256 result, temp;
    result = zero;
    temp =  a;
    for (int i = 0; i < MAXBIT; ++i)
    {  
        if (_index(&b, i) == 1)
            result = ipls(result, temp, n);
        if (_index(&temp, MAXBIT - 1) == 1) 
            temp = ipls(ipls(shiftleft(temp, 1), one, n), imod(maxval, n), n);
        else
            temp = imod(shiftleft(temp, 1), n); 
    }
    return result;
}

INT256 ipow(INT256 a, INT256 b, INT256 n)
{
    INT256 result;
    result = one;
    for (int i = MAXBIT - 1; i>= 0; --i)
    {
        result = imul(result, result, n);
        if (_index(&b, i) == 1) 
            result = imul(result, a, n);
    }
    return result;
}
INT256 imulInverse(INT256 a, INT256 n)
{
    INT256 a0 = a, n0 = n;
    INT256 t0 = {0}, t = one;
    INT256 q, r, temp;
    INT256 zero = {0};
 
    q = idiv(a0, n0);
    r = isub(a0, imul(q, n0, n0), n0);

    while (ile(r, zero)) {
        temp = isub(t0, imul(q, t, a0), a0);
        t0 = temp;
        a0 = n0;
        n0 = r;
        q = idiv(a0, n0);
        r = isub(a0, imul(q, n0, n0), n0);
    }

    if (!ieq(n0, one)) {
        printf("Cannot calculate inverse with module other than 1 \n");
        return zero;
    }
    return t;
}

INT256 imulInverse(INT256 a, INT256 n)
{
    INT256 a0 = a, n0 = n;
    INT256 t0 = {0}, t = one;
    INT256 q, r, temp;
    INT256 zero = {0};
 
    q = idiv(a0, n0);
    r = isub(a0, imul(q, n0, n0), n0);

    while (ile(r, zero)) {
        temp = isub(t0, imul(q, t, a0), a0);
        t0 = temp;
        a0 = n0;
        n0 = r;
        q = idiv(a0, n0);
        r = isub(a0, imul(q, n0, n0), n0);
    }

    if (!ieq(n0, one)) {
        printf("Cannot calculate inverse with module other than 1 \n");
        return zero;
    }
    return t;
}

void initInt()
{
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