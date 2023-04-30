#include <iostream>
#include <fstream>
#include <sdsl/suffix_arrays.hpp>
using namespace std;
using namespace sdsl;

int main() {
    csa_wt<> fm_index;
    string pattern, text, line;

    ifstream in("../input.fna");
    while (getline(in,line)) {
        text += line;
    }
    in.close();

    cout << "Enter pattern: ";
    cin >> pattern;

    construct_im(fm_index, text, 1);
    ofstream out("../output.txt");
    cout << "'" << pattern << "'" << " occurs " << count(fm_index,pattern) << " times." << endl;
    out << "'" << pattern << "'" << " occurs " << count(fm_index,pattern) << " times." << endl;
    for (auto index: locate(fm_index, pattern)) {
        out << "Found at index: " << index << endl;
    }
    out.close();

    return 0;
}