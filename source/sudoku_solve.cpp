// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
#include "dyn_assert.h"

#include <algorithm>
#include <tuple>
//#include "sudoku_print.h" // for debugging only
#include "sudoku_solve.h"
#include "sudoku_solve_helper.h"

using namespace std;

//
// routines to update sudoku candidate set
//

void sudoku_update_candidates_cell(Sudoku& s, int cnt) {
  dynamic_assert(s.is_valid_index(cnt), "Index out of range.");
  //
  // if the cell has an entry != 0 the candidate set must be cleared
  //
  // if the cell has an entry == 0 the candidate set must be updated:
  //   each cell entry value != 0 that occurs in a row, col, or block
  //   the cell belongs to has to be removed from the cell's candidate
  //   set
  //

  if (s(cnt).val != 0) {
    if (s(cnt).cand.size() != 0) s(cnt).cand.clear();
    return;
  }

  // from here on s(cnt) == 0, i.e. empty cells that should have candidate sets

  // remove all values ocurring in curr_row from candidate set
  int curr_row = s(cnt).ri;
  for (int j = 0; j < s.region_size; ++j) {
    int value = s.row(curr_row, j).val;
    if (value > 0) s(cnt).cand.erase(value);
  }

  // remove all values ocurring in curr_col from candidate set
  int curr_col = s(cnt).ci;
  for (int j = 0; j < s.region_size; ++j) {
    int value = s.col(curr_col, j).val;
    if (value > 0) s(cnt).cand.erase(value);
  }

  // remove all values ocurring in curr_block from candidate set
  int curr_block = s(cnt).bi;
  for (int j = 0; j < s.region_size; ++j) {
    int value = s.block(curr_block, j).val;
    if (value > 0) s(cnt).cand.erase(value);
  }

  return;
}

void sudoku_update_candidates_affected_by_cell(Sudoku& s, int cnt) {
  dynamic_assert(s.is_valid_index(cnt), "Index out of range.");
  //
  // update all candidate sets of cells in the regions the cell(cnt) belongs to
  //

  // update candidate sets of all cells in curr_row
  int curr_row = s(cnt).ri;
  for (int j = 0; j < s.region_size; ++j) {
    sudoku_update_candidates_cell(s, s.row_to_cnt(curr_row, j));
  }

  // update candidate sets of all cells in curr_col
  int curr_col = s(cnt).ci;
  for (int j = 0; j < s.region_size; ++j) {
    sudoku_update_candidates_cell(s, s.col_to_cnt(curr_col, j));
  }

  // update candidate sets of all cells in curr_block
  int curr_block = s(cnt).bi;
  for (int j = 0; j < s.region_size; ++j) {
    sudoku_update_candidates_cell(s, s.block_to_cnt(curr_block, j));
  }

  return;
}

void sudoku_update_candidates_all_cells(Sudoku& s) {
  for (int cnt = 0; cnt < s.total_size; ++cnt) { sudoku_update_candidates_cell(s, cnt); }
}

//////////////////////////////////////////////////////////////////////////////////////////
// check for valid and unique entries in region
//////////////////////////////////////////////////////////////////////////////////////////
bool sudoku_has_unique_entries_in_region(const Sudoku& s, const Region_t region) {
  for (int i = 0; i < s.region_size; ++i) {  // for each row / col / block

    for (int value = 1; value <= s.region_size; ++value) {  // for each potential value
      int count = 0;
      for (int j = 0; j < s.region_size; ++j) {  // occurences at each position
        if (s(s.region_to_cnt(region, i, j)).val == value) ++count;
      }
      if (count > 1) return false;
    }
  }

  return true;
}

bool sudoku_is_valid(const Sudoku& s) {
  // check for valid entry values:
  // 0 (=empty indicator, is valid too), valid entries: 1..region_size
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val < 0 || s(cnt).val > s.region_size) { return false; }
  }

  // check for valid and unique entries in regions
  if (!sudoku_has_unique_entries_in_region(s, Region_t::row)) return false;
  if (!sudoku_has_unique_entries_in_region(s, Region_t::col)) return false;
  if (!sudoku_has_unique_entries_in_region(s, Region_t::block)) return false;

  return true;
}

int sudoku_num_entries(const Sudoku& s) {
  // return no. of entries > 0
  int count = 0;
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val > 0) ++count;
  }

  return count;
}

int sudoku_num_empty(const Sudoku& s) {
  // return no. of empty entries, i.e. entries with value 0

  return s.total_size - sudoku_num_entries(s);
}

int sudoku_num_candidates(const Sudoku& s) {
  int num_candidates = 0;

  if (sudoku_num_empty(s) == 0) return num_candidates;

  // for all entries == 0 candidates available, if size of candidate set > 0
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val == 0 && s(cnt).cand.size() == 0) return num_candidates;
  }

  // if arrived here, there are still candidates available
  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val == 0) num_candidates += s(cnt).cand.size();
  }

  return num_candidates;
}

bool sudoku_has_candidates(const Sudoku& s) { return sudoku_num_candidates(s) > 0; }

//////////////////////////////////////////////////////////////////////////////////////////
// naked singles
//////////////////////////////////////////////////////////////////////////////////////////
single_vec sudoku_naked_singles(const Sudoku& s) {

  single_vec naked_singles;  // vector of tuples with (cell-no., region, subregion, value)
                             // region & subregion can be ignored for naked singles

  for (int cnt = 0; cnt < s.total_size; ++cnt) {
    if (s(cnt).val == 0 && s(cnt).cand.size() == 1) {
      naked_singles.push_back(make_tuple(cnt, Region_t::row, s.cnt_to_row(cnt).first,
                                         *(s(cnt).cand.begin())));
    }
  }

  // for (int i=0; i<naked_singles.size(); ++i) {
  //   cout << "\nFound naked single at cell no. " << get<0>(naked_singles[i]);
  //   cout << ", in region " << int(get<1>(naked_singles[i]));
  //   cout << ", subregion " << get<2>(naked_singles[i]);
  //   cout << ", of value " << get<3>(naked_singles[i]) << '\n';
  // }
  // if (naked_singles.size() > 0) cout << '\n';cout << '\n';

  return naked_singles;
}

