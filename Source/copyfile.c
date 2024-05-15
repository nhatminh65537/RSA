#include <stdio.h>

void copyFile(char *sourceFile, char *destinationFile) {
    FILE *source, *destination;
    char ch;

    source = fopen(sourceFile, "r");

    if (source == NULL) {
        printf("Unable to open source file to read.\n");
        return;
    }

    destination = fopen(destinationFile, "w");

    if (destination == NULL) {
        printf("Unable to open destination file to write.\n");
        fclose(source);
        return;
    }

    // Sao chép nội dung từ tệp tin nguồn sang tệp tin đích
    while ((ch = fgetc(source)) != EOF) {
        fputc(ch, destination);
    }

    printf("File copied successfully.\n");

    fclose(source);
    fclose(destination);
}
int main() {
    char sourceFile[] = "source.txt";
    char destinationFile[] = "destination.txt";

    copyFile(sourceFile, destinationFile);

    return 0;
}