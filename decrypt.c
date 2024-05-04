#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../header/int256.h"

void decrypt() {
 
  FILE* ciphertext_file = fopen("ciphertext.bin", "rb"); 
  if (ciphertext_file == NULL) {
    printf("Error opening ciphertext file.\n");
    fclose(key_file);
    return;
  }

  
  unsigned char d_str[MAXBYTE + 1], n_str[MAXBYTE + 1];
  fscanf(key_file, "%s %s", d_str, n_str);
  fclose(key_file);

  INT256 d = int256_c(d_str, HEXMODE);
  INT256 n = int256_c(n_str, HEXMODE);

  
  fseek(ciphertext_file, 0, SEEK_END);
  int ciphertext_size = ftell(ciphertext_file);
  fseek(ciphertext_file, 0, SEEK_SET); 

  
  int num_blocks;
  fread(&num_blocks, sizeof(int), 1, ciphertext_file);

 
  INT256* ciphertext_blocks = malloc(num_blocks * sizeof(INT256));
  if (ciphertext_blocks == NULL) {
    printf("Error allocating memory for ciphertext blocks.\n");
    fclose(ciphertext_file);
    return;
  }

  
  for (int i = 0; i < num_blocks; i++) {
    fread(ciphertext_blocks[i].value, sizeof(char), MAXBYTE, ciphertext_file);
  }

  
  INT256* plaintext_blocks = malloc(num_blocks * sizeof(INT256));
  if (plaintext_blocks == NULL) {
    printf("Error allocating memory for plaintext blocks.\n");
    free(ciphertext_blocks);
    fclose(ciphertext_file);
    return;
  }

  for (int i = 0; i < num_blocks; i++) {
    plaintext_blocks[i] = pow(ciphertext_blocks[i], d, n);
  }

  
  char output_filename[] = "plaintext.txt"; 
  FILE* plaintext_file = fopen(output_filename, "wb"); 

  if (plaintext_file == NULL) {
    printf("Error opening output file for plaintext.\n");
  } else {
    for (int i = 0; i < num_blocks; i++) {
      int block_size = (i < num_blocks - 1) ? MAXBYTE : (ciphertext_size % MAXBYTE);
      fwrite(plaintext_blocks[i].value, sizeof(char), block_size, plaintext_file);
    }
    printf("Plaintext written to %s\n", output_filename);
    fclose(plaintext_file);
  }

 
  free(ciphertext_blocks);
  free(plaintext_blocks);
  fclose(ciphertext_file);
}
