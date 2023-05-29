//
#ifndef BIOINFORMATICS_WAVELET_HPP
#define BIOINFORMATICS_WAVELET_HPP

#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include <queue>
#include <utility>

using namespace std;
using namespace sdsl;

struct Node {
    //Wow, I missed you switching to shared_ptrs!
    //EPIC!
    //You end up with cycles and leaked memory here.
    //parent should probably be weak_ptr!
    //Check that nodes actually get destroyed
    shared_ptr<Node> parent = nullptr;
    shared_ptr<Node> left = nullptr;
    shared_ptr<Node> right = nullptr;

    bit_vector b_vector;
    rank_support_v<0, 1> rank0;
    rank_support_v<1, 1> rank1;
    select_support_mcl<0, 1> select0;
    select_support_mcl<1, 1> select1;

    char chr = 0;

    explicit Node() = default;

    explicit Node(char c) {
        chr = c;
    }

    //since you are moving it, the argument here should be rvalue reference!
    //should probably be constructor of the node? see related comment in .cpp
    void construct_vector(bit_vector vec) {
        this->b_vector = std::move(vec);
        this->rank0 = rank_support_v<0, 1>(&(this->b_vector));
        this->rank1 = rank_support_v<1, 1>(&(this->b_vector));
        this->select0 = select_support_mcl<0, 1>(&(this->b_vector));
        this->select1 = select_support_mcl<1, 1>(&(this->b_vector));
    }
};

class Wavelet {
private:
    shared_ptr<Node> start;
    map<char, int> char_offsets;
    map<char, int> char_counts;
    unordered_map<char, string> labels;
    int text_len;

private:
    void build_impl(const shared_ptr<Node> &root, const string &str, vector<char> &alphas, const string &label = "");

    static tuple<vector<char>, vector<char>> get_alphabets(const vector<char> &alphas) {
        vector<char> left;
        vector<char> right;

        for (int i = 0; i < alphas.size() / 2; ++i) {
            left.push_back(alphas[i]);
        }
        for (int i = (int) alphas.size() / 2; i < alphas.size(); ++i) {
            right.push_back(alphas[i]);
        }

        return {left, right};
    }

public:
    //add const to methods that don't modify wavelet (e.g. most of them)
    explicit Wavelet() = default;

    explicit Wavelet(const string &str);

    shared_ptr<Node> get_start();

    int get_text_len() const;

    int get_char_offset(char c);

    int lf_mapping(char c, int index);

    void build(const string &str);

    char access(unsigned long index);

    int rank(char character, int index);

    int select(char character, int index);

    void print();
};

#endif