int sudoku_num_naked_singles(const Sudoku& s) { return sudoku_naked_singles(s).size(); }

bool sudoku_has_naked_singles(const Sudoku& s) { return sudoku_num_naked_singles(s) > 0; }

// remove naked singles: fill in candidate value & return no. of removed naked singles
int sudoku_remove_naked_singles(Sudoku& s) {

  single_vec naked_singles      = sudoku_naked_singles(s);
  int num_naked_singles_removed = naked_singles.size();

  for (int i = 0; i < num_naked_singles_removed; ++i) {
    // 1st element of tuple contains cell no.
    // 4th element of tuple contains candidate value
    s(get<0>(naked_singles[i])).val = get<3>(naked_singles[i]);
  }

  sudoku_update_candidates_all_cells(s);

  return num_naked_singles_removed;
}

//////////////////////////////////////////////////////////////////////////////////////////
// hidden singles
//////////////////////////////////////////////////////////////////////////////////////////

single_vec sudoku_hidden_singles_in_subregion(const Sudoku& s, const Region_t region,
                                              const int subregion) {

  // vector of tuples with (cell-no., region, subregion, value)
  single_vec hidden_singles;
  vector<int> cand_count{};      // candidate occurrance count
  vector<list<int>> cand_pos{};  // candidate position index

  // count how often which entry occurs in candidate sets
  cand_count = sudoku_count_candidate_entries_in_subregion(s, region, subregion);

  // store index where elements occur in current subregion (index i)
  cand_pos = sudoku_candidate_positions_in_subregion(s, region, subregion, cand_count);

  // identify hidden singles
  for (int k = 0; k < s.region_size; ++k) {  // for each value k+1 of the entry list
    if (cand_count[k] == 1) {  // condition missing to exclude naked singles at this point
                               // so those will be found as well
      // potential hidden single found at position cnt with value k+1
      int cnt   = s.region_to_cnt(region, subregion, cand_pos[k].front());
      int value = k + 1;
      // only add it to the list, if it is not a naked twin,
      // i.e. if the cell still has more than 1 candidate
      if (s(cnt).cand.size() > 1) {
        hidden_singles.push_back(make_tuple(cnt, region, subregion, value));
      }
    }
  }

  // for (int i = 0; i < hidden_singles.size(); ++i) {
  //   cout << "Found hidden single at cell no. " << get<0>(hidden_singles[i]);
  //   cout << ", in region " << int(get<1>(hidden_singles[i]));
  //   cout << ", subregion " << get<2>(hidden_singles[i]);
  //   cout << ", value " << get<3>(hidden_singles[i]) << '\n';
  // }
  // if (hidden_singles.size() > 0) cout << endl;

  return hidden_singles;
}

single_vec sudoku_hidden_singles_in_region(const Sudoku& s, const Region_t region) {

  // vector of tuples with (cell-no., region, subregion, value)
  single_vec hidden_singles;
  single_vec subregion_singles;

  for (int i = 0; i < s.region_size; ++i) {  // for each subregion within region
    subregion_singles = sudoku_hidden_singles_in_subregion(s, region, i);
    hidden_singles.insert(hidden_singles.end(), subregion_singles.begin(),
                          subregion_singles.end());
  }

  return hidden_singles;
}

single_vec sudoku_hidden_singles(const Sudoku& s) {

  // vector of tuples with (cell-no., region, subregion, value)
  single_vec hidden_singles;
  single_vec region_singles;

  // collect hidden singles from each region
  region_singles = sudoku_hidden_singles_in_region(s, Region_t::row);
  hidden_singles.insert(hidden_singles.end(), region_singles.begin(),
                        region_singles.end());
  region_singles = sudoku_hidden_singles_in_region(s, Region_t::col);
  hidden_singles.insert(hidden_singles.end(), region_singles.begin(),
                        region_singles.end());
  region_singles = sudoku_hidden_singles_in_region(s, Region_t::block);
  hidden_singles.insert(hidden_singles.end(), region_singles.begin(),
                        region_singles.end());

  // remove hidden single entries that occur more than once
  // (same hidden single can occur from perspective of different regions)
  sort(hidden_singles.begin(), hidden_singles.end(),
       // sort entries according to 1st element of tuple (cell no.)
       [](const tuple<int, Region_t, int, int>& a,
          const tuple<int, Region_t, int, int>& b) -> bool {
         return get<0>(a) < get<0>(b);
       });
  // erase duplicates for same cell no.
  // (even if the same cell is found from perspective of different regions, the value
  // can only be set once)
  auto last = unique(hidden_singles.begin(), hidden_singles.end(),
                     // eliminate entries with same 1st element
                     [](const tuple<int, Region_t, int, int>& a,
                        const tuple<int, Region_t, int, int>& b) -> bool {
                       return get<0>(a) == get<0>(b);
                     });
  hidden_singles.erase(last, hidden_singles.end());  // actually erase elements

  // for (int i = 0; i < hidden_singles.size(); ++i) {
  //   cout << "\nFound hidden single at cell no. " << get<0>(hidden_singles[i]);
  //   cout << ", in region " << int(get<1>(hidden_singles[i]));
  //   cout << ", subregion " << get<2>(hidden_singles[i]);
  //   cout << ", value " << get<3>(hidden_singles[i]) << '\n';
  // }
  // if (hidden_singles.size() > 0) cout << endl;

  return hidden_singles;
}

int sudoku_num_hidden_singles(const Sudoku& s) { return sudoku_hidden_singles(s).size(); }

bool sudoku_has_hidden_singles(const Sudoku& s) {
  return sudoku_num_hidden_singles(s) > 0;
}

