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

    
    int start = 0;
    map<int, int> count;
    map<int, pair<int, int>> buckets;
    vector<int> keys;

    for (int i = 0; i < T.size(); i++) {
        count[T[i]]++;
    }

    for (const auto& pair : count) {
        keys.push_back(pair.first);
    }

    sort(keys.begin(), keys.end());

    for (const auto& key : keys) {
        buckets[key] = make_pair(start, start + count[key]);
        start += count[key];
    }

    return buckets;
}

vector<int> sais(vector<int>& T) {

    int i = 0;

    vector<char> t(T.size() - 1, '_');
    t.push_back('S');
    t.push_back('0');

    for ( i = T.size()-1; i > 0; i--) {
        if (T[i - 1] == T[i]) {
            t[i - 1] = t[i];
        }
        else {
            t[i - 1] = T[i - 1] < T[i] ? 'S' : 'L';
        }
    }


    map<int, std::pair<int, int>> buckets = getBuckets(T);

    map<int, int> count;
    vector<int> SA(T.size(), -1);
    map<int, int> LMS;
    int end = NULL;
    int revoffset = 0;
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

    LMS[T.size() - 1] = T.size() - 1;
    count.clear();
    char32_t symbol = NULL;
    int offset = 0;
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

    count.clear();
    for (i = T.size() - 1; i > 0; i--) {
        if (SA[i] > 0) {
            if (t[SA[i] - 1] == 'S') {
                symbol = T[SA[i] - 1];
                revoffset = count[symbol] = ++count[symbol];
                SA[buckets[symbol].second - revoffset] = SA[i] - 1;
            }
        }
    }

    vector<int> namesp(T.size(), -1);
    int name = 0;
    int prev = -1;
    for (i = 0; i < SA.size(); i++) {
        if (SA[i] - 1 >= 0 && t[SA[i]] == 'S' && t[SA[i] - 1] == 'L') {
            if (prev != -1) {

                std::vector<int> sub1(T.begin() + SA[prev], T.begin() + LMS[SA[prev]]);
                std::vector<int> sub2(T.begin() + SA[i], T.begin() + LMS[SA[i]]);


                if (sub1 != sub2) {

                    name++;
                }
            }
            prev = i;
            namesp[SA[i]] = name;
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
    count.clear();
    int pos = 0;
    for (i = 0; i < names.size(); i++) {
        pos = SApIdx[names[i]];
        revoffset = count[T[pos]] = ++count[T[pos]];
        NewSA[buckets[T[pos]].second - revoffset] = pos;
    }


    count.clear();
    for (i = 0; i < T.size(); i++) {
        // cout << NewSA[i] << NewSA[i] - 1 << endl;
        if (NewSA[i] >= 0 && NewSA[i] - 1 >= 0) {
            if (t[NewSA[i] - 1] == 'L') {
                symbol = T[NewSA[i] - 1];
                offset = count[symbol];
                NewSA[buckets[symbol].first + offset] = NewSA[i] - 1;
                count[symbol] = offset + 1;
            }
        }
    }

    count.clear();
    for (i = T.size() - 1; i > 0; i--) {
        if (NewSA[i] > 0 && NewSA[i] - 1 >= 0) {
            if (t[NewSA[i] - 1] == 'S') {
                symbol = T[NewSA[i] - 1];
                revoffset = count[symbol] = ++count[symbol];
                NewSA[buckets[symbol].second - revoffset] = NewSA[i] - 1;
            }
        }
    }

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

string textToSearch() {
    string text = "";
    cout << "Introduzca la palabra a buscar: " << endl;
    cin >> text;

    return text;
}


vector<int> find_all_suffix_occurrences(const vector<int>& suffix_array, const string& target_suffix, const string& text) {
    int left = 0;
    int right = suffix_array.size() - 1;
    vector<int> occurrences;

    while (left <= right) {
        int mid = (left + right) / 2;
        string suffix = text.substr(suffix_array[mid]);

        if (suffix == target_suffix) {
            occurrences.push_back(suffix_array[mid]);  // Almacena la posición en el Suffix Array
            // Continuar buscando hacia la izquierda y la derecha
            left = mid + 1;
            right = mid - 1;
        }
        else if (suffix < target_suffix) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return occurrences;
}

int main() {
    clock_t start, end;
    start = clock();
    cout << "SAIS ALGORITHM\n";
    cout << "Josafat Garcia Sarmientos | A01275684\n";
    cout << "Raul Diaz Romero          | A01735839\n";

    string file = readFileToString();
    string text = textToSearch();

    vector<int> char_text(file.begin(), file.end());
    
    char_text.push_back(0);

    
    vector<int> suffix_result = sais(char_text);
    /*cout << "[";
    for (int j = 0; j < V.size()-1; j++) {
        cout << V[j] << ", ";
    }
    cout << V[V.size()-1] << "]";*/

    vector<int> allOccurrences = find_all_suffix_occurrences(suffix_result, text, file);

    cout << "[";
    for (int j = 0; j < allOccurrences.size(); j++) {
        cout << allOccurrences[j] << ", ";
    }
    cout << "]";

    // Recording the end clock tick.    
    end = clock();
    // Calculating total time taken by the program.
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by program is : " << time_taken << setprecision(5);
    cout << " sec " << endl;
    return 0;
}