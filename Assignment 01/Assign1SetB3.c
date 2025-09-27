#include <stdio.h>
#include <time.h>
int main() {
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();
    long long sum = 0;
    for (long long i = 0; i < 100000000; i++) {
        sum += i;
    }
    printf("Sum: %lld\n", sum);
    end_time = clock();
    cpu_time_used = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken to execute the loop: %.6f seconds\n", cpu_time_used);
    return 0;
}

