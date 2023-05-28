#ifndef BIOINFORMATICS_WAVELET_HPP
#define BIOINFORMATICS_WAVELET_HPP

#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include <queue>
#include <utility>

using namespace std;
using namespace sdsl;

struct Node {
    shared_ptr<Node> parent = nullptr;
    shared_ptr<Node> left = nullptr;
    shared_ptr<Node> right = nullptr;

    bit_vector b_vector;
    rank_support_v<0,1> rank0;
    rank_support_v<1,1> rank1;
    select_support_mcl<0,1> select0;
    select_support_mcl<1,1> select1;

    char chr = 0;

    explicit Node() = default;

    explicit Node(char c) {
        chr = c;
    }

    void construct_vector(bit_vector vec) {
        this->b_vector = std::move(vec);
        this->rank0 = rank_support_v<0,1>(&(this->b_vector));
        this->rank1 = rank_support_v<1,1>(&(this->b_vector));
        this->select0 = select_support_mcl<0,1>(&(this->b_vector));
        this->select1 = select_support_mcl<1,1>(&(this->b_vector));
    }
};

class Wavelet {
private:
    shared_ptr<Node> start;
    map<char, int> char_map;
    unordered_map<char, string> labels;

private:
    void build_impl(const shared_ptr<Node>& root, const string& str, vector<char>& alphas, const string& label = "");
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
    shared_ptr<Node> get_start();
    map<char, int> get_char_map();
    void build(const string& str);
    char access(unsigned long index);
    int rank(char character, int index);
    int select(char character, int index);
    void print();
};

#endif
