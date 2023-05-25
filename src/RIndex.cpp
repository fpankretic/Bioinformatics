#include "../include/RIndex.hpp"

RIndex::RIndex(const string &input) {
    csa_bitcompressed<> csa; construct_im(csa, input, 1);
    string bwt;
    for (int i = 0; i < csa.size(); ++i) {
        suffix_array.push_back((int) csa[i]);
        char c = (char) csa.bwt[i];

        if (c) {
            bwt += c;
        } else {
            bwt += '$';
        }
    }

    text_len = bwt.size();

    char curr = '\0';
    int run_start = -1;

    for (int i = 0; i < bwt.size(); ++i) {
        if (bwt[i] != curr) {
            if (run_start != -1) {
                if (!predecessor_struct.contains(bwt[i-1])) {
                    predecessor_struct.insert({bwt[i-1], map<int,int>()});
                }
                predecessor_struct[bwt[i-1]].insert({i - 1, suffix_array[i - 1] - 1});
            }
            run_start = i;
            curr = bwt[i];

            if (!predecessor_struct.contains(bwt[run_start])) {
                predecessor_struct.insert({bwt[run_start], map<int,int>()});
            }
            predecessor_struct[bwt[run_start]].insert({run_start, suffix_array[run_start] - 1});
        }
    }

    int bwt_size = bwt.size() - 1;
    if (!predecessor_struct.contains(bwt[bwt_size])) {
        predecessor_struct.insert({bwt[bwt_size], map<int,int>()});
    }
    predecessor_struct[bwt[bwt_size]].insert({bwt_size, suffix_array[bwt_size] - 1});

    int dlr_off = get<0>(*(predecessor_struct['$'].begin()));
    predecessor_struct['$'].clear();
    predecessor_struct['$'].insert({dlr_off, bwt_size});

    wavelet_tree = Wavelet(bwt);
}

map<char, map<int, int>> RIndex::get_predecessor_struct() {
    return predecessor_struct;
}

int RIndex::pred(char c, int offset) {
    map<int, int> curr_char_map = predecessor_struct[c];

    for (auto it = curr_char_map.rbegin(); it != curr_char_map.rend(); it++) {
        if (it->first <= offset) {
            return it->second;
        }
    }

    return -1;
}

pair<int, int> RIndex::match(const string& pattern) {
    if (pattern.empty()) {
        throw invalid_argument("Pattern must be at least one character.");
    }

    int top = 0;
    int bottom = (int) wavelet_tree.get_start()->b_vector.size();

    int i = (int) pattern.length() - 1;

    int text_offset;
    int bwt_offset;
    bool first = true;

    while (i >= 0 && bottom > top) {
        char c = pattern[i];
        if (first) {
            text_offset = pred(c, text_len - 1);
            bwt_offset = text_len - 1;
            first = false;
        }
        top = wavelet_tree.get_char_map()[c] + wavelet_tree.rank(c, top);
        bottom = wavelet_tree.get_char_map()[c] + wavelet_tree.rank(c, bottom);

        i = i - 1;
    }

    return {top, bottom};
}

int RIndex::count(const string& pattern) {
    auto [top, bottom] = match(pattern);

    if (top >= bottom) {
        return 0;
    }

    return bottom - top;
}

vector<int> RIndex::locate(const string& pattern) {
    auto [top, bottom] = match(pattern);

    vector<int> offsets;

    if (top < bottom) {
        for (int i = top; i < bottom; ++i) {
            offsets.push_back(suffix_array[i]);
        }
    }

    sort(offsets.begin(), offsets.end());
    return offsets;
}

void RIndex::print_suffix_array() {
    cout << "Suffix array: ";
    for (int i: suffix_array) {
        cout << i << " ";
    }
    cout << endl;
}

void RIndex::print_pattern_offsets(const string& pattern) {
    vector<int> occs = locate(pattern);
    if (!occs.empty()) {
        cout << endl << "Offsets of pattern in text: " << endl;
        for (int offset: occs) {
            cout << "Offset: " << offset << endl;
        }
    }
}