#include "projectObj.h"

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

char index(struct Int256* num, int pos)
{
    if (256 > num && num >= 0)
        return (num->value[pos/8] >> (pos%8)) & 1;
    else return 2; 
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
    // ASCII mode
    if (mode = 0)
        for (int i = MAXBYTE - 1; i >= 0; --i)
        {
            char first = 1;
            if (str[i] != 0 || first == 1)
            {
                char j = 7;
                while (str[i] >> j == 0) 
                    ;
                num->mostbit = 8*i + j;
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

struct Int256* pls(struct Int256* a, struct Int256* b, struct Int256* n)
{
    char overflow;
    for (int i = 0; i < MAXBYTE; ++i);

}

struct Int256* sub(struct Int256*, struct Int256*, struct Int256*);
struct Int256* mul(struct Int256*, struct Int256*, struct Int256*);
struct Int256* pow(struct Int256*, struct Int256*, struct Int256*);

struct Int256* mod(struct Int256* a, struct Int256* b)
{
    
}
struct Int256* div(struct Int256* a, struct Int256* b)
{
    if (a->mostbit-b->mostbit < 0) 
    {
        temp->mostbit == 0;

    }
    temp->mostbit = a->mostbit-b->mostbit
}

int main()
{
    return 0;
}