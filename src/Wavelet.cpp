#include <unordered_set>

#include "../include/Wavelet.hpp"

Wavelet::Wavelet(const string &str) {
    start = make_shared<Node>();
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

void Wavelet::build_impl(const shared_ptr<Node>& root, const string &str, vector<char> &alphas, const string &label) {
    auto [left, right] = get_alphabets(alphas);

    unordered_set<char> right_character_set(right.begin(), right.end());
    bit_vector b_vector(str.size(), 0);
    for (int i = 0; i < str.size(); ++i) {
        if (right_character_set.contains(str[i])) {
            b_vector[i] = true;
        }
    }
    root->construct_vector(b_vector);

    // we finish if we are here
    char c;
    if (right.size() == 1) {
        assert(left.size() <= 1);
        if (left.size() == 1) {
            c = left[0];
            auto left_child = make_shared<Node>(c);
            left_child->parent = root;
            root->left = left_child;
            labels.insert({c, label + '0'});
        }
        c = right[0];
        auto right_child = make_shared<Node>(c);
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

    shared_ptr<Node> new_left_node = make_shared<Node>();
    new_left_node->parent = root;
    root->left = new_left_node;
    build_impl(new_left_node, new_left_str, left, label + '0');

    shared_ptr<Node> new_right_node = make_shared<Node>();;
    new_right_node->parent = root;
    root->right = new_right_node;
    build_impl(new_right_node, new_right_str, right, label + '1');
}

shared_ptr<Node> Wavelet::get_start() {
    return start;
}

map<char, int> Wavelet::get_char_map() {
    return char_map;
}

char Wavelet::access(unsigned long index) {
    shared_ptr<Node> node = start;

    while (node->chr == 0) {
        bool bit = (node->b_vector)[index];

        if (!bit) {
            index = (int) node->rank0(index);
            node = node->left;
        } else {
            index = (int) node->rank1(index);
            node = node->right;
        }
    }

    return node->chr;
}

int Wavelet::rank(const char character, int index) {
    auto node = start;
    int k = 0;

    if (index > node->b_vector.size() || labels.find(character) == labels.end() ) {
        throw invalid_argument("Given input is invalid.");
    }

    while (node->chr == 0) {
        char b = labels[character][k];
        if (b == '0') {
            index = (int) node->rank0(index);
            node = node->left;
        } else {
            index = (int) node->rank1(index);
            node = node->right;
        }

        k++;
    }

    return index;
}

void Wavelet::print() {
    queue<pair<shared_ptr<Node>, int>> open;
    open.emplace(start, 0);

    while (!open.empty()) {
        pair<shared_ptr<Node>, int> curr = open.front();
        open.pop();

        shared_ptr<Node> curr_node = curr.first;
        int curr_depth = curr.second;

        if (curr_node->left != nullptr)
            open.emplace(curr_node->left, curr_depth + 1);
        if (curr_node->right != nullptr)
            open.emplace(curr_node->right, curr_depth + 1);

        string dpth;
        for (int i = 0; i < curr_depth; ++i) {
            dpth += "-";
        }

        if (!curr_node->b_vector.empty()) {
            cout << dpth << curr_node->b_vector << endl;
        } else {
            cout << dpth << curr_node->chr << endl;
        }
    }
}