#include <stdio.h>

int main() {
    int arr[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = (arr[0] + arr[1]);
    printf("%d\n", arr[2]);
    int matrix[2][2];
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 3;
    matrix[1][1] = ((matrix[0][0] + matrix[0][1]) + matrix[1][0]);
    printf("%d\n", matrix[1][1]);
    return 0;
}
