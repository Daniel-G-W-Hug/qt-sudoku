// let emacs know this is a C++ header: -*- C++ -*-
// 3456789012345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include "dyn_assert.h"    // dynamic_assert()

#include <iostream>
#include <set>
#include <string>
#include <utility>    // pair<T,T>(), make_pair()
#include <vector>

#include <fmt/format.h>    // friend access for printing

struct Sudoku_cell {
    Sudoku_cell(int t_cnt,             // cell number
                int t_ri, int t_rj,    // row indizes
                int t_ci, int t_cj,    // col indizes
                int t_bi, int t_bj)    // block indizes
        :
        cnt(t_cnt),
        ri(t_ri), rj(t_rj), ci(t_ci), cj(t_cj), bi(t_bi), bj(t_bj) {}

    const int cnt;           // cell index of this cell within sudoku
    const int ri;            // row index the cell belongs to
    const int rj;            // index within the cell's row
    const int ci;            // col index the cell belongs to
    const int cj;            // index within the cell's col
    const int bi;            // block index the cell belongs to
    const int bj;            // index within the cell's block
    int val{0};              // entry value 0: empty indicator; 1..N for set value
    std::set<int> cand{};    // set of remaining candidates for this cell
                             // (=remaining permissible entries)
};

//
// interface class Sudoku
//
// Sudoku keeps track of data structure to represent a sudoku of arbitrary size
//

class Sudoku;    // forward declaration for access class

enum class Region_t { row, col, block };    // region types

// access classes of Sudoku for various access schemes (row, col, block)
class Region_access {

    friend class fmt::formatter<Region_access>;    // allow printing of private members

    Sudoku* const m_s;          // const pointer to non-const Sudoku instance (non-owning)
    const Region_t m_region;    // region type for this instance
    std::vector<std::vector<Sudoku_cell*>> m_rv;    // region vectors (row/col/block)

  public:
    Region_access(Sudoku& t_sref, const Region_t t_region);

    // to be called AFTER Sudoku entries are set up in Sudoku constructor
    // in order to set up access references depending on region type
    void init_access();

    // access to region(i)
    std::vector<Sudoku_cell*>& operator()(int i);
    const std::vector<Sudoku_cell*>& operator()(int i) const;
    // element access at region(i,j)
    Sudoku_cell& operator()(int i, int j);
    const Sudoku_cell& operator()(int i, int j) const;

    // iterator access to region
    using iterator       = std::vector<std::vector<Sudoku_cell*>>::iterator;
    using const_iterator = std::vector<std::vector<Sudoku_cell*>>::const_iterator;
    iterator begin() { return m_rv.begin(); }
    iterator end() { return m_rv.end(); }
    const_iterator cbegin() const { return m_rv.begin(); }
    const_iterator cend() const { return m_rv.end(); }
};

class Sudoku {

    friend class Region_access;             // allow access to regions
    friend class fmt::formatter<Sudoku>;    // allow printing of private members

    std::vector<Sudoku_cell> m_cell;    // contains Sudoku entries

  public:
    const int region_size;       // no. of cells per region (= row / col / block)
    const int blocks_per_row;    // no. of blocks in "x-direction"
    const int blocks_per_col;    // no. of blocks in "y-direction"
    const int total_size;        // total size of sudoku = region_size*region_size

    // provide cell access in various forms for regions
    // (all addressing the same memory)
    Region_access row;
    Region_access col;
    Region_access block;

    // constructors
    Sudoku(int t_region_size, int t_blocks_per_row, int t_blocks_per_col);

    // copy
    Sudoku(const Sudoku& other_Sudoku);
    // copy assignment
    Sudoku& operator=(const Sudoku& other_Sudoku);
    // move
    Sudoku(Sudoku&& other_Sudoku) noexcept;
    // move assignment
    Sudoku& operator=(Sudoku&& other_Sudoku) noexcept;

    // element access
    Sudoku_cell& operator()(int cnt);
    const Sudoku_cell& operator()(int cnt) const;
    Sudoku_cell& operator()(int i, int j);
    const Sudoku_cell& operator()(int i, int j) const;

    // iterator access to Sudoku_cell (delegate to vector<Sudoku_cell> m_cell)
    using iterator       = std::vector<Sudoku_cell>::iterator;
    using const_iterator = std::vector<Sudoku_cell>::const_iterator;
    iterator begin() { return m_cell.begin(); }
    iterator end() { return m_cell.end(); }
    const_iterator cbegin() const { return m_cell.begin(); }
    const_iterator cend() const { return m_cell.end(); }

    // access by index (this is where the mapping happens)
    int row_to_cnt(int i, int j) const;
    int col_to_cnt(int i, int j) const;
    int block_to_cnt(int i, int j) const;
    int region_to_cnt(const Region_t region, int i, int j) const;

    std::pair<int, int> cnt_to_row(int cnt) const;
    std::pair<int, int> cnt_to_col(int cnt) const;
    std::pair<int, int> cnt_to_block(int cnt) const;
    std::pair<int, int> cnt_to_region(const Region_t region, int cnt) const;

    bool cell_is_in_affected_regions(int curr_block, int cnt);

    // helpers for checking index values
    bool is_valid_index(int cnt) const;
    bool is_valid_region_index(int i) const;
    bool is_valid_region_index(int i, int j) const;
};
