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
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    bit_vector* vector;
    rank_support_v<0,1> rank0;
    rank_support_v<1,1> rank1;
    select_support_mcl<0,1> select0;
    select_support_mcl<1,1> select1;
    char chr;

    explicit Node(string& str) {
        vector = new bit_vector(str.length(), 0);
        rank0 = rank_support_v<0,1>(vector);
        rank1 = rank_support_v<1,1>(vector);
        select0 = select_support_mcl<0,1>(vector);
        select1 = select_support_mcl<1,1>(vector);
        chr = 0;
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
    unordered_map<char, int> char_map;
    unordered_map<char, string> labels;
    unordered_set<char> alphabet;

    void build_impl(Node& root,
                    string& str,
                    unordered_set<char>& alphas,
                    const string& label = "") {
        auto lr = get_alphabets(alphas);
        unordered_set<char> left = get<0>(lr);
        unordered_set<char> right = get<1>(lr);

        if (left.size() == 1) {
            if (right.size() == 1) {
                Node right_child(*right.begin());
                root.right = &right_child;
            }
            Node left_child(*left.begin());
            root.right = &left_child;
            return;
        }

        bit_vector* v = root.vector;
        for (int i = 0; i < v->size(); ++i) {
            if (right.contains(str[i])) {
                (*v)[i] = true;
            }
        }

        lr = get_alphabets(left);
        left = get<0>(lr);
        right = get<1>(lr);
        string new_left_str;
        string new_right_str;
        for (const auto &item: str) {
            if (left.contains(item)) {
                new_left_str += item;
            } else {
                new_right_str += item;
            }
        }

        Node new_left_node(str);
        new_left_node.parent = &root;
        root.left = &new_left_node;
        build_impl(new_left_node, new_left_str, left, label + '0');

        Node new_right_node(str);
        new_right_node.parent = &root;
        root.right = &new_right_node;
        build_impl(new_right_node, new_right_str, right, label + '1');
    }

    static tuple<unordered_set<char>,unordered_set<char>> get_alphabets(unordered_set<char>& alphas) {
        unordered_set<char> left;
        unordered_set<char> right;
        bool isLeft = true;
        for (const auto &item: alphas) {
            if(isLeft) {
                left.insert(item);
            }
            else {
                right.insert(item);
            }
            isLeft = !isLeft;
        }
        return tuple<unordered_set<char>,unordered_set<char>>{left, right};
    }

public:
    Wavelet(string& str) {
        start = new Node(str);
        build(str);
    }

    void build(string& str) {
        for (const auto &item: str) {
            alphabet.insert(item);
            if(char_map.find(item) == char_map.end()) {
                char_map.insert({item, 0});
            }
            char_map[item] += 1;
        }

        assert(alphabet.size() >= 2);
        build_impl(*start, str, alphabet);
    }

    char access(int idx);
    int rank(char c, int idx);
    int select(char c, int idx);
    void print();
};

#endif //BIOINFORMATICS_WAVELET_H
