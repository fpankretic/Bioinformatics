#include <iostream>
#include <unordered_set>
#include "include/Wavelet.hpp"
#include "include/FMIndex.hpp"
#include "include/RIndex.hpp"

using namespace std;

int main() {
    string input = "ipssm$pissii";
    Wavelet tree(input);

    cout << "Wavelet tree: " << endl;
    tree.print();
    cout << endl;

    cout << "BWT: ipssm$pissii" << endl;
    cout << tree.select('$', 1) << endl;
    cout << tree.select('$', 2) << endl;
    cout << tree.select('$', 3) << endl;
    cout << tree.select('$', 4) << endl;

    FMIndex fm_index(input);

    string pattern = "s";

    fm_index.print_suffix_array();
    fm_index.print_pattern_offsets(pattern);

    cout << input << endl;
    RIndex r_index(input);
    cout << r_index.count(pattern) << endl;


    r_index.print_pattern_offsets(pattern);

    return 0;
}