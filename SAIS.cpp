#include <map>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

map<int, pair<int, int>> getBuckets(vector<int>& T) {
    // Inicializa la variable 'start' en 0, que se utilizará para rastrear el inicio de cada cubo.
    int start = 0;

    // Crea un mapa 'count' para contar cuántas veces aparece cada entero en el vector 'T'.
    map<int, int> count;

    // Crea un mapa 'buckets' que mapeará enteros a pares de enteros (rango de inicio y final).
    map<int, pair<int, int>> buckets;

    // Crea un vector 'keys' para almacenar las claves únicas en el mapa 'count'.
    vector<int> keys;

    // Contabiliza cuántas veces aparece cada entero en el vector 'T' y almacena los resultados en 'count'.
    for (int i = 0; i < T.size(); i++) {
        count[T[i]]++;
    }

    // Extrae las claves únicas (enteros) del mapa 'count' y las almacena en el vector 'keys'.
    for (const auto& pair : count) {
        keys.push_back(pair.first);
    }

    // Ordena el vector 'keys' en orden ascendente.
    sort(keys.begin(), keys.end());

    // Llena el mapa 'buckets' con pares de enteros que representan el rango de inicio y final
    // para cada entero en el vector 'T', utilizando el valor 'start' para calcular los rangos.
    for (const auto& key : keys) {
        buckets[key] = make_pair(start, start + count[key]);
        start += count[key];
    }

    // Devuelve el mapa 'buckets' que contiene los rangos para cada entero en 'T'.
    return buckets;
}


