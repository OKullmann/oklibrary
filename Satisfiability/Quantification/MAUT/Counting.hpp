// Oliver Kullmann, 1.5.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Counting maximal falsifying assignments and maximal clashfree sub-clause-sets

  For a clause-set F (as CNF) a total assignment f is "maximal falsifying",
  if one of the following equivalent statements is true:
   - the set of falsified clauses is maximal (there is no larger subset
     of F which is totally falsifiable)
   - the set of falsified clauses is maximal clashfree
   - for the set F' of clauses of F falsified by f holds: the partial
     assignment setting all literals in F' to false satisfies F - F'.

   The basic backtracking algorithms of this module implicitly enumerate
   the maximal falsifying assignments, in a compactified form via partial
   assignments.

   The real ideal task here is the enumeration of all maximal clashfree subsets
   of F.

   We consider here the problem of counting maximal falsifying assignments.
   For the underlying number classes compare
     Satisfiability/Solvers/TawSolver/plans/ctaw.hpp .

*/

#ifndef COUNTING_IpXiQM8Bfc
#define COUNTING_IpXiQM8Bfc

#include <string>
#include <ostream>

#include <gmp.h>

#include <Numerics/FloatingPoint.hpp>
#include <Transformers/Generators/Random/Numbers.hpp> // for iexp2

namespace MAUT {

  /* Prototype of a counting class

     The supported operations are:

      - default construction
      - add(e) : add 2^e
      - += : add another counting object
      - conversion to string
      - min_prec: flat80-value for guaranteed binary precision (number of bits)
      - id: string

  */

  constexpr const char* count_header = "cid cpr ca cn";

  struct Count64 {
    static constexpr FloatingPoint::float80 min_prec=64;
    static inline const std::string id = "uint64";
    typedef FloatingPoint::UInt_t count_t;
    Count64() noexcept : c(0) {}
    void add(count_t e) noexcept { c += RandGen::iexp2(e); }
    void operator += (const Count64 other) noexcept { c += other.c; }
    operator std::string() const noexcept { return std::to_string(c); }
    friend std::ostream& operator <<(std::ostream& out, const Count64 c) {
      return out << id << " " << min_prec << " " << c.c;
    }
    friend bool operator ==(const Count64 lhs, const Count64 rhs) {
      return lhs.c == rhs.c;
    }
  private :
    count_t c;
  };

  struct Count80 {
    static constexpr FloatingPoint::float80 min_prec=64;
    static inline const std::string id = "float80";
    typedef FloatingPoint::float80 count_t;
    Count80() noexcept : c(0) {}
    void add(count_t e) noexcept { c += FloatingPoint::exp2(e); }
    void operator += (const Count80 other) noexcept { c += other.c; }
    operator std::string() const noexcept {
      std::stringstream out;
      FloatingPoint::fullprec_float80(out);
      out << c;
      return out.str();
    }
    friend std::ostream& operator <<(std::ostream& out, const Count80 c) {
      return out << id << " " << min_prec << " " << std::string(c);
    }
    friend bool operator ==(const Count80 lhs, const Count80 rhs) {
      return lhs.c == rhs.c;
    }
  private :
    count_t c;
  };

  struct Count_mpz {
    static constexpr FloatingPoint::float80 min_prec=FloatingPoint::pinfinity;
    static inline const std::string id = "mpz";
    static constexpr int base = 10;
    Count_mpz() noexcept { mpz_init(c); }
    Count_mpz(const Count_mpz&) = delete;
    ~Count_mpz() noexcept { mpz_clear(c); }
    void add(FloatingPoint::UInt_t e) {
      mpz_t p; mpz_init(p); mpz_ui_pow_ui(p, 2, e);
      mpz_add(c, c, p);
    }
    void operator += (const Count_mpz& other) { mpz_add(c,c,other.c); }
    operator std::string() const {
      const auto size = mpz_sizeinbase(c, base) + 2;
      std::vector<char> str(size);
      mpz_get_str(&str[0], base, c);
      return std::string(&str[0]);
    }
    friend std::ostream& operator <<(std::ostream& out, const Count_mpz& c) {
      return out << id << " " << min_prec << " " << std::string(c);
    }
    friend bool operator ==(const Count_mpz& lhs, const Count_mpz& rhs) {
      return mpz_cmp(lhs.c, rhs.c) == 0;
    }
  private :
    mpz_t c;
  };

}

#endif
