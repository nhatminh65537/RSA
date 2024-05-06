#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../header/int256.h"

void decrypt(INT256 n , INT256 a , char* cpt, char* plt2) 
{
  FILE* cyphertext = fopen(cpt, "r"); 
  if (ciphertextFile == NULL) {
    return;
  }
  FILE* plaintextFile2 = fopen(cpt, "w"); 
  if (plaintextFile2 == NULL) {
    return;
  }

  INT256 y,d,p,q;
char hex[MAXHEX + 1], buff[MAXBYTE + 1];
while(feof(ciphertextFile)){
  INT256 d_p = imod(d, isub(p, one));
  INT256 d_q = imod(d, isub(q, one));
    
  INT256 M_p = imulInverse(q, p);
  INT256 M_q = imulInverse(p, q);

  INT256 x_p = ipow(y , d_p , p);
  INT256 x_q = ipow(y , d_q , q);
  INT256 x = 
}

}