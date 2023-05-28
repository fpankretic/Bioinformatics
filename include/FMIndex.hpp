#ifndef BIOINFORMATICS_FMINDEX_HPP
#define BIOINFORMATICS_FMINDEX_HPP

#include <vector>
#include <sdsl/csa_bitcompressed.hpp>
#include "Wavelet.hpp"

class FMIndex {
private:
    vector<int> suffix_array;
    Wavelet wavelet_tree;

public:
    explicit FMIndex(const string& input);
    pair<int, int> match(const string& pattern);
    int count(const string& pattern);
    vector<int> locate(const string& pattern);
    void print_suffix_array();
    void print_pattern_offsets(const string& pattern);
};

#endif
