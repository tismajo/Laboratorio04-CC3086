/*
* ------------------------------------------------------------
* dp_ompco.c
* dot product with omp leaf strategy and cut-off limit
* -------------------------------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* Programación de Microprocesadores
*
* Introducción OpenMP
* Descripción: programa paralelo para calcular
*              el producto punto de 2 vectores A & B.
*			   El resultado se almacena en result (int).
* Autor:	   K.B.
* Fecha:	   Agosto 11, 2024.
* -------------------------------------------------------------
*/
#include <stdio.h>
#include <omp.h>

#define N 32
#define MIN_SIZE 4
#define CUTOFF 4  // Definimos el cut-off para la profundidad

int result = 0;

void dot_product(int *A, int *B, int n) {
    int thread_id = omp_get_thread_num(); // Obtener el ID del hilo actual
    printf("Hilo %d - dot_product para n=%d\n", thread_id, n);
	
    for (int i = 0; i < n; i++) {
        #pragma omp atomic
        result += A[i] * B[i];
        printf("Hilo %d - A[%d] * B[%d] = %d * %d = %d, resultado parcial = %d\n", 
               thread_id, i, i, A[i], B[i], A[i] * B[i], result);
    }
}

void rec_dot_product(int *A, int *B, int n, int depth) {
	int thread_id = omp_get_thread_num(); // Obtener el ID del hilo actual
	printf("Hilo %d - rec_dot_product para n=%d, depth=%d\n", thread_id, n, depth);

    if (n > MIN_SIZE) {
        int n2 = n / 2;
        if (depth == CUTOFF) {
			printf("Hilo %d - Creando tareas paralelas en el cut-off depth=%d\n", thread_id, depth);
            #pragma omp task
            {
                rec_dot_product(A, B, n2, depth + 1);
                rec_dot_product(A + n2, B + n2, n - n2, depth + 1);
            }
        } else {
            rec_dot_product(A, B, n2, depth + 1);
            rec_dot_product(A + n2, B + n2, n - n2, depth + 1);
        }
    } else { // si la recursión ha terminado, necesita verificar si la tarea ha sido generada
        if (depth <= CUTOFF) {
            #pragma omp task
            dot_product(A, B, n);
        } else {
            dot_product(A, B, n);
        }
    }
}

int main() {
    // Inicialización de los vectores A y B
    int a[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 
				17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,};
				
    int b[N] = {32, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 
				16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    #pragma omp parallel
    {
        #pragma omp single
        rec_dot_product(a, b, N, 0);  // Comenzamos con una profundidad de 0
    }

    // Imprimir el resultado
    printf("El producto punto es: %d\n", result);

    return 0;
}
