// Oliver Kullmann, 21.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Partial squares

  Namespace PartialSquares, abbreviated "PS".

  Internally and for input and output the numbers are 0, ..., N-1.

   - typedef cell_t = vector<bool>

   - set_val(size_t N, size_t val) -> cell_t

   - struct Cell
    - wrapper for cell_t

   - typedef prow_t = vector<Cell>
   - typedef psquare_t = vector<prow_t>

   - functions valid
   - functions consistent
   - functions unit
   - construction functions

   - struct coord
    - wrapper for x, y : size_t
   - typedef std::map<coord, Cell> ps_map_t

   - struct PSquare
    - wrapper for ps : psquare_t, and s : Square

   - typedef psquares = vector<PSquare>
   - struct PSquares
    - wrapper for psqs : psquares_t, size_t N

TODOS:

0. Generalise the input-syntax to allow "maps", via triples
     i, j, Cell :
    - The point is to easily allow to restrict just a few cells (in possibly
      big squares).
    - Per square we either have the current matrix-format, or that map-syntax.
    - E.g.

A
+0 -
0,1  1
B m
0,0, -0,1
1,1, +1,0

    - Here the distinction between the two forms happens via the "m",
      at the end of the name-line.
    - In the matrix-form we have one row per line, in the map-form one
      cell per line.

1. Clarify the relations to Random/LatinSquares.hpp
 - Especially Random::ls_ip_t is similar to PSquares.
 - Though the context is different: in Random/LatinSquares.hpp one is
   concerned with direct computation with latin squares, while here
   we are concerned with representing conditions on (arbitrary) squares.

*/

#ifndef PARTIALSQUARES_xfWDmQELnQ
#define PARTIALSQUARES_xfWDmQELnQ

#include <vector>
#include <algorithm>
#include <istream>
#include <exception>
#include <type_traits>
#include <utility>
#include <numeric>
#include <sstream>
#include <initializer_list>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/NumBasicFunctions.hpp>

#include "Conditions.hpp"

namespace PartialSquares {

  namespace CD = Conditions;

  using size_t = CD::size_t;


  // Index i being true means value i is *disabled*:
  typedef std::vector<bool> cell_t;
  // Exactly one value set:
  cell_t set_val(const size_t N, const size_t val) {
    cell_t res(N, true);
    if (val < N) res[val] = false;
    return res;
  }

  struct Cell {
    cell_t c;

    Cell() noexcept = default;
    Cell(std::initializer_list<bool> L) noexcept : c(L) {}
    Cell(const cell_t c) : c(c) {}
    Cell(const size_t N) : c(N) {} // all N positions enabled
    Cell(const size_t N, const size_t val) : c(set_val(N,val)) {}

    size_t hash() const noexcept {
      size_t seed = c.size();
      for (const bool x : c) FloatingPoint::hash_combine(seed, size_t(x));
      return seed;
    }

    Cell& flip() noexcept { c.flip(); return *this; }

    size_t size() const noexcept { return c.size(); }
    bool consistent() const noexcept {
      return std::ranges::find(c, 0) != c.end();
    }
    // testing for exactly one 0:
    bool unit() const noexcept {
      const auto first = std::ranges::find(c, 0);
      const auto end = c.end();
      if (first == end) return false;
      return std::find(first+1, end, 0) == end;
    }
    // Index of first available slot (c.size() iff c is empty):
    size_t first() const noexcept {
      return std::ranges::find(c, 0) - c.begin();
    }
    size_t elimvals() const noexcept {
      return std::accumulate(c.begin(), c.end(), size_t(0));
    }
    size_t vals() const noexcept { return size() - elimvals(); }
    bool restricted() const noexcept { return elimvals() != 0; }

    // Intersection of cells:
    Cell& operator &=(const Cell& other) noexcept {
      assert(other.size() >= c.size());
      for (size_t i = 0; i < c.size(); ++i) c[i] = c[i] | other.c[i];
      return *this;
    }
    // Union of cells:
    Cell& operator |=(const Cell& other) noexcept {
      assert(other.size() >= c.size());
      for (size_t i = 0; i < c.size(); ++i) c[i] = c[i] & other.c[i];
      return *this;
    }

    void swap(Cell& other) noexcept {
      c.swap(other.c);
    }

