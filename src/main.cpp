#include <iostream>
#include "../include/Wavelet.h"

using namespace std;

int main() {
    string input = "mississippi";
    Wavelet tree(input);

    bit_vector v(input.size(), 0);
    cout << v;

    return 0;
}