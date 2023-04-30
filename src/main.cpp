#include <iostream>
#include "../include/Wavelet.h"

using namespace std;

int main() {
    string input = "mississippi";
    Wavelet tree(input);

//    bit_vector v(input.size(), 0);
//    v[2] = true;
//    v[3] = true;
//    v[5] = true;
//    v[6] = true;
//    v[8] = true;
//    v[9] = true;
//
//    rank_support_v<0,1> rank0(&v);
//    cout << rank0(4) << endl;
//    cout << rank0(5) << endl;
//    cout << rank0(6) << endl;
//    cout << rank0(7) << endl;
//
//    rank_support_v<1,1> rank1(&v);
//    cout << rank1(4) << endl;
//    cout << rank1(5) << endl;
//    cout << rank1(6) << endl;
//    cout << rank1(7) << endl;
//
//    cout << v;

    return 0;
}