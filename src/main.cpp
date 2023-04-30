#include <iostream>
#include <vector>
#include <sdsl/suffix_arrays.hpp>
#include "../include/Wavelet.h"

using namespace std;

int main() {
    string input = "mississippi";
    Wavelet tree(input);

    unordered_set<char> chars(input.begin(), input.end());

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

    csa_bitcompressed<> csa;
    construct_im(csa, input, 1);

    vector<int> sa;
    string bwt;

    for (int i = 0; i < csa.size(); ++i) {
        sa.push_back(csa[i]);
        char c = csa.bwt[i];

        if (c) {
            bwt += c;
        } else {
            bwt += '$';
        }
    }

    cout << "Suffix array: ";

    for (int i: sa) {
        cout << i << " ";
    }

    cout << endl << endl << "BWT: ";

    for (char c: bwt) {
        cout << c << " ";
    }

    cout << endl;

    string test_input = "abaaba";

    csa_bitcompressed<> csa_test;
    construct_im(csa_test, test_input, 1);

    vector<int> sa_test;
    string bwt_test;

    for (int i = 0; i < csa_test.size(); ++i) {
        sa_test.push_back(csa_test[i]);
        char c = csa_test.bwt[i];

        if (c) {
            bwt_test += c;
        } else {
            bwt_test += '$';
        }
    }

    Wavelet test_tree(bwt_test);

    return 0;
}