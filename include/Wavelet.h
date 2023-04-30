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
    bit_vector vector;
    rank_support_v<0,1> rank0;
    rank_support_v<1,1> rank1;
    select_support_mcl<0,1> select0;
    select_support_mcl<1,1> select1;
    char chr;
};

class Wavelet {
private:
    Node* start;
    unordered_map<char, int> char_map;
    //unordered_set<char> characters = { 'A', 'C', 'G', 'T', 'N', '$' };
    unordered_set<char> characters;

public:
    Wavelet(string& str) {
        build(str);
    }

    void build(string& str) {
        unordered_set<char> chars;
    }

    char access(int i) {
        Node* n = start;

        while(!n->chr) {
            bool b = n.vector[i];

            if (!b) {
                i = n.rank0(i);
                n = n->left;
            } else {
                i = n.rank1(i);
                n = n->right;
            }
        }

        return n->chr;
    }

    int rank(char x, int i) {
        Node* n = start;
        int k = 0;

        while (!n->chr) {
            char b = labels[x][k];

            if (b == '0') {
                i = n.rank0(i);
                n = n.left;
            } else {
                i = n.rank1(i);
                n = n.right;
            }

            k = k + 1;
        }

        return i;
    }

    int select(char x, int i) {
        Node* n = start;
        int k = 0;

        while (!n->chr) {
            char b = labels[x][k];

            if (b == '0') {
                n = n.left;
            } else {
                n = n.right;
            }

            k = k + 1;
        }

        k = labels[c].length() - 1;

        while (n->parent) {
            n = n->parent;
            char b = labels[x][k];

            if (b == '0') {
                i = n.select0(i);
            } else {
                i = n.select1(i);
            }

            k = k - 1;
        }

        return i;
    }
    
    void print();
};

#endif //BIOINFORMATICS_WAVELET_H
