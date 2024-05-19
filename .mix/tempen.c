
#include <stdio.h>

#include "../header/int256.h"

void encrypt(INT512 e, INT512 n, char *plt, char *cpt)
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
  while (!feof(plaintextFile)){
      INT512 x, y;
      buff[fread(buff, 1, MAXBYTE, plaintextFile)] = '\0';
      x = int256_c(buff, ASCIIMODE);
      y = ipow(x, e, n);
      conv2hex(hex, &y);
      fprintf(ciphertextFile, hex); 
    }
    // 
    

  fclose(plaintextFile);
  fclose(ciphertextFile);
}