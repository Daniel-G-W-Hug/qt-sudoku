// 345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_print.h"

using namespace std;

void sudoku_print(const Sudoku& s, const string msg) {
    cout << msg << ":";
    for (int cnt = 0; cnt < s.total_size; ++cnt) {
        if (cnt % s.region_size == 0) { cout << "\n" << cnt / s.region_size << ": "; }
        cout << setw(2) << s(cnt).val;
        if ((cnt + 1) % s.region_size != 0) cout << ",";
    }
    cout << "\n";
}

void sudoku_print_regions(const Sudoku& s, const string msg) {
    cout << msg << ", rows:\n";
    for (int i = 0; i < s.region_size; ++i) {
        cout << i << ": ";
        for (int j = 0; j < s.region_size; ++j) {
            cout << setw(2) << s.row(i, j).val;
            if (j < s.region_size - 1) cout << ", ";
        }
        cout << "\n";
    }
    cout << "\n";

    cout << msg << ", cols:\n";
    for (int i = 0; i < s.region_size; ++i) {
        cout << i << ": ";
        for (int j = 0; j < s.region_size; ++j) {
            cout << setw(2) << s.col(i, j).val;
            if (j < s.region_size - 1) cout << ", ";
        }
        cout << "\n";
    }
    cout << "\n";

    cout << msg << ", blocks:\n";
    for (int i = 0; i < s.region_size; ++i) {
        cout << i << ": ";
        for (int j = 0; j < s.region_size; ++j) {
            cout << setw(2) << s.block(i, j).val;
            if (j < s.region_size - 1) cout << ", ";
        }
        cout << "\n";
    }
}

void sudoku_print_cnt_to_x(const Sudoku& s, const string msg) {
    cout << msg << ", row_cnt:\n";
    for (int cnt = 0; cnt < s.total_size; ++cnt) {
        if (cnt > 0 && cnt % s.region_size == 0) cout << "\n";
        cout << setw(1) << s.cnt_to_row(cnt).first << s.cnt_to_row(cnt).second;
        if (cnt < s.total_size - 1) cout << ", ";
    }
    cout << "\n\n";

    cout << msg << ", col_cnt:\n";
    for (int cnt = 0; cnt < s.total_size; ++cnt) {
        if (cnt > 0 && cnt % s.region_size == 0) cout << "\n";
        cout << setw(1) << s.cnt_to_col(cnt).first << s.cnt_to_col(cnt).second;
        if (cnt < s.total_size - 1) cout << ", ";
    }
    cout << "\n\n";

    cout << msg << ", block_cnt:\n";
    for (int cnt = 0; cnt < s.total_size; ++cnt) {
        if (cnt > 0 && cnt % s.region_size == 0) cout << "\n";
        cout << setw(1) << s.cnt_to_block(cnt).first << s.cnt_to_block(cnt).second;
        if (cnt < s.total_size - 1) cout << ", ";
    }
    cout << "\n";
}

void sudoku_print_candidates(const Sudoku& s, const string msg, const Region_t region) {
    int cnt;

    switch (region) {
        case Region_t::row:

            cout << msg << ", candidates by row:\n";

            for (int i = 0; i < s.region_size; ++i) {
                for (int j = 0; j < s.region_size; ++j) {
                    cnt = s.region_to_cnt(region, i, j);

                    cout << setw(2) << cnt << ": ";
                    cout << "r(" << i << "," << j << "), ";

                    cout << "c(" << s.cnt_to_col(cnt).first << ",";
                    cout << s.cnt_to_col(cnt).second << "), ";

                    cout << "b(" << s.cnt_to_block(cnt).first << ",";
                    cout << s.cnt_to_block(cnt).second << "): ";

                    cout << "{";
                    for (auto item       = s(cnt).cand.cbegin(),
                              end_of_set = s(cnt).cand.cend();
                         item != end_of_set; ++item) {
                        cout << setw(2) << *item << " ";
                    }
                    cout << "}\n";
                }
                cout << "\n";
            }
            cout << "\n";

            break;

        case Region_t::col:

            cout << msg << ", candidates by col:\n";

            for (int i = 0; i < s.region_size; ++i) {
                for (int j = 0; j < s.region_size; ++j) {
                    cnt = s.region_to_cnt(region, i, j);

                    cout << setw(2) << cnt << ": ";
                    cout << "c(" << i << "," << j << "), ";

                    cout << "r(" << s.cnt_to_row(cnt).first << ",";
                    cout << s.cnt_to_row(cnt).second << "), ";

                    cout << "b(" << s.cnt_to_block(cnt).first << ",";
                    cout << s.cnt_to_block(cnt).second << "): ";

                    cout << "{";
                    for (auto item       = s(cnt).cand.cbegin(),
                              end_of_set = s(cnt).cand.cend();
                         item != end_of_set; ++item) {
                        cout << setw(2) << *item << " ";
                    }
                    cout << "}\n";
                }
                cout << "\n";
            }
            cout << "\n";

            break;

        case Region_t::block:

            cout << msg << ", candidates by block:\n";

            for (int i = 0; i < s.region_size; ++i) {
                for (int j = 0; j < s.region_size; ++j) {
                    cnt = s.region_to_cnt(region, i, j);

                    cout << setw(2) << cnt << ": ";
                    cout << "b(" << i << "," << j << "), ";

                    cout << "r(" << s.cnt_to_row(cnt).first << ",";
                    cout << s.cnt_to_row(cnt).second << "), ";

                    cout << "c(" << s.cnt_to_col(cnt).first << ",";
                    cout << s.cnt_to_col(cnt).second << "): ";

                    cout << "{";
                    for (auto item       = s(cnt).cand.cbegin(),
                              end_of_set = s(cnt).cand.cend();
                         item != end_of_set; ++item) {
                        cout << setw(2) << *item << " ";
                    }
                    cout << "}\n";
                }
                cout << "\n";
            }
            cout << "\n";

            break;
    }
}

void print_vector_int(const vector<int>& v) {
    cout << "[ ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << v[i];
        if (i < v.size() - 1) cout << ", ";
    }
    cout << " ]\n";
}

void print_vector_list_int(const vector<list<int>>& v) {
    cout << "[ ";
    for (size_t i = 0; i < v.size(); ++i) {
        cout << "{";
        for (auto p = v[i].begin(); p != v[i].end(); ++p) {
            cout << *p;
            if (p != prev(v[i].end())) cout << ", ";
        }
        cout << "}";

        if (i < v.size() - 1) cout << ", ";
    }
    cout << " ]\n";
}

void print_list_int(const list<int>& l) {
    cout << "{ ";
    for (auto lp = l.begin(); lp != l.end(); ++lp) {
        cout << *lp;
        if (lp != prev(l.end())) cout << ", ";
    }
    cout << " }\n";
}

void print_list_list_int(const list<list<int>>& ll) {
    cout << "{ ";
    for (auto llp = ll.begin(); llp != ll.end(); ++llp) {
        cout << "{";
        for (auto p = (*llp).begin(); p != (*llp).end(); ++p) {
            cout << *p;
            if (p != prev((*llp).end())) cout << ", ";
        }
        cout << "}";

        if (llp != prev(ll.end())) cout << ", ";
    }
    cout << " }\n";
}
