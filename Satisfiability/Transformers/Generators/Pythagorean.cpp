// Oliver Kullmann, 12.6.2016 (Swansea)
/* Copyright 2016 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Pythagorean.cpp
  \brief Generator for Pythagorean triples and tuples, and the related
  SAT problems (as CNFs in DIMACS format). The SAT-problems formulate the
  m-colouring of the hypergraph of tuples.

  See https://en.wikipedia.org/wiki/Pythagorean_triple for explanations on
  triples/tuples, and see
  https://en.wikipedia.org/wiki/Boolean_satisfiability_problem#SAT_problem_format
  for the DIMACS format. The 2-colouring (boolean) problem is discussed at
  https://en.wikipedia.org/wiki/Boolean_Pythagorean_triples_problem

  This program is available at
  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Pythagorean.cpp


  USAGE:

  For the boolean problems for triples, use
  > ./Pythagorean n 3 0 2
  or
  > ./Pythagorean n 3 1 2
  (doesn't matter here), which creates files "Pyth_n-3-0-2.cnf" resp.
  "Pyth_n-3-1-2.cnf".

  (Note that ">" denotes the command-line prompt, and that the command is
  issued in the same directory where the executable "Pythagorean" has been
  compiled.)

  For the boolean problem for quadruples, use
  > ./Pythagorean n 4 0 2
  while the injective form (all components different) is obtained by
  > ./Pythagorean n 4 1 2
  creating files Pyth_n-4-0-2.cnf resp. Pyth_n-4-1-2.cnf.

  The second parameter is K >= 3, the length of the Pythagorean tuple.

  For the third parameter d >= 0, also larger values can be used, which enforce
  a respective minimum distance between the (sorted) components of the tuples

    x_1^2 + ... + x_{K-1}^2 = x_K^2,

  that is, x_i + d <= x_{i+1} for 1 <= i <= K-1.

  The fourth parameter m >= 0 is the number of colours, with
   - m = 0: only output the max-occurring vertex and the number of hyperedges
   - m = 1: output the hypergraph
   - m = 2: output the boolean problem
   - m >= 3: currently uses the strong direct translation.

  In case of m=0, K=3, dist=0, the computation of the count uses a
  factorisation table for the natural numbers until n: This is much faster,
  but uses more memory (4 bytes * n).
  The 4-bytes-size comes from the 32-bit type Factorisation::base_t, which
  restricts the maximal n here to roughly 2*10^9. When using a 64-bit type
  instead, then this restrictions goes away, but then 8 bytes are used
  "per vertex".

  In case of m >= 3, the fifth parameter specifies the translation to a
  boolean clause-set; currently we have the following possibilities:
   - "S" the strong direct translation (with ALOAMO-clauses)
   - "W" the weak direct translation (only with ALO-clauses).

  An optional fifth/sixth parameter can be "-", in which case output is put to
  standard output, or "filename", in which case a file is created.
  Default output is to file "Pyth_n-K-d-m.cnf" for 1 <= m <= 2 resp.
  "Pyth_n-K-d-m-T.cnf" for m >= 3, where T is the acronym for the
  translation-type.

  Subsumption-elimination is first applied, and then for m >= 2 elimination
  of hyperedges containing a vertex occurring at most m-1 times (repeatedly),
  referred to as "colour-reduction".

  The output contains additional information for m >= 1:
   - Library and version information.
   - Information on parameters.
   - Information on number of hyperedges.
   - Information on vertex-degrees (m=1) resp. variable-degrees (m >= 2).

   After the comments (started with "c ") then comes the parameter line:
    - for SAT problems "p cnf max c", with "max" the maximal occurring
      variable-index, and with "c" the number of clauses;
    - for the hypergraph (m=1) "p hyp max h", with "max" the maximal occurring
      vertex (the largest hypotenuse), and with "h" the number of hyperedges.

  The CNF-output puts related clauses on the same line (note that in DIMACS,
  a clause is completed by "0", and thus you can have as many clauses on a
  line as you like).


  COMPILATION:

  Requires C++11. Compile with

  > g++ -Wall --std=c++11 -Ofast -o Pythagorean Pythagorean.cpp

  resp. (now asserts disabled)

  > g++ -Wall --std=c++11 -Ofast -DNDEBUG -o Pythagorean Pythagorean.cpp

  resp. (fastest, and without superfluous warnings)

  > g++ -Wall -Wno-parentheses --std=c++11 -Ofast -funroll-loops -DNDEBUG -o Pythagorean Pythagorean.cpp

  If on the other hand a debugging version is needed, use:

  > g++ -Wall --std=c++11 -g -o Pythagorean Pythagorean.cpp


  FURTHER DISCUSSIONS:

  TODO: implement intelligent methods for K>3.
  TODO: prove that subsumption-elimination does not happen for K=4.
  TODO: implement arbitrary K.
  TODO: implement output of additional symmetry-breaking clauses.
        m-1 clauses of length 1,2,...,m-1. Just using the m-1 vertices with
        highest degrees.
  TODO: implement multi-threaded computation.
        Easy with std::async, just dividing up the outer loops for computing
        the tuples (enumeration or counting). Since smaller numbers are easier,
        a bit of thought on an equal splitting is needed.
        The solution for splitting is, not to split the load into contiguous
        blocks, but to use "slicing": for the number nt >= 1 of threads,
        just use the congruence classes {1,...,n} mod i, i = 0,...,nt-1
        for the threads.
        nt=0 can be used for outputting just the header.
        It seems then that nt should become another compulsory parameter
        (the alternative is do use named parameters on the command-line).
        nt should not become part of the default filename.

  Hyperedge-counting links:

   - https://oeis.org/A224921 for the number of Pythagorean triples (K=3)
     up to n-1.
     This sequence is obtained for, say, the first 73 elements, on the
     command-line via (using Bash)

   > for ((n=0; n<=72; ++n)); do ./Pythagorean $n 3 0 0 - | cut -f2 -d" " | tr "\n" ","; done; echo

     Another example: The number of triples up to 2*10^9 is 6,380,787,008,
     obtained by "./Pythagorean 2000000000 3 0 0" in 335 sec, using 7.5 GB
     (on a standard 64-bit machine with 32 GB RAM).

   - Number of Pythagorean quadruples (K=4) or quintuples (K=5): not yet
     in OEIS.

  Pythagorean numbers established, generalising
    http://link.springer.com/chapter/10.1007%2F978-3-319-40970-2_15
    https://arxiv.org/abs/1605.00723
    Solving and Verifying the boolean Pythagorean Triples problem
      via Cube-and-Conquer

  Ptn(k_1, ..., k_m) is the smallest n >= 1 (if it exists, otherwise infinity),
  such that for every partition of {1,...,n} into m parts A_1, ..., A_m, there
  is 1 <= i <= m such that A_i contains a Pythagorean k_i-tuple.

  Experimental results are as follows.

  In square brackets [h;h';c], number of hyperedges before/after reduction and
  number of clauses; if "=" is used, then the reductions don't do anything
  here; algorithms "vw1" and "g2wsat" are from the UBCSAT suite of local-search
  algorithms, while "SplittingViaOKsolver" is the basic C&C-implementation in
  the OKlibrary; except for Ptn(3,3,3) the strong direction translation is
  used for >= 3 colours:

   - Ptn(3,3) = 7825 [9,472; 7,336; 14,672]
       http://cs.swan.ac.uk/~csoliver/papers.html#PYTHAGOREAN2016C
   - Ptn(3,3,3) > 6*10^6 [13,595,129; 10,583,938; 34,285,861], with 7,602,141
     occurring variables (g2wsat, second run with cutoff=150,000,000,
     "1 1 0 145820710 145820710 168201051")
   - Ptn(4,4) = 105 [639; 638; 1276] (known)
   - Ptn_i(4,4) = 163 [545; 544; 1088]
   - Ptn(4,4,4) > 1680 [158,627; =; 482,601]
     (vw1 with "2 1 0 6540594 3535491316"); 1681 [158,837; =; 483,235] hard to
     satisfy; conjecture Ptn(4,4,4) = 1681.
   - Ptn(5,5) = 37 [404; 254; 508] (known)
   - Ptn_i(5,5) = 75 [2,276; =; 4,552]
   - Ptn(5,5,5) = 191 [46,633; 41,963; 126,653]
     (vw1 for 190, found easily; C&C via SplittingViaOKsolver
     with D=20 and minisat-2.2.0 for 191: total run-time around 46 min).
   - Ptn_i(5,5,5) > 371 [312,548; =; 939,128]
     g2wsat with "51 1 0 3268756 2619231264".
   - Ptn(6,6) = 23 [311; 267; 534] (known)
   - Ptn_i(6,6) = 61 [6,770; =; 13,540]
   - Ptn(6,6,6) = 121; 120 [154,860; 151,105; 453,795] found satisfiable with
     C&C and D=25 as above, while 121 [159,697; 155,857; 468,055] found
     unsatisfiable with C&C and D=25 and solver="lingelingala-b02aa1a-121013",
     1,867 min.
   - Ptn(7,7) = 18 [306; 159; 318] (known)
   - Ptn_i(7,7) = 65 [44,589; =; 89,178]

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
#include <map>
#include <cstdint>

namespace {

  template <class C> using val_t = typename C::value_type;
  template <class C> using siz_t = typename C::size_type;
  template <class C> using it_t = typename C::iterator;

}

namespace Factorisation {

  typedef std::uint_least32_t base_t;
  typedef std::uint_least8_t exponent_t;

  // Compute table T, such that for 1 <= i <= n, T[i] is the
  // prime-factorisation of i, in map-form (basis -> exponent):
  template <typename E = exponent_t>
  std::vector<std::map<base_t,E>> table_factorisations(const base_t n) {
    std::vector<std::map<base_t,E>> T(n+1);
    std::vector<base_t> rem; rem.reserve(n+1);
    for (base_t i = 2; i <= n; ++i) rem[i] = i;
    for (base_t i = 2; i <= n; ++i)
      if (rem[i] != 1) {
        const base_t b = (T[i].empty()) ? i : T[i].begin()->first;
        for (base_t j = i; j <= n; j+=i) { ++T[j][b]; rem[j] /= b; }
      }
    return T;
  }
  // This function is not used here, since the following implicit
  // representation is faster and uses less memory.

  // Now the table T only contains one prime factor (the largest):
  template <typename B = base_t>
  std::vector<B> table_factor(const base_t n) {
    std::vector<B> T(n+1);
    for (B i = 2; i <= n; ++i)
      if (T[i] == 0) for (B j = i; j <= n; j+=i) T[j] = i;
    return T;
  }

  // Various functions for working with tables produced by table_factor:

  // Extracting the exponents of prime factors congruent 1 mod 4:
  template <class V>
  inline std::vector<val_t<V>> extract_exponents_1m4(const V& T, val_t<V> n) {
    assert(n >= 2);
    assert(n < T.size());
    typedef val_t<V> B;
    typedef std::vector<B> R;
    R res;
    siz_t<R> next = 0;
    B old_f = 0;
    do {
      const B f = T[n];
      if (f % 4 == 1)
        if (f == old_f) ++res[next-1];
        else { res.push_back(1); ++next; }
      n /= f;
      old_f = f;
    } while (n != 1);
    return res;
  }
  // Extracting the prime factorisation:
  template <class V, typename B, typename E = exponent_t>
  inline std::map<B,E> extract_factorisation(const V& T, B n) {
    assert(n >= 2);
    assert(n < T.size());
    std::map<B,E> res;
    do {
      const B f = T[n];
      ++res[f];
      n /= f;
    } while (n != 1);
    return res;
  }
  // The list of factors <= bound, given factorisation F:
  template <class V, typename B>
  inline std::vector<B> bounded_factors(const V& F, const B bound) {
    typedef std::vector<B> R;
    R res;
    typedef siz_t<R> size_t;
    size_t num_factors = 1;
    for (const auto& p : F) num_factors *= p.second+1;
    res.reserve(num_factors);
    const auto begin = res.cbegin();
    res.push_back(1);
    for (const auto p : F) {
      const auto end = res.cend();
      const B b = p.first;
      const auto max_e = p.second;
      for (auto i = begin; i != end; ++i) {
        const auto f = *i;
        B power = 1;
        for (decltype(p.second) e = 0; e < max_e; ++e) {
          power *= b;
          const auto new_f = f*power;
          if (new_f > bound) break;
          else res.push_back(new_f);
        }
      }
    }
    return res;
  }
}

namespace Pythagorean {

  constexpr auto factor3 = 0.149; // yields an upper bound for n <= 2*10^9
  template <typename C>
  constexpr double estimating_triples(const C n) noexcept {
    return factor3 * n * std::log(n);
  }

  // Counting triples:
  template <typename C1, typename C2>
  void triples_c(const C1 n, C1& max, C2& hn) {
    const auto T = Factorisation::table_factor(n);
    assert(T.size() == n+1);
    for (C1 i = 5; i <= n; ++i) {
      C2 prod = 1;
      for (const auto e : Factorisation::extract_exponents_1m4(T,i))
        prod *= 2*e + 1;
      if (prod == 1) continue;
      max = std::max(max, i);
      hn += (prod-1)/2;
    }
  }

  // Counting triples with minimum distance between (sorted) components
  // (much slower than above, but minimal space usage):
  template <typename C1, typename C2>
  void triples_c(const C1 n, const C1 dist, C1& max, C2& hn) noexcept {
    assert(dist >= 1);
    for (C1 r = 2; r <= C1(n/(1+std::sqrt(2))); r+=2) {
      const C1 rs = r*(r/2);
      for (C1 s = dist; s <= C1(std::sqrt(rs)); ++s)
        if (rs % s == 0) {
          const C1 t = rs / s;
          if (t >= n or t < s+dist) continue;
          const C1 c = r+s+t;
          if (c <= n) {
            max = std::max(max,c);
            ++hn;
          }
        }
    }
  }

  // Generating triples:
  template <class V, typename C1>
  V triples_e(const C1 n, const C1 dist) {
    V res; res.reserve(Pythagorean::estimating_triples(n));
    const C1 max_r = n/(1+std::sqrt(2));
    const auto T = Factorisation::table_factor(max_r);
    assert(T.size() == max_r+1);
    for (C1 r = 2; r <= max_r; r+=2) {
      auto F = Factorisation::extract_factorisation(T, r);
      for (auto& p : F) p.second *= 2;
      --F[2];
      const C1 rs = r*(r/2);
      const C1 bound = std::sqrt(rs);
      for (const C1 s : Factorisation::bounded_factors(F,bound)) {
        if (s < dist) continue;
        const C1 t = rs / s;
        if (t >= n or t < s+dist) continue;
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
    v.erase(std::remove_if(v.begin(), v.end(),
              [](const val_t<C>& x){return x.empty();}), v.end());
  }

}

namespace Subsumption {

  // Selecting some element from a non-empty sequence:
  template <class V>
  inline val_t<V> select(const V& t) noexcept {
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
    typedef val_t<V> tuple_t;
    std::sort(begin, end,
      [](const tuple_t& x, const tuple_t& y) {return x.size() < y.size();});
    typedef it_t<V> it_t;
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
    typedef val_t<V> tuple_t;
    typedef val_t<tuple_t> vert_t;
    bool changed;
    do {
      changed = false;
      for (auto& h : hyp)
        if (std::find_if(h.begin(), h.end(),
            [&](const vert_t v){return deg[v] < m;}) != h.end()) {
          changed = true;
          for (const auto v : h) --deg[v];
          h.clear();
        }
    } while (changed);
    Container::remove_empty_elements(hyp);
  }

}

namespace Translation {

  enum class Type {
    direct_strong,
    direct_weak,
    none,
    failure
  };

  std::ostream& operator <<(std::ostream& out, const Type t) {
    switch (t) {
    case Type::direct_strong : out << "direct-strong"; break;
    case Type::direct_weak : out << "direct-weak"; break;
    case Type::none : out << "no-translation"; break;
    case Type::failure : out << "FAILURE"; break;
    }
    return out;
  }

  Type get_type(const std::string& arg) noexcept {
    if (arg == "S") return Type::direct_strong;
    if (arg == "W") return Type::direct_weak;
    return Type::failure;
  }

  template <typename C1, typename C2>
  void pline_output(std::ostream* const out, const C2 n, const C2 c,
      const C1 m) {
    assert(*out);
    assert(m >= 1);
    switch (m) {
    case 1 :
      *out << "p hyp " << n << " " << c << "\n"; break;
    default :
      *out << "p cnf " << n << " " << c << "\n";
    }
  }

  template <typename C2, typename C1>
  inline C2 var(const C1 i, const C1 m, const C1 col) noexcept {
    assert(i >= 1);
    assert(col < m);
    return C2(i-1) * m + col + 1;
  }

  template <class Hyp, typename C1, typename C2, class Deg>
  void output_colouring_problem(std::ostream* out, const Hyp& G, const C1 m,
      const C2 max, const C2 c, const Deg deg, const Type t) {
    assert(m >= 1);
    pline_output(out, max, c, m);
    if (m == 1) {
      for (const auto& H : G) {
        for (const auto v : H) *out << v << " ";
        *out << "0\n";
      }
    }
    else if (m == 2) {
      for (const auto& H : G) {
        for (const auto v : H) *out << v << " "; *out << "0 ";
        for (const auto v : H) *out << "-" << v << " "; *out << "0\n";
      }
    }
    else {
      for (const auto& H : G) {
        for (C1 col = 0; col < m; ++col) {
          for (const auto v : H) *out << var<C2>(v,m,col) << " ";
          *out << "0"; if (col != m-1) *out << " ";
        }
        *out << "\n";
      }
      for (C1 i = 1; i < deg.size(); ++i)
        if (deg[i] != 0) {
          for (C1 col = 0; col < m; ++col)
            *out << "-" << var<C2>(i,m,col) << " ";
          *out << "0";
          if (t == Type::direct_strong)
            for (C1 col1 = 0; col1 < m; ++col1)
              for (C1 col2 = col1+1; col2 < m; ++col2)
                *out << " " << var<C2>(i,m,col1) << " " <<
                  var<C2>(i,m,col2) << " 0";
          *out << "\n";
        }
    }
  }

}

namespace {

  typedef unsigned long int uint_t; // vertices and variables
  typedef unsigned long long int cnum_t; // hyperedge/clause indices/numbers

  enum class Error {
    parameter = 1,
    too_large = 2,
    too_small = 3,
    not_yet = 4,
    file = 5,
    translation = 6
  };
  int v(Error e) {return static_cast<int>(e);}

  const std::string program = "Pythagorean";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.7.12";

  const std::string filename = "Pyth_";

  typedef std::vector<uint_t> tuple_t;
  typedef std::vector<tuple_t> hypergraph;

  constexpr uint_t max_n(const uint_t K, const uint_t dist, const uint_t m) noexcept {
    return (K==3) ?
    ((m>=1 or dist>=1) ? 2*std::sqrt(std::numeric_limits<uint_t>::max()) :
                         std::numeric_limits<Factorisation::base_t>::max()/2) :
    uint_t(std::sqrt(std::numeric_limits<uint_t>::max())) / K;
  }

  void oklib_output(std::ostream* const out) {
    assert(*out);
    *out << "c OKlibrary http://github.com/OKullmann/oklibrary/blob/"
      "master/Satisfiability/Transformers/Generators/Pythagorean.cpp\n"
      "c   Program " << program << ".cpp in version " << version <<
      ", timestamp " << std::time(nullptr) << ".\n";
  }

  void header_output(std::ostream* const out, const uint_t n, const uint_t K,
      const uint_t dist, const uint_t m, const Translation::Type t,
      const std::string& file) {
    assert(*out);
    assert(m >= 1);
    oklib_output(out);
    *out << "c Parameters (expanded): " << n << " " << K << " " << dist <<
      " " << m << " " << t << " \"" << file << "\"\n";
    switch (m) {
    case 1 :
      *out << "c Hypergraph of Pythagorean " << K << "-tuples, up to n=" <<
        n << ".\n";
      break;
    case 2 :
      *out << "c Boolean Pythagorean " << K << "-tuples problem, up to n=" <<
        n << ".\n";
      break;
    default :
      *out << "c " << m << "-Colour Pythagorean " << K << "-tuples problem, "
        "up to n=" << n << ".\n";
    }
    if (dist > 0)
      *out << "c   Minimum-distance between (sorted) tuple-components = " <<
        dist << ".\n";
  }

  void hn_output(std::ostream* const out, const cnum_t h0, const cnum_t h1,
  const cnum_t h2, const uint_t m, const uint_t K, const uint_t dist) {
    assert(*out);
    assert(h0 >= 1);
    assert(m >= 1);
    if (m == 1)
      if (K <= 4 or dist >= 1)
        *out << "c Number of hyperedges (tuples):\nc   "
          << h0 << "\n";
      else
        *out << "c Number of hyperedges (tuples) originally and after "
          "subsumption:\nc   " << h0 << " " << h1 << "\n";
    else
      if (K <= 4 or dist >= 1)
        *out << "c Number of hyperedges (tuples) originally and after"
          " colour-reduction:\nc   " << h0 << " " << h2 << "\n";
      else
        *out << "c Number of hyperedges (tuples) originally, after "
          "subsumption, and after further colour-reduction:\nc   " << h0 <<
          " " << h1 << " " << h2 << "\n";
  }

  template <class Vec>
  void count_output(std::ostream* const out, const cnum_t diff, const Vec& v, const uint_t K) {
    assert(*out);
    *out << "c Removed " << diff << " hyperedges.\n";
    if (K == 3) return;
    *out << "c Hyperedge counts:\n";
    for (siz_t<Vec> k=0; k < v.size(); ++k) {
      const auto c = v[k];
      if (c != 0)
        *out << "c   size " << k << ": " << c << "\n";
    }
  }

  void degree_output(std::ostream* const out,
      const cnum_t occ_n, const cnum_t min_d, const cnum_t max_d,
      const uint_t min_v, const uint_t max_v, const cnum_t sum_d,
      const uint_t m, const Translation::Type t) {
    assert(*out);
    assert(m >= 1);
    if (m == 1) {
      *out << "c Number of occurring vertices = " << occ_n << ".\n";
      *out << "c Minimum degree = " << min_d << ", attained for vertex " <<
      min_v << ".\n";
      *out << "c Maximum degree = " << max_d << ", attained for vertex " <<
      max_v << ".\n";
      *out << "c Average degree = " << double(sum_d) / occ_n << ".\n";
    }
    else if (m == 2) {
      *out << "c Number of occurring variables = " << occ_n << ".\n";
      *out << "c Minimum degree = " << 2*min_d << ", attained for variable " <<
        min_v << ".\n";
      *out << "c Maximum degree = " << 2*max_d << ", attained for variable " <<
        max_v << ".\n";
      *out << "c Average degree = " << 2*double(sum_d) / occ_n << ".\n";
    }
    else {
      *out << "c Number of occurring variables = " << m << " * " << occ_n
        << " = " << m*occ_n << ".\n";
      *out << "c Degrees, ignoring the ALO";
      if (t == Translation::Type::direct_strong) *out << "AMO";
      *out << "-clauses:\n";
      *out << "c   Minimum = " << min_d << ", attained for vertex " << min_v <<
        " (variables";
      using namespace Translation;
      for (uint_t col = 0; col < m; ++col) *out << " " <<
        var<cnum_t>(min_v,m,col);
      *out << ").\n";
      *out << "c   Maximum = " << max_d << ", attained for vertex " << max_v <<
        " (variables";
      for (uint_t col = 0; col < m; ++col) *out << " " <<
        var<cnum_t>(max_v,m,col);
      *out << ").\n";
      *out << "c   Average degree = " << double(sum_d) / occ_n << ".\n";
    }
  }

}

int main(const int argc, const char* const argv[]) {
  if (argc <= 4 or argc >= 8) {
    std::cerr << err << "Four to six arguments are needed:\n"
     " - The number n >= 0 of elements.\n"
     " - The size K >= 3 of the tuple.\n"
     " - The enforced distance d >= 0 between components.\n"
     " - The number m >= 0 of colours.\n"
     " - The translation type for m >= 3.\n"
     " - Optional: The filename or \"-\" for standard output.\n";
    return v(Error::parameter);
  }

  const uint_t n = std::stoul(argv[1]);

  const uint_t K = std::stoul(argv[2]);
  if (K < 3) {
    std::cerr << err << "Second input " << K << " must be at least 3.\n";
    return v(Error::too_small);
  }
  if ( K > 7) {
    std::cerr << err << "Second input " << K << " currently must be at most 7.\n";
    return v(Error::not_yet);
  }

  const uint_t dist = std::stoul(argv[3]);

  const uint_t m = std::stoul(argv[4]);

  if (m <= 2 and argc >= 7) {
    std::cerr << err << "In case of <= 2 colours only at most 5 arguments are allowed.\n";
    return v(Error::parameter);
  }

  const uint_t abs_max = max_n(K,dist,m);
  if (n > abs_max) {
    std::cerr << err << "First input " << n << " larger than maximal allowed value: " << abs_max << ".\n";
    return v(Error::too_large);
  }
  if (dist > abs_max) {
    std::cerr << err << "Third input " << dist << " larger than maximal "
      "allowed value: " << abs_max << ".\n";
    return v(Error::too_large);
  }

  if (m == 3) {
    if (argc == 5) {
      std::cerr << err << "In case of m >= 3 the translation type is required.\n";
      return v(Error::parameter);
    }
    if (Translation::get_type(argv[5]) == Translation::Type::failure) {
      std::cerr << err << "The fifth input \"" << argv[5] << "\" must be a "
        "valid acronym for the translation type (i.e., \"S\" or \"W\").\n";
      return v(Error::translation);
    }
  }
  const Translation::Type translation = (m <= 2) ?
    Translation::Type::none : Translation::get_type(argv[5]);

  const int file_param_position = (m >= 3) ? 6 : 5;
  const std::string file = (argc == file_param_position) ?
    filename + std::to_string(n) + "-" + std::to_string(K) + "-" +
      std::to_string(dist) + "-" + std::to_string(m) +
      ((translation==Translation::Type::none) ? std::string() : (std::string("-") + argv[5]))
      + ".cnf"
    : argv[file_param_position];
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
    return v(Error::file);
  }

  // Computing the list of Pythagorean tuples:
  hypergraph res;
  cnum_t hn = 0;
  uint_t max = 0;

  if (K == 3) {
    if (m == 0)
      if (dist == 0) Pythagorean::triples_c(n, max, hn);
      else Pythagorean::triples_c(n, dist, max, hn);
    else
      res = Pythagorean::triples_e<hypergraph>(n, dist);
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
  header_output(out, n, K, dist, m, translation, file);

  const cnum_t orig_hn = res.size();
  if (orig_hn == 0) {
    Translation::pline_output(out, 0, 0, m);
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
  hn_output(out, orig_hn, after_subs_hn, hn, m, K, dist);
  if (hn == 0) {
    Translation::pline_output(out, 0, 0, m);
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

  using namespace Translation;
  if (m == 1) {
    degree_output(out, occ_n, min_d, max_d, min_v, max_v, sum_d, m, translation);
    output_colouring_problem(out, res, m, cnum_t(max), hn, degree, translation);
  }
  else if (m == 2) {// DIMACS output:
    degree_output(out, occ_n, min_d, max_d, min_v, max_v, sum_d, m, translation);
    const cnum_t cn = 2 * hn;
    output_colouring_problem(out, res, m, cnum_t(max), cn, degree, translation);
 } else {
    assert(m >= 3);
    *out << "c Using translation " << translation << ".\n";
    degree_output(out, occ_n, min_d, max_d, min_v, max_v, sum_d, m, translation);
    const cnum_t cn = m * hn + occ_n +
      ((translation==Type::direct_strong) ? occ_n*(m*(m - 1)) / 2 : 0);
    const cnum_t vn = m * cnum_t(max);
    output_colouring_problem(out, res, m, vn, cn, degree, translation);
  }
}