// remove hidden singles: fill in candidate value & return no. of removed hidden singles
int sudoku_remove_hidden_singles(Sudoku& s) {

  single_vec hidden_singles      = sudoku_hidden_singles(s);
  int num_hidden_singles_removed = hidden_singles.size();

  for (const auto& e : hidden_singles) {
    const auto& [cnt, region, subregion, value] = e;
    s(cnt).val                                  = value;
  }
  sudoku_update_candidates_all_cells(s);

  return num_hidden_singles_removed;
}

//////////////////////////////////////////////////////////////////////////////////////////
// naked twins
//////////////////////////////////////////////////////////////////////////////////////////

twin_vec sudoku_naked_twins_in_subregion(const Sudoku& s, const Region_t region,
                                         const int subregion) {
  //
  // naked twin: Candidate sets of two cells in a region have the same two
  //             entries. Since these two cells must have those values,
  //             these candidate values can be removed in all other candidate
  //             sets of this region.
  //

  // vector of tuples with (cell-no.1, cell-no.2 region, subregion, value1, value2)
  twin_vec naked_twins;
  vector<int> cand_count{};  // candidate occurrence count

  // count how often which entry occurs in candidate sets in each subregion
  cand_count = sudoku_count_candidate_entries_in_subregion(s, region, subregion);

  for (int j = 0; j < s.region_size; ++j) {
    int cnt = s.region_to_cnt(region, subregion, j);
    if (s(cnt).val == 0 && s(cnt).cand.size() == 2) {
      // potential twin candidate found
      for (int k = j + 1; k < s.region_size; ++k) {
        int cnt_k = s.region_to_cnt(region, subregion, k);
        if (s(cnt_k).val == 0 && s(cnt_k).cand.size() == 2 &&
            s(cnt).cand == s(cnt_k).cand &&
            (cand_count[*(s(cnt).cand.cbegin()) - 1] > 2 ||
             cand_count[*(next(s(cnt).cand.cbegin())) - 1] > 2)) {
          // naked twin found
          naked_twins.push_back(make_tuple(cnt, cnt_k, region, subregion,
                                           *(s(cnt).cand.cbegin()),
                                           *(next(s(cnt).cand.cbegin()))));
        }
      }
    }
  }

  return naked_twins;
}

twin_vec sudoku_naked_twins_in_region(const Sudoku& s, const Region_t region) {

  // vector of tuples with (cell-no.1, cell-no.2 region, subregion, value1, value2)
  twin_vec naked_twins;
  twin_vec subregion_twins;

  for (int i = 0; i < s.region_size; ++i) {  // for each subregion within region
    subregion_twins = sudoku_naked_twins_in_subregion(s, region, i);
    naked_twins.insert(naked_twins.end(), subregion_twins.begin(), subregion_twins.end());
  }

  return naked_twins;
}

twin_vec sudoku_naked_twins(const Sudoku& s) {

  // vector of tuples with (cell-no.1, cell-no.2 region, subregion, value1, value2)
  twin_vec naked_twins;
  twin_vec region_twins;

  // collect naked twins from each region
  region_twins = sudoku_naked_twins_in_region(s, Region_t::row);
  naked_twins.insert(naked_twins.end(), region_twins.begin(), region_twins.end());
  region_twins = sudoku_naked_twins_in_region(s, Region_t::col);
  naked_twins.insert(naked_twins.end(), region_twins.begin(), region_twins.end());
  region_twins = sudoku_naked_twins_in_region(s, Region_t::block);
  naked_twins.insert(naked_twins.end(), region_twins.begin(), region_twins.end());

  // remove naked twin entries that occur more than once
  // (same naked twin can occur from perspective of different regions)
  sort(naked_twins.begin(), naked_twins.end(),
       // sort entries according to 1st element of tuple (cell no.)
       [](const tuple<int, int, Region_t, int, int, int>& a,
          const tuple<int, int, Region_t, int, int, int>& b) -> bool {
         return get<0>(a) < get<0>(b);
       });

  // DO NOT erase duplicates for same cell no., since they reflect row-block or row-column
  // interactions (e.g. twin within one block within the same row implies that the
  // candidates can be removed in the other cells of the block as well as of the row
  // (same reasoning is valid for columns)
  //
  // if removed the same naked twin would be found in the next step again
  // from the perspective of the second region involved
  //
  // auto last = unique(naked_twins.begin(), naked_twins.end(),
  //                    // eliminate entries with same 1st element
  //                    [](const tuple<int, int, Region_t, int, int, int>& a,
  //                       const tuple<int, int, Region_t, int, int, int>& b) -> bool {
  //                      return get<0>(a) == get<0>(b);
  //                    });
  // naked_twins.erase(last, naked_twins.end());  // actually erase elements

  // for (int i = 0; i < naked_twins.size(); ++i) {
  //   cout << "\nFound naked twin at cell1 no. " << get<0>(naked_twins[i]);
  //   cout << ", cell2 no. " << get<1>(naked_twins[i]);
  //   cout << ", in region " << int(get<2>(naked_twins[i]));
  //   cout << ", subregion " << get<3>(naked_twins[i]);
  //   cout << ", value1 " << get<4>(naked_twins[i]);
  //   cout << ", value2 " << get<5>(naked_twins[i]) << '\n';
  // }
  // if (naked_twins.size() > 0) cout << endl;

  return naked_twins;
}

int sudoku_num_naked_twins(const Sudoku& s) { return sudoku_naked_twins(s).size(); }

bool sudoku_has_naked_twins(const Sudoku& s) { return sudoku_num_naked_twins(s) > 0; }

int sudoku_remove_naked_twins(Sudoku& s) {

  twin_vec naked_twins        = sudoku_naked_twins(s);
  int num_naked_twins_removed = naked_twins.size();

  for (const auto& e : naked_twins) {
    const auto& [cnt1, cnt2, region, subregion, val1, val2] = e;
    for (int i = 0; i < s.region_size; ++i) {
      int cnt = s.region_to_cnt(region, subregion, i);
      if (cnt == cnt1 || cnt == cnt2) {
        continue;  // don't change cells that contain the naked twins
      }
      // for all other cells in subregion erase the twin values from their candidate sets
      s(cnt).cand.erase(val1);
      s(cnt).cand.erase(val2);
    }
  }

  return num_naked_twins_removed;
}

