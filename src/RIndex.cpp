#include "../include/RIndex.hpp"

//add wavelet intialization correctly
//: wavelet_tree(bwt(input))
RIndex::RIndex(const string &input) {
    csa_bitcompressed<> csa; construct_im(csa, input, 1);
    string bwt;
    vector<int> suffix_array;
    for (int i = 0; i < csa.size(); ++i) {
        suffix_array.push_back((int) csa[i]);
        char c = (char) csa.bwt[i];

        if (c) {
            bwt += c;
        } else {
            bwt += '$';
        }
    }

    text_len = bwt.size();

    char curr = '\0';
    int run_start = -1;

    for (int i = 0; i < bwt.size(); ++i) {
        if (bwt[i] != curr) {
            if (run_start != -1) {
                run_offsets[bwt[i - 1]].insert({i - 1, suffix_array[i - 1] - 1});
            }
            run_start = i;
            curr = bwt[i];

            run_offsets[bwt[run_start]].insert({run_start, suffix_array[run_start] - 1});
        }
    }

    int bwt_size_adjusted = bwt.size() - 1;
    run_offsets[bwt[bwt_size_adjusted]].insert({bwt_size_adjusted, suffix_array[bwt_size_adjusted] - 1});

    int dlr_off = get<0>(*(run_offsets['$'].begin()));
    run_offsets['$'].clear();
    run_offsets['$'].insert({dlr_off, bwt_size_adjusted});

    wavelet_tree = Wavelet(bwt);

    build_locate_structs(input, suffix_array);
}

void RIndex::build_locate_structs(const string& str, vector<int> &sa) {
    auto original = get_predecessor_struct();

    unordered_map<int, int> reverse_isa;
    for (const auto& map_ : original) {
        if (map_.first != '$') {
            for (const auto& entry : map_.second) {
                reverse_isa[entry.second] = entry.first;
            }
        }
    }

    phrase_starts = vector<int>();
    neighbours = vector<int>();

    for (int i = 0; i < str.size(); ++i) {
        if (reverse_isa.contains(i)) {
            phrase_starts.push_back(i);
            auto q = reverse_isa[i];
            neighbours.emplace_back(sa[q - 1]);
        }
    }
}

//potentially use rank and select, replace tuple with pair
int RIndex::find_neighbours_offset(int k) {
    for (int i = phrase_starts.size() - 1; i >= 0; --i) {
        if (k >= phrase_starts[i]) {
            //query only once
            return neighbours[i] + k - i;
        }
    }
    return -1;
}

pair<int, int> RIndex::pred(char c, int offset) {
    const map<int, int>& curr_char_map = run_offsets[c];

    // replace with rank and select
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
            tie(bwt_offset, text_offset) = pred(c, text_len - 1);
            first = false;
        } else if (wavelet_tree.access(bwt_offset) == c) {
            text_offset -= 1;
        } else {
            tie(bwt_offset, text_offset) = pred(c, bottom - 1);
        }
        bwt_offset = wavelet_tree.lf_mapping(c, bwt_offset);
        top = wavelet_tree.lf_mapping(c, top);
        bottom = wavelet_tree.lf_mapping(c, bottom);

        i--;
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
    assert(bwt_offset == bottom - 1);

    vector<int> offsets; offsets.push_back(text_offset);
    for (int i = 0; i < bottom - top - 1; ++i) {
        int left = find_neighbours_offset(text_offset - 1);
        offsets.push_back(left);
        text_offset = left;
    }

    sort(offsets.begin(), offsets.end());
    return offsets;
}

void RIndex::print_pattern_offsets(const string& pattern) {
    vector<int> occs = locate(pattern);
    if (!occs.empty()) {
        cout << endl << "Offsets of pattern " << pattern << " in text: " << endl;
        for (int offset: occs) {
            cout << "Offset: " << offset << endl;
        }
    }
}

map<char, map<int, int>> RIndex::get_predecessor_struct() {
    return this->run_offsets;
}
