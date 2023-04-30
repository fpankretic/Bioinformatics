//
// Created by fpankretic on 4/30/23.
//

#ifndef BIOINFORMATICS_WAVELET_H
#define BIOINFORMATICS_WAVELET_H

#include <sdsl/bit_vectors.hpp>
#include <string>
#include <unordered_set>

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
    //unordered_set<char> characters = { 'A', 'C', 'G', 'T', 'N', '$' };
    unordered_set<char> characters = { 'm', 'i', 's', 'p' };

public:
    Wavelet(string& str) {
        build(str);
    }

    void build(string& str) {

    }
    char access(int idx);
    int rank(char c, int idx);
    int select(char c, int idx);
    void print();
};

#endif //BIOINFORMATICS_WAVELET_H