//////////////////////////////////////////////////////////////////////////////////////////
// hidden twins
//////////////////////////////////////////////////////////////////////////////////////////

twin_vec sudoku_hidden_twins_in_subregion(const Sudoku& s, const Region_t region,
                                          const int subregion) {
  //
  // hidden twin:
  //
  // if two values occur only twice in the same region and if those two values
  // occur in two cells together exclusively, then all other candiates in those
  // two cells' candidate sets can be removed, because the two values can only
  // occur in these two cells (defined location)
  //

  // vector of tuples with (cell-no.1, cell-no.2 region, subregion, value1, value2)
  twin_vec hidden_twins;
  vector<int> cand_count{};      // candidate occurrance count
  vector<list<int>> cand_pos{};  // candidate position index

  // count how often which entry occurs in candidate sets in each subregion
  cand_count = sudoku_count_candidate_entries_in_subregion(s, region, subregion);

  // store index where elements occur in current subregion (index i)
  cand_pos = sudoku_candidate_positions_in_subregion(s, region, subregion, cand_count);

  // check for hidden twins
  for (int j = 0; j < s.region_size; ++j) {  // for each value
    if (cand_count[j] == 2) {                // potential hidden twin candidate
      list<int> curr_cand_pos = cand_pos[j];
      for (int k = j + 1; k < s.region_size; ++k) {
        int cnt1 = s.region_to_cnt(region, subregion, curr_cand_pos.front());
        int cnt2 = s.region_to_cnt(region, subregion, curr_cand_pos.back());
        if (cand_count[k] == 2 && curr_cand_pos == cand_pos[k] &&
            (s(cnt1).cand.size() > 2 || s(cnt2).cand.size() > 2)) {
          // hidden twin found
          hidden_twins.push_back(make_tuple(cnt1, cnt2, region, subregion, j + 1, k + 1));
        }
      }
    }
  }

  return hidden_twins;
}

twin_vec sudoku_hidden_twins_in_region(const Sudoku& s, const Region_t region) {

  // vector of tuples with (cell-no., region, subregion, value)
  twin_vec hidden_twins;
  twin_vec subregion_twins;

  for (int i = 0; i < s.region_size; ++i) {  // for each subregion within region
    subregion_twins = sudoku_hidden_twins_in_subregion(s, region, i);
    hidden_twins.insert(hidden_twins.end(), subregion_twins.begin(),
                        subregion_twins.end());
  }

  return hidden_twins;
}

twin_vec sudoku_hidden_twins(const Sudoku& s) {

  // vector of tuples with (cell-no.1, cell-no.2 region, subregion, value1, value2)
  twin_vec hidden_twins;
  twin_vec region_twins;

  // collect hidden twins from each region
  region_twins = sudoku_hidden_twins_in_region(s, Region_t::row);
  hidden_twins.insert(hidden_twins.end(), region_twins.begin(), region_twins.end());
  region_twins = sudoku_hidden_twins_in_region(s, Region_t::col);
  hidden_twins.insert(hidden_twins.end(), region_twins.begin(), region_twins.end());
  region_twins = sudoku_hidden_twins_in_region(s, Region_t::block);
  hidden_twins.insert(hidden_twins.end(), region_twins.begin(), region_twins.end());

  // remove hidden twin entries that occur more than once
  // (same hidden twin can occur from perspective of different regions)
  sort(hidden_twins.begin(), hidden_twins.end(),
       // sort entries according to 1st element of tuple (cell no.)
       [](const tuple<int, int, Region_t, int, int, int>& a,
          const tuple<int, int, Region_t, int, int, int>& b) -> bool {
         return get<0>(a) < get<0>(b);
       });

  // erase duplicates for same cell no.
  // (even if the same cell is found from perspective of different regions, the value
  // can only be set once)
  auto last = unique(hidden_twins.begin(), hidden_twins.end(),
                     // eliminate entries with same 1st element
                     [](const tuple<int, int, Region_t, int, int, int>& a,
                        const tuple<int, int, Region_t, int, int, int>& b) -> bool {
                       return get<0>(a) == get<0>(b);
                     });
  hidden_twins.erase(last, hidden_twins.end());  // actually erase elements

  // for (int i = 0; i < hidden_twins.size(); ++i) {
  //   cout << "\nFound hidden twin at cell1 no. " << get<0>(hidden_twins[i]);
  //   cout << ", cell2 no. " << get<1>(hidden_twins[i]);
  //   cout << ", in region " << int(get<2>(hidden_twins[i]));
  //   cout << ", subregion " << get<3>(hidden_twins[i]);
  //   cout << ", value1 " << get<4>(hidden_twins[i]);
  //   cout << ", value2 " << get<5>(hidden_twins[i]) << '\n';
  // }
  // if (hidden_twins.size() > 0) cout << endl;

  return hidden_twins;
}

int sudoku_num_hidden_twins(const Sudoku& s) { return sudoku_hidden_twins(s).size(); }

bool sudoku_has_hidden_twins(const Sudoku& s) { return sudoku_num_hidden_twins(s) > 0; }

int sudoku_remove_hidden_twins(Sudoku& s) {

  twin_vec hidden_twins        = sudoku_hidden_twins(s);
  int num_hidden_twins_removed = hidden_twins.size();

  for (const auto& e : hidden_twins) {
    const auto& [cnt1, cnt2, region, subregion, val1, val2] = e;

    // in cells s(cnt1) and s(cnt2) keep only candidates that correspond to val1 or val2
    for (auto iter = s(cnt1).cand.begin(); iter != s(cnt1).cand.end();) {
      if (*iter == val1 || *iter == val2) {
        ++iter;
      }  // keep those values
      else {
        iter = s(cnt1).cand.erase(iter);
      }  // get rid of those values
    }
    for (auto iter = s(cnt2).cand.begin(); iter != s(cnt2).cand.end();) {
      if (*iter == val1 || *iter == val2) {
        ++iter;
      }  // keep those values
      else {
        iter = s(cnt2).cand.erase(iter);
      }  // get rid of those values
    }
  }

  return num_hidden_twins_removed;
}

