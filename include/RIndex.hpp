#ifndef BIOINFORMATICS_RINDEX_HPP
#define BIOINFORMATICS_RINDEX_HPP

#include <vector>
#include <sdsl/csa_bitcompressed.hpp>
#include "Wavelet.hpp"

class RIndex {
private:
    map<char, map<int, int>> run_offsets;
    Wavelet wavelet_tree;
    bit_vector phrase_starts;
    rank_support_v<1,1> phrase_rank1;
    vector<int> neighbours;

public:
    explicit RIndex(const string &input);

    map<char, map<int, int>> get_predecessor_struct();

    void build_locate_structs(const string &str, vector<int> &sa);

    int find_neighbours_offset(int);

    pair<int, int> pred(char c, int offset);

    tuple<int, int, int, int> match(const string &pattern);

    int count(const string &pattern);

    vector<int> locate(const string &pattern);

    void print_pattern_offsets(const string &pattern);
};

#endif
