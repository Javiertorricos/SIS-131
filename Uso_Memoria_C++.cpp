#include <iostream>
#include <fstream>
#include <cstring> 

using namespace std;

int main() {
    const char* nombreArchivo = "datos_memoria.bin";

    {
        ofstream archivoEscritura(nombreArchivo, ios::binary);
        if (!archivoEscritura) {
            cerr << "Error al crear el archivo en disco." << endl;
            return 1;
        }

        int bufferAux[1000];
        for (int i = 0; i < 1000; i++) {
            bufferAux[i] = i + 1;
        }

        archivoEscritura.write(reinterpret_cast<const char*>(bufferAux), 1000 * sizeof(int));
        archivoEscritura.close();
    }

    int arr1[1000];
    ifstream archivoLectura(nombreArchivo, ios::binary);
    if (!archivoLectura) {
        cerr << "Error al leer el archivo desde memoria secundaria." << endl;
        return 1;
    }

    archivoLectura.read(reinterpret_cast<char*>(arr1), 1000 * sizeof(int));
    archivoLectura.close();

    int arr2[100];
    for (int i = 0; i < 100; i++) {
        arr2[i] = i + 1; 
    }

    cout << "--- Primeros 5 elementos de arr1 (leidos de disco) ---" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "arr1[" << i << "] = " << arr1[i] << endl;
    }

    cout << "\n--- Ultimos 5 elementos de arr1 (leidos de disco) ---" << endl;
    for (int i = 995; i < 1000; i++) {
        cout << "arr1[" << i << "] = " << arr1[i] << endl;
    }

    memcpy(arr2, arr1 + 500, 100 * sizeof(int));

    memset(arr1 + 500, 0, 100 * sizeof(int));

    cout << "\n--- Elementos transferidos al segundo arreglo (arr2) ---" << endl;
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

    cout << "\n--- Validacion del rango 500-599 en arr1 ---" << endl;
    if (todosCeros) {
        cout << "Resultado: Correcto. Los indices 500 al 599 estan en cero tras la operacion." << endl;
    } else {
        cout << "Resultado: Error. Existen valores distintos de cero en el rango." << endl;
    }

    return 0;
}
