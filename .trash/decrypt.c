#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../header/int256.h"

void decrypt(INT512 n , INT512 d, Int512 p , INT512 q , char* cpt, char* plt2) 
{
  FILE* cyphertextFile = fopen(cpt, "r"); 
  if (ciphertextFile == NULL) {
    return;
  }
  
  FILE* plaintextFile2 = fopen(cpt, "w"); 
  if (plaintextFile2 == NULL) {
    return;
  }
  
  INT512 d_p = imod(d, isub(p, one));
  INT512 d_q = imod(d, isub(q, one));
    
  INT512 M_p = imulInverse(q, p);
  INT512 M_q = imulInverse(p, q);

  INT512 y
buff2[MAXBYTE + 1];
while(!feof(ciphertextFile)){
    fread(buff, 1, MAXHEX, ciphertextFile);
    buff[MAXHEX] = '\0';
    x = int256_c(buff, HEXMODE);
    INT512 m_p = ipow(x, d_p, p);
    INT512 m_q = ipow(x, d_q, q);
    INT512 h = isub(m_p, m_q);
    if (icmp(h, zero) < 0) {
      h = iadd(h, p);
    }
    INT512 m = iadd(m_q, imul(h, M_q));
    conv2ascii(buff2, &m);
    fwrite(buff2, 1, MAXBYTE, plaintextFile2);
  }

  fclose(ciphertextFile);
  fclose(plaintextFile2);
}
int main(){
    INT512 n, e;
    initInt();
    e = int256_c("BB39", HEXMODE);
    n = int256_c("EC4B" , HEXMODE);
    INT512 p,q,d;
    p = int256_c("F1", HEXMODE);
    q = int256_c("FB", HEXMODE);
    d = int256_c("39", HEXMODE);
    decrypt(n, a, "ciphertext.txt", "plaintext2.txt");
}