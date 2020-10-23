
#include <stdio.h>
#include <math.h>

int is_prime(unsigned long n) {
    unsigned long limit, i;

    if (n < 2) {
        return 0;
    }

    limit = floor(sqrt(n));
    for (i = 2; i <= limit; ++i) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int main() {
    unsigned long n;

    if (scanf("%lu", &n) != 1) {
        return 1;
    }

    if (is_prime(n)) {
        printf("yes\n");
    } else {
        printf("no\n");
    }

    return 0;
}
