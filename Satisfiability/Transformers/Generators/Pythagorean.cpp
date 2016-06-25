// Oliver Kullmann, 12.6.2016 (Swansea)
/* Copyright 2016 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Pythagorean.cpp
  \brief CNF generator for the Pythagorean triples/tuples problem

  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Pythagorean.cpp

  For the boolean problems for triples, use
  > Pythagorean n 3 0 2
  or
  > Pythagorean n 3 1 2
  (doesn't matter here).

  For the boolean problem for quadruples, use
  > Pythagorean n 4 0 2
  while the injective form (all components different) is obtained by
  > Pythagorean n 4 1 2

  The second parameter is K >= 3, the length of the Pythagorean tuple.

  For the third parameter d >= 0, also larger values can be used, which enforce
  a respective minimum distance between the (sorted) components of the tuples

    x_1^2 + ... + x_{K-1}^2 = x_K^2,

  that is, x_i + d <= x_{i+1} for 1 <= i <= K-1.

  The fourth parameter m >= 0 is the number of colours, with
   - m = 0: only output the max-occuring vertex and the number of hyperedges
   - m = 1: output the hypergraph
   - m = 2: output the boolean problem (the default)
   - m >= 3: currently uses the strong direct translation.

  An optional fifth parameter can be "-", in which case output is put to
  standard output, or "filename", in which case a file is created.
  Default output is to file "Pyth_n_K_d_m.cnf".

  Subsumption-elimination is first applied, and then for m >= 2 elimination
  of hyperedges containing a vertex occurring at most m-1 times (repeatedly).

  The output contains additional information for m >= 1:
   - Library and version information.
   - Information on parameters.
   - Information on number of hyperedges.
   - Information on vertex-degrees (m=1) resp. variable-degrees (m >= 2).

  Requires C++11. Compile with

  > g++ -Wall --std=c++11 -Ofast -o Pythagorean Pythagorean.cpp

  resp.

  > g++ -Wall --std=c++11 -Ofast -DNDEBUG -o Pythagorean Pythagorean.cpp

  TODO: implement intelligent methods for K>3.
  TODO: prove that subsumption-elimination does not happen for K=4.
  TODO: implement arbitrary K.

  Hyperedge-counting links:
   - https://oeis.org/A224921 for number of Pythagorean triples (K=3)
     up to n-1.
   - Number of Pythagorean quadruples (K=4) or quintuples (K=5) not in OEIS.

  Pythagorean numbers established (in square brackets [h;c], number of
  hyperedges and number of clauses, without reductions):
   - Ptn(3,3) = 7825 [9,472; 18,944]
       http://cs.swan.ac.uk/~csoliver/papers.html#PYTHAGOREAN2016C
   - Ptn(3,3,3) > 2000000 [4,181,998; 19,504,238]
     (g2wsat, first run with cutoff=30000000)
   - Ptn(4,4) = 105 [639; 1278] (known)
   - Ptn_i(4,4) = 163 [545; 1090]
   - Ptn(4,4,4) > 1680 [158,627; 482,601]
     (vw1 with "2 1 0 6540594 3535491316"); 1681 [158,837; 483,235] hard to
     satisfy; weak conjecture = 1681.
   - Ptn(5,5) = 37 [404; 808] (known)
   - Ptn_i(5,5) = 75 [2,276; 4,552]
   - Ptn(5,5,5) = 191 [46,633; 140,663]
     (vw1 for 190, found easily; C&C via SplittingViaOKsolver
     with D=20 and minisat-2.2.0 for 191: total run-time around 46 min).
   - Ptn_i(5,5,5) > 370 [309,239; 929,197]
     g2wsat with "900 1 0 131253 3996273475".
     380 [334,548; 1,005,164] hard to satisfy (-cutoff 800000 -runs 2000).
   - Ptn(6,6) = 23 [311; 622] (known)
   - Ptn_i(6,6) = 61 [6,770; 13,540]
   - Ptn(6,6,6) > 120 [154,860; 465,060] (C&C with D=25 as above)
     <= 122 [165,477; 496,919] (C&C with D=25 and solver=
     "lingelingala-b02aa1a-121013", 2,000 min).
   - Ptn(7,7) = 18 [306; 612] (known)
   - Ptn_i(7,7) = 65 [44,589; 89,178]

   The sequence Ptn(k,k) for k=2,..., (which is 1, 7825, 105, 37, 23, 18, ...)
   is https://oeis.org/A250026 .

   The sequence Ptn_i(k,k) for k=3,..., is also of interest (7825,163,75,61,
   65).

   The sequence Ptn(k,k,k) for k=3,... (?,?,191,?) might be a bit doable
   except of the first two terms.

*/

