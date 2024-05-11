#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../header/int256.h"

void decrypt(INT256 n , INT256 d ,INT256 p , INT256 q, char* cpt, char* plt2) 
{
  FILE* ciphertextFile = fopen(cpt, "r"); 
  if (ciphertextFile == NULL) {
    return;
  }
  
  FILE* plaintextFile2 = fopen(cpt, "w"); 
  if (plaintextFile2 == NULL) {
    return;
  }
  INT256 d_p = imod(d, isub(p, one,maxval));
  INT256 d_q = imod(d, isub(q, one,maxval));

  INT256 M_p = imulInverse(q, p);
  INT256 M_q = imulInverse(p, q);
  
  char ascii[MAXBYTE + 1], buff2[MAXBYTE + 1];
  size_t bytesRead;
  while((bytesRead = fread(buff2, 1, MAXBYTE, ciphertextFile)) > 0){
    buff2[bytesRead] = '\0';
  INT256 y;
   INT256 x_p = ipow(y , d_p , p);
   INT256 x_q = ipow(y , d_q , q);

    INT256 x = ipls(imul(imul(M_p , q , maxval) , x_p , maxval) , imul(imul(M_q , p , maxval) , x_q , maxval) , n);
    conv2char(ascii, &x);

   fwrite( ascii, 1, MAXBYTE , plaintextFile2);
  }

  fclose(ciphertextFile);
  fclose(plaintextFile2);
}
int main(){
    INT256 n, e;
    int256Init();
    e = int256_c("BB39", HEXMODE);
    n = int256_c("EC4B" , HEXMODE);
    INT256 p,q,d;
    p = int256_c("F1", HEXMODE);
    q = int256_c("FB", HEXMODE);
    d = int256_c("39", HEXMODE);
    decrypt(n , d , p , q , "ciphertext.txt", "plaintext2.txt");
}