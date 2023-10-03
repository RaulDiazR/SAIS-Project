#include <map>
#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

map<char32_t, pair<int, int>> getBuckets(vector<char32_t>& T) {

    
    int start = 0;
    map<char32_t, int> count;
    map<char32_t, pair<int, int>> buckets;
    vector<char32_t> keys;

    for (int i = 0; i < T.size(); i++) {
        count[T[i]]++;
    }

    /*for (const auto& pair : count) {
      cout << "Elemento: " << pair.first << ", Frecuencia: " << pair.second << std::endl;
    }*/

    for (const auto& pair : count) {
        keys.push_back(pair.first);
    }

    sort(keys.begin(), keys.end());

    for (const auto& key : keys) {
        buckets[key] = make_pair(start, start + count[key]);
        start += count[key];
    }

    /*for (const auto& pair : keys) {
        std::cout << "Elemento: " << pair << ", Rango: [" << buckets[pair].first << ", " << buckets[pair].second << "]" << std::endl;
    }*/

    return buckets;
}

vector<char32_t> sais(vector<char32_t> T) {

    int i = 0;

    vector<char> t(T.size() - 1, '_');
    t.push_back('S');
    t.push_back('0');
    /*for (char c : t) {
        std::cout << c << " ";
    }*/

    for ( i = T.size()-1; i > 0; i--) {
        if (T[i - 1] == T[i]) {
            t[i - 1] = t[i];
        }
        else {
            t[i - 1] = T[i - 1] < T[i] ? 'S' : 'L';
        }
    }

    /*for (char c : t) {
        std::cout << c << " ";
    }*/

    map<char32_t, std::pair<int, int>> buckets = getBuckets(T);

    /*for (const auto& pair : buckets) {
        char32_t key = pair.first;
        int start = pair.second.first;
        int end = pair.second.second;
        std::cout << "Key: " << key << ", Start: " << start << ", End: " << end << std::endl;
    }*/

    

    map<char32_t, int> count;
    vector<int> SA(T.size(), -1);
    map<char32_t, int> LMS;
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

    /*std::cout << "Count 1:" << std::endl;
    for (const auto& pair : count) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << " ";
    }
    cout << std::endl;

    std::cout << "SA 1:" << std::endl;
    for (int i = 0; i < SA.size(); i++) {
        std::cout << SA[i] << " ";
    }
    cout << std::endl;

    std::cout << "LMS 1:" << std::endl;
    for (const auto& pair : LMS) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << " ";
    }
    cout << std::endl;*/

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

    /*std::cout << "Count 2:" << std::endl;
    for (const auto& pair : count) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << " ";
    }
    cout << std::endl;

    std::cout << "SA 2:" << std::endl;
    for (int i = 0; i < SA.size(); i++) {
        std::cout << SA[i] << " ";
    }
    cout << std::endl;

    std::cout << "LMS 2:" << std::endl;
    for (const auto& pair : LMS) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << " ";
    }
    cout << std::endl;*/

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

    /*std::cout << "Count 3:" << std::endl;
    for (const auto& pair : count) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << " ";
    }
    cout << std::endl;

    std::cout << "SA 3:" << std::endl;
    for (int i = 0; i < SA.size(); i++) {
        std::cout << SA[i] << " ";
    }
    cout << std::endl;

    std::cout << "LMS 3:" << std::endl;
    for (const auto& pair : LMS) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << " ";
    }
    cout << std::endl;*/

    vector<int> namesp(T.size(), -1);
    int name = 0;
    int prev = -1;
    for (i = 0; i < SA.size(); i++) {
        if (SA[i] - 1 >= 0 && t[SA[i]] == 'S' && t[SA[i] - 1] == 'L') {
            if (prev != -1) {

                std::vector<char32_t> sub1(T.begin() + SA[prev], T.begin() + LMS[SA[prev]]);
                std::vector<char32_t> sub2(T.begin() + SA[i], T.begin() + LMS[SA[i]]);


                if (sub1 != sub2) {

                    /*std::cout << "SA[prev]: " << SA[prev] << std::endl;
                    std::cout << "LMS[SA[prev]]: " << LMS[SA[prev]] << std::endl;
                    std::cout << "SA[i]: " << SA[i] << std::endl;
                    std::cout << "LMS[SA[i]]: " << LMS[SA[i]] << std::endl;*/

                    name++;
                }
            }
            prev = i;
            namesp[SA[i]] = name;
        }
    }

    /*std::cout << "Count 4:" << std::endl;
    for (const auto& pair : count) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << " ";
    }
    cout << std::endl;

    std::cout << "SA 4:" << std::endl;
    for (int i = 0; i < SA.size(); i++) {
        std::cout << SA[i] << " ";
    }
    cout << std::endl;

    std::cout << "LMS 4:" << std::endl;
    for (const auto& pair : LMS) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << " ";
    }
    cout << std::endl;

    std::cout << "NAME SP:" << std::endl;
    for (int i = 0; i < namesp.size(); i++) {
        std::cout << namesp[i] << " ";
    }
    std::cout << std::endl;*/

    vector<char32_t> names;
    vector<int> SApIdx;
    for (i = 0; i < T.size(); i++) {
        if (namesp[i] != -1) {
            names.push_back(namesp[i]);
            SApIdx.push_back(i);
        }
    }

    //cout << "NAME" << name << endl;
    //// Imprimir el vector 'names'
    //std::cout << "names vector:" << std::endl;
    //for (const char32_t& name : names) {
    //    std::cout << name << " ";
    //}
    //std::cout << std::endl;

    //// Imprimir el vector 'SApIdx'
    //std::cout << "SApIdx vector:" << std::endl;
    //for (const int& idx : SApIdx) {
    //    std::cout << idx << " ";
    //}
    //std::cout << std::endl;

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

    // Imprimir el vector 'names'
    /*std::cout << "Names:" << std::endl;
    for (char32_t name : names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;*/

    //// Imprimir el vector 'NewSA'
    //std::cout << "NewSA vector:" << std::endl;
    //for (int element : NewSA) {
    //    std::cout << element << " ";
    //}
    //std::cout << std::endl;

    //// Imprimir el map 'count'
    //std::cout << "count map:" << std::endl;
    //for (const auto& pair : count) {
    //    std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    //}

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

    vector<char32_t> FSA;

    for (i = 0; i < NewSA.size(); i++) {
        FSA.push_back(NewSA[i]);
        //cout << FSA[i] << " ";
    }
    //cout << endl;
   

    return FSA;
}

int main() {
    cout << "SAIS ALGORITHM\n";
    cout << "Josafat Garcia Sarmientos | A01275684\n";
    cout << "Raul Diaz Romero          | A01735839\n";

    string text = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.$0";
    vector<char32_t> char_text;

    for (int i = 0; i < text.length(); i++) {
        char_text.push_back(static_cast<char32_t>(text[i]));
    }

  
    vector<char32_t> V = sais(char_text);
    for (int j = 0; j < V.size(); j++) {
        cout << V[j] << " ";
    }


    return 0;
}