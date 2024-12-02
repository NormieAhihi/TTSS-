#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Ham doc du lieu tu file
void readData(const char *filename, double ***matrix, double **vector, int *m, int *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", m, n);

    // Cap phat bo  nho cho ma tran va vector
    *matrix = (double **)malloc(*m * sizeof(double *));
    for (int i = 0; i < *m; i++) {
        (*matrix)[i] = (double *)malloc(*n * sizeof(double));
    }
    *vector = (double *)malloc(*n * sizeof(double));

    // Doc ma tran
    for (int i = 0; i < *m; i++) {
        for (int j = 0; j < *n; j++) {
            fscanf(file, "%lf", &((*matrix)[i][j]));
        }
    }

    // Doc vector
    for (int i = 0; i < *n; i++) {
        fscanf(file, "%lf", &((*vector)[i]));
    }

    fclose(file);
}

// Ham nhan ma tran voi vector (tuan tu)
void multiplySequential(double **matrix, double *vector, double *result, int m, int n) {
    for (int i = 0; i < m; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

// Ham nhan ma tran voi vector (song song)
void multiplyParallel(double **matrix, double *vector, double *result, int m, int n) {
    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

// Ham in ket qua  
void printResult(double *result, int m) {
    for (int i = 0; i < m; i++) {
        printf("%.2lf ", result[i]);
    }
    printf("\n");
}

// Ham giai phong bo nho  
void freeMemory(double **matrix, double *vector, double *result, int m) {
    for (int i = 0; i < m; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);
}

int main() {
    //char filename[] = "input_test.txt";
    //char filename[] = "input_random_1.txt";
    char filename[] = "input_random_2.txt";
    double **matrix;
    double *vector;
    double *resultSeq, *resultPar;
    int m, n;

    // Doc du lieu tu file
    readData(filename, &matrix, &vector, &m, &n);

    // Cap phat bo nho cho ket qua  
    resultSeq = (double *)malloc(m * sizeof(double));
    resultPar = (double *)malloc(m * sizeof(double));

    // Nhan tuan tu  
    double startSeq = omp_get_wtime();
    multiplySequential(matrix, vector, resultSeq, m, n);
    double endSeq = omp_get_wtime();
    double timeSeq = endSeq - startSeq; 

    // Nhan song song
    double startPar = omp_get_wtime();
    multiplyParallel(matrix, vector, resultPar, m, n);
    double endPar = omp_get_wtime();
    double timePar = endPar - startPar; 

    // In ket qua  
    printf("Sequential Result: ");
    printResult(resultSeq, m);
    printf("Time (Sequential): %.2lf seconds\n\n", timeSeq);

    printf("Parallel Result: ");
    printResult(resultPar, m);
    printf("Time (Parallel): %.2lf seconds\n\n", timePar);
    
    printf("Percent accelerate: %.2lf %% \n", 100 * (timeSeq - timePar) / timeSeq);  

    // Giai phong bo nho  
    freeMemory(matrix, vector, resultSeq, m);
    free(resultPar);

    return 0;
}

