#ifndef BIOINFORMATICS_FMINDEX_HPP
#define BIOINFORMATICS_FMINDEX_HPP

#include <vector>
#include <sdsl/csa_bitcompressed.hpp>
#include "Wavelet.hpp"

class FMIndex {
private:
    // SA needed for locate function
    vector<int> suffix_array;
    // wavelet tree build upon BWT of original text
    Wavelet wavelet_tree;

public:
    // Constructor for FM index
    // params: input -- text upon which the FM index will be built 
    explicit FMIndex(const string &input);

    // Match helper function using FM index
    // params: pattern -- pattern used for querying the FM index
    // returns: upper and lower bounds of the SA interval (bottom exclusive)
    pair<int, int> match(const string &pattern) const;

    // Count function using FM index
    // params: pattern -- pattern used for querying the FM index
    // returns: number of occurences of the pattern in the original text
    int count(const string &pattern) const;

    // Locate function using FM index
    // params: pattern -- pattern used for querying the FM index
    // returns: vector of sorted offsets of the pattern occurences in the original text
    vector<int> locate(const string &pattern) const;

    // Function for printing the SA of the FM index
    void print_suffix_array() const;

    // Function for printing the sorted offsets of the pattern occurences in the original text
    // params: pattern -- pattern used for querying the FM index
    void print_pattern_offsets(const string &pattern) const;
};

#endif