//////////////////////////////////////////////////////////////////////////////////////////
// naked triples
//////////////////////////////////////////////////////////////////////////////////////////

triple_vec sudoku_naked_triples_in_subregion(const Sudoku& s, const Region_t region,
                                             const int subregion) {

  triple_vec naked_triples;

  // collect candidates for potential naked triples and skip other cells
  for (int i = 0; i < s.region_size; ++i) {  // potential first element of triple
    int cnt_i = s.region_to_cnt(region, subregion, i);
    // skip unsuitable elements
    if (s(cnt_i).val > 0 || s(cnt_i).cand.size() < 2 || s(cnt_i).cand.size() > 3)
      continue;

    for (int j = i + 1; j < s.region_size; ++j) {  // potential second element of triple
      int cnt_j = s.region_to_cnt(region, subregion, j);
      // skip unsuitable elements
      if (s(cnt_j).val > 0 || s(cnt_j).cand.size() < 2 || s(cnt_j).cand.size() > 3)
        continue;

      for (int k = j + 1; k < s.region_size; ++k) {  // potential third element of triple
        int cnt_k = s.region_to_cnt(region, subregion, k);
        // skip unsuitable elements
        if (s(cnt_k).val > 0 || s(cnt_k).cand.size() < 2 || s(cnt_k).cand.size() > 3)
          continue;

        // from here on we can combine candidate sets of the potential triple cells
        // cnt_i, cnt_j, cnt_k, i.e. cells from the current subregion with
        // at least 2 and at most 3 candidates and check whether the combinded set
        // has only 3 candidates (than it is a potential naked triple)

        set<int> combined_cand(s(cnt_i).cand);
        combined_cand.insert(s(cnt_j).cand.begin(), s(cnt_j).cand.end());
        combined_cand.insert(s(cnt_k).cand.begin(), s(cnt_k).cand.end());

        if (combined_cand.size() == 3 &&
            pairwise_different_if_size2(s(cnt_i).cand, s(cnt_j).cand, s(cnt_k).cand)) {
	  // found a potential naked triple (pairwise_different_if_size2 is required
	  // to make sure that there is a cyclic dependency between the three cells)

          // store triple values
          vector<int> val(combined_cand.begin(), combined_cand.end());

          // make sure, that the remaining cells still have candidates left to be removed
          // otherwise don't add this naked triple to the list of found triples
          set<int> combined_other_cand{};
          for (int h = 0; h < s.region_size; ++h) {
            int cnt_h = s.region_to_cnt(region, subregion, h);
            if (cnt_h == cnt_i || cnt_h == cnt_j || cnt_h == cnt_k) continue;
            combined_other_cand.insert(s(cnt_h).cand.begin(), s(cnt_h).cand.end());
          }
          int combined_other_remove_count = combined_other_cand.count(val[0]);
          combined_other_remove_count += combined_other_cand.count(val[1]);
          combined_other_remove_count += combined_other_cand.count(val[2]);

	  // store naked triple for return
          if (combined_other_remove_count > 1) { 
            naked_triples.push_back(make_tuple(cnt_i, cnt_j, cnt_k, region, subregion,
                                               val[0], val[1], val[2]));
          }
        }
      }
    }
  }

  return naked_triples;
}

triple_vec sudoku_naked_triples_in_region(const Sudoku& s, const Region_t region) {

  triple_vec naked_triples;
  triple_vec subregion_triples;

  for (int i = 0; i < s.region_size; ++i) {  // for each subregion within region
    subregion_triples = sudoku_naked_triples_in_subregion(s, region, i);
    naked_triples.insert(naked_triples.end(), subregion_triples.begin(),
                         subregion_triples.end());
  }

  return naked_triples;
}

triple_vec sudoku_naked_triples(const Sudoku& s) {

  triple_vec naked_triples;
  triple_vec region_triples;

  // collect naked triples from each region
  region_triples = sudoku_naked_triples_in_region(s, Region_t::row);
  naked_triples.insert(naked_triples.end(), region_triples.begin(), region_triples.end());
  region_triples = sudoku_naked_triples_in_region(s, Region_t::col);
  naked_triples.insert(naked_triples.end(), region_triples.begin(), region_triples.end());
  region_triples = sudoku_naked_triples_in_region(s, Region_t::block);
  naked_triples.insert(naked_triples.end(), region_triples.begin(), region_triples.end());

  sort(naked_triples.begin(), naked_triples.end(),
       // sort entries according to 1st element of tuple (cell no.)
       [](const tuple<int, int, int, Region_t, int, int, int, int>& a,
          const tuple<int, int, int, Region_t, int, int, int, int>& b) -> bool {
         return get<0>(a) < get<0>(b);
       });

  // DO NOT erase duplicates for same cell no., since they reflect row-block or row-column
  // interactions (e.g. triple within one block within the same row implies that the
  // candidates can be removed in the other cells of the block as well as of the row
  // (same reasoning is valid for columns)
  //
  // if removed the same naked triple would be found in the next step again
  // from the perspective of the second region involved
  // auto last =
  //     unique(naked_triples.begin(), naked_triples.end(),
  //            // eliminate entries with same 1st element
  //            [](const tuple<int, int, int, Region_t, int, int, int, int>& a,
  //               const tuple<int, int, int, Region_t, int, int, int, int>& b) -> bool {
  //              return get<0>(a) == get<0>(b);
  //            });
  // naked_triples.erase(last, naked_triples.end());  // actually erase elements

  // for (int i = 0; i < naked_triples.size(); ++i) {
  //   cout << "\nFound naked triple in cells no. " << get<0>(naked_triples[i]);
  //   cout << ", " << get<1>(naked_triples[i]);
  //   cout << ", " << get<2>(naked_triples[i]);
  //   cout << ", in region " << int(get<3>(naked_triples[i]));
  //   cout << ", subregion " << get<4>(naked_triples[i]);
  //   cout << ", values " << get<5>(naked_triples[i]);
  //   cout << ", " << get<6>(naked_triples[i]);
  //   cout << ", " << get<7>(naked_triples[i]) << '\n';
  // }
  // if (naked_triples.size() > 0) cout << endl;

  return naked_triples;
}

