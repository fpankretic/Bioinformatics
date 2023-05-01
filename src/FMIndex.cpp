//
// Created by fpankretic on 5/1/23.
//

#include "../include/FMIndex.hpp"

FMIndex::FMIndex(const std::string &input) {
    construct_im(csa, input, 1);
    bwt = "";
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

int FMIndex::count(string &pattern) {
    return wavelet_tree->count(pattern);
}

vector<int>* FMIndex::locate(string &pattern) {
    return wavelet_tree->locate(pattern, suffix_array);
}

void FMIndex::print_suffix_array() {
    cout << "Suffix array: ";
    for (int i: suffix_array) {
        cout << i << " ";
    }
    cout << endl;
}

void FMIndex::print_bwt() {
    cout << "BWT: ";
    for (char c: bwt) {
        cout << c << " ";
    }
    cout << endl;
}

void FMIndex::print_pattern_offsets(string& pattern) {
    vector<int>* occs = locate(pattern);
    if (!occs->empty()) {
        cout << endl << "Offsets of pattern in text: " << endl;
        for (int offset: *occs) {
            cout << "Offset: " << offset << endl;
        }
        cout << endl;
    }
}