#include <iostream>
#include <string>
#include <limits>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <forward_list>
#include <ctime>


namespace Pythagorean {

  // Counting triples:
  template <typename C1, typename C2>
  void triples_c(const C1 n, C1& max, C2& hn) noexcept {
    for (C1 r = 2; r <= C1(n/(1+std::sqrt(2))); r+=2) {
      const C1 rs = r*r/2;
      for (C1 s = 1; s <= C1(std::sqrt(rs)); ++s)
        if (rs % s == 0) {
          const C1 t = rs / s;
          const C1 c = r+s+t;
          if (c <= n) {
            max = std::max(max,c);
            ++hn;
          }
        }
    }
  }

  // Counting triples with minimum distance between (sorted) components:
  template <typename C1, typename C2>
  void triples_c(const C1 n, const C1 dist, C1& max, C2& hn) noexcept {
    assert(dist >= 2);
    for (C1 r = 2; r <= C1(n/(1+std::sqrt(2))); r+=2) {
      const C1 rs = r*r/2;
      for (C1 s = dist; s <= C1(std::sqrt(rs)); ++s)
        if (rs % s == 0) {
          const C1 t = rs / s;
          const C1 c = r+s+t;
          if (c <= n and s+dist <= t) {
            max = std::max(max,c);
            ++hn;
          }
        }
    }
  }
  // Enumerating triples:
  template <class V, typename C1>
  V triples_e(const C1 n) {
    V res;
    for (C1 r = 2; r <= C1(n/(1+std::sqrt(2))); r+=2) {
      const C1 rs = r*r/2;
      for (C1 s = 1; s <= C1(std::sqrt(rs)); ++s)
        if (rs % s == 0) {
          const C1 t = rs / s;
          const C1 c = r+s+t;
          if (c <= n) res.push_back({{r+s,r+t,c}});
        }
    }
    return res;
  }
  template <class V, typename C1>
  V triples_e(const C1 n, const C1 dist) {
    assert(dist >= 2);
    V res;
    for (C1 r = 2; r <= C1(n/(1+std::sqrt(2))); r+=2) {
      const C1 rs = r*r/2;
      for (C1 s = dist; s <= C1(std::sqrt(rs)); ++s)
        if (rs % s == 0) {
          const C1 t = rs / s;
          const C1 c = r+s+t;
          if (c <= n and s+dist <= t) res.push_back({{r+s,r+t,c}});
        }
    }
    return res;
  }
}

namespace Container {

  template <class C>
  void remove_empty_elements(C& v) noexcept {
    typedef typename C::value_type val_t;
    v.erase(std::remove_if(v.begin(), v.end(),
              [](const val_t& x){return x.empty();}), v.end());
  }

}

namespace Subsumption {

  // Selecting some element from a non-empty sequence:
  template <class V>
  inline typename V::value_type select(const V& t) noexcept {
    assert(not t.empty());
    return t[0];
  }
  // In general a random choice is better for algorithm min_elements, however
  // here it is not worth the effort.

