#include <stdio.h>

struct Point {
    int x;
    int y;
};

struct Person {
    int id;
    float salary;
};

int main() {
    struct Point p;
    p.x = 10;
    p.y = 20;
    printf("%d\n", (p.x + p.y));
    struct Person user;
    user.id = 101;
    user.salary = 5000.5;
    printf("%d\n", user.id);
    printf("%f\n", user.salary);
    return 0;
}
