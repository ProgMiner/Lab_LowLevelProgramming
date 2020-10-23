
#include <stdio.h>

int a[] = {1, 2, 3, 4, 5};
int b[] = {5, 4, 3, 2, 1};

long scalarProduct(int a[], int b[], int length) {
    long result = 0;

    size_t i;
    for (i = 0; i < length; ++i) {
        result += a[i] * b[i];
    }

    return result;
}

int main() {
    a[1] = 31;
    printf("Scalar product: %ld\n", scalarProduct(a, b, 5));
    return 0;
}
