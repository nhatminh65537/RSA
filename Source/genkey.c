#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Header/int256.h"

INT256 randint256(int N, int M)
{
    srand(time(NULL));
    INT256 min = zero ;
    INT256 max = zero;
    for (int i = 0; i < N; i++) {
        min.value[i/64] |= 1 << (i%64);
    }
    for (int i = 0; i < M; i++) {
        max.value[i/64] |= 1 << (i%64);
    }

    INT256 result;
    for (int i = 0; i < MAXBYTE; i++) {
        result.value[i] = min.value[i] + rand() % (max.value[i] - min.value[i] + 1);
    }
    return result;
}

int millerRabin(INT256 n, int iterations) {
    int N,M;
    if (ieq(n, one) || ieq(n, int256_c("2", ASCIIMODE))) {
        return 1;
    }
    if (ile(n, int256_c("2", ASCIIMODE)) || ile(imod(n, int256_c("2", ASCIIMODE)), zero)) {
        return 0;
    }

    INT256 s = zero;
    INT256 d = isub(n, one, NON);
    while (ile(imod(d, int256_c("2", ASCIIMODE)), zero)) {
        d = idiv(d, int256_c("2", ASCIIMODE));
        s = ipls(s, one, NON);
    }

    for (int i = 0; i < iterations; i++) {
        INT256 a = randint256(N,M);
        INT256 x = ipow(a, d, n);
        if (ieq(x, one) || ieq(x, isub(n, one, NON))) {
            continue;
        }
        for (INT256 j = zero; ile(j, isub(s, one, NON)); j = ipls(j, one, NON)) {
            x = ipow(x, int256_c("2", ASCIIMODE), n);
            if (ieq(x, one)) {
                return 0;
            }
            if (ieq(x, isub(n, one, NON))) {
                break;
            }
        }
        if (!ieq(x, isub(n, one, NON))) {
            return 0;
        }
    }
    return 1;
}

// Khong de quy
INT256 igcd_INT256(INT256 a, INT256 b) {
    while (!ieq(b, zero)) {
        INT256 r = imod(a, b);
        a = b;
        b = r;
    }
    return a;
}

void genkey(int mode , INT256* p, INT256* q, INT256* n, INT256* e, INT256* d, char* filename){
    int N,M;
    switch(mode){
        case 0:
            *p = randint256(N,M);
            *q = randint256(N,M);
            while (!millerRabin(*p,100) || !millerRabin(*p,100))
            {
                *p = randint256(N,M);
                *q = randint256(N,M);
            }
            *n = imul(*p, *q, NON);
            INT256 phi0 = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            INT256 e0 = int256_c("65537", ASCIIMODE);
            while (!ieq(igcd_INT256(e0, phi0) , one))
            {
                e0 = randint256(N,M);
            }
            INT256 d0 = imulInverse(phi0, e0);
            *e = e0;
            *d = d0;
            break;
        case 1:
            INT256 d1;
            srand(time(NULL));
            INT256 p1 = randint256(N,M);
            INT256 q1 = randint256(N,M);
            while (!millerRabin(p1,100) || !millerRabin(q1,100))
            {
                p1 = randint256(N,M);
                q1 = randint256(N,M);
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
            INT256 p2 = randint256(N,M);
            INT256 q2 = randint256(N,M);
            while (!millerRabin(p2,100) || !millerRabin(q2,100))
            {
                p2 = randint256(N,M);
                q2 = randint256(N,M);
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
            FILE* file = fopen(filename, "rb");
            if (file == NULL) {
                return;
            }
            fread(n->value, 1, MAXBYTE, file);
            fread(e.value, 1, MAXBYTE, file);
            fclose(file);
            *n = imul(*p, *q, NON);
            INT256 phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            e = imulInverse(phi, *d);
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
