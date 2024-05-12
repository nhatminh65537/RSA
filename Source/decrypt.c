#include <stdio.h>
#include "../header/int256.h"

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
  
  char ascii[MAXBYTE + 1], buff[MAXHEX + 1];
  int bytesRead;
  while((bytesRead = fread(buff, 1, MAXHEX, ciphertextFile)) > 0){
    buff[bytesRead] = '\0';
    INT256 y = int256_c(buff, HEXMODE);
    INT256 x_p = ipow(y , d_p , p);
    INT256 x_q = ipow(y , d_q , q);

    INT256 x  = ipls(imul(imul(M_p , q , n) , x_p , n), imul(imul(M_q , p , n) , x_q , n), n);
    conv2char(ascii, &x);

    fprintf(plaintextFile, ascii);
  }

  fclose(ciphertextFile);
  fclose(plaintextFile);
}
// int main(){
//     INT256 n, e;
//     initInt();
//     e = int256_c("BB39", HEXMODE);
//     n = int256_c("EC4B" , HEXMODE);
//     INT256 p,q,d;
//     p = int256_c("F1", HEXMODE);
//     q = int256_c("FB", HEXMODE);
//     d = int256_c("a9", HEXMODE);
//     decrypt( d , p , q , "ciphertext.txt", "plaintext2.txt");
// }