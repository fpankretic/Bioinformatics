#include <iostream>
#include <unordered_set>
#include "include/Wavelet.hpp"
#include "include/FMIndex.hpp"
#include "include/RIndex.hpp"

using namespace std;

int main() {
    string input = "mississippi";
    Wavelet tree(input);

    cout << "Wavelet tree: " << endl;
    tree.print();
    cout << endl;

    cout << "BWT: ipssm$pissii" << endl;
    cout << "m: " << tree.rank('m', tree.select('m', 0)) << endl;
    //cout << "m: " << tree.select('m', 1) << endl;
    //cout << "i: " << tree.select('i', 0) << endl;
    cout << "i: " << tree.rank('i',tree.select('i', 1)) << endl;
    cout << "i: " << tree.rank('i',tree.select('i', 2)) << endl;
    //cout << "i: " << tree.select('i', 3) << endl;
    //cout << "i: " << tree.select('i', 4) << endl;
    cout << "s: " << tree.rank('s',tree.select('s', 0)) << endl;
    cout << "s: " << tree.rank('s',tree.select('s', 1)) << endl;
    //cout << "s: " << tree.select('s', 2) << endl;
    //cout << "s: " << tree.select('s', 3) << endl;
    cout << "s: " << tree.rank('s',tree.select('s', 4)) << endl;
    cout << "p: " << tree.rank('p',tree.select('p', 0)) << endl;
    //cout << "p: " << tree.select('p', 1) << endl;
    //cout << "p: " << tree.select('p', 2) << endl;
    //cout << "$: " << tree.rank('$',tree.select('$', 0)) << endl;
    //cout << "$: " << tree.rank('$',tree.select('$', 1)) << endl;


    FMIndex fm_index(input);

    string pattern = "ssi";

    fm_index.print_suffix_array();
    fm_index.print_pattern_offsets(pattern);

    cout << input << endl;
    RIndex r_index(input);
    cout << r_index.count(pattern) << endl;


    r_index.print_pattern_offsets(pattern);

    return 0;
}