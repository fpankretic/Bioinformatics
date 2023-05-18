//
// Created by fpankretic on 5/1/23.
//

#include "../include/FMIndex.hpp"

FMIndex::FMIndex(const std::string &input) {
    csa_bitcompressed<> csa; construct_im(csa, input, 1);
    string bwt = "";
    for (int i = 0; i < csa.size(); ++i) {
        suffix_array.push_back((int) csa[i]);
        char c = (char) csa.bwt[i];

        if (c) {
            bwt += c;
        } else {
            bwt += '$';
        }
    }
    wavelet_tree = new Wavelet(bwt);
}

pair<int, int> FMIndex::match(const string& pattern) {
    int top = 0;
    int bottom = (int) wavelet_tree->get_start()->vector->size();

    int i = (int) pattern.length() - 1;

    while (i >= 0 && bottom > top) {
        char c = pattern[i];
        top = wavelet_tree->get_char_map()[c] + wavelet_tree->rank(c, top);
        bottom = wavelet_tree->get_char_map()[c] + wavelet_tree->rank(c, bottom);

        i = i - 1;
    }

    return {top, bottom};
}

int FMIndex::count(const string& pattern) {
    auto [top, bottom] = match(pattern);

    if (top >= bottom) {
        return 0;
    }

    return bottom - top;
}

vector<int> FMIndex::locate(const string& pattern) {
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

void FMIndex::print_suffix_array() {
    cout << "Suffix array: ";
    for (int i: suffix_array) {
        cout << i << " ";
    }
    cout << endl;
}

void FMIndex::print_pattern_offsets(const string& pattern) {
    vector<int> occs = locate(pattern);
    if (!occs.empty()) {
        cout << endl << "Offsets of pattern in text: " << endl;
        for (int offset: occs) {
            cout << "Offset: " << offset << endl;
        }
    }
}

