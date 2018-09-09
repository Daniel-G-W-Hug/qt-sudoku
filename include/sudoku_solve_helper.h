// let emacs know this is a C++ header: -*- C++ -*-
// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include "sudoku_class.h"

#include <algorithm> // equal()
#include <iterator>  // prev(), next()
#include <list>
#include <set>
#include <tuple>
#include <vector>

using std::list;
using std::multiset;
using std::set;
using std::vector;

//
// helper routines provinding general or recurring functionality for
// "sudoku_solve.cpp"
//

// concatenate candidate sets in subregion
multiset<int> sudoku_concatenate_candidate_sets_in_subregion(const Sudoku& s,
                                                             const Region_t region,
                                                             const int subregion);

// count how often which sudoku entry occurs in list
vector<int> sudoku_count_candidate_entries_in_subregion(const Sudoku& s,
                                                        const Region_t region,
                                                        const int subregion);

// store index where elements occur in current subregion
vector<list<int>>
sudoku_candidate_positions_in_subregion(const Sudoku& s, const Region_t region,
                                        const int subregion,
                                        const vector<int>& candidate_count);

// test whether sets are pairwise different (either different size() or different
// content) if size of a set is 2
bool pairwise_different_if_size2(const set<int>& a, const set<int>& b, const set<int>& c);
bool pairwise_different_if_size2(const set<int>& a, const set<int>& b,
                                 const set<int>& c, const set<int>& d);
