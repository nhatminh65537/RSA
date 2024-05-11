#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../header/int256.h"

void decrypt( INT256 d ,INT256 p , INT256 q, char* cpt, char* plt) 
{
  INT256 n = imul(p,q,NON);
  FILE* ciphertextFile = fopen(cpt, "r"); 
  if (ciphertextFile == NULL) {
    return;
  }
  
  FILE* plaintextFile2 = fopen(plt, "w"); 
  if (plaintextFile2 == NULL) {
    return;
  }
  INT256 d_p = imod(d, isub(p, one,NON));
  INT256 d_q = imod(d, isub(q, one,NON));

  INT256 M_q = imulInverse(q, p);
  INT256 M_p = imulInverse(p, q);
  
  char ascii[MAXBYTE + 1], buff2[MAXBYTE + 1];
  size_t bytesRead;
  while((bytesRead = fread(buff2, 1, MAXHEX, ciphertextFile)) > 0){
    buff2[bytesRead] = '\0';
    INT256 y;
    INT256 x_p = ipow(y , d_p , p);
    INT256 x_q = ipow(y , d_q , q);

    INT256 x = ipls( imul(imul(M_p , q , n) , x_p , n), imul(imul(M_q , p , n) , x_q , n), imul(p,q,NON));
    conv2char(ascii, &x);

    fwrite( ascii, 1, MAXBYTE , plaintextFile2);
  }

  fclose(ciphertextFile);
  fclose(plaintextFile2);
}
int main(){
    INT256 n, e;
    initInt();
    e = int256_c("BB39", HEXMODE);
    n = int256_c("EC4B" , HEXMODE);
    INT256 p,q,d;
    p = int256_c("F1", HEXMODE);
    q = int256_c("FB", HEXMODE);
    d = int256_c("a9", HEXMODE);
    decrypt( d , p , q , "ciphertext.txt", "plaintext2.txt");
}