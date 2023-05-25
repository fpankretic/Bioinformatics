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

    cout << bwt << endl;

    char curr = '\0';
    int run_start = -1;

    for (int i = 0; i < bwt.size(); ++i) {
        if (bwt[i] != curr) {
            if (run_start != -1) {
                if (bwt[i - 1] == '$') {
                    predecessor_struct.insert({i - 1, bwt.size() - 1});
                } else {
                    predecessor_struct.insert({i - 1, suffix_array[i - 1] - 1});
                }
            }
            run_start = i;
            curr = bwt[i];

            if (bwt[run_start] == '$') {
                predecessor_struct.insert({run_start, bwt.size() - 1});
            } else {
                predecessor_struct.insert({run_start, suffix_array[run_start] - 1});
            }
        }
    }
    
    if (bwt[bwt.size() - 1] == '$') {
        predecessor_struct.insert({bwt.size() - 1, bwt.size() - 1});
    } else {
        predecessor_struct.insert({bwt.size() - 1, suffix_array[bwt.size() - 1] - 1});
    }

    wavelet_tree = Wavelet(bwt);
}

map<int, int> RIndex::get_predecessor_struct() {
    return predecessor_struct;
}

pair<int, int> RIndex::match(const string& pattern) {
    if (pattern.empty()) {
        throw invalid_argument("Pattern must be at least one character.");
    }

    int top = 0;
    int bottom = (int) wavelet_tree.get_start()->b_vector.size();

    int i = (int) pattern.length() - 1;

    while (i >= 0 && bottom > top) {
        char c = pattern[i];
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