  // Remove subsumed elements from vector v, where the elements of v are
  // ordered sequences of unsigned integral type C1, with maximal value max:
  template <class V, typename C1>
  void min_elements(V& v, const C1 max) {
    if (v.empty()) return;
    const auto begin = v.begin(), end = v.end();
    typedef typename V::value_type tuple_t;
    std::sort(begin, end,
      [](const tuple_t& x, const tuple_t& y) {return x.size() < y.size();});
    typedef typename V::iterator it_t;
    std::vector<std::forward_list<it_t>> occ(max+1);
    it_t i = begin;
    for (const auto size = begin->size(); i != end and i->size()==size; ++i)
      occ[select(*i)].push_front(i);
    if (i == end) return;
    while (true) {
      const auto size = i->size();
      const it_t old_i = i;
      for (; i < end and i->size()==size; ++i) {
        for (const auto x : *i)
          for (const auto j : occ[x])
            if (std::includes(i->begin(),i->end(), j->begin(),j->end())) {
              i->clear();
              goto Outer;
            }
        Outer :;
      }
      if (i == end) break;
      for (auto j = old_i; j != i; ++j)
        if (not j->empty()) occ[select(*j)].push_front(j);
    }
    Container::remove_empty_elements(v);
  }

}

namespace Reduction {

  // Iteratively removing all hyperedges containing some vertex occurring
  // at most m-1 time, computing the (final) vertex-degrees in deg:
  template <class V, class SV, typename C1>
  void basic_colour_red(V& hyp, SV& deg, const C1 m) noexcept {
    for (const auto& h : hyp) for (const auto v : h) ++deg[v];
    if (m <= 1) return;
    typedef typename V::value_type tuple_t;
    typedef typename tuple_t::value_type vert_t;
    bool changed;
    do {
      changed = false;
      for (auto& h : hyp)
        if (std::find_if(h.begin(), h.end(), [&](const vert_t v){return deg[v] < m;}) != h.end()) {
          changed = true;
          for (const auto v : h) --deg[v];
          h.clear();
        }
    } while (changed);
    Container::remove_empty_elements(hyp);
  }

}

namespace {

  typedef unsigned long int uint_t; // vertices and variables
  typedef unsigned long long int cnum_t; // hyperedge/clause indices/numbers

  enum {
    errcode_parameter = 1,
    errcode_too_large = 2,
    errcode_too_small = 3,
    errcode_not_yet = 4,
    errcode_file = 5
  };

  const std::string program = "Pythagorean";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.6.1";

  const std::string filename = "Pyth_";

  typedef std::vector<uint_t> tuple_t;
  typedef std::vector<tuple_t> hypergraph;

  void oklib_output(std::ostream* const out) {
    assert(*out);
    *out << "c OKlibrary http://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Pythagorean.cpp\n"
    "c  Program " << program << ".cpp in version " << version << ", timestamp " << std::time(nullptr) << ".\n";
  }

  void header_output(std::ostream* const out, const uint_t n, const uint_t K, const uint_t dist, const uint_t m, const std::string& file) {
    assert(*out);
    assert(m >= 1);
    oklib_output(out);
    *out << "c Parameters: " << n << " " << K << " " << dist << " " << m << " \"" << file << "\"\n";
    switch (m) {
    case 1 :
      *out << "c Hypergraph of Pythagorean " << K << "-tuples, up to n=" << n << ".\n";
      break;
    case 2 :
      *out << "c Boolean Pythagorean " << K << "-tuples problem, up to n=" << n << ".\n";
      break;
    default :
      *out << "c " << m << "-Colour Pythagorean " << K << "-tuples problem, up to n=" << n << ".\n";
    }
    if (dist > 0)
      *out << "c  Minimum-distance between (sorted) tuple-components = " << dist << ".\n";
  }

  void pline_output(std::ostream* const out, const uint_t n, const cnum_t c, const uint_t m) {
    assert(*out);
    assert(m >= 1);
    switch (m) {
    case 1 :
      *out << "p hyp " << n << " " << c << "\n"; break;
    default :
      *out << "p cnf " << n << " " << c << "\n";
    }
  }