vector<int> sais(vector<int>& T) {

    int i = 0; // Variable para usar en los bucles

    // Crea un vector 't' que almacena caracteres ('S' o 'L') para representar el tipo de sufijo.
    vector<char> t(T.size() - 1, '_');
    t.push_back('S'); // Agrega 'S' al final de 't' para marcar el sufijo final.
    t.push_back('0'); // Agrega '0' al final de 't' para evitar problemas de índice.

    // Bucle para calcular el tipo de sufijo ('S' o 'L') para cada sufijo en 'T'.
    for ( i = T.size()-1; i > 0; i--) {
        if (T[i - 1] == T[i]) {
            t[i - 1] = t[i];
        }
        else {
            t[i - 1] = T[i - 1] < T[i] ? 'S' : 'L';
        }
    }

    // Obtiene los cubos (buckets) de 'T' utilizando la función 'getBuckets' definida en otra parte del código.
    map<int, std::pair<int, int>> buckets = getBuckets(T);
    // Crea variables y estructuras de datos necesarias.
    map<int, int> count;
    vector<int> SA(T.size(), -1);  // Vector 'SA' que almacenará la sufijos ordenados.
    map<int, int> LMS;  // Mapa que almacenará los sufijos más cortos.
    int end = NULL; // Inicializa 'end' como nulo
    int revoffset = 0; // Variable para rastrear el offset.

    // Bucle para procesar sufijos y generar el vector 'SA'
    for (i = T.size() - 1; i > 0; i--) {
        if (t[i] == 'S' && t[i-1] == 'L') {
            revoffset = ++count[T[i]];
            // cout << SA[buckets[T[i]].second - revoffset] << endl;
            SA[buckets[T[i]].second - revoffset] = i;
            if (end != NULL) {
                LMS[i] = end;
            }
            end = i;
        }
    }

    // Agrega un sufijo especial al final de 'LMS'.
    LMS[T.size() - 1] = T.size() - 1;
    count.clear(); // Limpia el mapa 'count'.

    // Inicializa variables para procesar los sufijos y nombres.
    char32_t symbol = NULL;
    int offset = 0;

    // Bucle para procesar los sufijos y generar el nuevo vector 'SA'.
    for (i = 0; i < T.size(); i++) {
        if (SA[i] >= 0 && SA[i] - 1 >= 0) {
            if (t[SA[i] - 1] == 'L') {
                symbol = T[SA[i] - 1];
                offset = count[symbol];
                SA[buckets[symbol].first + offset] = SA[i] - 1;
                count[symbol] = offset + 1;
            }
        }
    }

    // Limpia el mapa 'count' nuevamente.
    count.clear();

    // Bucle para procesar los sufijos y generar el nuevo vector 'SA'.
    for (i = T.size() - 1; i > 0; i--) {
        if (SA[i] > 0) {
            if (t[SA[i] - 1] == 'S') {
                symbol = T[SA[i] - 1];
                revoffset = count[symbol] = ++count[symbol];
                SA[buckets[symbol].second - revoffset] = SA[i] - 1;
            }
        }
    }

    vector<int> namesp(T.size(), -1); // Inicializa un vector 'namesp' del mismo tamaño que 'T' con -1 en cada posición, que se utilizará para almacenar los nombres de los sufijos.
    int name = 0; // Inicializa una variable 'name' para rastrear el nombre actual.
    int prev = -1; // Inicializa 'prev' en -1 para rastrear el sufijo anterior durante el procesamiento.

    // Bucle para iterar a través de los sufijos en 'SA'.
    for (i = 0; i < SA.size(); i++) {
        if (SA[i] - 1 >= 0 && t[SA[i]] == 'S' && t[SA[i] - 1] == 'L') {

            // Verifica si el sufijo actual cumple ciertas condiciones:
            // 1. El sufijo anterior existe (SA[i] - 1 >= 0).
            // 2. El sufijo actual es de tipo 'S' (t[SA[i]] == 'S').
            // 3. El sufijo anterior es de tipo 'L' (t[SA[i] - 1] == 'L').

            if (prev != -1) {

                // Si el sufijo anterior ya ha sido procesado:

                // Extrae dos subcadenas 'sub1' y 'sub2' de 'T' utilizando los índices de 'SA[prev]' y 'SA[i]'.

                std::vector<int> sub1(T.begin() + SA[prev], T.begin() + LMS[SA[prev]]);
                std::vector<int> sub2(T.begin() + SA[i], T.begin() + LMS[SA[i]]);

                // Compara las subcadenas 'sub1' y 'sub2'.
                if (sub1 != sub2) {
                    // Si las subcadenas son diferentes, incrementa el nombre actual.
                    name++;
                }
            }
            prev = i; // Actualiza 'prev' para el sufijo actual.
            namesp[SA[i]] = name; // Asigna el nombre al sufijo en 'namesp'.
        }
    }

    vector<int> names;
    vector<int> SApIdx;
    for (i = 0; i < T.size(); i++) {
        if (namesp[i] != -1) {
            names.push_back(namesp[i]);
            SApIdx.push_back(i);
        }
    }

    if (name < names.size() - 1) {
      
        names = sais(names);
    }

    reverse(names.begin(), names.end());

    vector<int> NewSA(T.size(), -1);

    // Limpia el mapa 'count' nuevamente.
    count.clear();
    int pos = 0;
    for (i = 0; i < names.size(); i++) {
        // El bucle itera a través de los nombres de los sufijos LMS en 'names'.
        pos = SApIdx[names[i]];
        // Obtiene el índice del sufijo en el vector 'SApIdx' utilizando el nombre actual 'names[i]'.
        revoffset = count[T[pos]] = ++count[T[pos]];
        // Incrementa el contador para el símbolo T[pos] y almacena el valor actual en 'revoffset'.
        NewSA[buckets[T[pos]].second - revoffset] = pos;
        // Asigna el índice 'pos' del sufijo al vector 'NewSA' en una posición calculada
        // utilizando la información del cubo (bucket) para el símbolo T[pos].
    }

    // Limpia el mapa 'count' nuevamente.
    count.clear();
    for (i = 0; i < T.size(); i++) {
        // El bucle itera a través de los sufijos en orden ascendente.
        if (NewSA[i] >= 0 && NewSA[i] - 1 >= 0) {
            // Comprueba si el sufijo actual (NewSA[i]) y su sufijo anterior (NewSA[i] - 1) existen.
            if (t[NewSA[i] - 1] == 'L') {
                // Verifica si el sufijo anterior (NewSA[i] - 1) es de tipo 'L'.
                symbol = T[NewSA[i] - 1];
                // Obtiene el carácter (símbolo) correspondiente en la posición del sufijo anterior en el vector 'T'.
                offset = count[symbol];
                // Obtiene el valor actual del contador para el símbolo.
                NewSA[buckets[symbol].first + offset] = NewSA[i] - 1;
                // Asigna el índice del sufijo anterior (NewSA[i] - 1) a la posición adecuada en el vector 'NewSA',
                // basándose en la información de los cubos (buckets) para el símbolo.
                count[symbol] = offset + 1;
                // Incrementa el contador para el símbolo en uno, para indicar que se ha procesado un sufijo 'L' para ese símbolo.
            }
        }
    }

    // Limpia el mapa 'count' nuevamente.
    count.clear();
    for (i = T.size() - 1; i > 0; i--) {
        // El bucle itera a través de los sufijos en orden inverso.
        if (NewSA[i] > 0 && NewSA[i] - 1 >= 0) {
            // Comprueba si el sufijo actual (NewSA[i]) y su sufijo anterior (NewSA[i] - 1) existen.
            if (t[NewSA[i] - 1] == 'S') {
                // Verifica si el sufijo anterior (NewSA[i] - 1) es de tipo 'S'.
                symbol = T[NewSA[i] - 1];
                // Obtiene el carácter (símbolo) correspondiente en la posición del sufijo anterior en el vector 'T'.
                revoffset = count[symbol] = ++count[symbol];
                // Incrementa el contador para el símbolo actual y asigna el valor actual a 'revoffset'.
                NewSA[buckets[symbol].second - revoffset] = NewSA[i] - 1;
                // Asigna el índice del sufijo anterior (NewSA[i] - 1) a la posición adecuada en el vector 'NewSA'.
            }
        }
    }

    // Al final, se prepara el vector 'FSA' a partir de 'NewSA' y se lo devuelve.
    vector<int> FSA;

    for (i = 0; i < NewSA.size(); i++) {
        FSA.push_back(NewSA[i]);
    }

    return FSA;
}

