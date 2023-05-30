#include <unordered_set>

#include "../include/Wavelet.hpp"

Wavelet::Wavelet(const string &str) {
    unordered_set<char> alphabet;
    for (const auto &item: str) {
        alphabet.insert(item);
        if (char_counts.find(item) == char_counts.end()) {
            char_counts.insert({item, 0});
        }
        char_counts[item] += 1;
    }

    if (alphabet.size() < 2) {
        throw invalid_argument("There should be at least 2 different characters.");
    }

    int prev_cnt = 0;
    for (const auto &item: char_counts) {
        int temp = item.second + prev_cnt;
        char_offsets[item.first] = prev_cnt;
        prev_cnt = temp;
    }

    text_len = (int) str.size();

    vector<char> alphas(alphabet.begin(), alphabet.end());
    sort(alphas.begin(), alphas.end());
    start = build_tree(str, alphas);
}

shared_ptr<Node> Wavelet::build_tree(const string &str, const vector<char> &alphas, const string &label) {
    if (alphas.size() == 1) return make_shared<Node>(alphas[0]);
    if (alphas.empty()) return nullptr;

    auto [left, right] = get_alphabets(alphas);
    auto node = make_shared<Node>(str, right);

    string new_left_str;
    string new_right_str;
    unordered_set<char> right_character_set(right.begin(), right.end());
    for (const auto &item: str) {
        if (right_character_set.contains(item)) {
            new_right_str += item;
        } else {
            new_left_str += item;
        }
    }

    node->left = build_tree(new_left_str, left, label + "0");
    if (node->left != nullptr) node->left->parent = node;
    if (node->left != nullptr && node->left->chr) labels.insert({node->left->chr, label + '0'});

    node->right = build_tree(new_right_str, right, label + "1");
    if (node->right != nullptr) node->right->parent = node;
    if (node->left != nullptr && node->left->chr) labels.insert({node->right->chr, label + '1'});

    return node;
}

shared_ptr<Node> Wavelet::get_start() const {
    return start;
}

int Wavelet::get_text_len() const {
    return text_len;
}

int Wavelet::get_char_offset(char c) const {
    return char_offsets.at(c);
}

char Wavelet::access(unsigned long index) const {
    shared_ptr<Node> node = start;

    if (index > node->b_vector.size()) {
        throw invalid_argument("Given input is invalid.");
    }

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

int Wavelet::rank(char character, int index) const {
    auto node = start;
    int k = 0;

    if (index > node->b_vector.size() || labels.find(character) == labels.end()) {
        throw invalid_argument("Given input is invalid.");
    }

    while (node->chr == 0) {
        char b = labels.at(character)[k];
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

int Wavelet::select(const char character, int index) const {
    auto node = start;
    int k = 0;

    if (index > node->b_vector.size() || labels.find(character) == labels.end()) {
        throw invalid_argument("Given input is invalid.");
    }

    if(char_counts.at(character) <= index) {
        return text_len - 1;
    }

    while (node->chr == 0) {
        char b = labels.at(character).at(k);

        if (b == '0') {
            node = node->left;
        } else {
            node = node->right;
        }

        k++;
    }

    k = (int) labels.at(character).length() - 1;

    while (node->parent.lock()) {
        node = node->parent.lock();
        char b = labels.at(character)[k];

        if (b == '0') {
            index = (int) node->select0(index + 1);
        } else {
            index = (int) node->select1(index + 1);
        }

        k--;
    }

    return index;
}

void Wavelet::print() const {
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

int Wavelet::lf_mapping(char c, int index) const {
    return get_char_offset(c) + rank(c, index);
}