  void hn_output(std::ostream* const out, const cnum_t h0, const cnum_t h1, const cnum_t h2, const uint_t m) {
    assert(*out);
    assert(h0 >= 1);
    if (m == 1)
      *out << "c Number of hyperedges (tuples) originally and after subsumption:\nc  "
        << h0 << " " << h1 << "\n";
    else
      *out << "c Number of hyperedges (tuples) originally, after subsumption, and after further colour-reduction:\nc  "
        << h0 << " " << h1 << " " << h2 << "\n";
  }

  template <class Vec>
  void count_output(std::ostream* const out, const cnum_t diff, const Vec& v, const uint_t K) {
    assert(*out);
    *out << "c Removed " << diff << " hyperedges.\n";
    if (K == 3) return;
    *out << "c Hyperedge counts:\n";
    for (typename Vec::size_type k=0; k < v.size(); ++k) {
      const auto c = v[k];
      if (c != 0)
        *out << "c  size " << k << ": " << c << "\n";
    }
  }

  inline cnum_t var_number(const uint_t i, const uint_t m, const uint_t col) noexcept {
    assert(i >= 1);
    assert(col < m);
    return cnum_t(i-1) * m + col + 1;
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc <= 4 or argc >= 7) {
    std::cerr << err << "Four or five arguments are needed:\n"
     " - The number n >= 0 of elements.\n"
     " - The size K >= 3 of the tuple.\n"
     " - The enforced distance d >= 0 between components.\n"
     " - The number m >= 0 of colours.\n"
     " - The filename or \"-\" for standard output.";
    return errcode_parameter;
  }

  const uint_t n = std::stoul(argv[1]);

  const uint_t K = std::stoul(argv[2]);
  if (K < 3) {
    std::cerr << err << "Second input " << K << " must be at least 3.\n";
    return errcode_too_small;
  }
  if ( K > 7) {
    std::cerr << err << "Second input " << K << " currently must be at most 7.\n";
    return errcode_not_yet;
  }
  const uint_t abs_max = uint_t(std::sqrt(std::numeric_limits<uint_t>::max())) / K;
  if (n > abs_max) {
    std::cerr << err << "First input " << n << " larger than maximal allowed value: " << abs_max << ".\n";
    return errcode_too_large;
  }

  const uint_t dist = std::stoul(argv[3]);
  if (dist > abs_max) {
    std::cerr << err << "Third input " << dist << " larger than maximal allowed value: " << abs_max << ".\n";
    return errcode_too_large;
  }

  const uint_t m = std::stoul(argv[4]);

  const std::string file = (argc == 5) ?
    filename + std::to_string(n) + "-" + std::to_string(K) + "-" + std::to_string(dist) + "-" + std::to_string(m) + ".cnf"
    : argv[5];
  const bool del = (file != "-");
  std::ostream* const out = (del) ? new std::ofstream(file) : &std::cout;
  class Delete_wrapper {
    const std::ostream* const p;
  public :
    Delete_wrapper(const std::ostream* const out) : p(out) {}
    ~Delete_wrapper() {delete p;}
  } const D((del) ? out : nullptr);
  if (not *out) {
    std::cerr << err << "Couldn't open file " << file << " for writing.\n";
    return errcode_file;
  }

  // Computing the list of Pythagorean tuples:
  hypergraph res;
  cnum_t hn = 0;
  uint_t max = 0;

