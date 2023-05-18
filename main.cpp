#include <iostream>
#include <unordered_set>
#include "include/Wavelet.hpp"
#include "include/FMIndex.hpp"

using namespace std;

int main() {
    string input = "mississippi";
    Wavelet tree(input);

    cout << "Wavelet tree: " << endl;
    tree.print();
    cout << endl;

    FMIndex fm_index(input);
    fm_index.print_suffix_array();
    fm_index.print_pattern_offsets("ssis");

    return 0;
}