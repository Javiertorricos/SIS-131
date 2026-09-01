#include <stdio.h>

int main(){
    int longitud = 10;
    int Suma[longitud];
    int suma = 0;
    for (int i = 0; i < longitud; i++)
    {
        Suma[i] = i+1;
        suma += Suma[i];
    }
    
    printf("La suma total del arreglo es: %d\n" , suma);
    return 0;
}