    bool operator ==(const Cell&) const noexcept = default;
    auto operator <=>(const Cell&) const noexcept = default;
  };
  void swap(Cell& c1, Cell& c2) noexcept { c1.swap(c2); }
  static_assert(std::is_move_assignable_v<Cell>);
  static_assert(std::is_move_constructible_v<Cell>);
  static_assert(std::is_swappable_v<Cell>);

  Cell flip(Cell c) { c.flip(); return c; }
  Cell operator &(const Cell& c1, const Cell& c2) {
    Cell res(c1); res &= c2; return res;
  }
  Cell operator |(const Cell& c1, const Cell& c2) {
    Cell res(c1); res |= c2; return res;
  }

  std::ostream& operator <<(std::ostream& out, const Cell& c) {
    std::vector<size_t> content;
    for (size_t i = 0; i < c.size(); ++i) if (not c.c[i]) content.push_back(i);
    if (content.empty()) out << "E";
    else Environment::out_line(out, content, ",");
    return out;
  }


  typedef std::vector<Cell> prow_t;
  size_t hash_prow(const prow_t& pr) noexcept {
    return FloatingPoint::hash_UInt_range().apply(pr, [](const auto& x){
                                                    return x.hash();});
  }

  typedef std::vector<prow_t> psquare_t;
  size_t hash_psquare(const psquare_t& ps) noexcept {
    return FloatingPoint::hash_UInt_range().apply(ps, [](const auto& x){
                                                    return hash_prow(x);});
  }

  bool valid(const prow_t& pr, const size_t N) noexcept {
    return pr.size() == N and
      std::ranges::all_of(pr, [&N](const Cell& c){return c.size()==N;});
  }
  bool valid(const prow_t& pr) noexcept {
    return valid(pr, pr.size());
  }
  bool valid(const psquare_t& ps, const size_t N) noexcept {
    return ps.size() == N and
      std::ranges::all_of(ps, [&N](const prow_t& r){return valid(r,N);});
  }
  bool valid(const psquare_t& ps) noexcept {
    return valid(ps, ps.size());
  }

  bool consistent(const prow_t& pr) noexcept {
    return std::ranges::all_of(pr, [](const Cell& c){return c.consistent();});
  }
  bool consistent(const psquare_t& ps) noexcept {
    return std::ranges::all_of(ps, [](const prow_t& r){return consistent(r);});
  }

  bool unit(const prow_t& pr) noexcept {
    return std::ranges::all_of(pr, [](const Cell& c){return c.unit();});
  }
  bool unit(const psquare_t& ps) noexcept {
    return std::ranges::all_of(ps, [](const prow_t& r){return unit(r);});
  }

  prow_t empty_prow(const size_t N) { // "empty" = "all open"
    return prow_t(N, cell_t(N));
  }
  prow_t full_prow(const size_t N) {
    return prow_t(N,cell_t(N,1) );
  }
  psquare_t empty_psquare(const size_t N) {
    return psquare_t(N, empty_prow(N));
  }
  psquare_t full_psquare(const size_t N) {
    return psquare_t(N, full_prow(N));
  }

  void flipm(prow_t& pr) noexcept {
    for (Cell& c : pr) c.flip();
  }
  prow_t flip(prow_t pr) {
    flipm(pr); return pr;
  }
  void flipm(psquare_t& ps) noexcept {
    for (prow_t& pr : ps) flipm(pr);
  }
  psquare_t flip(psquare_t ps) {
    flipm(ps); return ps;
  }


  struct coord {
    size_t x, y;
    bool operator ==(const coord&) const noexcept = default;
    auto operator <=>(const coord&) const noexcept = default;
  };

  typedef std::map<coord, Cell> ps_map_t;
  psquare_t map2ps(const ps_map_t& m, const size_t N) {
    psquare_t res = empty_psquare(N);
    for (const auto& [co, c] : m)
      if (co.x < N and co.y < N) res[co.x][co.y] &= c;
    return res;
  }

  struct PSquare {
    psquare_t ps;
    CD::Square s;

    PSquare(const size_t N) : ps(empty_psquare(N)), s(0) {}
    PSquare(psquare_t ps, const CD::Square s) : ps(ps), s(s) {}
    PSquare(const ps_map_t& m , const size_t N, const CD::Square s) :
      ps(map2ps(m,N)), s(s) {}

    size_t hash() const noexcept {
      size_t seed = hash_psquare(ps);
      FloatingPoint::hash_combine(seed, s.i);
      FloatingPoint::hash_combine(seed, size_t(s.v));
      return seed;
    }

