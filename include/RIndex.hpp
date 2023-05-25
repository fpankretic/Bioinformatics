#ifndef BIOINFORMATICS_RINDEX_HPP
#define BIOINFORMATICS_RINDEX_HPP

#include <vector>
#include <sdsl/csa_bitcompressed.hpp>
#include "Wavelet.hpp"

class RIndex {
private:
    vector<int> suffix_array;
    map<char, map<int, int>> predecessor_struct;
    Wavelet wavelet_tree = Wavelet("abc");
    int text_len;

public:
    explicit RIndex(const string& input);
    map<char, map<int, int>> get_predecessor_struct();
    pair<int, int> pred(char c, int offset);
    tuple<int, int, int, int> match(const string& pattern);
    int count(const string& pattern);
    vector<int> locate(const string& pattern);
    void print_suffix_array();
    void print_pattern_offsets(const string& pattern);
};

#endif
