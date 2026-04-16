#include <stdio.h>

int main() {
    struct Point {
        int x;
        int y;
    };

    struct Point p;
    p.x = 10;
    p.y = 20;
    printf("%d\n", (p.x + p.y));
    struct Person {
        int id;
        float salary;
    };

    struct Person user;
    user.id = 101;
    user.salary = 5000.5;
    printf("%d\n", user.id);
    printf("%d\n", user.salary);
    return 0;
}
