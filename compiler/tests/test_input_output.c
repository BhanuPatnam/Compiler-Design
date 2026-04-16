#include <stdio.h>

int calculate_sum(int n) {
    int sum = 0;
    for (int i = 1; i <= n; i++) {
        sum = (sum + i);
    }
    return sum;
}

int main() {
    int n = 10;
    int result = calculate_sum(n);
    printf("%d\n", result);
    if (result > 50) {
        printf("%d\n", 1);
    } else {
        printf("%d\n", 0);
    }
    return 0;
}
