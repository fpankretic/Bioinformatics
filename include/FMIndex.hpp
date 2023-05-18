//
// Created by fpankretic on 5/1/23.
//

#ifndef BIOINFORMATICS_FMINDEX_HPP
#define BIOINFORMATICS_FMINDEX_HPP

#include <vector>
#include <sdsl/csa_bitcompressed.hpp>
#include "Wavelet.hpp"

class FMIndex {
private:
    vector<int> suffix_array;
    Wavelet* wavelet_tree;

public:
    explicit FMIndex(const string& input);
    pair<int, int> match(const string& p);
    int count(const string& p);
    vector<int> locate(const string& p);
    void print_suffix_array();
    void print_pattern_offsets(string& pattern);
};

#endif //BIOINFORMATICS_FMINDEX_HPP
