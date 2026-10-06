#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    int mb_asignados = 0;
    size_t tamaño_bloque = 10 * 1024 * 1024; // Bloques de 10 MB
    void *bloque;

    printf("Iniciando consumo de memoria RAM...\n");

    while (1) {
        bloque = malloc(tamaño_bloque);
        if (bloque == NULL) {
            printf("\nMemoria física y Swap agotados. Total asignado: %d MB\n", mb_asignados);
            break;
        }
        
        // Es obligatorio escribir en la memoria (memset) para forzar a Linux 
        // a asignar las páginas físicas reales, de lo contrario solo las reserva.
        memset(bloque, 0, tamaño_bloque);
        mb_asignados += 10;
        
        printf("Memoria bloqueada: %d MB\r", mb_asignados);
        fflush(stdout);
        
        usleep(100000); // Pausa de 100ms para no bloquear la terminal instantáneamente
    }

    return 0;
}