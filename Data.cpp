#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Ham sinh du lieu ngau nhien
void generateRandomData(const char *filename, int m, int n) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Cannot open file to write");
        exit(EXIT_FAILURE);
    }

    // Ghi so hang va cot
    fprintf(file, "%d %d\n", m, n);

    // Sinh ma tran ngau nhien
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%.2f ", (double)(rand() % 1000)); // So thuc ng?u nhiên [0, 1000)
        }
        fprintf(file, "\n");
    }

    // Sinh vector ngau nhien
    for (int j = 0; j < n; j++) {
        fprintf(file, "%.2f ", (double)(rand() % 1000)); // So thuc ng?u nhiên [0, 1000)
    }
    fprintf(file, "\n");

    fclose(file);
}

int main() {
    const char *filename = "input_random.txt";
    int m = 10000; // So hang cua ma tran
    int n = 10000; // So cot cua ma tran

    // Khoi tao bo sinh so ngau nhien
    srand((unsigned int)time(NULL));

    // Sinh du lieu
    generateRandomData(filename, m, n);

    printf("Random data generated in file: %s\n", filename);

    return 0;
}

