#include <stdio.h>
#include <Windows.h>
#define MOVEXY(x, y) printf("\x1b[%d;%dH", x, y)
#define MAXX 150
#define MAXY 30

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    printf("\u2570");    
    while (1)
        ;
    return 0;
}

