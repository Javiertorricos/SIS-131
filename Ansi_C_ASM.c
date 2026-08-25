#include <stdio.h>
int main(void){
    int a = 10;
    int b = 20;
    int resultado = 0;
    //Se bloquea asm en linea para sumar dos variables
    __asm__("movl %1, %%eax\n\t"
    "addl %2, %%eax\n\t"
    "movl %%eax, %0\n\t"
    : "=r" (resultado)  //Se espera (%0)
    : "r" (a), "r" (b)  //La entrada sera (%1, %2)
    :"%eax"             //El registro que se modifica
    );
    printf("El resultado de la suma es: %d\n", resultado);
    return 0;
}