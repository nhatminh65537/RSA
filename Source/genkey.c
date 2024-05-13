#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../Header/int256.h"

#define BITS 255

INT256 randint256()
{
    srand(time(NULL));
    INT256 result;
    for (int i  = 0; i < MAXBYTE; ++i)
    {
        result.value[i] = rand() % 256;
        printf("%2x", result.value[i]);
    }
    printf("\n");
    return result;
}

bool millerRabin(INT256 n, int iterations) {
    if (ieq(n, one) || ieq(n, int256_c("2", ASCIIMODE))) {
        return true;
    }
    if (ile(n, int256_c("2", ASCIIMODE)) || ile(imod(n, int256_c("2", ASCIIMODE)), zero)) {
        return false;
    }

    INT256 s = zero;
    INT256 d = isub(n, one, NON);
    while (ile(imod(d, int256_c("2", ASCIIMODE)), zero)) {
        d = idiv(d, int256_c("2", ASCIIMODE));
        s = ipls(s, one, NON);
    }

    for (int i = 0; i < iterations; i++) {
        INT256 a = ipls(randint256(), one, NON);
        INT256 x = ipow(a, d, n);
        if (ieq(x, one) || ieq(x, isub(n, one, NON))) {
            continue;
        }
        for (INT256 j = zero; ile(j, isub(s, one, NON)); j = ipls(j, one, NON)) {
            x = ipow(x, int256_c("2", ASCIIMODE), n);
            if (ieq(x, one)) {
                return false;
            }
            if (ieq(x, isub(n, one, NON))) {
                break;
            }
        }
        if (!ieq(x, isub(n, one, NON))) {
            return false;
        }
    }
    return true;
}
INT256 igcd_INT256(INT256 a, INT256 b) {
    if (ieq(b, zero)) {
        return a;
    }
    return igcd_INT256(b, imod(a, b));
}

void genkey(int mode , INT256* p, INT256* q, INT256* n, INT256* e, INT256* d, char* filename){
    switch(mode){
        case 0:
            srand(time(NULL));
            INT256 p0 = randint256();
            INT256 q0 = randint256();
            while (!millerRabin(p0,100) || !millerRabin(q0,100))
            {
                p0 = randint256();
                q0 = randint256();
            }
            INT256 n0 = imul(p0, q0,NON);
            INT256 phi0 = imul(isub(p0, one,NON), isub(q0, one,NON),NON);
            INT256 e0 = int256_c("65537", ASCIIMODE);
            while (!ieq(igcd_INT256(e0, phi0) , one))
            {
                e0 = randint256();
            }
            INT256 d0 = imulInverse(phi0, e0);
            *p = p0;
            *q = q0;
            *n = n0;
            *e = e0;
            *d = d0;
            break;
        case 1:
            INT256 d1;
            srand(time(NULL));
            INT256 p1 = randint256();
            INT256 q1 = randint256();
            while (!millerRabin(p1,100) || !millerRabin(q1,100))
            {
                p1 = randint256();
                q1 = randint256();
            }
            INT256 n1 = imul(p1, q1, NON);
            INT256 phi1 = imul(isub(p1, one, NON), isub(q1, one, NON),NON);
            INT256 e1 = imulInverse(phi1, d1);
            *p = p1;
            *q = q1;
            *n = n1;
            *e = e1;
            *d = d1;
            break;
        case 2:
            INT256 e;
            srand(time(NULL));
            INT256 p2 = randint256();
            INT256 q2 = randint256();
            while (!millerRabin(p2,100) || !millerRabin(q2,100))
            {
                p2 = randint256();
                q2 = randint256();
            }
            INT256 n2 = imul(p2, q2, NON);
            INT256 phi2 = imul(isub(p2, one, NON), isub(q2, one, NON),NON);
            INT256 e2 = imulInverse(phi2, e);
            *p = p2;
            *q = q2;
            *n = n2;
            e = e2;
            break;
        case 3:
             FILE *file = fopen(filename, "r");
            if (file != NULL) {
                fscanf(file, "%d %d %d", &(p->value), &(q->value), &(d->value));
                fclose(file);
                *n = imul(*p, *q, NON);
                INT256 phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
                *d = d1;
                e = imulInverse(phi, *d);
            }
            break;
    }
}

void printInt256(INT256 num) 
{
    for (int i = MAXBYTE - 1; i >= 0; i--) {
        printf("%02x", num.value[i]);
    }
    printf("\n");
}

int main() {
    // INT256 result;
    // result = randint256();
    // int i;

    // printf("Prime number in Int256 format: ");
    // printInt256(result);

    // return 0;
}