// Función para leer un archivo
string readFileToString() {
    ifstream inputFile;
    string book, path;
  
    do {
        cout << "Introduzca el nombre del archivo: " << endl;
        cin >> book;
        path = "./Libros/"+book+".txt";
        inputFile.open(path);
        if (inputFile.fail()) {
            cout << "No se pudo abrir el archivo: " << path << endl << endl;
        }
    }
    while (inputFile.fail());
    string fileContents;
    stringstream buffer;
    buffer << inputFile.rdbuf();
    fileContents = buffer.str();
    inputFile.close();

    return fileContents;
}

// Función para obtener el texto que deseamos encontrar
string textToSearch() {
    string text = "";
    cout << "Introduzca la palabra a buscar: " << endl;
    cin >> text;

    return text;
}


std::vector<int> all_occurrences(const std::vector<int>& suffix_array, const std::string& target_suffix, const std::string& text) {
    std::vector<int> list_indexes;  // Crea un vector para almacenar los índices de las ocurrencias.
    int left = 0;  // Inicializa una variable "left" en 0.
    int right = suffix_array.size() - 1;  // Inicializa "right" con el tamaño del vector "suffix_array" - 1.
    int middle = (right - left) / 2;  // Calcula el valor medio entre "left" y "right".
    int index = 0;  // Inicializa una variable "index" en 0.

    while (left <= right) {  // Inicia un bucle mientras "left" sea menor o igual a "right".
        middle = (left + right) / 2;  // Calcula nuevamente el valor medio.

        if (text[suffix_array[middle]] == target_suffix[0]) {  // Comprueba si el primer carácter de "target_suffix" coincide con el carácter correspondiente en "text".

            index = 0;  // Inicializa "index" en 0.

            while (text[suffix_array[middle + index]] == target_suffix[0]) {  // Entra en un bucle mientras los caracteres coincidan.
                if (text.substr(suffix_array[middle + index], target_suffix.size()) == target_suffix) {  // Comprueba si la subcadena coincide con "target_suffix".
                    list_indexes.push_back(suffix_array[middle + index]);  // Agrega el índice al vector "list_indexes".
                }
                index++;  // Incrementa "index".
            }

            index = -1;  // Restablece "index" a -1.
            while (text[suffix_array[middle + index]] == target_suffix[0]) {  // Entra en un bucle inverso mientras los caracteres coincidan.
                if (text.substr(suffix_array[middle + index], target_suffix.size()) == target_suffix) {  // Comprueba si la subcadena coincide con "target_suffix".
                    list_indexes.push_back(suffix_array[middle + index]);  // Agrega el índice al vector "list_indexes".
                }
                index--;  // Decrementa "index".
            }

            return list_indexes;  // Retorna el vector "list_indexes" con los índices de ocurrencias.
        }
        else if (text[suffix_array[middle]] < target_suffix[0]) {  // Si el carácter de "text" es menor que el carácter de "target_suffix".
            left = middle + 1;  // Actualiza "left" para buscar en la mitad derecha del arreglo.
        }
        else {  // Si el carácter de "text" es mayor que el carácter de "target_suffix".
            right = middle - 1;  // Actualiza "right" para buscar en la mitad izquierda del arreglo.
        }
    }
}


int main() {
    // variables para mantener un registro del tiempo de ejecución
    //clock_t start, end;
    // start = clock();

    cout << "SAIS ALGORITHM\n";
    cout << "Josafat Garcia Sarmientos | A01275684\n";
    cout << "Raul Diaz Romero          | A01735839\n";

    string file = readFileToString();
    string text = textToSearch();

    

    vector<int> char_text(file.begin(), file.end());
    
    char_text.push_back(0);

    
    vector<int> suffix_result = sais(char_text);
    
    std::vector<int> allOccurrences = all_occurrences(suffix_result, text, file);
    int count = 0;

    cout << endl << "Arreglo de posiciones en donde se encuentra la palabra:" << endl;
    if (allOccurrences.size() > 0){
      cout << "[";
      for (int j = 0; j < allOccurrences.size()-1; j++) {
          cout << allOccurrences[j] << ", ";
          count++;
      }
      cout << allOccurrences[allOccurrences.size()-1] << "]" << endl;
      count++;
    }
    

    cout << "Ocurrencias Totales: " << count << endl;

    // Esta parte del código se encarga de recabar el tiempo de ejecución del programa con ticks del reloj interno de la computadora
    // // Recording the end clock tick.    
    // end = clock();
    // // Calculating total time taken by the program.
    // double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    // cout << "Time taken by program is : " << time_taken << setprecision(5);
    // cout << " sec " << endl;
    return 0;
}