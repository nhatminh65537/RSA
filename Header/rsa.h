#ifndef __RSA__
#define __RSA__

#include "int512.h"

void decrypt(INT512 d ,INT512 p , INT512 q, char* cpt, char* plt);
void encrypt(INT512 e, INT512 n, char *plt, char *cpt);
void genkey(int mode , INT512* p, INT512* q, INT512* n, INT512* e, INT512* d, char* filename);

#endif