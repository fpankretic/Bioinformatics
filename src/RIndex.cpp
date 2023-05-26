#include "../include/RIndex.hpp"

RIndex::RIndex(const string &input) {
    csa_bitcompressed<> csa; construct_im(csa, input, 1);
    string bwt;
    for (int i = 0; i < csa.size(); ++i) {
        suffix_array.push_back((int) csa[i]);
        char c = (char) csa.bwt[i];

        if (c) {
            bwt += c;
        } else {
            bwt += '$';
        }
    }

    cout << "SA: " << suffix_array << endl;
    cout << "BWT: " << bwt << endl;

    text_len = bwt.size();

    char curr = '\0';
    int run_start = -1;

    for (int i = 0; i < bwt.size(); ++i) {
        if (bwt[i] != curr) {
            if (run_start != -1) {
                if (!predecessor_struct.contains(bwt[i-1])) {
                    predecessor_struct.insert({bwt[i-1], map<int,int>()});
                }
                predecessor_struct[bwt[i-1]].insert({i - 1, suffix_array[i - 1] - 1});
            }
            run_start = i;
            curr = bwt[i];

            if (!predecessor_struct.contains(bwt[run_start])) {
                predecessor_struct.insert({bwt[run_start], map<int,int>()});
            }
            predecessor_struct[bwt[run_start]].insert({run_start, suffix_array[run_start] - 1});
        }
    }

    int bwt_size = bwt.size() - 1;
    if (!predecessor_struct.contains(bwt[bwt_size])) {
        predecessor_struct.insert({bwt[bwt_size], map<int,int>()});
    }
    predecessor_struct[bwt[bwt_size]].insert({bwt_size, suffix_array[bwt_size] - 1});

    int dlr_off = get<0>(*(predecessor_struct['$'].begin()));
    predecessor_struct['$'].clear();
    predecessor_struct['$'].insert({dlr_off, bwt_size});

    wavelet_tree = Wavelet(bwt);
}

map<char, map<int, int>> RIndex::get_predecessor_struct() {
    return predecessor_struct;
}

void build_structs(const string& str) {
    vector<int> sa{};

    auto original = get_predecessor_struct();
    unordered_map<int, int> reverse_struct;

    for (const auto& entry : original) {
        reverse_struct[entry.second] = entry.first;
    }

    vector<int> P;
    vector<tuple<int, int>> N;
    for (int i = 0; i < str.size(); ++i) {
        if (reverse_struct.contains(str.at(i))) {
            P.push_back(i);
            auto q = reverse_struct[str.at(i)];
            N.emplace_back(sa[q-1], sa[q+1]);
        }
    }
}

tuple<int, int> querry(int k) {
    vector<int> P;
    vector<tuple<int, int>> N;
    for (int i = 0; i < P.size(); ++i) {
        if (k >= P[i]) {
            return {get<0>(N[i]) + k - i, get<1>(N[i]) + k - i};
        }
    }
    return {-1, -1};
}

pair<int, int> RIndex::pred(char c, int offset) {
    map<int, int> curr_char_map = predecessor_struct[c];

    for (auto it = curr_char_map.rbegin(); it != curr_char_map.rend(); it++) {
        if (it->first <= offset) {
            return {it->first, it->second};
        }
    }

    return {-1, -1};
}

tuple<int, int, int, int> RIndex::match(const string& pattern) {
    if (pattern.empty()) {
        throw invalid_argument("Pattern must be at least one character.");
    }

    int top = 0;
    int bottom = (int) wavelet_tree.get_start()->b_vector.size();

    int i = (int) pattern.length() - 1;

    int text_offset;
    int bwt_offset;
    bool first = true;

    while (i >= 0 && bottom > top) {
        char c = pattern[i];
        if (first) {
            auto temp_tuple = pred(c, text_len - 1);
            tie(bwt_offset, text_offset) = temp_tuple;
            first = false;
        } else if (wavelet_tree.access(bwt_offset) == c) {
            cout << "here" << endl;
            text_offset -= 1;
            bwt_offset = wavelet_tree.get_char_map()[c] + wavelet_tree.rank(c , bwt_offset);
        } else {
            auto temp_tuple = pred(c, bottom - 1);
            tie(bwt_offset, text_offset) = temp_tuple;
        }
        top = wavelet_tree.get_char_map()[c] + wavelet_tree.rank(c, top);
        bottom = wavelet_tree.get_char_map()[c] + wavelet_tree.rank(c, bottom);

        i = i - 1;

        cout << "Text offset: " << text_offset << " BWT offset: " << bwt_offset << endl;
    }

    return {top, bottom, bwt_offset, text_offset};
}

int RIndex::count(const string& pattern) {
    auto [top, bottom, bwt_offset, text_offset] = match(pattern);

    if (top >= bottom) {
        return 0;
    }

    cout << "Text offset: " << text_offset << endl;

    return bottom - top;
}

vector<int> RIndex::locate(const string& pattern) {
    auto [top, bottom, bwt_offset, text_offset] = match(pattern);

    vector<int> offsets;

    if (top < bottom) {
        for (int i = top; i < bottom; ++i) {
            offsets.push_back(suffix_array[i]);
        }
    }

    sort(offsets.begin(), offsets.end());
    return offsets;
}

void RIndex::print_suffix_array() {
    cout << "Suffix array: ";
    for (int i: suffix_array) {
        cout << i << " ";
    }
    cout << endl;
}

void RIndex::print_pattern_offsets(const string& pattern) {
    vector<int> occs = locate(pattern);
    if (!occs.empty()) {
        cout << endl << "Offsets of pattern in text: " << endl;
        for (int offset: occs) {
            cout << "Offset: " << offset << endl;
        }
    }
}