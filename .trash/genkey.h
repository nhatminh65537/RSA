#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../Header/int256.h"
#define NUM_GKTHREADS 32

INT512 irand(int minByte, int maxByte)
{
    INT512 result = zero;
    for (int i = 0; i < maxByte; i++) {
        result.value[i] = rand() % 256;
    }
    int check = 0;
    for (int i = minByte; i < maxByte; ++i) if (result.value[i] != 0) check = 1;
    if (!check) result = irand(minByte, maxByte);
    return result;
}


int millerRabin(INT512 n, int iterations) {
    if (ieq(n, int256_c("2", HEXMODE))){
        return 1;
    }
    if ( ieq(n, zero) 
         || ieq(imod(n, int256_c("2", HEXMODE)), zero)
         || ieq(imod(n, int256_c("3", HEXMODE)), zero)
         || ieq(imod(n, int256_c("5", HEXMODE)), zero)
         || ieq(imod(n, int256_c("7", HEXMODE)), zero)
         || ieq(imod(n, int256_c("B", HEXMODE)), zero)
         || ieq(imod(n, int256_c("D", HEXMODE)), zero)){
        return 0;
    }

    INT512 s = zero;
    INT512 d = isub(n, one, NON);
    while (ieq(imod(d, int256_c("2", HEXMODE)), zero)) {
        d = idiv(d, int256_c("2", HEXMODE));
        s = ipls(s, one, NON);
    }

    for (int i = 0; i < iterations; i++) {
        int f = 1;
        INT512 a = irand(0, MAXBYTE/2);
        while (!(igt(a, one) && (ile(a, n)))) a = irand(0, MAXBYTE/2);
        INT512 x = ipow(a, d, n);
        if (ieq(x, one)) {
            break;
        }

        for (INT512 j = zero; ile(j, isub(s, one, NON)); j = ipls(j, one, NON)) {
            if (ieq(x, isub(n, one, NON))) {
                break;
                f = 0;
            } else {
                x = ipow(x, int256_c("2", HEXMODE), n);
            }
            if (f == 0) break;
        }
        return 0;
    }
    return 1;
}

INT512 igcd(INT512 a, INT512 b) {
    while (!ieq(b, zero)) {
        INT512 r = imod(a, b);
        a = b;
        b = r;
    }
    return a;
}

typedef struct Agrs
{
    INT512 n;
    pthread_t * threadL;
} ARGS;


ARGS pList[NUM_GKTHREADS];
void * prime(void * arg)
{
    srand(time(NULL));
    ARGS* num = (ARGS*) arg;
    while(!millerRabin(num->n, 75))
        num->n = irand(MAXBYTE/4, MAXBYTE/2);
    for (int i = 0; i < NUM_GKTHREADS; ++i)
        pthread_cancel(num->threadL[i]);
}

void genkey(int mode , INT512* p, INT512* q, INT512* n, INT512* e, INT512* d, char* filename){
    switch(mode){
        INT512 phi;
        case 0:
            pthread_t threads[NUM_GKTHREADS];
            int cnt = 2;
            // while (cnt > 0){
                *p = irand(MAXBYTE/4, MAXBYTE/2);
                *q = irand(MAXBYTE/4, MAXBYTE/2);
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pList[i].threadL = threads;
                    pthread_create(&threads[i], NULL, prime, (void *) &pList[i]);
                }
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pthread_join(threads[i], NULL);
                    if (!ieq(pList[i].n, zero)) *p = pList[i].n;
                } 
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pthread_create(&threads[i], NULL, prime, (void *) &pList[i]);
                }
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pthread_join(threads[i], NULL);
                    if (!ieq(pList[i].n, zero)) *q = pList[i].n;
                } 
            // }

            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = irand(MAXBYTE/2, MAXBYTE);
            while (!ieq(igcd(*e, phi) , one))
            {
                *e = irand(MAXBYTE/2, MAXBYTE);
            }
            *d = imulInverse(phi, *e);
            break;
        case 1: // has d
            *p = irand(MAXBYTE/4, MAXBYTE/2);
            *q = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*p,100)) 
                *p = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*q,100)) 
                *q = irand(MAXBYTE/4, MAXBYTE/2);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = imulInverse(phi, *d);
            break;
        case 2:
            *p = irand(MAXBYTE/4, MAXBYTE/2);
            *q = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*p,100)) 
                *p = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*q,100)) 
                *q = irand(MAXBYTE/4, MAXBYTE/2);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *d = imulInverse(phi, *e);
            break;
        case 3:
            FILE* file = fopen(filename, "rb");
            if (file == NULL) {
                return;
            }
            fread(p->value, 1, MAXBYTE, file);
            fread(q->value, 1, MAXBYTE, file);
            fread(d->value, 1, MAXBYTE, file);
            fclose(file);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = imulInverse(phi, *d);
            break;
    }
}