int sudoku_num_naked_triples(const Sudoku& s) { return sudoku_naked_triples(s).size(); }

bool sudoku_has_naked_triples(const Sudoku& s) { return sudoku_num_naked_triples(s) > 0; }

int sudoku_remove_naked_triples(Sudoku& s) {

  triple_vec naked_triples      = sudoku_naked_triples(s);
  int num_naked_triples_removed = naked_triples.size();

  for (const auto& e : naked_triples) {
    const auto& [cnt1, cnt2, cnt3, region, subregion, val1, val2, val3] = e;
    for (int i = 0; i < s.region_size; ++i) {
      int cnt = s.region_to_cnt(region, subregion, i);
      if (cnt == cnt1 || cnt == cnt2 || cnt == cnt3) {
        continue;  // don't change cells that contain the naked triples
      }
      // for all other cells in subregion erase the triple values from their candidate
      // sets
      s(cnt).cand.erase(val1);
      s(cnt).cand.erase(val2);
      s(cnt).cand.erase(val3);
    }
  }

  return num_naked_triples_removed;
}

//////////////////////////////////////////////////////////////////////////////////////////
// hidden triples
//////////////////////////////////////////////////////////////////////////////////////////

triple_vec sudoku_hidden_triples_in_subregion(const Sudoku& s, const Region_t region,
                                              const int subregion) {
  //
  // hidden triple:
  //
  // if three values occur in three different cells such that they form a size 3 set,
  // then all other candiates in those three cells' candidate sets can be removed,
  // because the three values can only occur in these three cells (defined location)
  //

  triple_vec hidden_triples;

  // set shows which candidate value occurs in which cell of the region
  // index of vector is interpreted as value of candidate (thus index 0 unused)
  // values don't occur because the cell does have it as value are unused as well
  vector<set<int>> occurrence;

  // fill occurrence sets (which candidate value occurs in which cell)
  set<int> empty_set{};
  for (int i = 0; i <= s.region_size; ++i)
    occurrence.push_back(empty_set);  // occurence[0] unused
                                      // occurence[1] holds cell idx which contains 1
                                      // occurence[2] holds cell idx which contains 2
                                      // ...

  for (int i = 0; i < s.region_size; ++i) {  // for each cell in subregion
    int cnt = s.region_to_cnt(region, subregion, i);
    if (s(cnt).val != 0) continue;  // skip filled cells (no candidates left)
    for (int cand_val = 1; cand_val <= s.region_size; ++cand_val) {
      // for each candidate value
      if (s(cnt).cand.count(cand_val) == 1) {
        // cand_value occurs in this cell, so mark it in corresponding occurence set
        occurrence[cand_val].insert(i);
        // cout << "Region: " << int(region) << ", subregion: " << subregion;
        // cout << ", i: " << i << ", cnt: " << cnt;
        // cout << ", s(cnt).val: " << s(cnt).val;
        // cout << ", s(cnt).cand.count(cand_val): " << s(cnt).cand.count(cand_val);
        // cout << ", cand_val: " << cand_val << endl;
      }
    }
  }

  // find out which combined cell occurence sets do only have combined sets of size 3
  // those are potential hidden triples
  for (int cand1 = 1; cand1 <= s.region_size; ++cand1) {  // 1st value
    if (occurrence[cand1].size() == 0) continue;          // skip unused values

    for (int cand2 = cand1 + 1; cand2 <= s.region_size; ++cand2) {  // 2nd value
      if (occurrence[cand2].size() == 0) continue;                  // skip unused values

      set<int> combined_occurrence(occurrence[cand1]);
      combined_occurrence.insert(occurrence[cand2].begin(), occurrence[cand2].end());

      if (combined_occurrence.size() > 3) {  // skip, if combined set is already too large
        continue;
      } else {
        for (int cand3 = cand2 + 1; cand3 <= s.region_size; ++cand3) {  // 3rd value
          if (occurrence[cand3].size() == 0) continue;  // skip unused values
          combined_occurrence.insert(occurrence[cand3].begin(), occurrence[cand3].end());
          if (combined_occurrence.size() != 3 ||
              !pairwise_different_if_size2(
                  occurrence[cand1], occurrence[cand2],
                  occurrence[cand3])) {  // skip, if combined set does not fit
            continue;
          } else {

            // here we have found a hidden triple cand1, cand2, cand3
            int idx1 = *combined_occurrence.begin();
            int idx2 = *next(combined_occurrence.begin());
            int idx3 = *next(next(combined_occurrence.begin()));

            int cnt1 = s.region_to_cnt(region, subregion, idx1);
            int cnt2 = s.region_to_cnt(region, subregion, idx2);
            int cnt3 = s.region_to_cnt(region, subregion, idx3);

            // make sure, that the remaining cells still have candidates left to be
            // removed otherwise don't add this hidden triple to the list of found triples
            set<int> combined_cand{};
            for (int h = 0; h < s.region_size; ++h) {
              int cnt_h = s.region_to_cnt(region, subregion, h);
              if (cnt_h == cnt1 || cnt_h == cnt1 || cnt_h == cnt3) {
                combined_cand.insert(s(cnt_h).cand.begin(), s(cnt_h).cand.end());
              }
            }

            // store naked triple for return
            if (combined_cand.size() > 3) {

              hidden_triples.push_back(
                  make_tuple(cnt1, cnt2, cnt3, region, subregion, cand1, cand2, cand3));
            }
          }
        }
      }
    }
  }

  return hidden_triples;
}