    // inconsistent iff one cell has emtpy domain:
    bool consistent() const noexcept { return PartialSquares::consistent(ps); }
    // whether all cells are set to one value:
    bool unit() const noexcept { return PartialSquares::unit(ps); }
    // count the number of unit-cells (so unit() == true iff
    // unit_count() == number of cells of ps):
    size_t unit_count() const noexcept {
      size_t count = 0;
      for (const auto& row : ps)
        for (const auto& c : row) count += c.unit();
      return count;
    }
    size_t elimvals() const noexcept {
      size_t sum = 0;
      for (const auto& row : ps)
        for (const auto& c : row) sum += c.elimvals();
      return sum;
    }
    bool restricted() const noexcept {
      for (const auto& row : ps)
        for (const auto& c : row) if (c.restricted()) return true;
      return false;
    }
    size_t restricted_count() const noexcept {
      size_t count = 0;
      for (const auto& row : ps)
        for (const auto& c : row) count += c.restricted();
      return count;
    }

    void swap(PSquare& other) noexcept {
      ps.swap(other.ps); std::swap(s, other.s);
    }

    bool operator ==(const PSquare&) const noexcept = default;
    auto operator <=>(const PSquare&) const noexcept = default;
  };
  void swap(PSquare& p1, PSquare& p2) noexcept { p1.swap(p2); }
  static_assert(std::is_move_assignable_v<PSquare>);
  static_assert(std::is_move_constructible_v<PSquare>);
  static_assert(std::is_swappable_v<PSquare>);

  std::ostream& operator <<(std::ostream& out, const PSquare& ps) {
    out << ps.s << "\n";
    const size_t N = ps.ps.size();
    if (N >= 1) {
      const size_t max_width = Environment::printsize(out, N-1);
      Environment::out_lines(out, ps.ps, "\n", " ", max_width);
    }
    return out;
  }

  bool valid(const PSquare& ps, const size_t N) {
    return valid(ps.ps, N);
  }
  void flipm(PSquare& ps) noexcept {
    flipm(ps.ps);
  }
  PSquare flip(const PSquare& ps) noexcept {
    PSquare res(ps);
    flipm(res);
    return res;
  }


  typedef std::vector<PSquare> psquares_t;

  struct PSquares {
    psquares_t psqs;
    const size_t N;

    PSquares(const size_t N, std::istream& in) : psqs(read(in,N)), N(N) {}
    PSquares(const size_t N, std::string in) : psqs(read(in,N)), N(N) {}

    PSquares(const size_t N, const psquares_t p) : psqs(p), N(N) {
      assert(valid());
    }
    bool valid() const noexcept {
      return std::ranges::all_of(psqs, [this](const auto& s){
                                   return PartialSquares::valid(s,N);});
    }
    PSquares(const PSquares&) = default;
    PSquares(PSquares&& rhs) :
      psqs(std::move(rhs.psqs)), N(rhs.N) { assert(N == rhs.N); }
    PSquares& operator =(const PSquares& rhs) {
      assert(N == rhs.N);
      psqs = rhs.psqs;
      return *this;
    }
    PSquares& operator =(PSquares&& rhs) {
      assert(N == rhs.N);
      psqs = std::move(rhs.psqs);
      return *this;
    }

    size_t hash() const noexcept {
      size_t seed = FloatingPoint::hash_UInt_range().apply(psqs,
                      [](const auto& x) {return x.hash();});
      FloatingPoint::hash_combine(seed, N);
      return seed;
    }

    bool empty() const noexcept { return psqs.empty(); }
    size_t size() const noexcept { return psqs.size(); }
    bool consistent() const noexcept {
      return std::ranges::all_of(psqs,
        [](const PSquare& ps){return ps.consistent();});
    }
    bool unit() const noexcept {
      return std::ranges::all_of(psqs,
        [](const PSquare& ps){return ps.unit();});
    }
    size_t unit_count() const noexcept {
      size_t sum = 0;
      for (const auto& ps : psqs) sum += ps.unit_count();
      return sum;
    }
    size_t elimvals() const noexcept {
      size_t sum = 0;
      for (const auto& ps : psqs) sum += ps.elimvals();
      return sum;
    }
    bool restricted() const noexcept {
      return std::ranges::any_of(psqs,
                                 [](const auto& s){return s.restricted();});
    }
    size_t restricted_count() const noexcept {
      size_t count = 0;
      for (const auto& ps : psqs) count += ps.restricted_count();
      return count;
    }

