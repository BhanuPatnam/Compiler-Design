#include <stdio.h>

int main() {
    int x;
    int* p;
    int** pp;
    x = 42;
    p = (&x);
    pp = (&p);
    printf("%d\n", x);
    printf("%d\n", (*p));
    printf("%d\n", (**pp));
    (**pp) = 100;
    printf("%d\n", x);
    printf("%d\n", (*p));
    return 0;
}
