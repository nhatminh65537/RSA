#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../header/int256.h"
#define NUM_THREADS 5

struct argsThread
{
  INT256 *x;
  INT256 *y;
  INT256 *e;
  INT256 *n;
};

struct argsThread argsThreadList[NUM_THREADS];

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
      buff[fread(buff, 1, MAXBYTE, plaintextFile)] = '\0';
      *x = int256_c(buff, ASCIIMODE);
      struct argsThread *args = argsThreadList + threadCnt;
      args->x = x;
      args->y = y;
      args->e = &e;
      args->n = &n;
      ++threadCnt;
      pthread_create(&threads[i], NULL, powThread, (void *) args);
    }
    for (int i = 0; i < threadCnt; ++i){
      pthread_join(threads[i], NULL);
      
      conv2hex(hex, argsThreadList[i].y);
      fprintf(ciphertextFile, hex);
      free(argsThreadList[i].y);
      free(argsThreadList[i].x);
    }
  }

  fclose(plaintextFile);
  fclose(ciphertextFile);
}

