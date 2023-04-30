#include <iostream>
#include "../include/Wavelet.h"

using namespace std;

int main() {
    string input = "mississippi";
    Wavelet tree(input);

    unordered_set<char> chars(input.begin(), input.end());

    for (int i = 0; i < input.length(); ++i) {
        cout << tree.access(i);
    }
    cout << endl;


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
    cout << tree.select('s', 3) << endl;

    tree.print();

    return 0;
}