  if (K == 3) {
    if (m == 0)
      if (dist <= 1) Pythagorean::triples_c(n, max, hn);
      else Pythagorean::triples_c(n, dist, max, hn);
    else
      if (dist <= 1) res = Pythagorean::triples_e<hypergraph>(n);
      else res = Pythagorean::triples_e<hypergraph>(n, dist);
  }
  else if (K == 4) {
    const uint_t n2 = n*n;
    for (uint_t a = 1; a < n; ++a) {
      const uint_t a2 = a*a;
      for (uint_t b = a+dist; b < n; ++b) {
        const uint_t b2 = b*b;
        for (uint_t c = b+dist; c < n; ++c) {
          const uint_t c2 = c*c;
          const uint_t d2 = a2 + b2 + c2;
          if (d2 > n2) break;
          const uint_t d = std::sqrt(d2);
          if (d*d != d2) continue;
          if (d < c+dist) continue;
          if (m == 0) {
            max = std::max(max,d);
            ++hn;
          }
          else res.push_back({{a,b,c,d}});
        }
      }
    }
  }
  else if (K == 5) {
    const uint_t n2 = n*n;
    for (uint_t a = 1; a < n; ++a) {
      const uint_t a2 = a*a;
      for (uint_t b = a+dist; b < n; ++b) {
        const uint_t b2 = b*b;
        for (uint_t c = b+dist; c < n; ++c) {
          const uint_t c2 = c*c;
          for (uint d = c+dist; d < n; ++d) {
            const uint_t d2 = d*d;
            const uint_t e2 = a2 + b2 + c2 + d2;
            if (e2 > n2) break;
            const uint_t e = std::sqrt(e2);
            if (e*e != e2) continue;
            if (e < d+dist) continue;
            if (m == 0) {
              max = std::max(max,e);
              ++hn;
            }
            else res.push_back({{a,b,c,d,e}});
          }
        }
      }
    }
  }
  else if (K == 6) {
    const uint_t n2 = n*n;
    for (uint_t a = 1; a < n; ++a) {
      const uint_t a2 = a*a;
      for (uint_t b = a+dist; b < n; ++b) {
        const uint_t b2 = b*b;
        for (uint_t c = b+dist; c < n; ++c) {
          const uint_t c2 = c*c;
          for (uint d = c+dist; d < n; ++d) {
            const uint_t d2 = d*d;
            for (uint e = d+dist; e < n; ++e) {
              const uint e2 = e*e;
              const uint_t f2 = a2 + b2 + c2 + d2 + e2;
              if (f2 > n2) break;
              const uint_t f = std::sqrt(f2);
              if (f*f != f2) continue;
              if (f < e+dist) continue;
              if (m == 0) {
                max = std::max(max,f);
                ++hn;
              }
              else res.push_back({{a,b,c,d,e,f}});
            }
          }
        }
      }
    }
  } else {
    const uint_t n2 = n*n;
    for (uint_t a = 1; a < n; ++a) {
      const uint_t a2 = a*a;
      for (uint_t b = a+dist; b < n; ++b) {
        const uint_t b2 = b*b;
        for (uint_t c = b+dist; c < n; ++c) {
          const uint_t c2 = c*c;
          for (uint d = c+dist; d < n; ++d) {
            const uint_t d2 = d*d;
            for (uint e = d+dist; e < n; ++e) {
              const uint e2 = e*e;
              for (uint f = e+dist; f < n; ++f) {
                const uint f2 = f*f;
                const uint_t g2 = a2 + b2 + c2 + d2 + e2 + f2;
                if (g2 > n2) break;
                const uint_t g = std::sqrt(g2);
                if (g*g != g2) continue;
                if (g < f+dist) continue;
                if (m == 0) {
                  max = std::max(max,g);
                  ++hn;
                }
                else res.push_back({{a,b,c,d,e,f,g}});
              }
            }
          }
        }
      }
    }
  }

  if (m == 0) {
    *out << max << " " << hn << "\n";
    return 0;
  }
  header_output(out, n, K, dist, m, file);

  const cnum_t orig_hn = res.size();
  if (orig_hn == 0) {
    pline_output(out, 0, 0, m);
    return 0;
  }

  // removing duplicates:
  for (auto& x : res) x.erase(std::unique(x.begin(), x.end()), x.end());

  if (K >= 5 and dist == 0) Subsumption::min_elements(res,n);

  // anti-lexicographical sorting:
  std::sort(res.begin(), res.end(),
    [](const tuple_t& x, const tuple_t& y) {
      return std::lexicographical_compare(x.rbegin(), x.rend(), y.rbegin(), y.rend());
    }
  );

  const cnum_t after_subs_hn = res.size();
  const cnum_t after_subs_max = res.back().back();

