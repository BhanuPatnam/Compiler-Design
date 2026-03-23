#include <stdio.h>

int test_types(int a, int b, int c) {
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
    return a;
}

int main() {
    int x = 10;
    float y = 3.14;
    char z = 'A';
    printf("%d\n", x);
    printf("%d\n", y);
    printf("%d\n", z);
    int res = test_types(x, y, z);
    printf("%d\n", res);
    return 0;
}
