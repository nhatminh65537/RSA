#include <stdio.h>

void nhap(char* );

int main()
{
    nhap("1234567890");
    return 0;
}

struct demo
{
    int x[2];
};

void strcpy(char *x, char *y)
{
    while (*y != 0){
        *x = *y;
        ++x;
        ++y;
    }
    *x = 0;
}

void assign(int *c, int* r)
{
    c[0] = r[0];
    c[1] = r[1];
}
void nhap(char* t)
{
    // char a[12] = "qwerty";
    // strcpy(a, t);
    // printf("%s", a);
    // char x[12] = "123";
    // struct demo a, b;
    // a.y = 2;
    // strcpy(a.x, "asdfghj");
    // b = a;
    // // strcpy(a.x, b.x);
    // printf("%s %d\n", b.x, b.y);
    // strcpy(b.x, "zxcvbnm");
    // printf("%s", b.x);
    int c[2] = {1, 2}, d[2] = {2 ,3};
    struct demo a, b;
    assign(a.x, c);
    b = a;
    printf("%d %d\n", a.x[0], a.x[1]);
    assign(a.x, d);
    printf("%d %d", b.x[0], b.x[1]);
}

