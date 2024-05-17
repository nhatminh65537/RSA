#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "int256.h"
#include <pthread.h>
#define NUM_GKTHREADS 32

int millerRabin(INT256 n, int iterations) {
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

    INT256 s = zero;
    INT256 d = isub(n, one, NON);
    while (ieq(imod(d, int256_c("2", HEXMODE)), zero)) {
        d = idiv(d, int256_c("2", HEXMODE));
        s = ipls(s, one, NON);
    }

    for (int i = 0; i < iterations; i++) {
        int f = 1;
        INT256 a = irand(0, MAXWORD/2);
        while (!(igt(a, one) && (ile(a, n)))) a = irand(0, MAXWORD/2);
        INT256 x = ipow(a, d, n);
        if (ieq(x, one)) {
            break;
        }

        for (INT256 j = zero; ile(j, isub(s, one, NON)); j = ipls(j, one, NON)) {
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

INT256 pList[NUM_GKTHREADS];
void * prime(void * arg)
{
    INT256* num = (INT256*) arg;
    if(!millerRabin(*num, 75)) *num = zero;
}

void genkey(int mode , INT256* p, INT256* q, INT256* n, INT256* e, INT256* d, char* filename)
{
    INT256 phi;
    pthread_t threads[NUM_GKTHREADS];
    int cnt = 2;
    switch(mode){
        case 0:
            while (cnt > 0){
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pList[i] = irand(MAXWORD/4, MAXWORD/2);
                    pthread_create(&threads[i], NULL, prime, (void *) &pList[i]);
                }
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pthread_join(threads[i], NULL);
                    if (!ieq(pList[i], zero) && cnt == 2){
                        *p = pList[i];
                        --cnt;
                        continue;
                    }
                    if (!ieq(pList[i], zero) && cnt == 1){
                        *q = pList[i];
                        --cnt;
                        continue;
                    } 
                }
            }

            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = irand(MAXWORD/4, MAXWORD/4*3);
            while (!ieq(igcd(*e, phi) , one) || !ile(*e, phi)){
                *e = irand(MAXWORD/4, MAXWORD/4*3);
            }
            *d = imulInverse(phi, *e);
            break;
        case 1: 
            while (cnt > 0){
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pList[i] = irand(MAXWORD/4, MAXWORD/2);
                    pthread_create(&threads[i], NULL, prime, (void *) &pList[i]);
                }
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pthread_join(threads[i], NULL);
                    if (!ieq(pList[i], zero) && cnt == 2){
                        *p = pList[i];
                        --cnt;
                        continue;
                    }
                    if (!ieq(pList[i], zero) && cnt == 1){
                        *q = pList[i];
                        phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
                        if (ieq(igcd(*d, phi) , one) && ile(*d, phi)) --cnt;
                        else ++cnt;
                        continue;
                    } 
                }       
            }
            *n = imul(*p, *q, NON);
            *e = imulInverse(phi, *d);
            break;
        case 2:
            while (cnt > 0){
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pList[i] = irand(MAXWORD/4, MAXWORD/2);
                    pthread_create(&threads[i], NULL, prime, (void *) &pList[i]);
                }
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pthread_join(threads[i], NULL);
                    if (!ieq(pList[i], zero) && cnt == 2){
                        *p = pList[i];
                        --cnt;
                        continue;
                    }
                    if (!ieq(pList[i], zero) && cnt == 1){
                        *q = pList[i];
                        --cnt;
                        phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
                        if (ieq(igcd(*e, phi) , one) && ile(*e, phi)) --cnt;
                        else ++cnt;
                        continue;
                    } 
                }
            }            
            *n = imul(*p, *q, NON);
            *d = imulInverse(phi, *e);
            break;
        case 3:
            FILE* file = fopen(filename, "rb");
            if (file == NULL) {
                return;
            }
            fread(p->value, 1, MAXWORD, file);
            fread(q->value, 1, MAXWORD, file);
            fread(d->value, 1, MAXWORD, file);
            fclose(file);
            *n = imul(*p, *q, NON);
            phi = imul(isub(*p, one, NON), isub(*q, one, NON), NON);
            *e = imulInverse(phi, *d);
            break;
    }
}

void printInt256(INT256 num) 
{
    char buff[128];
    conv2dec(buff, &num);
    printf(buff);
    printf("\n");
}

int main() {
    initInt();
    // printInt256(ipow(int256_c("125678", HEXMODE), int256_c("123456", HEXMODE), int256_c("12ff56", HEXMODE)));
    int cnt = 10, st = 0;
    while (cnt-- > 0){
        long long t = time(NULL);
        srand(time(NULL));
        INT256 e, d, n, p, q;
        genkey(0, &p, &q, &n, &e, &d, "");
        printInt256(p);
        printInt256(n);
        printf("%lld\n", time(NULL) - t);
        st += time(NULL) - t;
    }
    printf("\n%d\n", st);
    return 0;
}