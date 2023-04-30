//
// Created by fpankretic on 4/30/23.
//

#ifndef BIOINFORMATICS_WAVELET_H
#define BIOINFORMATICS_WAVELET_H

#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include <string>
#include <unordered_set>
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

    char access(int i) {
        Node* n = start;

        while(!n->chr) {
            bool b = n->vector[i];

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

    void print() {
        queue<pair<Node*, int>> open;
        open.push(make_pair(start, 0));

        while (!open.empty()) {
            pair<Node*, int> curr = open.front();
            open.pop();

            Node* curr_node = curr.first;
            int curr_depth = curr.second;

            open.push(make_pair(curr_node->left, curr_depth + 1));
            open.push(make_pair(curr_node->right, curr_depth + 1));

            string dpth = "";
            for (int i = 0; i < curr_depth; ++i) {
                dpth += "-";
            }

            if (curr_node->vector) {
                cout << dpth << curr_node->vector << endl;
            } else {
                cout << dpth << curr_node->chr << endl;
            }
        }
    }
};

#endif //BIOINFORMATICS_WAVELET_H
