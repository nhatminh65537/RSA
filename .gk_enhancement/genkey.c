#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "int256.h"
#include <pthread.h>
#define NUM_GKTHREADS 32

INT512 cnt;

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
    INT512 d = isub(n, one, NON), nso = isub(n, one, NON);
    int k = 0;
    for (int i = 0; i < MAXBIT; ++i){
        if (n.value[i/WORD] >> (i%WORD) & 1 == 0){
            d = idiv(d, int256_c("2", HEXMODE));
            ++k;
        } else break;
    }

    for (int i = 0; i < iterations; i++) {
        int f = 1;
        INT512 a = irand(0, MAXWORD/2);
        while (!(igt(a, one) && (ile(a, n)))) a = irand(0, MAXWORD/2);
        INT512 x = ipow(a, d, n);
        if (ieq(x, one)) {
            continue;
        }

        for (int j = 0; j < k; ++j) {
            if (ieq(x, nso)) {
                f = 0;
                break;
            } else {
                x = imul(x, x, n);
            }
        }
        if (f == 0) continue;
        return 0;
    }
    return 1;
}

INT512 pList[NUM_GKTHREADS];
void * prime(void * arg)
{
    cnt = ipls(cnt, one, NON);
    INT512* num = (INT512*) arg;
    if(!millerRabin(*num, 50)) *num = zero;
}

void genkey(int mode , INT512* p, INT512* q, INT512* n, INT512* e, INT512* d, char* filename)
{
    INT512 phi;
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

void printInt256(INT512 num) 
{
    char buff[512];
    conv2dec(buff, &num);
    printf(buff);
    printf("\n");
}

int main() {
    initInt();
    printInt256(imul(int256_c("FF2B2ACD", HEXMODE), int256_c("F1E31231", HEXMODE), int256_c("a231", HEXMODE)));
    long long t = time(NULL);
    srand(time(NULL));
    INT512 e, d, n, p, q;
    genkey(0, &p, &q, &n, &e, &d, "");
    printInt256(p);
    printInt256(q);
    printInt256(n);
    printf("%lld\n", time(NULL) - t);
    printInt256(cnt);
    return 0;
}