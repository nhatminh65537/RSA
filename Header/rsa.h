#ifndef __RSA__
#define __RSA__

#include "int256.h"

void decrypt(INT256 d ,INT256 p , INT256 q, char* cpt, char* plt);
void encrypt(INT256 e, INT256 n, char *plt, char *cpt);
void genkey(int mode , INT256* p, INT256* q, INT256* n, INT256* e, INT256* d, char* filename);

#endif