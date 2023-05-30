#ifndef BIOINFORMATICS_RINDEX_HPP
#define BIOINFORMATICS_RINDEX_HPP

#include <vector>
#include <sdsl/csa_bitcompressed.hpp>
#include "Wavelet.hpp"

class RIndex {
private:
    Wavelet wavelet_tree;
    map<char, map<int, int>> run_offsets;
    vector<int> neighbours;
    bit_vector phrase_starts;
    rank_support_v<1, 1> phrase_rank1;

public:
    explicit RIndex(const string &input);

    void build_locate_structs(const string &str, const vector<int> &sa);

    int find_neighbours_offset(int k) const;

    pair<int, int> pred(char c, int offset) const;

    tuple<int, int, int, int> match(const string &pattern) const;

    int count(const string &pattern) const;

    vector<int> locate(const string &pattern) const;

    void print_pattern_offsets(const string &pattern) const;
};

#endif
