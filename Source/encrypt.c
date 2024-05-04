#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../header/int256.h"
// #include "../header/outlog.h"

<<<<<<< HEAD
void encrypt(INT256 e, INT256 n, char* plt, char* cpt) {
=======
void encrypt(char* pltname) {
>>>>>>> 033ebbaf0692c5824cb7ae05ecf4199e753ef77b
  
  FILE* plaintextFile = fopen(plt, "r"); 
  if (plaintextFile == NULL) {
    // addError("Error opening plaintext file.\n");
    return;
  }

<<<<<<< HEAD
  FILE* ciphertextFile = fopen(cpt, "w"); 
  if (ciphertextFile == NULL) {
    // addError("Error opening ciphertext file.\n");
=======
  FILE* plaintext_file = fopen(pltname, "rb"); 
  if (plaintext_file == NULL) {
    printf("Error opening plaintext file.\n");
    fclose(key_file);
>>>>>>> 033ebbaf0692c5824cb7ae05ecf4199e753ef77b
    return;
  }
 
  INT256 x, y;
  char hex[MAXHEX + 1], buff[MAXBYTE + 1];;
  while(!feof(plaintextFile)){
    fread(buff, 1, MAXBYTE, plaintextFile);
    buff[MAXBYTE] = '\0';
    x = int256_c(buff, ASCIIMODE);
    y = ipow(x, e, n);
    conv2hex(hex, &y);
    fwrite(hex, 1, MAXHEX, ciphertextFile);
  }
  
  fclose(plaintextFile);
  fclose(ciphertextFile);
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
    encrypt(e, n, "plaintext.txt", "ciphertext.txt");
}