void printInt256(INT512 num) 
{
    char buff[128];
    conv2dec(buff, &num);
    printf(buff);
    printf("\n");
}




int main() {
    initInt();
    long long t = time(NULL);
    srand(time(NULL));
    INT512 e, d, n, p, q;
    genkey(0, &p, &q, &n, &e, &d, "");
    printInt256(p);
    printInt256(n);
    printf("%lld", time(NULL) - t);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Header/int256.h"

INT512 irand(int minByte, int maxByte)
{
    INT512 result = zero;
    for (int i = 0; i < maxByte; i++) {
        result.value[i] = rand() % 256;
    }
    int check = 0;
    for (int i = minByte; i < maxByte; ++i) if (result.value[i] != 0) check = 1;
    if (!check) result = irand(minByte, maxByte);
    return result;
}

int millerRabin(INT512 n, int iterations) {
    if (ieq(n, one) || ieq(n, int256_c("2", HEXMODE))) {
        return 1;
    }
    if (ieq(imod(n, int256_c("2", HEXMODE)), zero)) {
        return 0;
    }

    INT512 s = zero;
    INT512 d = isub(n, one, NON);
    while (ieq(imod(d, int256_c("2", HEXMODE)), zero)) {
        d = idiv(d, int256_c("2", HEXMODE));
        s = ipls(s, one, NON);
    }

    for (int i = 0; i < iterations; i++) {
        int f = 1;
        INT512 a = irand(0, MAXBYTE/2);
        while (!(igt(a, one) && (ile(a, n)))) a = irand(0, MAXBYTE/2);
        INT512 x = ipow(a, d, n);
        if (ieq(x, one)) {
            break;
        }

        for (INT512 j = zero; ile(j, isub(s, one, NON)); j = ipls(j, one, NON)) {
            if (ieq(x, isub(n, one, NON))) {
                break;
                f = 0;
            } else {
                x = ipow(x, int256_c("2", HEXMODE), n);
            }
            if (f == 0) break;
        }
        return 0;
    }
    return 1;
}



INT512 igcd(INT512 a, INT512 b) {
    while (!ieq(b, zero)) {
        INT512 r = imod(a, b);
        a = b;
        b = r;
    }
    return a;
}

void genkey(int mode , INT512* p, INT512* q, INT512* n, INT512* e, INT512* d, char* filename){
    switch(mode){
        INT512 phi;
        case 0:
            *p = irand(MAXBYTE/4, MAXBYTE/2);
            *q = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*p,100)) 
                *p = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*q,100)) 
                *q = irand(MAXBYTE/4, MAXBYTE/2);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = irand(MAXBYTE/2, MAXBYTE);
            while (!ieq(igcd(*e, phi) , one))
            {
                *e = irand(MAXBYTE/2, MAXBYTE);
            }
            *d = imulInverse(phi, *e);
            break;
        case 1: // has d
            *p = irand(MAXBYTE/4, MAXBYTE/2);
            *q = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*p,100)) 
                *p = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*q,100)) 
                *q = irand(MAXBYTE/4, MAXBYTE/2);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = imulInverse(phi, *d);
            break;
        case 2:
            *p = irand(MAXBYTE/4, MAXBYTE/2);
            *q = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*p,100)) 
                *p = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*q,100)) 
                *q = irand(MAXBYTE/4, MAXBYTE/2);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *d = imulInverse(phi, *e);
            break;
        case 3:
            FILE* file = fopen(filename, "rb");
            if (file == NULL) {
                return;
            }
            fread(p->value, 1, MAXBYTE, file);
            fread(q->value, 1, MAXBYTE, file);
            fread(d->value, 1, MAXBYTE, file);
            fclose(file);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = imulInverse(phi, *d);
            break;
    }
}

void printInt256(INT512 num) 
{
    char buff[128];
    conv2dec(buff, &num);
    printf(buff);
    printf("\n");
}

