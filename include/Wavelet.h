//
// Created by fpankretic on 4/30/23.
//

#ifndef BIOINFORMATICS_WAVELET_H
#define BIOINFORMATICS_WAVELET_H

#include <sdsl/bit_vectors.hpp>
#include <string>
using namespace std;
using namespace sdsl;

struct Node {
    Node* parent;
    Node* left;
    Node* right;
};

struct BitNode : Node {
    bit_vector vector;
};

struct CharNode : Node {
    char chr;
};

class Wavelet {
private:
    Node* start;
    unordered_map<char, int> char_map;

public:
    Wavelet(string& str) {
        build(str);
    }

    /*
     * Izgraditi nase stablo
     * Postavi mapu charactera
     * */
    void build(string& str);

    char access(int idx);
    int rank(char c, int idx);
    int select(char c, int idx);
    void print();
};

#endif //BIOINFORMATICS_WAVELET_H
