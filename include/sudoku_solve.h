// let emacs know this is a C++ header: -*- C++ -*-
// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include "sudoku_class.h"

#include <algorithm>  // std::unique
#include <map>
#include <tuple>

enum Sudoku_solution_t {
  naked_single,
  hidden_single,
  naked_twin,
  hidden_twin,
  naked_triple,
  hidden_triple,
  naked_quadruple,
  enum_count // helper for counting entries in enum
             // (add additional entries before count)
};  // solution types

//////////////////////////////////////////////////////////////////////////////////////////
//  naked_single:  only one candidate left in empty cell
//////////////////////////////////////////////////////////////////////////////////////////
//  hidden_single: candidate occurs only once in all candidate sets in region
//////////////////////////////////////////////////////////////////////////////////////////
//  naked_twin:    Candidate sets of two cells in a region have the same two
//                 entries. Since these two cells must have those values,
//                 these candidate values can be removed in all other candidate
//                 sets of this region.
//////////////////////////////////////////////////////////////////////////////////////////
//  hidden_twin:   if two values occur only twice in the same region and if those two
//                 values occur in two cells together exclusively, then all other
//                 candiates in those two cells' candidate sets can be removed, because
//                 the two values can only occur in these two cells (defined location)
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// solution types for printing
//////////////////////////////////////////////////////////////////////////////////////////
const map<Sudoku_solution_t, string> Sudoku_solution_type{
    {Sudoku_solution_t::naked_single, "single"},
    {Sudoku_solution_t::hidden_single, "hidden single"},
    {Sudoku_solution_t::naked_twin, "twin"},
    {Sudoku_solution_t::hidden_twin, "hidden twin"},
    {Sudoku_solution_t::naked_triple, "triple"},
    {Sudoku_solution_t::hidden_triple, "hidden triple"},
    {Sudoku_solution_t::naked_quadruple, "quadruple"}
};

//////////////////////////////////////////////////////////////////////////////////////////
// solution vectors (used for listing solution types)
//////////////////////////////////////////////////////////////////////////////////////////

// single solution type vector (size() = no. of solutions of this type):
//
// tuple< cnt (=cell no.), region, subregion, val>
using single_vec = vector<tuple<int, Region_t, int, int>>;

// twin solution type vector (size() = no. of solutions of this type):
//
// tuple< cnt1, cnt2, region, subregion, val1, val2>
using twin_vec = vector<tuple<int, int, Region_t, int, int, int>>;

// triple solution type vector (size() = no. of solutions of this type):
//
// tuple< cnt1, cnt2, cnt3, region, subregion, val1, val2, val3>
using triple_vec = vector<tuple<int, int, int, Region_t, int, int, int, int>>;

// quadruple solution type vector (size() = no. of solutions of this type):
//
// tuple< cnt1, cnt2, cnt3, cnt4, region, subregion, val1, val2, val3, val4>
using quad_vec = vector<tuple<int, int, int, int, Region_t, int, int, int, int, int>>;

