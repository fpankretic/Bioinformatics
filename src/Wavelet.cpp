//
// Created by fpankretic on 5/1/23.
//

#include <unordered_set>
#include <exception>

#include "../include/Wavelet.hpp"

Wavelet::Wavelet(const string &str) {
    start = new Node(str);
    build(str);
}

void Wavelet::build(const string &str) {
    unordered_set<char> alphabet;
    for (const auto &item: str) {
        alphabet.insert(item);
        if (char_map.find(item) == char_map.end()) {
            char_map.insert({item, 0});
        }
        char_map[item] += 1;
    }

    if (alphabet.size() < 2) {
        throw invalid_argument("There should be at least 2 different characters.");
    }

    int prev_cnt = 0;
    for (const auto &item: char_map) {
        int temp = item.second + prev_cnt;
        char_map[item.first] = prev_cnt;
        prev_cnt = temp;
    }

    vector<char> alphas(alphabet.begin(), alphabet.end());
    sort(alphas.begin(), alphas.end());
    build_impl(start, str, alphas);
}

void Wavelet::build_impl(Node *root, const string &str, vector<char> &alphas, const string &label) {
    auto [left, right] = get_alphabets(alphas);

    unordered_set<char> right_character_set(right.begin(), right.end());
    bit_vector *bit_vector = root->vector;
    for (int i = 0; i < str.size(); ++i) {
        if (right_character_set.contains(str[i])) {
            (*bit_vector)[i] = true;
        }
    }

    root->rank0 = rank_support_v<0,1>(bit_vector);
    root->rank1 = rank_support_v<1,1>(bit_vector);
    root->select0 = select_support_mcl<0,1>();
    root->select1 = select_support_mcl<1,1>();

    // TODO: node.initialize();
    // TODO: try to return newly created node

    // we finish if we are here
    char c;
    if (right.size() == 1) {
        assert(left.size() <= 1);
        if (left.size() == 1) {
            c = left[0];
            Node *left_child = new Node(c);
            left_child->parent = root;
            root->left = left_child;
            labels.insert({c, label + '0'});
        }
        c = right[0];
        Node *right_child = new Node(c);
        right_child->parent = root;
        root->right = right_child;
        labels.insert({c, label + '1'});
        return;
    }

    string new_left_str;
    string new_right_str;
    for (const auto &item: str) {
        if (right_character_set.contains(item)) {
            new_right_str += item;
        } else {
            new_left_str += item;
        }
    }

    Node *new_left_node = new Node(new_left_str);
    new_left_node->parent = root;
    root->left = new_left_node;
    build_impl(new_left_node, new_left_str, left, label + '0');

    Node *new_right_node = new Node(new_right_str);
    new_right_node->parent = root;
    root->right = new_right_node;
    build_impl(new_right_node, new_right_str, right, label + '1');
}

Node* Wavelet::get_start() {
    return start;
}

map<char, int> Wavelet::get_char_map() {
    return char_map;
}

char Wavelet::access(unsigned long i) {
    Node *n = start;

    while (n->chr == 0) {
        bool b = (*n->vector)[i];

        if (!b) {
            i = (int) n->rank0(i);
            n = n->left;
        } else {
            i = (int) n->rank1(i);
            n = n->right;
        }
    }

    return n->chr;
}

int Wavelet::rank(const char x, int i) {
    Node *n = start;
    int k = 0;

    if (i > n->vector->size() || labels.find(x) == labels.end() ) {
        throw invalid_argument("Given input is invalid.");
    }

    while (n->chr == 0) {
        char b = labels[x][k];
        if (b == '0') {
            i = (int) n->rank0(i);
            n = n->left;
        } else {
            i = (int) n->rank1(i);
            n = n->right;
        }

        k = k + 1;
    }

    return i;
}

void Wavelet::print() {
    queue<pair<Node *, int>> open;
    open.emplace(start, 0);

    while (!open.empty()) {
        pair<Node *, int> curr = open.front();
        open.pop();

        Node *curr_node = curr.first;
        int curr_depth = curr.second;

        if (curr_node->left != nullptr)
            open.emplace(curr_node->left, curr_depth + 1);
        if (curr_node->right != nullptr)
            open.emplace(curr_node->right, curr_depth + 1);

        string dpth;
        for (int i = 0; i < curr_depth; ++i) {
            dpth += "-";
        }

        if (curr_node->vector != nullptr) {
            cout << dpth << *(curr_node->vector) << endl;
        } else {
            cout << dpth << curr_node->chr << endl;
        }
    }
}