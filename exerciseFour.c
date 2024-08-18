#include <stdio.h>
#include <omp.h>
#include <math.h>

int main() {
    int hilos;
    printf("Indique si quiere trabajar con 8, 16 o 32 hilos: ");
    scanf("%d", &hilos);

    if (hilos != 8 && hilos != 16 && hilos != 32) {
        printf("Numero de hilos no valido, solo pueden ser 8, 16 o 32 hilos.\n");
        return -1;
    }

    int x[hilos];
    double y[hilos];

    for (int i = 0; i < hilos; i++) {
        x[i] = (i + 1) * 2;
    }

    // Calcular los valores de y en paralelo en un ciclo for.
    #pragma omp parallel for num_threads(hilos)
    for (int i = 0; i < hilos; i++) {
        printf("Hilo %d - Calculando y para x = %d\n", omp_get_thread_num(), x[i]); // Muestra el número de hilo que trabaja el resultado.
        double a = 5.0;
        y[i] = (0.5) * a + pow(x[i], 2);
    }

    printf("x\ty\n");
    for (int i = 0; i < hilos; i++) {
        printf("%d\t%.2f\n", x[i], y[i]); // Aproximar el resultado.
    }

    return 0;
}