triple_vec sudoku_hidden_triples_in_region(const Sudoku& s, const Region_t region) {

  // vector of tuples with (cell-no., region, subregion, value)
  triple_vec hidden_triples;
  triple_vec subregion_triples;

  for (int i = 0; i < s.region_size; ++i) {  // for each subregion within region
    subregion_triples = sudoku_hidden_triples_in_subregion(s, region, i);
    hidden_triples.insert(hidden_triples.end(), subregion_triples.begin(),
                          subregion_triples.end());
  }

  return hidden_triples;
}

triple_vec sudoku_hidden_triples(const Sudoku& s) {

  // vector of tuples with (cell-no.1, cell-no.2 region, subregion, value1, value2)
  triple_vec hidden_triples;
  triple_vec region_triples;

  // collect hidden triples from each region
  region_triples = sudoku_hidden_triples_in_region(s, Region_t::row);
  hidden_triples.insert(hidden_triples.end(), region_triples.begin(),
                        region_triples.end());
  region_triples = sudoku_hidden_triples_in_region(s, Region_t::col);
  hidden_triples.insert(hidden_triples.end(), region_triples.begin(),
                        region_triples.end());
  region_triples = sudoku_hidden_triples_in_region(s, Region_t::block);
  hidden_triples.insert(hidden_triples.end(), region_triples.begin(),
                        region_triples.end());

  // remove hidden twin entries that occur more than once
  // (same hidden twin can occur from perspective of different regions)
  sort(hidden_triples.begin(), hidden_triples.end(),
       // sort entries according to 1st element of tuple (cell no.)
       [](const tuple<int, int, int, Region_t, int, int, int, int>& a,
          const tuple<int, int, int, Region_t, int, int, int, int>& b) -> bool {
         return get<0>(a) < get<0>(b);
       });

  // erase duplicates for same cell no.
  // (even if the same cell is found from perspective of different regions, the value
  // can only be set once)
  auto last =
      unique(hidden_triples.begin(), hidden_triples.end(),
             // eliminate entries with same 1st element
             [](const tuple<int, int, int, Region_t, int, int, int, int>& a,
                const tuple<int, int, int, Region_t, int, int, int, int>& b) -> bool {
               return get<0>(a) == get<0>(b);
             });
  hidden_triples.erase(last, hidden_triples.end());  // actually erase elements

  // for (int i = 0; i < hidden_triples.size(); ++i) {
  //   cout << "\nFound hidden triple in cells no. " << get<0>(hidden_triples[i]);
  //   cout << ", " << get<1>(hidden_triples[i]);
  //   cout << ", " << get<2>(hidden_triples[i]);
  //   cout << ", in region " << int(get<3>(hidden_triples[i]));
  //   cout << ", subregion " << get<4>(hidden_triples[i]);
  //   cout << ", values " << get<5>(hidden_triples[i]);
  //   cout << ", " << get<6>(hidden_triples[i]);
  //   cout << ", " << get<7>(hidden_triples[i]) << '\n';
  // }
  // if (hidden_triples.size() > 0) cout << endl;

  return hidden_triples;
}

int sudoku_num_hidden_triples(const Sudoku& s) { return sudoku_hidden_triples(s).size(); }

bool sudoku_has_hidden_triples(const Sudoku& s) {
  return sudoku_num_hidden_triples(s) > 0;
}

int sudoku_remove_hidden_triples(Sudoku& s) {

  triple_vec hidden_triples      = sudoku_hidden_triples(s);
  int num_hidden_triples_removed = hidden_triples.size();

  for (const auto& e : hidden_triples) {
    const auto& [cnt1, cnt2, cnt3, region, subregion, val1, val2, val3] = e;

    // in cells s(cnt1), s(cnt2) and s(cnt3) keep only candidates that
    // correspond to val1, val2 and val3
    for (auto iter = s(cnt1).cand.begin(); iter != s(cnt1).cand.end();) {
      if (*iter == val1 || *iter == val2 || *iter == val3) {
        ++iter;
      }  // keep those values
      else {
        iter = s(cnt1).cand.erase(iter);
      }  // get rid of those values
    }
    for (auto iter = s(cnt2).cand.begin(); iter != s(cnt2).cand.end();) {
      if (*iter == val1 || *iter == val2 || *iter == val3) {
        ++iter;
      }  // keep those values
      else {
        iter = s(cnt2).cand.erase(iter);
      }  // get rid of those values
    }
    for (auto iter = s(cnt3).cand.begin(); iter != s(cnt3).cand.end();) {
      if (*iter == val1 || *iter == val2 || *iter == val3) {
        ++iter;
      }  // keep those values
      else {
        iter = s(cnt3).cand.erase(iter);
      }  // get rid of those values
    }
  }

  return num_hidden_triples_removed;
}

//////////////////////////////////////////////////////////////////////////////////////////
// naked quadruples
//////////////////////////////////////////////////////////////////////////////////////////

