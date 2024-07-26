#include <stdio.h>
#include <math.h>

double factorial(int n) {
    double result = 1;
    int i;
    for (i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int n = 25;
    double total_keys = factorial(n);
    double unique_keys = total_keys / (factorial(5) * pow(5, 5));

    printf("Total possible keys (approx as power of 2): 2^%.2f\n", log2(total_keys));
    printf("Effectively unique keys (approx as power of 2): 2^%.2f\n", log2(unique_keys));

    return 0;
}

