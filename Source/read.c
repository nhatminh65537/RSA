#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../header/int256.h"
// đọc file nhị phân rồi trả về n,e ?? Ok, good job.
void readfilebin(char* filename, INT256* n, INT256* e)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        return;
    }
    fread(n->value, 1, MAXBYTE, file);
    fread(e->value, 1, MAXBYTE, file);
    fclose(file);
}