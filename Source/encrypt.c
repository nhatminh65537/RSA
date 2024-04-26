#include <stdio.h>
#include<stdlib.h>
#include "../header/int256.h"

void encrypt() {
  
  FILE* key_file = fopen("key.txt", "r");
  if (key_file == NULL) {
    printf("Error opening key file.\n");
    return;
  }

  FILE* plaintext_file = fopen("plaintext.txt", "rb"); 
  if (plaintext_file == NULL) {
    printf("Error opening plaintext file.\n");
    fclose(key_file);
    return;
  }

 
  unsigned char e_str[MAXBYTE + 1], n_str[MAXBYTE + 1];
  fscanf(key_file, "%s %s", e_str, n_str);
  fclose(key_file);

  INT256 e = int256_c(e_str, HEXMODE);
  INT256 n = int256_c(n_str, HEXMODE);

 
  fseek(plaintext_file, 0, SEEK_END);
  int plaintext_size = ftell(plaintext_file);
  fseek(plaintext_file, 0, SEEK_SET); 

 
  unsigned char* plaintext = malloc(plaintext_size * sizeof(char));
  if (plaintext == NULL) {
    printf("Memory allocation failed for plaintext.\n");
    fclose(plaintext_file);
    return;
  }

 
  int num_bytes_read = fread(plaintext, sizeof(char), plaintext_size, plaintext_file);
  if (num_bytes_read != plaintext_size) {
    printf("Error reading plaintext file.\n");
    free(plaintext);
    fclose(plaintext_file);
    return;
  }

  
  int num_blocks = (plaintext_size + MAXBYTE - 1) / MAXBYTE; 
  INT256* plaintext_blocks = malloc(num_blocks * sizeof(INT256));
  if (plaintext_blocks == NULL) {
    printf("Memory allocation failed for plaintext blocks.\n");
    free(plaintext);
    fclose(plaintext_file);
    return;
  }

  for (int i = 0; i < num_blocks; i++) {
    int block_size = (i < num_blocks - 1) ? MAXBYTE : (plaintext_size % MAXBYTE);
    plaintext_blocks[i] = int256_c(&plaintext[i * MAXBYTE], ASCIIMODE);
  }

 
  INT256* ciphertext_blocks = malloc(num_blocks * sizeof(INT256));
  if (ciphertext_blocks == NULL) {
    printf("Memory allocation failed for ciphertext blocks.\n");
    free(plaintext);
    free(plaintext_blocks);
    fclose(plaintext_file);
    return;
  }

  for (int i = 0; i < num_blocks; i++) {
    ciphertext_blocks[i] = pow(plaintext_blocks[i], e, n);
  }

 char output_filename[] = "ciphertext.bin"; 
  FILE* ciphertext_file = fopen(output_filename, "wb"); 

  if (ciphertext_file == NULL) {
    printf("Error opening output file for ciphertext.\n");
  } else {
    
    int num_blocks = (plaintext_size + MAXBYTE - 1) / MAXBYTE;
    fwrite(&num_blocks, sizeof(int), 1, ciphertext_file);

   
    for (int i = 0; i < num_blocks; i++) {
      fwrite(ciphertext_blocks[i].value, sizeof(char), MAXBYTE, ciphertext_file);
    }
    printf("Ciphertext written to %s\n", output_filename);
    fclose(ciphertext_file);
  }


  for (int i = 0; i < num_blocks; i++) {
    show(ciphertext_blocks[i], HEXMODE);
    printf(" ");
  }
  printf("\n");

  free(plaintext);
  free(plaintext_blocks);
  free(ciphertext_blocks);
  fclose(plaintext_file);
}
int main(){
    INT256 n, e;
    e = int256_c("BB39", HEXMODE);
    n = int256_c("EC4B" , HEXMODE);
    INT256 p,q,d;
    p = int256_c("F1", HEXMODE);
    q = int256_c("FB", HEXMODE);
    d = int256_c("39", HEXMODE);
    char hn[MAXHEX+1];
    conv2hex(hn,&e);
    printf(hn);
}