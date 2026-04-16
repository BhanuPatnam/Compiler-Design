#include <stdio.h>

int find_min(int a, int b) {
    if (a == b) {
        return a;
    } else {
        return b;
    }
}

int main() {
    int x = 10;
    int y = 20;
    int min_val = find_min(x, y);
    printf("%d\n", min_val);
    return 0;
}