int main() {
    initInt();
    srand(time(NULL));
    long long t = time(NULL);
    INT512 e, d, n, p, q;
    genkey(0, &p, &q, &n, &e, &d, "");
    printInt256(p);
    printInt256(q);
    printf("%lld", time(NULL) - t);
    return 0;
    //  2 << 32;
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "../Header/int256.h"
#define NUM_GKTHREADS 32

INT512 irand(int minByte, int maxByte)
{
    INT512 result = zero;
    for (int i = 0; i < maxByte; i++) {
        result.value[i] = rand() % 256;
    }
    int check = 0;
    for (int i = minByte; i < maxByte; ++i) if (result.value[i] != 0) check = 1;
    if (!check) result = irand(minByte, maxByte);
    return result;
}


int millerRabin(INT512 n, int iterations) {
    if (ieq(n, int256_c("2", HEXMODE))){
        return 1;
    }
    if ( ieq(n, zero) 
         || ieq(imod(n, int256_c("2", HEXMODE)), zero)
         || ieq(imod(n, int256_c("3", HEXMODE)), zero)
         || ieq(imod(n, int256_c("5", HEXMODE)), zero)
         || ieq(imod(n, int256_c("7", HEXMODE)), zero)
         || ieq(imod(n, int256_c("B", HEXMODE)), zero)
         || ieq(imod(n, int256_c("D", HEXMODE)), zero)){
        return 0;
    }

    INT512 s = zero;
    INT512 d = isub(n, one, NON);
    while (ieq(imod(d, int256_c("2", HEXMODE)), zero)) {
        d = idiv(d, int256_c("2", HEXMODE));
        s = ipls(s, one, NON);
    }

    for (int i = 0; i < iterations; i++) {
        int f = 1;
        INT512 a = irand(0, MAXBYTE/2);
        while (!(igt(a, one) && (ile(a, n)))) a = irand(0, MAXBYTE/2);
        INT512 x = ipow(a, d, n);
        if (ieq(x, one)) {
            break;
        }

        for (INT512 j = zero; ile(j, isub(s, one, NON)); j = ipls(j, one, NON)) {
            if (ieq(x, isub(n, one, NON))) {
                break;
                f = 0;
            } else {
                x = ipow(x, int256_c("2", HEXMODE), n);
            }
            if (f == 0) break;
        }
        return 0;
    }
    return 1;
}

INT512 igcd(INT512 a, INT512 b) {
    while (!ieq(b, zero)) {
        INT512 r = imod(a, b);
        a = b;
        b = r;
    }
    return a;
}

typedef struct Agrs
{
    INT512 n;
    pthread_t * threadL;
} ARGS;


ARGS pList[NUM_GKTHREADS];
void * prime(void * arg)
{
    srand(time(NULL));
    ARGS* num = (ARGS*) arg;
    while(!millerRabin(num->n, 75))
        num->n = irand(MAXBYTE/4, MAXBYTE/2);
    for (int i = 0; i < NUM_GKTHREADS; ++i)
        pthread_cancel(num->threadL[i]);
}

void genkey(int mode , INT512* p, INT512* q, INT512* n, INT512* e, INT512* d, char* filename){
    switch(mode){
        INT512 phi;
        case 0:
            pthread_t threads[NUM_GKTHREADS];
            int cnt = 2;
            // while (cnt > 0){
                *p = irand(MAXBYTE/4, MAXBYTE/2);
                *q = irand(MAXBYTE/4, MAXBYTE/2);
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pList[i].threadL = threads;
                    pList[i].n = irand(MAXBYTE/4, MAXBYTE/2);
                    pthread_create(&threads[i], NULL, prime, (void *) &pList[i]);
                }
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pthread_join(threads[i], NULL);
                    if (!ieq(pList[i].n, zero)) *p = pList[i].n;
                } 
                
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pList[i].threadL = threads;
                    pList[i].n = irand(MAXBYTE/4, MAXBYTE/2);
                    pthread_create(&threads[i], NULL, prime, (void *) &pList[i]);
                }
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pthread_join(threads[i], NULL);
                    if (!ieq(pList[i].n, zero)) *q = pList[i].n;
                } 
            // }

            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = irand(MAXBYTE/2, MAXBYTE);
            while (!ieq(igcd(*e, phi) , one))
            {
                *e = irand(MAXBYTE/2, MAXBYTE);
            }
            *d = imulInverse(phi, *e);
            break;
        case 1: // has d
            *p = irand(MAXBYTE/4, MAXBYTE/2);
            *q = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*p,100)) 
                *p = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*q,100)) 
                *q = irand(MAXBYTE/4, MAXBYTE/2);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = imulInverse(phi, *d);
            break;
        case 2:
            *p = irand(MAXBYTE/4, MAXBYTE/2);
            *q = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*p,100)) 
                *p = irand(MAXBYTE/4, MAXBYTE/2);
            while (!millerRabin(*q,100)) 
                *q = irand(MAXBYTE/4, MAXBYTE/2);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *d = imulInverse(phi, *e);
            break;
        case 3:
            FILE* file = fopen(filename, "rb");
            if (file == NULL) {
                return;
            }
            fread(p->value, 1, MAXBYTE, file);
            fread(q->value, 1, MAXBYTE, file);
            fread(d->value, 1, MAXBYTE, file);
            fclose(file);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = imulInverse(phi, *d);
            break;
    }
}

void printInt256(INT512 num) 
{
    char buff[128];
    conv2dec(buff, &num);
    printf(buff);
    printf("\n");
}




int main() {
    initInt();
    long long t = time(NULL);
    srand(time(NULL));
    INT512 e, d, n, p, q;
    genkey(0, &p, &q, &n, &e, &d, "");
    printInt256(p);
    printInt256(n);
    printf("%lld", time(NULL) - t);
    return 0;
}