//////////////////////////////////////////////////////////////////////////////////////////
//  routines to solve Sudoku
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
// update candidate lists
//////////////////////////////////////////////////////////////////////////////////////////
void sudoku_update_candidates_cell(Sudoku& s, int cnt);
void sudoku_update_candidates_affected_by_cell(Sudoku& s, int cnt);
void sudoku_update_candidates_all_cells(Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// validation of sudoku
//////////////////////////////////////////////////////////////////////////////////////////
// check for valid and unique entries in region
bool sudoku_has_unique_entries_in_region(const Sudoku& s, const Region_t region);
bool sudoku_is_valid(const Sudoku& s);  // valid and unique entries in each region
int sudoku_num_entries(
    const Sudoku& s);                   // return no. of entries != 0 (non empty entries)
int sudoku_num_empty(const Sudoku& s);  // return no. of entries == 0 (empty entries)

// has_candidates is true, if for entries == 0 there are still candidates available
int sudoku_num_candidates(const Sudoku& s);
bool sudoku_has_candidates(const Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// naked singles
//////////////////////////////////////////////////////////////////////////////////////////
single_vec sudoku_naked_singles(const Sudoku& s);
int sudoku_num_naked_singles(const Sudoku& s);
bool sudoku_has_naked_singles(const Sudoku& s);
int sudoku_remove_naked_singles(Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// hidden singles
//////////////////////////////////////////////////////////////////////////////////////////
single_vec sudoku_hidden_singles_in_subregion(const Sudoku& s, const Region_t region,
                                              const int subregion);
single_vec sudoku_hidden_singles_in_region(const Sudoku& s, const Region_t region);
single_vec sudoku_hidden_singles(const Sudoku& s);
int sudoku_num_hidden_singles(const Sudoku& s);
bool sudoku_has_hidden_singles(const Sudoku& s);
int sudoku_remove_hidden_singles(Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// naked twins
//////////////////////////////////////////////////////////////////////////////////////////
twin_vec sudoku_naked_twins_in_subregion(const Sudoku& s, const Region_t region,
                                         const int subregion);
twin_vec sudoku_naked_twins_in_region(const Sudoku& s, const Region_t region);
twin_vec sudoku_naked_twins(const Sudoku& s);
int sudoku_num_naked_twins(const Sudoku& s);
bool sudoku_has_naked_twins(const Sudoku& s);
// solution steps (return no. of removed naked twins)
// just reduces candidate lists of cells in region, does not fill in cells
int sudoku_remove_naked_twins(Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// hidden twins
//////////////////////////////////////////////////////////////////////////////////////////
twin_vec sudoku_hidden_twins_in_subregion(const Sudoku& s, const Region_t region,
                                          const int subregion);
twin_vec sudoku_hidden_twins_in_region(const Sudoku& s, const Region_t region);
twin_vec sudoku_hidden_twins(const Sudoku& s);
int sudoku_num_hidden_twins(const Sudoku& s);
bool sudoku_has_hidden_twins(const Sudoku& s);
// solution steps (return no. of removed hidden twins)
// just reduces candidate lists of cells in region, does not fill in cells
int sudoku_remove_hidden_twins(Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// naked triples
//////////////////////////////////////////////////////////////////////////////////////////
triple_vec sudoku_naked_triples_in_subregion(const Sudoku& s, const Region_t region,
                                             const int subregion);
triple_vec sudoku_naked_triples_in_region(const Sudoku& s, const Region_t region);
triple_vec sudoku_naked_triples(const Sudoku& s);
int sudoku_num_naked_triples(const Sudoku& s);
bool sudoku_has_naked_triples(const Sudoku& s);
// solution steps (return no. of removed naked triples)
// just reduces candidate lists of cells in region, does not fill in cells
int sudoku_remove_naked_triples(Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// hidden triples
//////////////////////////////////////////////////////////////////////////////////////////
triple_vec sudoku_hidden_triples_in_subregion(const Sudoku& s, const Region_t region,
                                              const int subregion);
triple_vec sudoku_hidden_triples_in_region(const Sudoku& s, const Region_t region);
triple_vec sudoku_hidden_triples(const Sudoku& s);
int sudoku_num_hidden_triples(const Sudoku& s);
bool sudoku_has_hidden_triples(const Sudoku& s);
// solution steps (return no. of removed hidden triples)
// just reduces candidate lists of cells in region, does not fill in cells
int sudoku_remove_hidden_triples(Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// naked quadruples
//////////////////////////////////////////////////////////////////////////////////////////
quad_vec sudoku_naked_quadruples_in_subregion(const Sudoku& s, const Region_t region,
					      const int subregion);
quad_vec sudoku_naked_quadruples_in_region(const Sudoku& s, const Region_t region);
quad_vec sudoku_naked_quadruples(const Sudoku& s);
int sudoku_num_naked_quadruples(const Sudoku& s);
bool sudoku_has_naked_quadruples(const Sudoku& s);
// solution steps (return no. of removed naked quadruples)
// just reduces candidate lists of cells in region, does not fill in cells
int sudoku_remove_naked_quadruples(Sudoku& s);

//////////////////////////////////////////////////////////////////////////////////////////
// remove all types of singles, twins, etc. automatically
//////////////////////////////////////////////////////////////////////////////////////////
int sudoku_remove_all(Sudoku& s);