  typedef std::vector<cnum_t> stat_vec_t;
  stat_vec_t degree(after_subs_max+1, 0);
  Reduction::basic_colour_red(res, degree, m);

  hn = res.size();
  hn_output(out, orig_hn, after_subs_hn, hn, m);
  if (hn == 0) {
    pline_output(out, 0, 0, m);
    return 0;
  }
  max = res.back().back();

  stat_vec_t counts(K+1,0);
  for (const auto& x : res) ++counts[x.size()];
  cnum_t occ_n = 0, min_d = -1, max_d = 0, sum_d = 0;
  uint_t min_v = 0, max_v = 0;
  for (stat_vec_t::size_type i = 1; i < degree.size(); ++i) {
    const auto deg = degree[i];
    if (deg != 0) {
      ++occ_n; sum_d += deg;
      if (deg < min_d) {min_d = deg; min_v = i;}
      if (deg > max_d) {max_d = deg; max_v = i;}
    }
  }
  assert(occ_n >= 1);

  count_output(out, orig_hn-hn, counts, K);

  if (m == 1) {
    *out << "c Number of occurring vertices = " << occ_n << ".\n";
    *out << "c Minimum degree = " << min_d << ", attained for vertex " << min_v << ".\n";
    *out << "c Maximum degree = " << max_d << ", attained for vertex " << max_v << ".\n";
    *out << "c Average degree = " << double(sum_d) / occ_n << ".\n";
    pline_output(out, max, hn, m);
    for (const auto& x : res) {
      for (const auto i : x) *out << i << " ";
      *out << "0\n";
    }
  }
  else if (m == 2) {// DIMACS output:
    *out << "c Number of occurring variables = " << occ_n << ".\n";
    *out << "c Minimum degree = " << 2*min_d << ", attained for variable " << min_v << ".\n";
    *out << "c Maximum degree = " << 2*max_d << ", attained for variable " << max_v << ".\n";
    *out << "c Average degree = " << 2*double(sum_d) / occ_n << ".\n";
    const cnum_t cn = 2 * hn;
    pline_output(out, max, cn, m);
    for (const auto& x : res) {
      for (const auto i : x) *out << i << " "; *out << "0 ";
      for (const auto i : x) *out << "-" << i << " "; *out << "0\n";
    }
  } else {
    assert(m >= 3);
    *out << "c Using the strong direct translation.\n";
    *out << "c Number of occurring variables = " << m*occ_n << ".\n";
    *out << "c Degrees, ignoring the ALOAMO-clauses:\n";
    *out << "c  Minimum = " << min_d << ", attained for vertex " << min_v << " (variables";
    for (uint_t col = 0; col < m; ++col) *out << " " << var_number(min_v,m,col);
    *out << ").\n";
    *out << "c  Maximum = " << max_d << ", attained for vertex " << max_v << " (variables";
    for (uint_t col = 0; col < m; ++col) *out << " " << var_number(max_v,m,col);
    *out << ").\n";
    *out << "c  Average degree = " << double(sum_d) / occ_n << ".\n";
    const cnum_t cn = m * hn + occ_n * (1 + (m * (m - 1)) / 2);
    const cnum_t vn = m * cnum_t(max);
    pline_output(out, vn, cn, m);
    for (const auto& x : res) {
      for (uint_t col = 0; col < m; ++col) {
        for (const auto i : x) *out << var_number(i,m,col) << " ";
        *out << "0"; if (col != m-1) *out << " ";
      }
      *out << "\n";
    }
    for (uint_t i = 1; i < degree.size(); ++i)
      if (degree[i] != 0) {
        for (uint_t col = 0; col < m; ++col)
          *out << "-" << var_number(i,m,col) << " ";
        *out << "0";
        for (uint_t col1 = 0; col1 < m; ++col1)
          for (uint_t col2 = col1+1; col2 < m; ++col2)
            *out << " " << var_number(i,m,col1) << " " << var_number(i,m,col2) << " 0";
        *out << "\n";
      }
  }
}
