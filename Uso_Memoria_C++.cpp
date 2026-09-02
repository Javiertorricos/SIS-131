#include <iostream>
#include <cstring> 
using namespace std;

int main() {
    int arr1[1000];
    for (int i = 0; i < 1000; i++) {
        arr1[i] = i + 1;
    }

    int arr2[100];
    for (int i = 0; i < 100; i++) {
        arr2[i] = i + 1; 
    }

    cout << "--- Primeros 5 elementos de arr1 (antes) ---" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "arr1[" << i << "] = " << arr1[i] << endl;
    }

    cout << "\n--- Ultimos 5 elementos de arr1 (antes) ---" << endl;
    for (int i = 995; i < 1000; i++) {
        cout << "arr1[" << i << "] = " << arr1[i] << endl;
    }

    memcpy(arr2, arr1 + 500, 100 * sizeof(int));

    memset(arr1 + 500, 0, 100 * sizeof(int));

    cout << "\n--- Elementos transferidos a arr2 (100 elementos) ---" << endl;
    for (int i = 0; i < 100; i++) {
        cout << "arr2[" << i << "] = " << arr2[i] << " ";
        if ((i + 1) % 10 == 0) cout << endl; 
    }

    bool todosCeros = true;
    for (int i = 500; i <= 599; i++) {
        if (arr1[i] != 0) {
            todosCeros = false;
            break;
        }
    }

    cout << "\n--- Validacion del rango 500-599 en arr1 tras la operacion ---" << endl;
    if (todosCeros) {
        cout << "Resultado: Correcto. Todos los elementos del indice 500 al 599 estan en cero." << endl;
    } else {
        cout << "Resultado: Error. Existen valores distintos de cero en el rango." << endl;
    }

    return 0;
}