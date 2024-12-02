#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Ham sinh du lieu ngau nhien va ghi vao file
void generateRandomArray(const char *filename, int n) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Cannot open file to write");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d\n", n); // Ghi kich thuoc mang

    // Sinh mang ngau nhien
    int i;  
    for (i = 0; i < n; i++) {
        fprintf(file, "%.2f ", (double)(rand() % 100)); // So thuc ngau nhien [0, 100)
    }
    fprintf(file, "\n");

    fclose(file);
    printf("Random array generated in file: %s\n\n", filename);
}

// Ham doc du lieu tu file
double* readArrayFromFile(const char *filename, int *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open file to read");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", n); // Doc kich thuoc mang
    double *array = (double *)malloc((*n) * sizeof(double));

    // Doc cac phan tu mang
    int i;  
    for (i = 0; i < *n; i++) {
        fscanf(file, "%lf", &array[i]);
    }

    fclose(file);
    return array;
}

// Tinh tong cac phan tu (tuan tu)
double sumSequential(double *array, int n) {
    double sum = 0.0;
    int i;  
    for (i = 0; i < n; i++) {
        sum += array[i];
    }
    return sum;
}

// Tinh tong cac phan tu (song song)
double sumParallel(double *array, int n) {
    double sum = 0.0;
    int i; 

    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < n; i++) {
        sum += array[i];
    }

    return sum;
}

// Tinh tich cac phan tu (tuan tu)
double productSequential(double *array, int n) {
    double product = 1.0;
    int i; 
    for (i = 0; i < n; i++) {
        product *= array[i];
    }
    return product;
}

// Tinh tich cac phan tu (song song)
double productParallel(double *array, int n) {
    double product = 1.0;
    int i; 

    #pragma omp parallel for reduction(*:product)
    
    for (i = 0; i < n; i++) {
        product *= array[i];
    }

    return product;
}

int main() {
    const char *filename = "array_data.txt";
    //const char *filename = "array_data_test.txt";
    int n = 10000000; // So phan tu cua mang
    //int n = 5; 

    // Sinh du lieu ngau nhien
    srand((unsigned int)time(NULL));
    generateRandomArray(filename, n);

    // Doc du lieu tu file
    int size;
    double *array = readArrayFromFile(filename, &size);

    // Tinh tong va tich theo cach tuan tu  
    double startSeq = omp_get_wtime();
    double sumSeq = sumSequential(array, size);
    double productSeq = productSequential(array, size);
    double endSeq = omp_get_wtime();

    // Tinh tong va tich theo cach song song
    double startPar = omp_get_wtime();
    double sumPar = sumParallel(array, size);
    double productPar = productParallel(array, size);
    double endPar = omp_get_wtime();
    
    double timeSeq = endSeq - startSeq;
    double timePar = endPar - startPar; 

    // Hien thi ket qua 
    printf("Sequential Sum: %.2f\n", sumSeq);
    printf("Sequential Product: %.2f\n", productSeq);
    printf("Time (Sequential): %.2f seconds\n\n", timeSeq);

    printf("Parallel Sum: %.2f\n", sumPar);
    printf("Parallel Product: %.2f\n", productPar);
    printf("Time (Parallel): %.2f seconds\n\n", timePar);
    
    printf("Percent (Accelerate): %.2f %%\n", (timeSeq - timePar) / timeSeq); 

    // Giai phong bo nho  
    free(array);

    return 0;
}

