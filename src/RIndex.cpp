#include "../include/RIndex.hpp"

RIndex::RIndex(const string &input) {
    csa_bitcompressed<> csa;
    construct_im(csa, input, 1);

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

void RIndex::build_locate_structs(const string& str, const vector<int> &sa) {
    unordered_map<int, int> reverse_sa;
    for (const auto& curr_char_offsets : run_offsets) {
        if (curr_char_offsets.first != '$') {
            for (const auto& entry : curr_char_offsets.second) {
                reverse_sa[entry.second] = entry.first;
            }
        }
    }

    phrase_starts = bit_vector(str.size());
    for (int i = 0; i < str.size(); ++i) {
        if (reverse_sa.contains(i)) {
            phrase_starts[i] = true;
            neighbours.emplace_back(sa[reverse_sa[i] - 1]);
        }
    }
    phrase_rank1 = rank_support_v(&phrase_starts);
}

int RIndex::find_neighbours_offset(int k) const {
    auto index = phrase_rank1(k);
    if (!phrase_starts[k]) index--;
    return neighbours[index] + k - index;
}

pair<int, int> RIndex::pred(char c, int offset) const {
    const map<int, int>& curr_char_map = run_offsets.at(c);

    int rank = wavelet_tree.rank(c, offset);
    if (wavelet_tree.access(offset) != c) {
        rank--;
    }

    int bwt_offset = wavelet_tree.select(c, rank);
    int text_offset = curr_char_map.at(bwt_offset);

    return {bwt_offset, text_offset};
}

tuple<int, int, int, int> RIndex::match(const string& pattern) const {
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
            tie(bwt_offset, text_offset) = pred(c, wavelet_tree.get_text_len() - 1);
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

int RIndex::count(const string& pattern) const {
    auto [top, bottom, bwt_offset, text_offset] = match(pattern);

    assert(bwt_offset == bottom - 1);

    if (top >= bottom) {
        return 0;
    }

    return bottom - top;
}

vector<int> RIndex::locate(const string& pattern) const {
    auto [top, bottom, bwt_offset, text_offset] = match(pattern);

    assert(bwt_offset == bottom - 1);

    if (top == bottom) {
        return {};
    }

    vector<int> offsets;
    offsets.push_back(text_offset);

    for (int i = 0; i < bottom - top - 1; ++i) {
        int left = find_neighbours_offset(text_offset - 1);
        offsets.push_back(left);
        text_offset = left;
    }

    sort(offsets.begin(), offsets.end());
    return offsets;
}

void RIndex::print_pattern_offsets(const string& pattern) const {
    vector<int> occs = locate(pattern);
    if (!occs.empty()) {
        cout << endl << "Offsets of pattern " << pattern << " in text: " << endl;
        for (int offset: occs) {
            cout << "Offset: " << offset << endl;
        }
    }
}