quad_vec sudoku_naked_quadruples_in_subregion(const Sudoku& s, const Region_t region,
                                              const int subregion) {

  quad_vec naked_quadruples;

  // collect candidates for potential naked quadruples and skip other cells
  for (int i = 0; i < s.region_size; ++i) {  // potential first element of quad
    int cnt_i = s.region_to_cnt(region, subregion, i);
    // skip unsuitable elements
    if (s(cnt_i).val > 0 || s(cnt_i).cand.size() < 2 || s(cnt_i).cand.size() > 4)
      continue;

    for (int j = i + 1; j < s.region_size; ++j) {  // potential second element of quad
      int cnt_j = s.region_to_cnt(region, subregion, j);
      // skip unsuitable elements
      if (s(cnt_j).val > 0 || s(cnt_j).cand.size() < 2 || s(cnt_j).cand.size() > 4)
        continue;

      for (int k = j + 1; k < s.region_size; ++k) {  // potential third element of quad
        int cnt_k = s.region_to_cnt(region, subregion, k);
        // skip unsuitable elements
        if (s(cnt_k).val > 0 || s(cnt_k).cand.size() < 2 || s(cnt_k).cand.size() > 4)
          continue;

        for (int l = k + 1; l < s.region_size; ++l) {  // potential forth element of quad
          int cnt_l = s.region_to_cnt(region, subregion, l);
          // skip unsuitable elements
          if (s(cnt_l).val > 0 || s(cnt_l).cand.size() < 2 || s(cnt_l).cand.size() > 4)
            continue;

          // from here on we can combine candidate sets of the potential quad cells
          // cnt_i, cnt_j, cnt_k, cnt_l, i.e. cells from the current subregion with
          // at least 2 and at most 4 candidates and check whether the combinded set
          // has only 4 candidates (than it is a potential naked quad)

          set<int> combined_cand(s(cnt_i).cand);
          combined_cand.insert(s(cnt_j).cand.begin(), s(cnt_j).cand.end());
          combined_cand.insert(s(cnt_k).cand.begin(), s(cnt_k).cand.end());
          combined_cand.insert(s(cnt_l).cand.begin(), s(cnt_l).cand.end());

          if (combined_cand.size() == 4 &&
              pairwise_different_if_size2(s(cnt_i).cand, s(cnt_j).cand, s(cnt_k).cand,
                                          s(cnt_l).cand)) {
            // found a potential naked quad (pairwise_different_if_size2 is required
            // to make sure that there is a cyclic dependency between the four cells)

            // store quad values
            vector<int> val(combined_cand.begin(), combined_cand.end());

            // make sure, that the remaining cells still have candidates left to be
            // removed otherwise don't add this naked quad to the list of found quadruples
            set<int> combined_other_cand{};
            for (int h = 0; h < s.region_size; ++h) {
              int cnt_h = s.region_to_cnt(region, subregion, h);
              if (cnt_h == cnt_i || cnt_h == cnt_j || cnt_h == cnt_k || cnt_h == cnt_l)
                continue;
              combined_other_cand.insert(s(cnt_h).cand.begin(), s(cnt_h).cand.end());
            }
            int combined_other_remove_count = combined_other_cand.count(val[0]);
            combined_other_remove_count += combined_other_cand.count(val[1]);
            combined_other_remove_count += combined_other_cand.count(val[2]);
            combined_other_remove_count += combined_other_cand.count(val[3]);

            // store naked quad for return
            if (combined_other_remove_count > 1) {
              naked_quadruples.push_back(make_tuple(cnt_i, cnt_j, cnt_k, cnt_l, region,
                                                    subregion, val[0], val[1], val[2],
                                                    val[3]));
            }
          }
        }
      }
    }
  }

  return naked_quadruples;
}

quad_vec sudoku_naked_quadruples_in_region(const Sudoku& s, const Region_t region) {

  quad_vec naked_quadruples;
  quad_vec subregion_quadruples;

  for (int i = 0; i < s.region_size; ++i) {  // for each subregion within region
    subregion_quadruples = sudoku_naked_quadruples_in_subregion(s, region, i);
    naked_quadruples.insert(naked_quadruples.end(), subregion_quadruples.begin(),
                            subregion_quadruples.end());
  }

  return naked_quadruples;
}

quad_vec sudoku_naked_quadruples(const Sudoku& s) {

  quad_vec naked_quadruples;
  quad_vec region_quadruples;

  // collect naked quadruples from each region
  region_quadruples = sudoku_naked_quadruples_in_region(s, Region_t::row);
  naked_quadruples.insert(naked_quadruples.end(), region_quadruples.begin(),
                          region_quadruples.end());
  region_quadruples = sudoku_naked_quadruples_in_region(s, Region_t::col);
  naked_quadruples.insert(naked_quadruples.end(), region_quadruples.begin(),
                          region_quadruples.end());
  region_quadruples = sudoku_naked_quadruples_in_region(s, Region_t::block);
  naked_quadruples.insert(naked_quadruples.end(), region_quadruples.begin(),
                          region_quadruples.end());

  // for (int i = 0; i < naked_quadruples.size(); ++i) {
  //   cout << "\nFound naked quad in cells no. " << get<0>(naked_quadruples[i]);
  //   cout << ", " << get<1>(naked_quadruples[i]);
  //   cout << ", " << get<2>(naked_quadruples[i]);
  //   cout << ", " << get<3>(naked_quadruples[i]);
  //   cout << ", in region " << int(get<4>(naked_quadruples[i]));
  //   cout << ", subregion " << get<5>(naked_quadruples[i]);
  //   cout << ", values " << get<6>(naked_quadruples[i]);
  //   cout << ", " << get<7>(naked_quadruples[i]);
  //   cout << ", " << get<8>(naked_quadruples[i]);
  //   cout << ", " << get<9>(naked_quadruples[i]) << '\n';
  // }
  // if (naked_quadruples.size() > 0) cout << endl;

  return naked_quadruples;
}

int sudoku_num_naked_quadruples(const Sudoku& s) {
  return sudoku_naked_quadruples(s).size();
}

bool sudoku_has_naked_quadruples(const Sudoku& s) {
  return sudoku_num_naked_quadruples(s) > 0;
}

int sudoku_remove_naked_quadruples(Sudoku& s) {

  quad_vec naked_quadruples        = sudoku_naked_quadruples(s);
  int num_naked_quadruples_removed = naked_quadruples.size();

  for (const auto& e : naked_quadruples) {
    const auto& [cnt1, cnt2, cnt3, cnt4, region, subregion, val1, val2, val3, val4] = e;
    for (int i = 0; i < s.region_size; ++i) {
      int cnt = s.region_to_cnt(region, subregion, i);
      if (cnt == cnt1 || cnt == cnt2 || cnt == cnt3 || cnt == cnt4) {
        continue;  // don't change cells that contain the naked quadruples
      }
      // for all other cells in subregion erase the quad values from their candidate
      // sets
      s(cnt).cand.erase(val1);
      s(cnt).cand.erase(val2);
      s(cnt).cand.erase(val3);
      s(cnt).cand.erase(val4);
    }
  }

  return num_naked_quadruples_removed;
}
