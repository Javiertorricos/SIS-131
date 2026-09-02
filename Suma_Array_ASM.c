#include <stdio.h>

int main() {
    int longitud = 10;
    int Suma[10] = {0}; // Inicializado en ceros
    int suma = 0;

    __asm__ (
        "movl $0, %%ecx\n\t"             // %%ecx = contador del bucle (i = 0)
        "movl $0, %%edx\n\t"             // %%edx = acumulador de la suma (suma = 0)
        
        "bucle_suma:\n\t"                // Etiqueta de inicio del bucle
        "cmpl %2, %%ecx\n\t"             // Compara 'i' (%%ecx) con 'longitud' (%2)
        "jge fin_bucle\n\t"              // Si i >= longitud, sale del bucle
        
        // Asignación: Suma[i] = i + 1
        "movl %%ecx, %%eax\n\t"          // %%eax = i
        "addl $1, %%eax\n\t"             // %%eax = i + 1
        
        // Guardar en memoria: base + (índice * tamaño_elemento_4_bytes)
        "movl %%eax, (%1, %%rcx, 4)\n\t" // Suma[i] = %%eax
        
        // Acumular el total: suma += Suma[i]
        "addl %%eax, %%edx\n\t"          // %%edx = %%edx + %%eax
        
        "incl %%ecx\n\t"                 // i++ (incrementa contador)
        "jmp bucle_suma\n\t"             // Salta de vuelta al inicio
        
        "fin_bucle:\n\t"
        "movl %%edx, %0\n\t"             // Mueve el total acumulado a la variable de salida
        
        : "=r" (suma)                    // %0 = Operando de salida (variable 'suma' en C)
        : "r" (Suma), "r" (longitud)     // %1 = dirección base del arreglo, %2 = longitud
        : "%rax", "%rcx", "%rdx", "memory" // Registros modificados + aviso de escritura en memoria
    );

    // Verificación de los valores
    for (int i = 0; i < longitud; i++) {
        printf("Suma[%d] = %d\n", i, Suma[i]);
    }
    printf("La suma total del arreglo es: %d\n", suma);
    
    return 0;
}