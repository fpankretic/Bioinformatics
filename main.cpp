#include <iostream>
#include "include/Wavelet.hpp"
#include "include/FMIndex.hpp"
#include "include/RIndex.hpp"

using namespace std;

int main() {
    string input = "mississippi";
    string pattern = "pp";

    Wavelet tree(input);

    cout << "Wavelet tree: " << endl;
    tree.print();
    cout << endl;

    cout << "Input: " << input << endl;
    cout << "Pattern: " << pattern << endl;

    FMIndex fm_index(input);
    fm_index.print_pattern_offsets(pattern);
    cout << "Count of pattern appearances in text: " << fm_index.count(pattern) << endl;

    RIndex r_index(input);
    r_index.print_pattern_offsets(pattern);
    cout << "Count of pattern appearances in text: " << r_index.count(pattern) << endl;

    return 0;
}