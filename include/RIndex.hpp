#ifndef BIOINFORMATICS_RINDEX_HPP
#define BIOINFORMATICS_RINDEX_HPP

#include <vector>
#include <sdsl/csa_bitcompressed.hpp>
#include "Wavelet.hpp"

class RIndex {
private:
    // wavelet tree build upon BWT of original text
    Wavelet wavelet_tree;
    // map containing sampled offsets in BWT and original text for each character
    map<char, map<int, int>> run_offsets;
    // vector containing locations of left neighbours in suffix array of phrase starts
    vector<int> neighbours;
    // bit vector for marking the starts of phrases in the original text
    bit_vector phrase_starts;
    // rank support for phrase_starts bit vector and pattern '1'
    rank_support_v<1, 1> phrase_rank1;

public:
    // Constructor for R index
    // params: input -- text upon which the R index will be built 
    explicit RIndex(const string &input);

    // Constructor of helper structs needed for locate function
    // params: str -- original text upon which the R index is built
    //         sa -- suffix array of original text
    void build_locate_structs(const string &str, const vector<int> &sa);

    // Function for finding the offset of previous occurence for given offset of some pattern
    // params: k -- given offset of some pattern
    // returns: offset of previous occurence of the same pattern
    int find_neighbours_offset(int k) const;

    // Function for finding the offset in BWT and in the original text of character in BWT preceding lexicographically largest suffix (search limited by given offset)
    // params: c -- character in BWT
    //         offset -- offset limiting the search area in BWT
    pair<int, int> pred(char c, int offset) const;

    // Match helper function using R index
    // params: pattern -- pattern used for querying the R index
    // returns: upper and lower bounds of the SA interval (bottom exclusive), offset of some occurance of the pattern in BWT and in the original text
    tuple<int, int, int, int> match(const string &pattern) const;

    // Count function using R index
    // params: pattern -- pattern used for querying the R index
    // returns: number of occurences of the pattern in the original text
    int count(const string &pattern) const;

    // Locate function using R index
    // params: pattern -- pattern used for querying the R index
    // returns: vector of sorted offsets of the pattern occurences in the original text
    vector<int> locate(const string &pattern) const;

    // Function for printing the sorted offsets of the pattern occurences in the original text
    // params: pattern -- pattern used for querying the R index
    void print_pattern_offsets(const string &pattern) const;
};

#endif
