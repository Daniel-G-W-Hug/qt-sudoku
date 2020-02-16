// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#include "sudoku_solve_helper.h"
#include "sudoku_print.h"

#include <algorithm>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
// return number of empty entries in subregion
///////////////////////////////////////////////////////////////////////////////////////////
int sudoku_num_empty_in_subregion(const Sudoku& s, const Region_t region,
                                  const int subregion) {

    int num_empty = 0;
    for (int j = 0; j < s.region_size; ++j) {    // for each cell in subregion
        int cnt = s.region_to_cnt(region, subregion, j);
        if (s(cnt).val == 0) ++num_empty;
    }
    return num_empty;
}

///////////////////////////////////////////////////////////////////////////////////////////
// return number of unique candidates in subregion
///////////////////////////////////////////////////////////////////////////////////////////
int sudoku_num_unique_candidates_in_subregion(const Sudoku& s, const Region_t region,
                                              const int subregion) {

    set<int> union_set{};
    for (int j = 0; j < s.region_size; ++j) {    // for each cell in subregion
        int cnt = s.region_to_cnt(region, subregion, j);
        if (s(cnt).val == 0) {
           union_set.insert(s(cnt).cand.begin(),s(cnt).cand.end());
        }
    }
    return union_set.size();
}

///////////////////////////////////////////////////////////////////////////////////////////
// concatenate single candidate sets in subregion
///////////////////////////////////////////////////////////////////////////////////////////
multiset<int> sudoku_concatenate_single_candidate_sets_in_subregion(const Sudoku& s,
                                                                    const Region_t region,
                                                                    const int subregion) {
    multiset<int> concatenated_sets{};
    for (int j = 0; j < s.region_size; ++j) {    // for each cell in subregion
        int cnt = s.region_to_cnt(region, subregion, j);
        if (s(cnt).val == 0 &&            // if cell is empty, there should be candidates
            s(cnt).cand.size() == 1) {    // we have a single candidate set
            concatenated_sets.insert(s(cnt).cand.begin(), s(cnt).cand.end());
        }
    }
    // cout << "concatenated cand sets: ";
    // print_multiset_int(concatenated_sets);

    return concatenated_sets;
}

///////////////////////////////////////////////////////////////////////////////////////////
// concatenate candidate sets in subregion
///////////////////////////////////////////////////////////////////////////////////////////
multiset<int> sudoku_concatenate_candidate_sets_in_subregion(const Sudoku& s,
                                                             const Region_t region,
                                                             const int subregion) {
  multiset<int> concatenated_sets{};
  for (int j = 0; j < s.region_size; ++j) {  // for each cell in subregion
    int cnt = s.region_to_cnt(region, subregion, j);
    if (s(cnt).val == 0) {  // if cell is empty, there should be candidates
      concatenated_sets.insert(s(cnt).cand.begin(), s(cnt).cand.end());
    }
  }
  // cout << "concatenated cand sets: ";
  // print_multiset_int(concatenated_sets);

  return concatenated_sets;
}

///////////////////////////////////////////////////////////////////////////////////////////
// count how often which sudoku entry (1 .. s.region_size) occurs in multiset
///////////////////////////////////////////////////////////////////////////////////////////
vector<int> sudoku_count_single_candidate_entries_in_subregion(const Sudoku& s,
                                                               const Region_t region,
                                                               const int subregion) {

    // concatenate candidate lists in current subregion
    std::multiset<int> ms =
        sudoku_concatenate_single_candidate_sets_in_subregion(s, region, subregion);

    vector<int> candidate_count{};
    for (int j = 1; j <= s.region_size; ++j) { candidate_count.push_back(ms.count(j)); }
    // cout << "candidate_count:         ";
    // print_vector_int(candidate_count);

    return candidate_count;
}

///////////////////////////////////////////////////////////////////////////////////////////
// count how often which sudoku entry (1 .. s.region_size) occurs in multiset
///////////////////////////////////////////////////////////////////////////////////////////
vector<int> sudoku_count_candidate_entries_in_subregion(const Sudoku& s,
                                                        const Region_t region,
                                                        const int subregion) {

  // concatenate candidate lists in current subregion
  std::multiset<int> ms =
      sudoku_concatenate_candidate_sets_in_subregion(s, region, subregion);

  vector<int> candidate_count{};
  for (int j = 1; j <= s.region_size; ++j) { candidate_count.push_back(ms.count(j)); }
  // cout << "candidate_count:         ";
  // print_vector_int(candidate_count);

  return candidate_count;
}

///////////////////////////////////////////////////////////////////////////////////////////
// store index where elements occur in current subregion (index i)
///////////////////////////////////////////////////////////////////////////////////////////
vector<list<int>>
sudoku_candidate_positions_in_subregion(const Sudoku& s, const Region_t region,
                                        const int subregion,
                                        const vector<int>& candidate_count) {
  list<int> lp{};                   // position helper list
  vector<list<int>> candidate_pos;  // index position of candidates in region

  for (int j = 0; j < s.region_size; ++j) {  // for each value (=j+1)
    lp.clear();
    int value = j + 1;
    for (int k = 0; k < s.region_size; ++k) {  // for each candidate list
      int cnt = s.region_to_cnt(region, subregion, k);
      if (s(cnt).val == 0 &&         // has candidate list
          candidate_count[j] > 0 &&  // element does occur in region
          s(cnt).cand.count(value) > 0) {
        lp.push_back(k);
      }
    }
    candidate_pos.push_back(lp);
  }
  // cout << "pos:                     ";
  // print_vector_list_int(cand_pos);
  // cout << "\n";

  return candidate_pos;
}

bool pairwise_different_if_size2(const set<int>& a, const set<int>& b,
                                 const set<int>& c) {

  // classical pairwise different
  // if (equal(a.begin(),a.end(),b.begin())) return false;
  // if (equal(a.begin(),a.end(),c.begin())) return false;
  // if (equal(b.begin(),b.end(),c.begin())) return false;

  // pairwise different only required for sets with 2 entries
  if (a.size() < 2) return false;
  if (b.size() < 2) return false;
  if (c.size() < 2) return false;
  if (a.size() == 2 && b.size() == 2 && equal(a.begin(), a.end(), b.begin())) return false;
  if (a.size() == 2 && c.size() == 2 && equal(a.begin(), a.end(), c.begin())) return false;
  if (b.size() == 2 && c.size() == 2 && equal(b.begin(), b.end(), c.begin())) return false;

  return true;
}

bool pairwise_different_if_size2(const set<int>& a, const set<int>& b,
                                 const set<int>& c, const set<int>& d) {

  // pairwise different only required for sets with 2 entries
  if (a.size() == 2 && b.size() == 2 && equal(a.begin(), a.end(), b.begin())) return false;
  if (a.size() == 2 && c.size() == 2 && equal(a.begin(), a.end(), c.begin())) return false;
  if (a.size() == 2 && d.size() == 2 && equal(a.begin(), a.end(), d.begin())) return false;
  if (b.size() == 2 && c.size() == 2 && equal(b.begin(), b.end(), c.begin())) return false;
  if (b.size() == 2 && d.size() == 2 && equal(b.begin(), b.end(), d.begin())) return false;
  if (c.size() == 2 && d.size() == 2 && equal(c.begin(), c.end(), d.begin())) return false;

  return true;
}

