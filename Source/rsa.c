#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../header/int256.h"
#define NUM_THREADS 16

struct argsThread
{
    INT256 *x;
    INT256 *y;
    INT256 *e;
    INT256 *n;
};

struct argsThread enATL[NUM_THREADS], deATL[NUM_THREADS];

void *powThread(void *argv)
{
    struct argsThread *args = (struct argsThread *) argv;
    *args->y = ipow(*args->x, *args->e,*args->n);
} 

void encrypt(INT256 e, INT256 n, char *plt, char *cpt)
{
    FILE *plaintextFile = fopen(plt, "r");
    if (plaintextFile == NULL){
        return;
    }

    FILE *ciphertextFile = fopen(cpt, "w");
        if (ciphertextFile == NULL){
            return;
    }

    char hex[MAXHEX + 1], buff[MAXBYTE + 1];
    pthread_t threads[NUM_THREADS];
    while (!feof(plaintextFile)){
        int threadCnt = 0;

        for (int i = 0; (i < NUM_THREADS) && !feof(plaintextFile); ++i){
            INT256 *x, *y;
            x = malloc(sizeof(INT256));
            y = malloc(sizeof(INT256));
            buff[fread(buff, 1, MAXBYTE/2, plaintextFile)] = '\0';
            *x = int256_c(buff, ASCIIMODE);
            struct argsThread *args = enATL + threadCnt;
            args->x = x;
            args->y = y;
            args->e = &e;
            args->n = &n;
            ++threadCnt;
            pthread_create(&threads[i], NULL, powThread, (void *) args);
        }

        for (int i = 0; i < threadCnt; ++i){
            pthread_join(threads[i], NULL);
            
            conv2hex(hex, enATL[i].y);
            fprintf(ciphertextFile, hex);
            free(enATL[i].y);
            free(enATL[i].x);
        }
    }

    fclose(plaintextFile);
    fclose(ciphertextFile);
}

void decrypt(INT256 d ,INT256 p , INT256 q, char* cpt, char* plt) 
{
  INT256 n = imul(p,q,NON);
  
  FILE* ciphertextFile = fopen(cpt, "r"); 
  if (ciphertextFile == NULL) {
    return;
  }
  
  FILE* plaintextFile = fopen(plt, "w"); 
  if (plaintextFile == NULL) {
    return;
  }
  
  INT256 d_p = imod(d, isub(p, one,NON));
  INT256 d_q = imod(d, isub(q, one,NON));

  INT256 M_q = imulInverse(q, p);
  INT256 M_p = imulInverse(p, q);
  
  char ascii[MAXBYTE + 1], buff[MAXBYTE + 1];
  pthread_t threads_p[NUM_THREADS/2], threads_q[NUM_THREADS/2];
  while(!feof(ciphertextFile)){
    int threadCnt = 0;

    for (int i = 0; (i < NUM_THREADS/2) && !feof(ciphertextFile); ++i){
      INT256 *y, *x_q, *x_p;
      y   = malloc(sizeof(INT256));
      x_p = malloc(sizeof(INT256));
      x_q = malloc(sizeof(INT256));
      buff[fread(buff, 1, MAXHEX, ciphertextFile)] = '\0';
      *y = int256_c(buff, HEXMODE);
      struct argsThread *args_p = deATL + threadCnt++, 
                        *args_q = deATL + threadCnt++;
      args_p->x = y;
      args_p->y = x_p;
      args_p->e = &d_p;
      args_p->n = &p;

      args_q->x = y;
      args_q->y = x_q;
      args_q->e = &d_q;
      args_q->n = &q;

      pthread_create(&threads_p[i], NULL, powThread, (void *) args_p);
      pthread_create(&threads_q[i], NULL, powThread, (void *) args_q);
    }

    int argsInd = 0;
    for (int i = 0; i < threadCnt/2; ++i){
      pthread_join(threads_p[i], NULL);
      pthread_join(threads_q[i], NULL);

      INT256 *y   = (deATL + argsInd  )->x;
      INT256 *x_p = (deATL + argsInd++)->y;
      INT256 *x_q = (deATL + argsInd++)->y;
          
      INT256 x  = ipls(imul(imul(M_p , q , n) , *x_p , n),
                       imul(imul(M_q , p , n) , *x_q , n), n);
      
      conv2char(ascii, &x);
      fprintf(plaintextFile, ascii);

      free(y  );
      free(x_p);
      free(x_q);
    }    
  }

  fclose(ciphertextFile);
  fclose(plaintextFile);
}

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
    INT256 d = isub(n, one, NON), nso = isub(n, one, NON);
    int k = 0;
    for (int i = 0; i < MAXBIT; ++i){
        if (n.value[i/8] >> (i%8) & 1 == 0){
            d = idiv(d, int256_c("2", HEXMODE));
            ++k;
        } else break;
    }

    for (int i = 0; i < iterations; i++) {
        int p = 0;
        INT256 a = irand(0, MAXBYTE/2);
        while (!(igt(a, one) && (ile(a, n)))) a = irand(0, MAXBYTE/2);
        INT256 x = ipow(a, d, n);
        if (ieq(x, one)) {
            continue;
        }

        for (int j = 0; j < k; ++j) {
            if (ieq(x, nso)) {
                p = 1;
                break;
            } else {
                x = imul(x, x, n);
            }
        }
        if (p) continue;
        return 0;
    }
    return 1;
}

INT256 pList[NUM_GKTHREADS];
void * prime(void * arg)
{
    INT256* num = (INT256*) arg;
    if(!millerRabin(*num, 50)) *num = zero;
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
                    pList[i] = irand(MAXBYTE/4, MAXBYTE/2);
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
            *e = irand(MAXBYTE/4, MAXBYTE/4*3);
            while (!ieq(igcd(*e, phi) , one) || !ile(*e, phi)){
                *e = irand(MAXBYTE/4, MAXBYTE/4*3);
            }
            *d = imulInverse(phi, *e);
            break;
        case 1: 
            while (cnt > 0){
                for (int i = 0; i < NUM_GKTHREADS; ++i){
                    pList[i] = irand(MAXBYTE/4, MAXBYTE/2);
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
                    pList[i] = irand(MAXBYTE/4, MAXBYTE/2);
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