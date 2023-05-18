//
// Created by fpankretic on 4/30/23.
//

#ifndef BIOINFORMATICS_WAVELET_HPP
#define BIOINFORMATICS_WAVELET_HPP

#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include <queue>

using namespace std;
using namespace sdsl;

struct Node {
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    bit_vector* vector;
    rank_support_v<0,1> rank0;
    rank_support_v<1,1> rank1;
    select_support_mcl<0,1> select0;
    select_support_mcl<1,1> select1;
    char chr = 0;

    explicit Node(const string& str) {
        vector = new bit_vector(str.length(), 0);
        rank0 = rank_support_v<0,1>(vector);
        rank1 = rank_support_v<1,1>(vector);
        select0 = select_support_mcl<0,1>();
        select1 = select_support_mcl<1,1>();
    }

    explicit Node(char c) {
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        vector = nullptr;
        chr = c;
    }
};

class Wavelet {
private:
    Node* start;
    map<char, int> char_map;
    unordered_map<char, string> labels;

private:
    void build_impl(Node* root, const string& str, vector<char>& alphas, const string& label = "");
    static tuple<vector<char>,vector<char>> get_alphabets(vector<char>& alphas) {
        vector<char> left;
        vector<char> right;

        for (int i = 0; i < alphas.size() / 2; ++i) {
            left.push_back(alphas[i]);
        }
        for (int i = (int) alphas.size() / 2; i < alphas.size(); ++i) {
            right.push_back(alphas[i]);
        }

        return tuple<vector<char>, vector<char>>{left, right};
    }

public:
    explicit Wavelet(const string& str);
    Node* get_start();
    map<char, int> get_char_map();
    void build(const string& str);
    char access(unsigned long i);
    int rank(char x, int i);
    void print();
};

#endif //BIOINFORMATICS_WAVELET_HPP
