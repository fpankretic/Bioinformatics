#include <iostream>
#include <unordered_set>
#include "include/Wavelet.hpp"
#include "include/FMIndex.hpp"
#include "include/RIndex.hpp"

using namespace std;

int main() {
    string input = "misssisssippi";
    Wavelet tree(input);

    cout << "Wavelet tree: " << endl;
    tree.print();
    cout << endl;

    FMIndex fm_index(input);
    fm_index.print_suffix_array();
    fm_index.print_pattern_offsets("ssis");

    // cout << input << endl;
    // RIndex r_index(input);
    // for (const auto& item: r_index.get_predecessor_struct()) {
    //     cout << "BWT index " << item.first << " Offset " << item.second << endl;
    // }

    return 0;
}