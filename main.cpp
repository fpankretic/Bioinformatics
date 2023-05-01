#include <iostream>
#include "include/Wavelet.hpp"
#include "include/FMIndex.hpp"

using namespace std;

int main() {
    string input = "mississippi";
    unordered_set<char> chars(input.begin(), input.end());
    Wavelet tree(input);

    cout << "Access queries: ";
    for (int i = 0; i < input.length(); ++i) {
        cout << tree.access(i);
    }

    cout << endl << endl << "Rank queries for each character and index: " << endl;
    for (const auto &c: chars) {
        cout << c << ": ";
        for (int i = 0; i < input.length(); ++i) {
            cout << tree.rank(c, i);
        }
        cout << endl;
    }

//    for (const auto &c: chars) {
//        cout << c << ": ";
//        for (int i = 0; i < input.length(); ++i) {
//            cout << tree.select(c, i);
//        }
//        cout << endl;
//    }
//    cout << tree.select('s', 3) << endl;

    cout << endl << "Wavelet tree: " << endl;
    tree.print();
    cout << endl;

    FMIndex fm_index(input);
    fm_index.print_suffix_array();

    // --------------------------
    // FM Index testing
    // --------------------------

    string test_input = "abaaba";
    string pattern = "aba";

    FMIndex test_fm_index(test_input);

    cout << "Number of occurances of pattern in text: " << test_fm_index.count(pattern) << endl;
    test_fm_index.print_pattern_offsets(pattern);

    return 0;
}