    struct Error : std::runtime_error {
      Error(const std::string s) noexcept :
      std::runtime_error("ERROR[PSquares]: " + s) {}
    };

    static psquares_t read(std::istream& in, const size_t N) {
      const auto lines = Environment::split2_cutoff(in, '\n', '#');
      if (lines.size() % (N+1) != 0) {
        std::ostringstream s;
        s << "The number of lines should be a multiple of N+1=" <<
          N+1 << ", but is " << lines.size() << ".";
        throw Error(s.str());
      }
      const size_t k = lines.size() / (N+1);
      psquares_t res(k, N);

      for (size_t i = 0; i < k; ++i) { // k squares
        const size_t i0 = i*(N+1);
        size_t j = 0;
        const auto osq = CD::Square::read(lines[i0], j);
        if (not osq) {
          std::ostringstream s;
          s << "Invalid name of square number " << i+1 <<
            " in the header-line: \"";
          Environment::out_line(s, lines[i0]);
          s << "\".";
          throw Error(s.str());
        }
        res[i].s = osq.value();
        if (j < lines[i0].size()) {
          std::ostringstream s;
          s << "In square number " << i+1 <<
            " there is content after the name \"" << res[i].s <<
            "\" of the square.";
          throw Error(s.str());
        }

        for (size_t ip = 0; ip < N; ++ip) { // N rows of square i
          const size_t i1 = i0+1 + ip;
          const auto& line = lines[i1];
          if (line.size() != N) {
            std::ostringstream s;
            s << "In square number " << i+1 << " in row " << ip+1 <<
              " there should be exactly " << N << " entries, but there are " <<
              line.size() << ";\n the whole line is: \"";
             Environment::out_line(s, line);
             s << "\".";
             throw Error(s.str());
          }
          for (size_t j = 0; j < N; ++j) { // N entries of line ip
            if (line[j] == "*") continue;
            const bool exclude = line[j][0] == '-';
            const bool sign = exclude or (line[j][0] == '+');
            if (not exclude) res[i].ps[ip][j].flip();
            const auto items = Environment::split(
              sign ? line[j].substr(1) : line[j], ',');
            for (const std::string& item : items) {
              size_t x;
              try {x = FloatingPoint::to_UInt(item);}
              catch (std::exception& e) {
                std::ostringstream s;
                s << "In square number " << i+1 << ", row " << ip+1 <<
                  ", the cell " << j+1 << " is \"" << line[j] << "\".\n" <<
                  " Its item \"" << item << "\" could not"
                  " be parsed as a proper natural number:\n"
                  " The parsing-exception says \"" << e.what() << "\"";
                throw Error(s.str());
              }
              if (x >= N) {
                std::ostringstream s;
                s << "In square number " << i+1 << ", row " << ip+1 <<
                  ", cell " << j+1 << ", the item \"" << item << "\" has "
                  "value " << x << " >= " << N << ".";
                throw Error(s.str());
              }
              res[i].ps[ip][j].c[x] = exclude;
            }
          }
        }
      }
      return res;
    }

    static psquares_t read(std::string in, const size_t N) {
      std::istringstream ss(in);
      return read(ss, N);
    }


    void swap(PSquares& other) noexcept {
      assert(N == other.N);
      psqs.swap(other.psqs);
    }

    bool operator ==(const PSquares&) const noexcept = default;
    auto operator <=>(const PSquares&) const noexcept = default;
  };
  static_assert(std::is_move_assignable_v<PSquares>);
  static_assert(std::is_move_constructible_v<PSquares>);
  void swap(PSquares& p1, PSquares& p2) noexcept { p1.swap(p2); }
  static_assert(std::is_swappable_v<PSquares>);

  std::ostream& operator <<(std::ostream& out, const PSquares& psqs) {
    Environment::out_line(out, psqs.psqs, "");
    return out;
  }

  bool included(const PSquares& ps, const CD::AConditions& ac) noexcept {
    return std::ranges::all_of(ps.psqs, [&ac](const PSquare& p){
                                 return ac.contains(p.s);});
  }

  bool valid(const PSquares& ps, const CD::AConditions& ac) noexcept {
    return included(ps, ac) and ps.consistent();
  }

}

#endif


