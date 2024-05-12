#include <stdio.h>
#include "../header/int256.h"

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

  INT256 x, y;
  char hex[MAXHEX + 1], buff[MAXBYTE + 1];
  while (!feof(plaintextFile)){
    buff[fread(buff, 1, MAXBYTE, plaintextFile)] = '\0';
    x = int256_c(buff, ASCIIMODE);
    y = ipow(x, e, n);
    conv2hex(hex, &y);
    fprintf(plaintextFile, hex);
  }

  fclose(plaintextFile);
  fclose(ciphertextFile);
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
//     encrypt(e, n, "plaintext.txt", "ciphertext.txt");
// }