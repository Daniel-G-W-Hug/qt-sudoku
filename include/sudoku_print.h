// let emacs know this is a C++ header: -*- C++ -*-
// 345678901234567890123456789012345678901234567890123456789012345678901234567890

#pragma once

#include <iomanip>    // setw()
#include <iostream>
#include <string>
#include <memory>     // address_of
#include <utility>    // pair<T,T>().first/.second
#include <list>
#include <set>
#include <vector>
#include <iterator>        // prev(), next()
#include <fmt/format.h>    // formating & printing
#include <fmt/ranges.h>    // support for vectors, sets, etc.

#include "sudoku_class.h"

//
//  printing routines for class sudoku
//  (made friend of Sudoku in Sudoku_class.h)
//

void sudoku_print(const Sudoku& s, const std::string msg);
void sudoku_print_regions(const Sudoku& s, const std::string msg);
void sudoku_print_cnt_to_x(const Sudoku& s, const std::string msg);
void sudoku_print_candidates(const Sudoku& s, const std::string msg,
                             const Region_t region = Region_t::row);

//
// for debugging
//

void print_vector_int(const std::vector<int>& v);
void print_vector_list_int(const std::vector<std::list<int>>& v);

void print_list_int(const std::list<int>& l);
void print_list_list_int(const std::list<std::list<int>>& ll);

//
// extend fmt to print my types
//
template <> struct fmt::formatter<Sudoku_cell> {
    template <typename ParseContext> constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Sudoku_cell& c, FormatContext& ctx) {
        // ctx.out() is an output iterator to write to.
        auto out = ctx.out();

        out = format_to(out, "Sudoku_cell({}):\n", c.cnt);
        out = format_to(out, "  val = {}\n", c.val);
        out = format_to(out, "  ri = {}, rj = {}\n", c.ri, c.rj);
        out = format_to(out, "  ci = {}, cj = {}\n", c.ci, c.cj);
        out = format_to(out, "  bi = {}, bj = {}\n", c.bi, c.bj);
        out = format_to(out, "  cand = {}", c.cand);

        return out;
    }
};

template <> struct fmt::formatter<Region_t> : formatter<string_view> {
    // parse is inherited from formatter<string_view>.
    template <typename FormatContext> auto format(const Region_t& r, FormatContext& ctx) {
        string_view name = "unknown";
        switch (r) {
            case Region_t::row: name = "row"; break;
            case Region_t::col: name = "col"; break;
            case Region_t::block: name = "block"; break;
        }
        return formatter<string_view>::format(name, ctx);
    }
};

template <> struct fmt::formatter<Region_access> {
    template <typename ParseContext> constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const Region_access& ra, FormatContext& ctx) {
        // ctx.out() is an output iterator to write to.
        auto out = ctx.out();

        // friend class fmt::formatter<Region_access>;	// allow printing of private
        // members (in sudoku_class.h)
        out = format_to(out, "address of Sudoku: {}\n", std::addressof(ra.m_s));
        out = format_to(out, "  m_region = {}\n", ra.m_region);
        out = format_to(out, "  m_rv = {}", ra.m_rv);

        return out;
    }
};
