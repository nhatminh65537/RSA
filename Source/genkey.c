#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../Header/int256.h"

#define BITS 255

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
        d = idiv(d, int256_c("2", ASCIIMODE), NON);
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

void genkey(int mode , INT256* p, INT256* q, INT256* n, INT256* e, INT256* d, char* filename){
    switch(mode){
        case 0:
            srand(time(NULL));
            INT256 p0, q0, n0, e0, d0;
            INT256 p0 = randint256();
            INT256 q0 = randint256();
            while (!millerRabin(p1) || !millerRabin(q1))
            {
                p0 = randint256();
                q0 = randint256();
            }
            INT256 n0 = imul(p1, q1,NON);
            INT256 phi0 = imul(isub(p1, one,NON), isub(q1, one,NON));
            INT256 e0 = int256_c("65537", ASCIIMODE);
            while (igcd(e0, phi) != one)
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
            srand(time(NULL));
            INT256 p1, q1, n1, e1;
            p1 = randint256();
            q1 = randint256();
            while (!millerRabin(p1) || !millerRabin(q1))
            {
                p1 = randint256();
                q1 = randint256();
            }
            n1 = imul(p1, q1, NON);
            INT256 phi1 = imul(isub(p1, one, NON), isub(q1, one, NON));
            e1 = imulInverse(phi1, d);
            *p = p1;
            *q = q1;
            *n = n1;
            *e = e1;
            break;
        case 2:
            srand(time(NULL));
            INT256 p2, q2, n2, d2;
            p2 = randint256();
            q2 = randint256();
            while (!millerRabin(p2) || !millerRabin(q2))
            {
                p2 = randint256();
                q2 = randint256();
            }
            n2 = imul(p2, q2, NON);
            INT256 phi2 = imul(isub(p1, one, NON), isub(q1, one, NON),NON);
            d2 = imulInverse(phi2, e);
            *p = p2;
            *q = q2;
            *n = n2;
            *e = e2;

        case 3:
             FILE *file = fopen("filename.txt", "r");
            if (file != NULL) {
                fscanf(file, "%d %d %d", &(*p), &(*q), &(*d));
                fclose(file);
            }
            break;
    }
}

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



void printInt256(INT256 num) 
{
    for (int i = MAXBYTE - 1; i >= 0; i--) {
        printf("%02x", num.value[i]);
    }
    printf("\n");
}

int main() {
    // // uint64_t bigPrime = getBigPrime();
    // INT256 result;
    // result = randint256();
    // int i;

    // printf("Prime number in Int256 format: ");
    // printInt256(result);

    // return 0;
}
