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
  (doesn't matter here), which creates files "Pyth_n-3-0-2-SB.cnf" resp.
  "Pyth_n-3-1-2-SB.cnf". This is with symmetry-breaking, but if it shall be
  excluded, use
  > ./Pythagorean n 3 0 2 sb=off
  or
  > ./Pythagorean n 3 1 2 sb=off
  , creating files "Pyth_n-3-0-2.cnf" resp. "Pyth_n-3-1-2.cnf"

  (Note that ">" denotes the command-line prompt, and that the command is
  issued in the same directory where the executable "Pythagorean" has been
  compiled.)

  For the boolean problem for quadruples, use
  > ./Pythagorean n 4 0 2
  while the injective form (all components different) is obtained by
  > ./Pythagorean n 4 1 2
  creating files Pyth_n-4-0-2-SB.cnf resp. Pyth_n-4-1-2-SB.cnf.
  To deactivate symmetry-breaking, use e.g.
  > ./Pythagorean n 4 0 2 sb=off
  creating file Pyth_n-4-0-2.cnf

  The second parameter is K >= 3, the length of the Pythagorean tuple.

  For the third parameter d >= 0, also larger values can be used, which enforce
  a respective minimum distance between the (sorted) components of the tuples

    x_1^2 + ... + x_{K-1}^2 = x_K^2,

  that is, x_i + d <= x_{i+1} for 1 <= i <= K-1.

  An optional fourth parameter is "format=SD" (the default) resp.
  "format=D" (for "strict DIMACS" versus "DIMACS"): in the former case,
  variables are renamed, so that they start with 1, without gaps.
  Furthermore, "format=R" is possible, which replaces the hypergraph
  with a random hypergraph having the same hyperedge-sizes and vertex-degrees
  (after subsumptiion-elimination, and for m >= 2 after core-reduction).

  The fourth/fifth parameter m >= 0 is the number of colours, with
   - m = 0: only output the max-occurring vertex, the number hn of hyperedges,
     and hn / (n^(K-2) * log(n)) (the factor in the estimation).
   - m = 1: output the hypergraph
   - m = 2: output the boolean problem
   - m >= 3: currently strong or weak direct/nested translation available.

  In case of m=0, K=3, dist=0, the computation of the count uses a
  factorisation table for the natural numbers until n: This is much faster,
  but uses more memory (4 bytes * n).
  The 4-bytes-size comes from the 32-bit type Factorisation::base_t, which
  restricts the maximal n here to roughly 2*10^9. When using a 64-bit type
  instead, then this restrictions goes away, but then 8 bytes are used
  "per vertex".

  In case of m >= 3, the fifth/sixth parameter specifies the translation to a
  boolean clause-set; currently we have the following possibilities:
   - "S" the strong direct translation (with ALOAMO-clauses)
   - "W" the weak direct translation (only with ALO-clauses)
   - "N" the (weak) nested translation (no AMO-clauses)
   - "NS" the strong nested translation (with AMO-clauses)

  An optional fifth/sixth/seventh parameter can be "sb=off", which deactivates
  symmetry-breaking, while "sb=on" is the default.

  A further optional parameter can be "-", in which case output is put to
  standard output, or "filename", in which case a file is created.
  Default output is to file "Pyth_n-K-d-m.cnf" for 1 <= m <= 2 resp.
  "Pyth_n-K-d-m-T.cnf" for m >= 3, where T is the acronym for the
  translation-type, in both cases without symmetry-breaking, while with
  we get "Pyth_n-K-d-m-SB.cnf" resp. "Pyth_n-K-d-m-T-SB.cnf".

  Subsumption-elimination is first applied, and then for m >= 2 elimination
  of hyperedges containing a vertex occurring at most m-1 times (repeatedly),
  that is, the "m-core" of the hypergraph is computed.

  The output contains additional information for m >= 1:
   - Library and version information.
   - Information on parameters.
   - Information on number of hyperedges.
   - Information on vertex-degrees (m >= 1) and variable-degrees (m >= 2).
     The variable-degrees ignore additional clauses by symmetry-breaking or
     the non-boolean translation.

   After the comments (started with "c ") then comes the parameter line:
    - for SAT problems "p cnf max c", with "max" the maximal occurring
      variable-index, and with "c" the number of clauses;
    - for the hypergraph (m=1) "p hyp max h", with "max" the maximal occurring
      vertex (the largest hypotenuse), and with "h" the number of hyperedges.

  The CNF-output puts related clauses on the same line (note that in DIMACS,
  a clause is completed by "0", and thus you can have as many clauses on a
  line as you like).

  The order of the clauses is anti-lexicographically.


  COMPILATION:

  Requires C++11. Compile with

  > g++ -Wall --std=c++11 -Ofast -o Pythagorean Pythagorean.cpp

  resp. (now asserts disabled)

  > g++ -Wall --std=c++11 -Ofast -DNDEBUG -o Pythagorean Pythagorean.cpp

  resp. (fastest, and without superfluous warnings)

  > g++ -Wall -Wno-parentheses --std=c++11 -Ofast -funroll-loops -DNDEBUG -o Pythagorean Pythagorean.cpp

  If on the other hand a debugging version is needed, use:

  > g++ -Wall --std=c++11 -g -o Pythagorean Pythagorean.cpp


  FURTHER WORK:

  TODO: Implement output-"format" "R", which replaces the hypergraph with
        a random hypergraph with the given hypergraph-edge-sizes and
        -vertex-degrees. The seed should also be an input; perhaps here
        "R0", "R177" etc.
  TODO: implement "super-strict" DIMACS output, with all clauses on their own
        line.
  TODO: prove that subsumption does not happen for K=4.
  TODO: implement arbitrary K.
  TODO: implement multi-threaded computation.
        Easy with std::async, just dividing up the outer loops for computing
        the tuples (enumeration or counting). Since smaller numbers are easier,
        a bit of thought on an equal splitting is needed.
        The solution for splitting is, not to split the load into contiguous
        blocks, but to use "slicing": for the number nt >= 1 of threads,
        just use the congruence classes {1,...,n} mod i, i = 0,...,nt-1
        for the threads. It seems the computation of the factorisation-table
        can not be parallelised.
        nt=0 could be used for outputting just the header? Or just the
        estimation?!
        It seems then that "nt=.." should become another optional parameter
        (default-value 1).
        nt should not become part of the default filename.
  TODO: implement mixed k_i.
        Likely best here to use for example "[3,4]", that is, enclosing the
        list into square brackets on the parameter line; and demanding, that
        this is one parameter, so that when spaces are used, then the whole
        must be quoted.
  TODO: Check that the translations are in line with the OKlibrary at Maxima
        level.
  TODO: Estimate number N of tuples for arbitrary K; the conjecture is
          N ~ factor_K * n^(k-2) * log(n).

        factor3 ~ 0.1494 (n <= 2^32-2); The Sierpinski formula yields for
          factor3 = 1/2pi = 0.15915494309189533.., with two additional terms
          "+ B n" with B as below, and "+ O(n^(1/2))" (using newer estimates).
          However this is quite a bit bigger than the above factor -- our n's
          are "too small".
          The original Sierpinski-formula needs to be divided by 8=2^3, since
          it considers a^2+b^2=c^2 with a,b arbitrary integers, and thus we
          have 2^2 sign-possibilities, and the swap a <-> b.

According to "Mathematical Constants By Steven R. Finch", in Maxima-code:

G : 1/2/%pi*beta(1/4,1/2); // https://en.wikipedia.org/wiki/Gauss%27s_constant
S : log(exp(2*%gamma)/2/G^2); // K / pi for https://en.wikipedia.org/wiki/Sierpi%C5%84ski%27s_constant
A : 1.2824271291006226368753425688; // http://mathworld.wolfram.com/Glaisher-KinkelinConstant.html http://oeis.org/A074962
zp2 : 1/6*%pi^2*(%gamma + log(2*%pi) - 12*log(A)); // zeta'(2) http://mathworld.wolfram.com/RiemannZetaFunction.html
Sh : %gamma + S + 12 / %pi^2 * zp2 + log(2)/3 - 1/%pi;
B : Sh / 2 / %pi;
float(B);
  .027511221451371352

        factor4 ~ 0.005668 (n <= 2*10^4)
        factor5 ~ 0.000788 (n <= 3200)
        factor6 ~ 0.00010193 (n <= 800)
        factor7 ~ 1.0329e-05 (n <= 400)
        While for K=3 the factor seems to be increasing, for K >= 4 it seems
        decreasing, and they seem to converge. Would be good to have a more
        precise approximation (perhaps having another term "+n^(k-2)" ?).
        And would be good to have for K>3 faster computation.
  TODO: implement intelligent methods for K>3. Start with the generalised
        Dickson-method for K=4.
  TODO: Implement reductions triggered by the symmetry-breaking clauses.
        We have unit-clause propagation and subsumption. Possibly this enables
        further m-core-reductions (if correct?)?


  FURTHER DISCUSSIONS:

  Hyperedge-counting links:

   - https://oeis.org/A224921 for the number of Pythagorean triples (K=3)
     up to n-1.
     This sequence is obtained for, say, the first 73 elements, on the
     command-line via (using Bash)

   > for ((n=0; n<=72; ++n)); do ./Pythagorean $n 3 0 0 - | cut -f2 -d" " | tr "\n" ","; done; echo

     (This computation is rather wasteful, since instead of running just once
     through n=1,...,72, computing the triples with hypotenuse =n, and adding
     tehm up, it uses a quadratic effort, by starting again and again at the
     beginning. One could easily add a special output mode, which avoids this,
     if needed. But for small numbers it is very fast anyway. For example

   > for ((n=1000000; n<=1000010; ++n)); do ./Pythagorean $n 3 0 0 - | cut -f2 -d" " | tr "\n" ","; done; echo

     which produces

     23471475,23471475,23471476,23471477,23471478,23471479,23471479,23471480,
     23471481,23471481,23471494,

     takes 16s on an older 2.4 GHz machine.)

     Another example: The number of triples up to 2*10^9 is 6,380,787,008,
     obtained by "./Pythagorean 2000000000 3 0 0" in 335 sec, using 7.5 GB
     (on a standard 64-bit machine with 32 GB RAM).
     While the number of triples for n=4,294,967,294=2^32-2 is 14,225,080,520
     obtained by "./Pythagorean 4294967294 3 0 0" in 960 sec, using 16 GB.

   - Number of Pythagorean quadruples (K=4) or quintuples (K=5): not yet
     in OEIS.

     The number of quadruples up to 20000 is 22,452,483, obtained by
     "./Pythagorean 20000 4 0 0" in 134m10s (2.4 GHz) (using very
     little memory; faster methods are needed here).

     The number of quintuples up to 3200 is 208,319,099, obtained by
     "./Pythagorean 3200 5 0 0" in 258m42s (as above).

     The number of sixtuples up to 800 is 279,072,340, obtained by
     "./Pythagorean 800 6 0 0" in 87m52s (as above).

     The number of seventuples up to 400 is 633,708,884, obtained by
     "./Pythagorean 400 7 0 0" in 96m38s (as above).


  Pythagorean numbers Ptn(...) established, generalising
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
  the OKlibrary; except where stated "W", the strong direction translation is
  used for >= 3 colours, and symmetry-breaking is used iff stated ("SB"):

   - Ptn(3,3) = 7825 SB [9,472; 7,336; 14,673]
       http://cs.swan.ac.uk/~csoliver/papers.html#PYTHAGOREAN2016C
     Finding a solution for 7824 SB [9,465; 7,326; 14,653]: strongest seems
     ddfw from ubcsat, with the following success-rate for the cutoff-values:
     10^6 -> 2%, 5*10^6 -> 12%, 10^7 -> 21%, 10^8 -> 49%.
     Without SB:
     10^6 -> 0%, 5*10^6 -> 10%, 10^7 -> 15%, 10^8 -> 60%.
     For 7825, maxsat is all-except-of-one-clause (with and without SB).
   - Ptn(3,3,3) > 1.2 * 10^7 N [28,513,855; 22,539,999; 67,619,997], with
     10,342,796 occurring variables: g2wsat, runs with cutoff=1,500,000,000
     success at run 3.
     It seems this lower bound is still far away from the truth. So this
     problem is excessively hard.
   - Ptn(4,4) = 105 SB [639; 638; 1277] (known)
   - Ptn_i(4,4) = 163 SB [545; 544; 1089]
   - Ptn(4,4,4) > 1724 (using cutoff=2*10^7).
     Best seems saps with S-SB.
     1680: S-SB [158,627; =; 482,604], cutoff=10^7 has success rate 17%, and
     5*10^7 has 28%.
     1687: S-SB 10^7 -> 10%, 1710: S-SB 10^7 -> 1%.
     1718: S-SB 2*10^7 -> 1%.
     1719: S-SB 2*10^7 -> 1%, 200 runs.
     1724: S-SB 2*10^7 -> 0.2%, 500 runs.
     1725: S-SB 2*10^7 -> 0%, 500 runs.
     Seems to be a very hard problem, too hard for current methods.
   - Ptn(5,5) = 37 SB [404; 254; 509] (known)
   - Ptn_i(5,5) = 75 SB [2,276; =; 4,553]
   - Ptn(5,5,5) = 191 S-SB [46,633; 41,963; 126,656]
     Satisfying for 190, cutoff=10^5, success-rate:
     vw1 for S: 15%, S-SB: 5%;
     vw1 for W: 14%, for W-SB: 8%;
     walksat-tabu N: 8%, N-SB: 1%;
     walksat-tabu NS: 47%, NS-SB: 36%.
     C&C via SplittingViaOKsolver, S-SB, with D=20 and minisat-2.2.0 for 191:
     total run-time around 15 min (while for D=30 around 54 min);
     for N-SB, D=20, around 26 min, and for NS-SB around 36 min).
   - Ptn_i(5,5,5) > 468
     saps with S-SB seems best:
     410 NS with g2wsat, cutoff=10^6: 18% success.
     410 W with vw1, cutoff=10^6: 0% success.
     410 S-SB with saps, cutoff=10^6: 31% success.
     467: cutoff=10^6: 7% success.
     468: cutoff=2*10^6: 500 runs, 8.4% success.
     469: cutoff=10^6: 300 runs, 0% success. 2*10^6, 700 runs: 0%.
     Conjecture: Ptn_i(5,5,5) = 469.
   - Ptn(6,6) = 23 SB [311; 267; 535] (known)
   - Ptn_i(6,6) = 61 SB [6,770; =; 13,541]
   - Ptn(6,6,6) = 121;
     120 S-SB [154,860; 151,105; 453,798] found satisfiable with saps:
     cutoff=10^5 has success-rate 2%.
     121 [159,697; 155,857; 468,058] found
     unsatisfiable with C&C and D=20 and solver="lingelingala-b02aa1a-121013",
     370 min.
   - Ptn(7,7) = 18 SB [306; 159; 319] (known)
   - Ptn_i(7,7) = 65 SB [44,589; =; 89,179]
   - Ptn(7,7,7) = 102 S-SB [789,310; 694,898; 2,085,104]; for 101 C&C with D=20
     and glucose-2.2, splitting into ~ 1500 instances, instance 151
     satisfiable; also D=20 for 102, same C&C, 4,390 min.

   The sequence Ptn(k,k) for k=2,..., (which is 1, 7825, 105, 37, 23, 18, ...)
   is https://oeis.org/A250026 .

   The sequence Ptn_i(k,k) for k=3,..., is also of interest (7825,163,75,61,
   65). This can be further extended.

   The sequence Ptn(k,k,k) for k=3,... (?,?,191,121,102) is of interest, since
   these are the only 3-colour-problems solved until now. Can likely be further
   extended (with good effort).

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
#include <chrono>
#include <map>
#include <cstdint>
#include <set>
#include <utility>
#include <stdexcept>
#include <iomanip>
#include <random>

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
    typedef siz_t<std::vector<B>> size_t;
    assert(n <= std::numeric_limits<size_t>::max() / 2);
    std::vector<B> T(size_t(n)+1);
    for (B i = 2; i <= n; ++i)
      if (T[i] == 0) for (size_t j = i; j <= n; j+=i) T[j] = i;
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
      if (f%4==1) if (f==old_f) ++res[next-1]; else {res.push_back(1); ++next;}
      n /= f; old_f = f;
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

  constexpr double factor3 = 0.1494; // yields an upper bound for n <= 2^32-2.
  constexpr double pi = 2 * std::asin(1);
  constexpr double factor3_theory = 1.0 / 2 / pi;
  template <typename C>
  constexpr double estimating_triples(const C n) noexcept {
    return factor3 * n * std::log(n);
  }
  template <typename C1, typename C2>
  constexpr long double estimating_tuples_factor(const C1 n, const C1 K, const C2 N) noexcept {
    return N / (std::pow((long double)n,K-2) * std::log((long double)n));
  }

  // Counting triples:
  template <typename C1, typename C2>
  void triples_c(const C1 n, C1& max, C2& hn) {
    using namespace Factorisation;
    const auto T = table_factor(n);
    assert(T.size() == n+1);
    for (C1 i = 5; i <= n; ++i) {
      C2 prod = 1;
      for (const auto e : extract_exponents_1m4(T,i)) prod *= 2*e + 1;
      if (prod != 1) {max = std::max(max, i); hn += (prod-1)/2;}
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
          if (c <= n) {max = std::max(max,c); ++hn;}
        }
    }
  }

  // Generating triples:
  template <class V, typename C1>
  V triples_e(const C1 n, const C1 dist, C1& max) {
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
        if (c <= n and s+dist <= t) {
          res.push_back({{r+s,r+t,c}});
          max = std::max(max,c);
        }
      }
    }
    return res;
  }

  // Counting quadruples :
  template <typename C1, typename C2>
  void quadruples_c(const C1 n, const C1 dist, C1& max, C2& hn) noexcept {
    const C1 n2 = n*n;
    for (C1 a = 1; a < n; ++a) {
      const C1 a2 = a*a;
      const C1 bbound = std::sqrt(n2-a2);
      for (C1 b = a+dist; b <= bbound; ++b) {
        const C1 b2 = a2+b*b;
        const C1 cbound = std::sqrt(n2-b2);
        for (C1 c = b+dist; c <= cbound; ++c) {
          const C1 d2 = b2 + c*c;
          const C1 d = std::sqrt(d2);
          if (d*d == d2 and d >= c+dist) {max = std::max(max,d); ++hn;}
        }
      }
    }
  }

  // Generating quadruples:
  template <class V, typename C1>
  V quadruples_e(const C1 n, const C1 dist, C1& max) {
    V res;
    const C1 n2 = n*n;
    for (C1 a = 1; a < n; ++a) {
      const C1 a2 = a*a;
      const C1 bbound = std::sqrt(n2-a2);
      for (C1 b = a+dist; b <= bbound; ++b) {
        const C1 b2 = a2+b*b;
        const C1 cbound = std::sqrt(n2-b2);
        for (C1 c = b+dist; c <= cbound; ++c) {
          const C1 d2 = b2 + c*c;
          const C1 d = std::sqrt(d2);
          if (d*d == d2 and d >= c+dist) {
            res.push_back({{a,b,c,d}});
            max = std::max(max,d);
          }
        }
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
  // In general a random choice is better for algorithm min_elements, and best
  // should be a choice of minimum degree, however here it is not worth the effort.

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
  void core_red(V& hyp, SV& deg, const C1 m) noexcept {
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

namespace Random {

  template <class Hyp, class LDist, class DDist, typename vertex_t, class Gen, typename S>
  void randomise(Hyp& hyp, const LDist& hc, const DDist& vc, const vertex_t max, Gen& rgen, const S seed) {
    assert(not hyp.empty());
    assert(not hc.empty());
    assert(not vc.empty());
    typedef val_t<LDist> count_t;
    using gen_uint_t = S;
    rgen.seed(seed);
    typedef std::uniform_int_distribution<gen_uint_t> dist_t;
    typedef std::map<vertex_t, count_t> deg_map;
    typedef std::vector<deg_map> vec_deg_map;
    vec_deg_map enh_deg(max+1);
    for (const auto& h : hyp) {
      const vertex_t size = h.size();
      for (const auto v : h) ++enh_deg[v][size];
    }
    
  }

}

namespace Translation {

  enum class Type {
    direct_strong,
    direct_weak,
    nested,
    nested_strong,
    none,
    failure
  };

  // explicit output:
  std::ostream& operator <<(std::ostream& out, const Type t) {
    switch (t) {
    case Type::direct_strong : out << "direct-strong"; break;
    case Type::direct_weak : out << "direct-weak"; break;
    case Type::nested : out << "nested"; break;
    case Type::nested_strong : out << "nested-strong"; break;
    case Type::none : out << "no-translation"; break;
    case Type::failure : out << "FAILURE"; break;
    }
    return out;
  }

  // from and to type-abbreviations:
  Type get_type(const std::string& arg) noexcept {
    if (arg == "S") return Type::direct_strong;
    else if (arg == "W") return Type::direct_weak;
    else if (arg == "N") return Type::nested;
    else if (arg == "NS") return Type::nested_strong;
    else return Type::failure;
  }
  std::string type_abbr(const Type t) noexcept {
    switch (t) {
    case Type::direct_strong :  return "S";
    case Type::direct_weak : return "W";
    case Type::nested : return "N";
    case Type::nested_strong : return "NS";
    default : return "";
    }
  }
  std::string list_abbr() noexcept { return "\"S\", \"W\", \"N\" or \"NS\""; }
  constexpr bool is_direct(const Type t) noexcept {
    return t == Type::direct_weak or t == Type::direct_strong;
  }
  constexpr bool is_nested(const Type t) noexcept {
    return t == Type::nested or t == Type::nested_strong;
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
  inline C2 num_var(const C1 max, const C1 m, const Type t) noexcept {
    assert(m >= 2);
    assert(t != Type::failure);
    switch (t) {
    case Type::direct_strong : return C2(m) * max;
    case Type::direct_weak : return C2(m) * max;
    case Type::nested : return C2(m-1) * max;
    case Type::nested_strong : return C2(m-1) * max;
    default : return max;
    }
  }
  template <typename C1, typename C2>
  inline C2 num_cl(const C1 occ_n, const C1 m, const C2 hn, const Type t, const bool sb) noexcept {
    assert(m >= 2);
    assert(t != Type::failure);
    const C2 m2 = m;
    const C2 sbc = (not sb) ? 0 :
      ((is_nested(t)) ? m2-1 : (m2*(m2-1))/2);
    switch (t) {
    case Type::direct_weak : return m2 * hn + occ_n + sbc;
    case Type::direct_strong :
      return m2 * hn + occ_n + occ_n*(m2*(m2 - 1)) / 2 + sbc;
    case Type::nested_strong :
      return m2 * hn + occ_n*((m2-1)*(m2 - 2)) / 2 + sbc;
    default : return m2 * hn + sbc;
    }
  }

  // For the direct translation (strong or weak), translate vertex v and
  // colour col into non-boolean variable var(v,m,col),
  // expressing that v does not get colour col:
  template <typename C2, typename C1>
  inline C2 var_d(const C1 v, const C1 m, const C1 col) noexcept {
    assert(v >= 1);
    assert(m >= 3);
    assert(col < m);
    return C2(v-1) * m + col + 1;
  }
  // The variables related to vertex v for the nested translation:
  template <typename C2, typename C1>
  inline C2 var_n(const C1 v, const C1 m, const C1 i) noexcept {
    assert(v >= 1);
    assert(m >= 3);
    assert(i < m-1);
    return C2(v-1) * (m-1) + i + 1;
  }

  // Output the literals for the nested translation; the UHIT(1) clause-set is
  // {-v_1}, {v_1,-v_2}, {v_1,v_2,-v_3}, ..., {v_1,...,v_{m-2},-v_{m-1}},
  // {v_1,...,v_{m-1}} (all literals flipped, compared to literature, due to
  // minimising "-"-symbols in output).
  template <typename C2, typename C1>
  inline void lits_n(const C1 v, const C1 m, const C1 col, std::ostream* const out) {
    assert(v >= 1);
    assert(col < m);
    if (col == 0) *out << "-" << var_n<C2,C1>(v,m,0);
    else {
      *out << var_n<C2,C1>(v,m,0);
      for (C1 col2 = 1; col2 < col; ++col2) *out << " " << var_n<C2>(v,m,col2);
      if (col < m-1) *out << " -" << var_n<C2>(v,m,col);
    }
  }

  template <class Hyp, typename C1, typename C2, class Deg, class T>
  void output_colouring_problem(std::ostream* const out, const Hyp& G, const C1 m,
      const C2 max, const C2 c, const Deg deg, const C1 md_v,
      const Type t, const bool sb, const T& re) {
    assert(m >= 1);
    bool r = not re.empty();
    pline_output(out, max, c, m);
    if (m == 1) {
      assert(not sb);
      for (const auto& H : G) {
        for (const auto v : H) *out << ((r)?re[v]:v) << " ";
        *out << "0\n";
      }
    }
    else if (m == 2) {
      if (sb) *out << ((r)?re[md_v]:md_v) << " 0\n";
      for (const auto& H : G) {
        for (const auto v : H) *out << ((r)?re[v]:v) << " "; *out << "0 ";
        for (const auto v : H) *out << "-" << ((r)?re[v]:v) << " "; *out << "0\n";
      }
    }
    else {
      if (sb) {
        // First handling of the max-vertex:
        if (is_direct(t))
          for (C1 col = 1; col < m; ++col)
            *out << var_d<C2>((r)?re[md_v]:md_v,m,col) << ((col==m-1) ? std::string(" 0") : std::string(" 0 "));
        else *out << var_n<C2,C1>((r)?re[md_v]:md_v,m,0) << " 0";
        // Find the other m-2 vertices of highest degree and put into "store":
        std::vector<C1> store; store.reserve(m-2);
        {std::set<C1> avoid; avoid.insert(md_v);
        const auto size = deg.size();
        for (C1 i = 0; i < m-2; ++i) {
          C2 max_d = 0; C1 max_v = 0;
          const auto end = avoid.end();
          for (C1 v = 1; v < size; ++v) {
            const auto d = deg[v];
            if (d > max_d and avoid.find(v) == end) {
              max_d = d; max_v = v;
            }
          }
          avoid.insert(max_v);
          store.push_back((r)?re[max_v]:max_v);
        }}
        // Finally handling of the other vertices:
        if (is_direct(t)) {
          C1 exclude_col = 2;
          for (const auto v : store) {
            for (C1 col = exclude_col; col < m; ++col)
              *out << " " << var_d<C2>(v,m,col) << " 0";
            ++exclude_col;
          }
          *out << "\n";
        } else {
          C1 include_col = 1;
          for (const auto v : store) {
            for (C1 col = 0; col <= include_col; ++col)
              *out << " " << var_n<C2>(v,m,col);
            *out << " 0";
            ++include_col;
          }
          *out << "\n";
        }
      }
      // Translating the clauses:
      if (is_direct(t))
        for (const auto& H : G) {
          for (C1 col = 0; col < m; ++col) {
            for (const auto v : H) *out << var_d<C2>((r)?re[v]:v,m,col) << " ";
            *out << "0"; if (col != m-1) *out << " ";
          }
          *out << "\n";
        }
      else {
         for (const auto& H : G) {
          for (C1 col = 0; col < m; ++col) {
            for (const auto v : H) {lits_n<C2>((r)?re[v]:v,m,col,out); *out << " ";}
            *out << "0"; if (col != m-1) *out << " ";
          }
          *out << "\n";
        }
      }
      // Adding the support-clauses:
      if (is_direct(t)) {
        for (C1 i = 1; i < deg.size(); ++i)
          if (deg[i] != 0) {
            const auto ir = (r)?re[i]:i;
            // ALO:
            for (C1 col = 0; col < m; ++col)
              *out << "-" << var_d<C2>(ir,m,col) << " ";
            *out << "0";
            // AMO:
            if (t == Type::direct_strong)
              for (C1 col1 = 0; col1 < m; ++col1)
                for (C1 col2 = col1+1; col2 < m; ++col2)
                  *out << " " << var_d<C2>(ir,m,col1) << " " <<
                    var_d<C2>(ir,m,col2) << " 0";
            *out << "\n";
          }
      }
      else if (t == Type::nested_strong) {
        // AMO:
        for (C1 i = 1; i < deg.size(); ++i)
          if (deg[i] != 0) {
            const auto ir = (r)?re[i]:i;
            for (C1 col1 = 0; col1 < m-2; ++col1)
              for (C1 col2 = col1+1; col2 < m-1; ++col2)
                *out << "-" << var_n<C2>(ir,m,col1) << " -" <<
                  var_n<C2>(ir,m,col2) <<
                  ((col1==m-3 and col2==m-2) ? std::string(" 0") : std::string(" 0 "));
            *out << "\n";
          }
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
    translation = 6,
    symmetry = 7,
    format = 8,
    parse = 9
  };
  int v(Error e) {return static_cast<int>(e);}

  const std::string program = "Pythagorean";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.10.0";

  const std::string file_prefix = "Pyth_";

  const auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
  typedef decltype(timestamp) time_t;

  std::mt19937_64 rgen;
  typedef std::mt19937_64::result_type gen_uint_t;

  typedef std::vector<uint_t> tuple_t;
  typedef std::vector<tuple_t> hypergraph;

  std::pair<bool, uint_t> read_uint(const std::string& a) noexcept {
    uint_t n = 0;
    bool success = true;
    try { n = std::stoul(a); }
    catch(std::exception) { success = false; }
    return {success, n};
  }

  // Maximal value for n:
  constexpr uint_t max_n(const uint_t K, const uint_t dist, const uint_t m) noexcept {
    return (K==3) ?
    ((m>=1 or dist>=1) ? 2*std::sqrt(std::numeric_limits<uint_t>::max()) :
                         std::numeric_limits<Factorisation::base_t>::max()-1) :
    uint_t(std::sqrt(std::numeric_limits<uint_t>::max())) / K;
  }

  enum class Format {
    dimacs,
    strict_dimacs,
    random,
    random_ts,
    failure
  };
  inline constexpr bool is_random(const Format f) noexcept {
    return f == Format::random or f == Format::random_ts;
  }
  inline constexpr bool is_strict(const Format f) noexcept {
    return f == Format::strict_dimacs or is_random(f);
  }

  std::ostream& operator <<(std::ostream& out, const Format f) {
    switch (f) {
    case Format::dimacs : out << "DIMACS"; break;
    case Format::strict_dimacs : out << "strict-DIMACS"; break;
    case Format::random : out << "Random"; break;
    case Format::random_ts : out << "Random_timestamp"; break;
    case Format::failure : out << "FAILURE"; break;
    }
    return out;
  }
  Format get_format(const std::string& arg) noexcept {
    if (arg == "D") return Format::dimacs;
    else if (arg == "SD") return Format::strict_dimacs;
    else if (arg == "RT") return Format::random_ts;
    else if (arg.find("R") != 0) return Format::failure;
    if (arg.size() != 1) {
      try { std::stoull(arg.substr(1)); }
      catch(std::exception) { return Format::failure; }
    }
    return Format::random;
  }
  std::string format_abbr(const Format f) noexcept {
    assert(f != Format::failure);
    if (f == Format::dimacs) return "D";
    else if (f == Format::strict_dimacs) return "SD";
    else if (f == Format::random) return "R";
    else return "RT";
  }
  std::string list_format_abbr() noexcept { return "\"D\", \"SD\", \"R\", or \"RT\""; }
  bool check_format_arg(const std::string& arg) noexcept {
    return arg.find("format=") == 0;
  }
  // Assume get_format(arg) == Format::random:
  gen_uint_t extract_seed(const std::string& arg) {
    if (arg.size() == 1) return 0;
    else return std::stoull(arg.substr(1));
  }

  bool check_sb_arg(const std::string& arg) noexcept {
    return arg.find("sb=") == 0;
  }
  bool test_sb_arg(const std::string& arg) noexcept {
    return arg == "on" or arg == "off";
  }
  // assuming test_sb_arg yields true:
  bool symmetry_breaking(const std::string& arg) noexcept {
    return arg.find("on", 3) == 3;
  }
  std::string expand_sb_arg(const bool sb) noexcept {
    if (sb) return "on";
    else return "off";
  }

  std::string default_filename(const std::string& f, const uint_t n,
      const uint_t K, const uint_t dist, const Format format, const uint_t m,
      const Translation::Type t, const bool sb) noexcept {
    return f +
      std::to_string(n) +
      "-" + std::to_string(K) +
      "-" + std::to_string(dist) +
      ((format==Format::strict_dimacs) ? std::string() :
              (std::string("-") + format_abbr(format))) +
      "-" + std::to_string(m) +
      ((t==Translation::Type::none) ? std::string() :
              (std::string("-") + Translation::type_abbr(t))) +
      ((sb) ? std::string("-SB") : std::string()) +
      + ".cnf";
  }

  void oklib_output(std::ostream* const out, const time_t ts) {
    assert(*out);
    *out << "c OKlibrary http://github.com/OKullmann/oklibrary/blob/"
      "master/Satisfiability/Transformers/Generators/Pythagorean.cpp\n"
      "c   Program " << program << ".cpp in version " << version <<
      ", timestamp " << ts << ".\n";
  }

  void header_output(std::ostream* const out, const uint_t n, const uint_t K,
      const uint_t dist, const uint_t m, const Translation::Type t,
      const bool sb, const std::string& file, const Format f, const time_t ts, const gen_uint_t seed) {
    assert(*out);
    assert(m >= 1);
    oklib_output(out, ts);
    *out << "c Parameters (expanded):\nc   n=" << n << ", K=" << K <<
      ", d=" << dist << ", format=" << f;
      if (is_random(f)) *out << ", seed=" << seed;
      *out << ", m=" << m << ", translation="
      << t << ", symmetry-breaking=" << expand_sb_arg(sb) << ", file=\""
      << file << "\".\n";
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
        *out << "c Number of hyperedges (tuples) originally and after " <<
          m << "-core-reduction:\nc   " << h0 << " " << h2 << "\n";
      else
        *out << "c Number of hyperedges (tuples) originally, after "
          "subsumption, and after further " << m << "-core-reduction:\nc   "
          << h0 << " " << h1 << " " << h2 << "\n";
  }

  uint_t num_decdigits(const uint_t x) {
    return std::log10(x)+1;
  }

  template <class Vec, class Map>
  void count_output(std::ostream* const out, const cnum_t diff, const Vec& v, const uint_t K, const Map& M, const cnum_t maxd) {
    // v and M contain size/degree -> frequency, as a vector/map.
    assert(K >= 3);
    assert(not v.empty());
    assert(not M.empty());
    assert(maxd >= 1);
    assert(*out);
    *out << "c Removed " << diff << " hyperedges.\n";
    if (K != 3) {
      const auto num_dig_length = num_decdigits(v.size());
      *out << "c Hyperedge counts:\n";
      for (siz_t<Vec> k=0; k < v.size(); ++k) {
        const auto c = v[k];
        if (c != 0) *out << "c   size " << std::setw(num_dig_length) <<
          k << ": " << c << "\n";
      }
    }
    *out << "c Degree counts (for the hypergraph):\n";
    const auto num_dig_degree = num_decdigits(maxd);
    for (const auto& p : M)
      if (p.first != 0) *out << "c  " << std::setw(num_dig_degree) <<
        p.first << " : " << p.second << "\n";
  }

  void degree_output(std::ostream* const out,
      const cnum_t occ_n, const cnum_t min_d, const cnum_t max_d,
      const uint_t min_v, const uint_t max_v, const cnum_t sum_d,
      const uint_t m, const Translation::Type t,
      const std::vector<uint_t>& re) {
    assert(*out);
    assert(m >= 1);
    const bool r = not re.empty();
    if (m == 1) {
      *out << "c Number of occurring vertices = " << occ_n << ".\n";
      *out << "c Minimum degree = " << min_d << ", attained for vertex "
        << min_v;
      if (r) *out << " (-> " << re[min_v] << ")";
      *out << ".\nc Maximum degree = " << max_d << ", attained for vertex " <<
      max_v;
      if (r) *out << " (-> " << re[max_v] << ")";
      *out << ".\nc Average degree = " << double(sum_d) / occ_n << ".\n";
    }
    else if (m == 2) {
      *out << "c Number of occurring variables = " << occ_n << ".\n";
      *out << "c Minimum degree = " << 2*min_d << ", attained for variable " <<
        min_v;
      if (r) *out << " (-> " << re[min_v] << ")";
      *out << ".\nc Maximum degree = " << 2*max_d << ", attained for variable "
        << max_v;
      if (r) *out << " (-> " << re[max_v] << ")";
      *out << ".\nc Average degree = " << 2*double(sum_d) / occ_n << ".\n";
    }
    else {
      using namespace Translation;
      const cnum_t m2 = m;
      if (is_direct(t)) {
        *out << "c Number of occurring variables = " << m2 << " * " << occ_n
          << " = " << m2*occ_n << ".\n";
        *out << "c Degrees (only considering the core clauses):\n";
        *out << "c   Minimum = " << min_d << ", attained for vertex " << min_v <<
          " (variables";
        for (uint_t col=0; col<m; ++col)
          *out << " " << var_d<cnum_t>((r)?re[min_v]:min_v,m,col);
        *out << ").\n";
        *out << "c   Maximum = " << max_d << ", attained for vertex " << max_v <<
          " (variables";
        for (uint_t col=0; col<m; ++col)
          *out << " " << var_d<cnum_t>((r)?re[max_v]:max_v,m,col);
        *out << ").\n";
        *out << "c   Average degree = " << double(sum_d) / occ_n << ".\n";
      }
      else {
        *out << "c Number of occurring variables = " << m2-1 << " * " << occ_n
          << " = " << (m2-1)*occ_n << ".\n";
        *out << "c Degrees (only considering the core clauses):\n";
        *out << "c   Minimum = " << min_d << ", attained for vertex " << min_v <<
          " (variable " << var_n<cnum_t>((r)?re[min_v]:min_v,m,m-2) << ", degree " <<
            2*min_d << ").\n";
         *out << "c   Maximum = " << max_d << ", attained for vertex " << max_v <<
          " (variable " << var_n<cnum_t,uint_t>((r)?re[max_v]:max_v, m, 0) << ", degree " <<
            m2*max_d << ").\n";
      }
    }
  }

}

int main(const int argc, const char* const argv[]) {
  constexpr int argc_min = 5, argc_max = 9;
  constexpr uint_t K_max = 7;
  if (argc < argc_min or argc > argc_max) {
    std::cerr << err << argc_min-1 << " to " << argc_max-1 << " arguments are needed:\n"
     " - The number n >= 0 of elements.\n"
     " - The size K >= 3 of the tuple.\n"
     " - The enforced distance d >= 0 between components.\n"
     " - Optional: \"format=D\" or \"format=SD\" (the default).\n"
     " - The number m >= 0 of colours.\n"
     " - The translation type in case of m >= 3.\n"
     " - Optional: symmetry-breaking \"sb=on\" (the default) or \"sb=off\".\n"
     " - Optional: The filename or \"-\" for standard output.\n";
    return v(Error::parameter);
  }

  const auto p1 = read_uint(argv[1]);
  if (not p1.first) {
     std::cerr << err << "First input \"" << argv[1] << "\" must be an unsigned "
       "integer from 0 to " << std::numeric_limits<unsigned long>::max() << ".\n";
     return v(Error::parse);
  }
  const uint_t n = p1.second;

  const auto p2 = read_uint(argv[2]);
  if (not p2.first) {
     std::cerr << err << "Second input \"" << argv[2] << "\" must be an unsigned "
       "integer from 0 to " << std::numeric_limits<unsigned long>::max() << ".\n";
     return v(Error::parse);
  }
  const uint_t K = p2.second;
  if (K < 3) {
    std::cerr << err << "Second input \"" << K << "\" must be at least 3.\n";
    return v(Error::too_small);
  }
  if ( K > K_max) {
    std::cerr << err << "Second input \"" << K << "\" currently can be at most " << K_max << ".\n";
    return v(Error::not_yet);
  }

  const auto p3 = read_uint(argv[3]);
  if (not p3.first) {
     std::cerr << err << "Third input \"" << argv[3] << "\" must be an unsigned "
       "integer from 0 to " << std::numeric_limits<unsigned long>::max() << ".\n";
     return v(Error::parse);
  }
  const uint_t dist = p3.second;

  const std::string arg4 = argv[4];
  const bool with_format_argument = check_format_arg(arg4);
  const Format format = (with_format_argument) ?
    get_format(arg4.substr(7)) : Format::strict_dimacs;
  if (format == Format::failure) {
    std::cerr << err << "Fourth input \"" << arg4 << "\" must use a valid "
      "format acronym (" << list_format_abbr() << ").\n";
    return v(Error::format);
  }
  const gen_uint_t seed = is_random(format) ?
    ((format==Format::random) ? extract_seed(arg4.substr(7)) : timestamp) : 0;
  if (with_format_argument and argc == argc_min) {
    std::cerr << err << "The number of colours is missing.\n";
    return v(Error::parameter);
  }
  const int next_position = (with_format_argument) ? 5 : 4;

  const auto pnext = read_uint(argv[next_position]);
  if (not pnext.first) {
     std::cerr << err << "The input at position " << next_position << " with "
       "value \"" << argv[next_position] << "\" must be an unsigned "
       "integer from 0 to " << std::numeric_limits<unsigned long>::max() << ".\n";
     return v(Error::parse);
  }
  const uint_t m = pnext.second;

  const int new_argc_max = (with_format_argument) ?
    ((m >= 3) ? argc_max : argc_max-1) :
    ((m >= 3) ? argc_max-1 : argc_max-2);
  const int new_argc_min = (with_format_argument) ? argc_min+1 : argc_min;
  if (m <= 2 and argc > new_argc_max) {
    std::cerr << err << "Only " << new_argc_max-1 << " arguments allowed "
      "for <= 2 colours.\n";
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

  if (m >= 3) {
    if (argc == new_argc_min) {
      std::cerr << err << "In case of m >= 3 the translation type is required.\n";
      return v(Error::parameter);
    }
    if (Translation::get_type(argv[next_position+1]) == Translation::Type::failure) {
      std::cerr << err << "The input \"" << argv[next_position+1] << "\" must be a "
        "valid acronym for the translation type (i.e., " <<
        Translation::list_abbr() << ").\n";
      return v(Error::translation);
    }
  }
  const Translation::Type translation = (m <= 2) ?
    Translation::Type::none : Translation::get_type(argv[next_position+1]);

  const int optional_position = (m >= 3) ? next_position+2 : next_position+1;

  const bool with_sb_argument = (argc == optional_position) ?
    false : check_sb_arg(argv[optional_position]);
  if (with_sb_argument and not test_sb_arg(std::string(argv[optional_position]).substr(3))) {
    std::cerr << err << "Symmetry breaking: after \"sb=\" must come \"on\" "
      "or \"off\".\n";
    return v(Error::symmetry);
  }
  if (not with_sb_argument and argc == new_argc_max) {
    std::cerr << err << "Too many arguments, when not using the optional argument \"sb=..\".\n";
    return v(Error::parameter);
  }
  const bool symm_break = (with_sb_argument) ?
    symmetry_breaking(argv[optional_position]) : (m >= 2);
  if (symm_break and m <= 1) {
    std::cerr << err << "Symmetry breaking only with at least 2 colours.\n";
    return v(Error::symmetry);
  }

  const int final_argc_max = (with_sb_argument) ? new_argc_max : new_argc_max-1;
  if (argc > final_argc_max) {
    std::cerr << err << "Too many arguments (namely " << argc-1 << "), while "
      "only " << final_argc_max-1 << " are allowed here.\n";
    return v(Error::parameter);
  }

  const int file_position = (with_sb_argument) ?
    optional_position+1 : optional_position;
  const std::string file = (argc == file_position) ?
    default_filename(file_prefix, n, K, dist, format, m, translation, symm_break) :
    argv[file_position];
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

  if (K == 3)
    if (m == 0)
      if (dist == 0) Pythagorean::triples_c(n, max, hn);
      else Pythagorean::triples_c(n, dist, max, hn);
    else
      res = Pythagorean::triples_e<hypergraph>(n, dist, max);
  else if (K == 4)
    if (m == 0) Pythagorean::quadruples_c(n, dist, max, hn);
    else res = Pythagorean::quadruples_e<hypergraph>(n, dist, max);
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
            max = std::max(max,e);
            if (m == 0) ++hn;
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
              max = std::max(max,f);
              if (m == 0) ++hn;
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
                max = std::max(max,g);
                if (m == 0) ++hn;
                else res.push_back({{a,b,c,d,e,f,g}});
              }
            }
          }
        }
      }
    }
  }

  if (m == 0) {
    *out << max << " " << hn << " " <<
      Pythagorean::estimating_tuples_factor(n,K,hn) << "\n";
    return 0;
  }
  header_output(out, n, K, dist, m, translation, symm_break, file, format, timestamp, seed);

  const cnum_t orig_hn = res.size();
  if (orig_hn == 0) {
    Translation::pline_output(out, 0, 0, m);
    return 0;
  }

  // removing duplicates:
  for (auto& x : res) x.erase(std::unique(x.begin(), x.end()), x.end());

  if (K >= 5 and dist == 0) Subsumption::min_elements(res,max);

  const cnum_t after_subs_hn = res.size();
  // Core-reduction and vertex-degrees:
  typedef std::vector<cnum_t> stat_vec_t;
  stat_vec_t degree(max+1, 0);
  Reduction::core_red(res, degree, m);

  hn = res.size();
  hn_output(out, orig_hn, after_subs_hn, hn, m, K, dist);
  if (hn == 0) {
    Translation::pline_output(out, 0, 0, m);
    return 0;
  }

  // Extremal degrees:
  uint_t occ_n = 0, min_v = 0, max_v = 0;
  cnum_t min_d = -1, max_d = 0, sum_d = 0;
  for (stat_vec_t::size_type i = 1; i < degree.size(); ++i) {
    const auto deg = degree[i];
    if (deg != 0) {
      ++occ_n; sum_d += deg;
      if (deg < min_d) {min_d = deg; min_v = i;}
      if (deg > max_d) {max_d = deg; max_v = i;}
    }
  }
  assert(occ_n >= 1);

  // Counts of lengths and degrees:
  {stat_vec_t h_counts(K+1,0);
   for (const auto& h : res) ++h_counts[h.size()];
   std::map<cnum_t, cnum_t> v_counts;
   for (uint_t v = 1; v <= max; ++v) ++v_counts[degree[v]];
   count_output(out, orig_hn-hn, h_counts, K, v_counts, max_d);
   if (format == Format::random)
     Random::randomise(res, h_counts, v_counts, max, rgen, seed);
  }

  // anti-lexicographical sorting:
  std::sort(res.begin(), res.end(),
    [](const tuple_t& x, const tuple_t& y) {
      return std::lexicographical_compare(x.rbegin(), x.rend(), y.rbegin(), y.rend());
    }
  );

  max = res.back().back();

  std::vector<uint_t> renaming;
  if (is_strict(format)) {
    renaming.resize(max+1);
    uint_t index = 0;
    for (uint_t v = 1; v <= max; ++v)
      if (degree[v] != 0) renaming[v] = ++index;
  }

  using namespace Translation;
  if (is_strict(format)) max = occ_n;
  if (m == 1) {
    degree_output(out, occ_n, min_d, max_d, min_v, max_v, sum_d, m, translation, renaming);
    output_colouring_problem(out, res, m, cnum_t(max), hn, degree, max_v, translation, symm_break, renaming);
  }
  else if (m == 2) {
    degree_output(out, occ_n, min_d, max_d, min_v, max_v, sum_d, m, translation, renaming);
    const cnum_t cn = num_cl(max,m,hn,translation,symm_break);
    output_colouring_problem(out, res, m, cnum_t(max), cn, degree, max_v, translation, symm_break, renaming);
 } else {
    assert(m >= 3);
    *out << "c Using translation " << translation << ".\n";
    degree_output(out, occ_n, min_d, max_d, min_v, max_v, sum_d, m, translation, renaming);
    const cnum_t vn = num_var<cnum_t>(max,m,translation);
    const cnum_t cn = num_cl(occ_n, m, hn, translation, symm_break);
    output_colouring_problem(out, res, m, vn, cn, degree, max_v, translation, symm_break, renaming);
  }
}
