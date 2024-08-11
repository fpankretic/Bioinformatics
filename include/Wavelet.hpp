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
    // pointer to parent node
    weak_ptr<Node> parent;
    // pointer to left child node
    shared_ptr<Node> left = nullptr;
    // pointer to right child node
    shared_ptr<Node> right = nullptr;

    // bit vector representing the current string
    bit_vector b_vector;
    // rank support for current string bit vector and pattern '0'
    rank_support_v<0, 1> rank0;
    // rank support for current string bit vector and pattern '1'
    rank_support_v<1, 1> rank1;
    // select support for current string bit vector and pattern '0'
    select_support_mcl<0, 1> select0;
    // select support for current string bit vector and pattern '1'
    select_support_mcl<1, 1> select1;

    // character of leaf node
    char chr = 0;

    // Constructor for Node
    // params: str --  current string for which the node bit vector will be built
    //         right -- characters which will be represented with a 1 in the bit vector
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

    // Constructor for Node (Leaf)
    // params: c -- character of leaf node
    explicit Node(char c) {
        chr = c;
    }
};

class Wavelet {
private:
    // pointer to root node
    shared_ptr<Node> start;
    // map containing offsets in SA for each unique character
    map<char, int> char_offsets;
    // map containing counts of each unique character
    map<char, int> char_counts;
    // map containing labels for each unique character (e.g. '00')
    unordered_map<char, string> labels;
    // total length of text
    int text_len = 0;

private:
    // Function for recursive building of the tree
    // params: str -- current string for which the node bit vector will be built
    //         alphas -- sorted characters contained in string
    //         label -- current node level label 
    // returns: pointer to built node
    shared_ptr<Node> build_tree(const string &str, const vector<char> &alphas, const string &label = "");

    // Function for splitting the current alphabet into two sets (split roughly into halves)
    // params: alphas -- sorted characters contained in some string
    // returns: first set of characters, second set of characters
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
    // Default constructor for wavelet tree
    explicit Wavelet() = default;

    // Constructor for wavelet tree
    // params: str -- string upon which the wavelet tree will be built
    explicit Wavelet(const string &str);

    // Getter for root node
    // returns: pointer to root node
    shared_ptr<Node> get_start() const;

    // Getter for text length of the string upon which the wavelet tree was built
    // returns: text length
    int get_text_len() const;

    // Getter for char offset in the SA for some character
    // params: c -- character for which the offset is needed
    // returns: offset of the character c in the SA
    int get_char_offset(char c) const;

    // Function for calculating the offset in BWT after applying LF mapping
    // params: c -- character for which LF mapping is used
    //         index -- current offset in BWT (before applying LF mapping)
    // returns: offset of character after applying LF mapping
    int lf_mapping(char c, int index) const;

    // Function for finding the character at some index 
    // params: index -- offset in original string
    // returns: character at given index in original string
    char access(unsigned long index) const;

    // Function for calculating the rank of a given character at some index
    // params: character -- character for which rank is calculated
    //         index -- offset of character for which rank is calculated
    // returns: calculated rank operation result
    int rank(char character, int index) const;

    // Function for calculating the select operation of a given character and given rank
    // params: character -- character for which select is calculated
    //         index -- rank of character for which select is calculated
    // returns: calculated select operation result
    int select(char character, int index) const;

    // Function for printing out the wavelet tree structure
    void print() const;
};

#endif
