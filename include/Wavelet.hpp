#ifndef BIOINFORMATICS_WAVELET_HPP
#define BIOINFORMATICS_WAVELET_HPP

#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include <queue>
#include <utility>
#include <unordered_set>

using namespace std;
using namespace sdsl;

struct Node {
    weak_ptr<Node> parent;
    shared_ptr<Node> left = nullptr;
    shared_ptr<Node> right = nullptr;

    bit_vector b_vector;
    rank_support_v<0, 1> rank0;
    rank_support_v<1, 1> rank1;
    select_support_mcl<0, 1> select0;
    select_support_mcl<1, 1> select1;

    char chr = 0;

    Node(const string &str, const vector<char> &right) {
        unordered_set<char> right_character_set(right.begin(), right.end());
        b_vector = bit_vector(str.size(), 0);
        for (int i = 0; i < str.size(); ++i) {
            if (right_character_set.contains(str[i])) {
                b_vector[i] = true;
            }
        }
        this->rank0 = rank_support_v<0, 1>(&(this->b_vector));
        this->rank1 = rank_support_v<1, 1>(&(this->b_vector));
        this->select0 = select_support_mcl<0, 1>(&(this->b_vector));
        this->select1 = select_support_mcl<1, 1>(&(this->b_vector));
    };

    explicit Node(char c) {
        chr = c;
    }
};

class Wavelet {
private:
    shared_ptr<Node> start;
    map<char, int> char_offsets;
    map<char, int> char_counts;
    unordered_map<char, string> labels;
    int text_len = 0;

private:
    shared_ptr<Node> build_tree(const string &str, const vector<char> &alphas, const string &label = "");

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
    explicit Wavelet() = default;

    explicit Wavelet(const string &str);

    shared_ptr<Node> get_start() const;

    int get_text_len() const;

    int get_char_offset(char c) const;

    int lf_mapping(char c, int index) const;

    char access(unsigned long index) const;

    int rank(char character, int index) const;

    int select(char character, int index) const;

    void print() const;
};

#endif
