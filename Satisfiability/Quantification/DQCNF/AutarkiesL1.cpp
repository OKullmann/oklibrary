// Oliver Kullmann, 28.6.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Quantification/DQCNF/AutarkiesL1.cpp
  \brief A simple translation of the level-1 autarky problem for DQCNFs
  to SAT, using the direct encoding.

  COMPILE with

> g++ --std=c++11 -Wall -Ofast -DNDEBUG -o autL1 AutarkiesL1.cpp

  Alternatively the makefile (called "makefile") in this
  directory can be used:

> make all

  USAGE:

> autL1 input [output] [log]

*/

#include <limits>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <type_traits>
#include <exception>
#include <algorithm>
#include <set>
#include <array>
#include <map>
#include <iterator>

#include <cstdlib>
#include <cstdint>
#include <cassert>

namespace {

// --- General input and output ---

const std::string version = "0.3.1";
const std::string date = "3.7.2018";

const std::string program = "autL1";

enum class Error {
  file_reading=1,
  file_writing=2,
  file_pline=3,
  num_vars=4,
  allocation=5,
  literal_read=6,
  variable_value=7,
  number_clauses=8,
  empty_clause=9,
  a_rep_line=11,
  e_rep_line=12,
  a_read=13,
  e_read=14,
  a_rep=15,
  e_rep=16,
  a_line_read=17,
  e_line_read=18,
  a_empty=19,
  e_empty=21,
  d_empty=22,
  d_bada=23,
  pseudoempty_clause=24,
};
/* Extracting the underlying code of enum-classes (scoped enums) */
template <typename EC>
inline constexpr int code(const EC e) noexcept {return static_cast<int>(e);}


// Error output with ERROR-prefix, and each on a new line:
struct Outputerr {
  const std::string e = "ERROR[" + program + "]: ";
  template <typename T>
  const Outputerr& operator <<(const T& x) const {
    std::cerr << e << x << "\n";
    return *this;
  }
};
Outputerr errout;

/* Class for output-objects solout and logout, which are initialised
   by function set_output from the command-line parameters. The two
   public members (besides the constructor) are
     out << x;
     out.endl();
  which send the output to the internally stored ostream *p, if set.
  The destructor deletes *p iff member del = true.
*/
class Output {
  std::ostream* p = nullptr;
  bool del = false;
  friend void set_output(const int, const char* const*) noexcept;
public :
  ~Output() { if (del) delete p; }
  template <typename T>
  const Output& operator <<(const T& x) const { if (p) *p << x; return *this; }
  void endl() const { if (p) {*p << "\n"; p->flush();} }
};
Output solout;
Output logout;

/* Assuming argv starts with
     input [output] [log]
   and setting solout, logout accordingly, with special cases for "-nil"
   (discarding output) and "-cout", "-cerr" and "-clog", while otherwise
   files are created (for output=log only one file):
*/
void set_output(const int argc, const char* const argv[]) noexcept {
  std::ios_base::sync_with_stdio(false);
  logout.p = &std::cout;
  if (argc == 2) { solout.p = &std::cout; return; }
  const std::string outname(argv[2]);
  if (outname == "-cout") solout.p = &std::cout;
  else if (outname == "-cerr") solout.p = &std::cerr;
  else if (outname == "-clog") solout.p = &std::clog;
  else if (outname != "-nil") {
    solout.p = new std::ofstream(outname, std::ios::app);
    if (not *solout.p) {
      errout << ("Invalid output file: \"" + outname + "\".");
      std::exit(code(Error::file_writing));
    }
    solout.del = true;
  }
  if (argc == 3) return;
  const std::string logname(argv[3]);
  if (logname == "-cerr") logout.p = &std::cerr;
  else if (logname == "-clog") logout.p = &std::clog;
  else if (logname == "-nil") logout.p = nullptr;
  else if (logname != "-cout") {
    if (logname == outname) logout.p = solout.p;
    else {
      logout.p = new std::ofstream(logname, std::ios::app);
      if (not *logout.p) {
        errout << ("Invalid output file: \"" + logname + "\".");
        std::exit(code(Error::file_writing));
      }
      logout.del = true;
    }
  }
}

/* Input object, initialised with a "filename", which can be "-cin", in which
   case the input comes from standard input; for objects in of type Input,
   the instream is accessed via *in:
*/
class Input {
  std::istream* const p;
  const bool del;
  Input(const Input&) = delete;
  Input(Input&&) = delete;
public :
  Input(const std::string& f) : p(f == "-cin" ? &std::cin : new std::ifstream(f)), del(f != "-cin") {
    if (not *p) {
      errout << "Invalid input filename.";
      std::exit(code(Error::file_reading));
    }
  }
  ~Input() { if (del) delete p; }
  std::istream& operator *() const { return *p; }
};


// --- Data structures for literals and variables ---

/*
  The basic classes are Var (variables) and Lit (literals), where
  Lit contains Lit_int, "Literals as integers", which are signed integers,
  while variables are unsigned.

  Polarities pos, neg are expressed via the enumeration-type Pol.

  Operations for Lit_int x, Lit y,y', Var v, Pol p:

   - Lit() (the singular literal)
   - copy-construction, assignment for Lit
   - Lit(x) (non-converting)
   - Lit(v, p)
   - bool(y) (explicit; true iff x is not singular)
   - -y, -p, and y.neg() (negation in-place)
   - y == y', y != y'
   - y < y', p < p'
   - var(y) (yields Var)
   - sign(y) (yields Polarity)
   - y.posi(), y.negi() (properties)
   - y.index() (yields Lit_int)
   - for (Pol p : Polarities)
   - ostream << y, istream >> y

   Lit-literals are constructed by n_l for unsigned long-long n.
*/

#ifndef LIT_TYPE
# define LIT_TYPE std::int32_t
#endif
typedef LIT_TYPE Lit_int;
static_assert(std::is_signed<Lit_int>::value, "Type \"Lit_int\" must be signed integral.");
static_assert(sizeof(Lit_int) != 1, "Lit_int = char (or int8_t) doesn't work with reading (since not numbers are read, but characters).");
constexpr Lit_int max_lit = std::numeric_limits<Lit_int>::max();
static_assert(- -max_lit == max_lit, "Problem with negating max_lit.");

typedef std::make_unsigned<Lit_int>::type Var;
static_assert(Lit_int(Var(max_lit)) == max_lit, "Problem with Var and Lit_int.");
inline constexpr bool valid(const Var v) noexcept { return v <= Var(max_lit); }

enum class Pol { n, p };
inline constexpr Pol operator -(const Pol p) noexcept {
  return (p==Pol::p) ? Pol::n : Pol::p;
}
constexpr std::array<Pol,2> Polarities {{Pol::n, Pol::p}};

static_assert(std::is_pod<Pol>::value, "Pol is not POD.");
static_assert(static_cast<int>(Pol::n) == 0, "Pol::n != 0.");
static_assert(static_cast<int>(Pol::p) == 1, "Pol::p != 1.");
static_assert(Pol::n < Pol::p, "Order problem with Pol.");
static_assert(-Pol::n == Pol::p and -Pol::p == Pol::n, "Negation problem with Pol.");
static_assert(Pol(false) == Pol::n and Pol(true) == Pol::p, "Conversion problem from bool to Pol.");
static_assert(Pol(0) == Pol::n and Pol(1) == Pol::p, "Conversion problem from int to Pol.");
static_assert(Polarities.size() == 2 and Polarities[0] == Pol::n and Polarities[1] == Pol::p, "Problem with array Polarities");

class Lit {
  Lit_int x;
public :
  Lit() = default;
  constexpr explicit Lit(const Lit_int x) noexcept : x(x) {}
  constexpr Lit(const Var v, const Pol p) noexcept : x(p==Pol::p?v:-Lit_int(v)) {}

  constexpr explicit operator bool() const noexcept { return x; }
  constexpr Lit_int index() const noexcept {return x;}
  constexpr bool posi() const noexcept { return x > 0; }
  constexpr bool negi() const noexcept { return x < 0; }

  constexpr Lit operator -() const noexcept { return Lit(-x); }
  void neg() noexcept { x=-x; }

  constexpr bool operator ==(const Lit y) const noexcept { return x == y.x; }
  constexpr bool operator !=(const Lit y) const noexcept { return x != y.x; }

  friend constexpr Var var(const Lit x) noexcept { return std::abs(x.x); }
  friend constexpr Pol sign(const Lit x) noexcept {return Pol(x.x >= 0);}

  friend constexpr bool operator <(const Lit a, const Lit b) noexcept {
    return var(a)<var(b) or (var(a)==var(b) and sign(a)<sign(b));
  }
  friend constexpr bool operator <=(const Lit a, const Lit b) noexcept {
    return a<b or a==b;
  }

  friend std::ostream& operator <<(std::ostream& out, const Lit x) {
    return out << x.x;
  }
  friend std::istream& operator >>(std::istream& in, Lit& x) {
    return in >> x.x;
  }
};
static_assert(std::is_pod<Lit>::value, "Lit is not POD.");

inline constexpr Lit operator"" _l(const unsigned long long x) noexcept {return Lit(x);}
static_assert(0_l == Lit(), "Problem with default construction of Lit.");
// Remark: As usual, as a local variable, the declaration "Lit x;" does not
// initialise x.
static_assert(1_l==Lit(1) and -1_l==Lit(-1), "Problem with construction of Lit.");
static_assert(not 0_l, "Problem with conversion of singular literal to bool.");
static_assert(1_l and -1_l, "Problem with conversion of valid literal to bool.");
static_assert(1_l != -1_l, "Problem with negation and/or inequality.");
static_assert(- -1_l == 1_l, "Problem with double negation.");
static_assert(0_l == -0_l, "Problem with negation of singular literal.");
static_assert(var(-1_l) == 1, "Problem with var().");
static_assert(sign(-1_l) == Pol::n, "Problem with sign().");
static_assert(1_l == Lit(1,Pol::p) and -1_l == Lit(1,Pol::n), "Problem with polarity.");
static_assert(0_l < 1_l, "Singular literal is not smallest.");
static_assert(-1_l < 1_l, "Negation is not smaller than unnegated.");
static_assert((1_l).index() == 1, "Index extraction wrong.");
static_assert((-1_l).index() == -1, "Index extraction wrong.");
static_assert((1_l).posi(), "Positivity determination wrong.");
static_assert((-1_l).negi(), "Negativity determination wrong.");
static_assert(Lit(1_l) == 1_l, "Problem with copy-construction.");


// Boolean function type (nonconstant, false, or true):
enum class BFt { nc=0, f, t };
inline constexpr BFt operator -(const BFt t) noexcept {
  return (t==BFt::nc) ? t : ((t==BFt::f) ? BFt::t : BFt::f);
}
static_assert(code(BFt::nc) == 0, "Underlying integer of nc is not zero.");
static_assert(-BFt::nc == BFt::nc, "Problem with negating BFt.");
static_assert(-BFt::t == BFt::f, "Problem with negating BFt.");
static_assert(-BFt::f == BFt::t, "Problem with negating BFt.");


/* Literals plus true/false (the boolean functions with at most one var);
   the linear order is 0,false,true,-1,1,-2,2, ... .
   Constructors:
    - Litc() (singular)
    - Litc(x) for Lit x
    - Litc(t) for BFt t
   And bf(b) for bool b yields Litc.
   Exactly one of x.sing(), x.constant(), x.variable() is true for Litc x.
   Operations:
    - explicit conversions to Lit and BFt
    - negation (operator - and in-place member neg()).
*/
class Litc  {
  Lit x;
  BFt t;
  /* Class invariants:
      - assert(t==BFt::nc or not x;);
      - exactly one of sing(), constant() or variable() is true.
  */
  constexpr Litc(const Lit x, const BFt t) noexcept : x(x), t(t) {}
public :
  Litc() = default;
  constexpr explicit Litc(const Lit x) noexcept : x(x), t(BFt::nc) {}
  constexpr explicit Litc(const BFt t) noexcept : x(0), t(t) {}

  constexpr explicit operator BFt() const noexcept { return t; }
  constexpr explicit operator Lit() const noexcept { return x; }

  constexpr Litc operator -() const noexcept { return Litc(-x,-t); }
  void neg() noexcept { x = -x; t = -t; }

  constexpr bool sing() const noexcept { return not x and t==BFt::nc; }
  constexpr bool constant() const noexcept { return not x and t!=BFt::nc; }
  constexpr bool variable() const noexcept { return bool(x); }

  constexpr bool operator ==(const Litc y) noexcept {
    return x==y.x and t==y.t;
  }
  constexpr bool operator !=(const Litc y) noexcept {
    return not (*this == y);
  }
  friend constexpr bool operator<(const Litc x, const Litc y) {
    return x.x < y.x or (BFt(x) != BFt::nc and BFt(y) == BFt::nc) or
      (BFt(x) != BFt::nc and BFt(y) != BFt::nc and BFt(x) < BFt(y)) or
      (not bool(Lit(x)) and BFt(x) == BFt::nc and BFt(y) != BFt::nc);
  }
};
static_assert(std::is_pod<Litc>::value, "Litc is not POD.");

inline constexpr Litc bf(const bool b) { return (b) ? Litc(BFt::t) : Litc(BFt::f); }

static_assert(Lit(Litc()) == 0_l, "Default construction of Litc does not yield singular literal.");
static_assert(BFt(Litc()) == BFt::nc, "Default construction of Litc is not nonconstant.");
// Remark: As usual, as a local variable, the declaration "Litc x;" does not
// initialise x.
static_assert(Lit(Litc(1_l)) == 1_l, "Construction of Litc does not pass literal.");
static_assert(BFt(Litc(1_l)) == BFt::nc, "Construction of Litc with literal is constant.");
static_assert(Lit(bf(false)) == 0_l, "Construction of Litc with constant does not make literal singular.");
static_assert(BFt(bf(false)) == BFt::f, "Construction of Litc with false does not yield false.");
static_assert(Lit(bf(true)) == 0_l, "Construction of Litc with constant does not make literal singular.");
static_assert(BFt(bf(true)) == BFt::t, "Construction of Litc with false does not yield false.");
static_assert(Litc() == Litc(0_l), "Default construction not equal to explicit construction.");
static_assert(Litc() != bf(false), "Default construction equal to constant function.");
static_assert(-Litc() == Litc(), "Problem with negation.");
static_assert(-bf(true) == bf(false), "Problem with negation.");
static_assert(-bf(false) == bf(true), "Problem with negation.");
static_assert(-Litc(1_l) == Litc(-1_l), "Problem with negation.");
static_assert(-Litc(-1_l) == Litc(1_l), "Problem with negation.");
static_assert(Litc(0_l) < bf(false), "Singular literal is not smallest.");
static_assert(bf(false) < bf(true), "False is not smaller than true.");
static_assert(bf(true) < Litc(-1_l), "Constant literal true is not smaller than nonconstant.");
static_assert(bf(false) < Litc(-1_l), "Constant literal false is not smaller than nonconstant.");
static_assert(Litc(-1_l) < Litc(1_l), "Litc with literal -1 is not smaller than with literal 1.");
static_assert(Litc().sing(), "Problem with singularity determination.");
static_assert(not Litc(1_l).sing(), "Problem with singularity determination.");
static_assert(not bf(true).sing(), "Problem with singularity determination.");
static_assert(not bf(false).sing(), "Problem with singularity determination.");
static_assert(not Litc().constant(), "Problem with constancy determination.");
static_assert(bf(true).constant(), "Problem with constancy determination.");
static_assert(bf(false).constant(), "Problem with constancy determination.");
static_assert(not Litc(1_l).constant(), "Problem with constancy determination.");
static_assert(not Litc().variable(), "Problem with variability determination.");
static_assert(not Litc(BFt::f).variable(), "Problem with variability determination.");
static_assert(not Litc(BFt::t).variable(), "Problem with variability determination.");
static_assert(Litc(1_l).variable(), "Problem with variability determination.");
static_assert(Litc(bf(true)) == bf(true), "Problem with copy-construction.");
static_assert(not (Litc(bf(true)) < Litc(bf(true))), "Problem with < for Litc.");
static_assert(not (Litc(bf(false)) < Litc(bf(false))), "Problem with < for Litc.");
static_assert(not (Litc() < Litc()), "Problem with < for Litc.");
static_assert(not (Litc(Lit(1)) < Litc(Lit(1))), "Problem with < for Litc.");


// --- Data structures for clause and clause-sets ---

typedef std::uint_fast64_t Count_t;

enum class VT { und=0, fa, fe, a, e }; // variable types, with "f" for "formal"
typedef std::vector<VT> VTvector;
inline constexpr bool et(const VT t) noexcept {return t==VT::fe or t==VT::e;}
inline constexpr bool at(const VT t) noexcept {return t==VT::fa or t==VT::a;}

typedef std::set<Var> Varset;
typedef std::set<Varset> VarSetsystem;
typedef VarSetsystem::const_iterator Dependency;
typedef std::vector<Dependency> Dvector;
typedef VarSetsystem::const_pointer Dependency_p;
typedef std::map<Dependency_p, Count_t> DepCounts;

typedef std::set<Lit> Clause;
typedef std::pair<Clause,Clause> PairClause; // all-exists
struct DClause {
  PairClause P; // A-E
  void clear() noexcept {P.first.clear(); P.second.clear();}
  bool pseudoempty() const noexcept {return P.second.empty();}
  bool empty() const noexcept {return P.first.empty() and P.second.empty();}
  bool operator ==(const DClause C) const noexcept {return P == C.P;}
  bool operator !=(const DClause C) const noexcept {return P != C.P;}
  friend bool operator <(const DClause& C, const DClause& D) noexcept {
    return C.P < D.P;
  }
};

typedef std::vector<Count_t> Degree_vec;

typedef std::set<DClause> DCLS;
typedef DCLS::const_iterator dclause_it;

struct ClauseSet {
  DCLS F;
  VTvector vt;
  VarSetsystem dep_sets;
  Dvector D;
  DepCounts dc;
  // Statistics:
  //   from the parameter line:
  Var n_pl;
  Count_t c_pl;
  //   from dependency-specification:
  Var na_d = 0, ne_d = 0;
  //   actually occurring in clauses (with tautological clauses removed):
  Degree_vec vardeg;
  Var max_a_index=0, max_e_index=0, max_index=0; // maximal occurring variable-index
  Var na=0, ne=0, n=0; // number occurring e/a/both variables
  Var max_a_length=0, max_e_length=0, max_c_length=0; // max number of a/e/both literals in clauses
  Var max_s_dep=0, min_s_dep=max_lit, count_dep=0;
  Count_t c=0; // number of clauses (without tautologies)
  Count_t la=0, le=0, l=0; // number of a/e/both literal occurrences
  Count_t t=0; // number of tautological clauses
};

typedef std::map<Var,Litc> Pass;
typedef std::set<Pass> PassSet;


// --- Input ---

enum class ConformityLevel {normal=0, strict, verystrict, general};
/* "Strict" yields error on pseudoempty clauses (without existential
    variables); having such clauses can make sense for autarky search.
*/
std::ostream& operator <<(std::ostream& out, const ConformityLevel cl) noexcept {
  switch (cl) {
  case ConformityLevel::general : return out << "general";
  case ConformityLevel::normal : return out << "normal";
  case ConformityLevel::strict : return out << "strict";
  case ConformityLevel::verystrict : return out << "verystrict";
  default : return out << "normal";
  }
}
// String to ConformityLevel:
ConformityLevel s2conlev(const std::string& s) {
  if (s == "s") return ConformityLevel::strict;
  else return ConformityLevel::normal;
}

class ReadDimacs {

std::istream& in;
ClauseSet F;
ConformityLevel conlev;

// Aborts via std::exit in case of input-errors:
void read_header() noexcept {
  assert(in.exceptions() == 0);
  assert(in.good());
  std::string line;
  while (true) {
    std::getline(in, line);
    assert(not line.empty());
    const auto c = line[0];
    if (c == '\0') { // empty line
      errout << "Empty line (no p-line found).";
      std::exit(code(Error::file_reading));
    }
    if (c == 'p') break;
    if (c != 'c') {
      errout << "Comment lines must start with \"c\".";
      std::exit(code(Error::file_reading));
    }
  }
  assert(line[0] == 'p');
  std::stringstream s(line);
  {std::string inp; s >> inp;
   if (inp != "p") {
     errout << "Syntax error in parameter line (\"p\" not followed by space).";
     std::exit(code(Error::file_pline));
   }
   assert(s);
   if (s.eof()) {
     errout << "Syntax error in parameter line (p-line ends after \"p\").";
     std::exit(code(Error::file_pline));
   }
   s >> inp;
   if (not s or inp != "cnf") {
     errout << "Syntax error in parameter line (no \"cnf\").";
     std::exit(code(Error::file_pline));
   }
  }
  s >> F.n_pl;
  if (not s) {
    errout << "Reading error with parameter maximal-variable-index "
      "(too big or not-a-number).";
    std::exit(code(Error::file_pline));
  }
  if (not valid(F.n_pl)) {
    errout << "Parameter maximal-variable-index n=" << F.n_pl <<
      " is too big for numeric_limits<Lit_int>::max=" << max_lit << ".";
    std::exit(code(Error::num_vars));
  }
  s >> F.c_pl;
  if (not s) {
    errout << "Reading error with parameter number-of-clauses "
      "(too big or not-a-number).";
    std::exit(code(Error::file_pline));
  }
  if (not s.eof()) {
    errout << "Syntax error in parameter line (something after c-parameter).";
    std::exit(code(Error::file_pline));
  }
}

void read_dependencies() noexcept {
  assert(in.exceptions() == 0);
  assert(in.good());
  try { F.vt.resize(F.n_pl+1); F.D.resize(F.n_pl+1); }
  catch (const std::bad_alloc&) {
    errout << "Allocation error for dependency-vector of size "<<F.n_pl<<".";
    std::exit(code(Error::allocation));
  }
  struct Finish { // marking unset variables as fe with empty domain
    ClauseSet& F0;
    Finish(ClauseSet& F) : F0(F) {}
    ~Finish() {
      const Dependency emptyset = F0.dep_sets.find(Varset());
      assert(emptyset != F0.dep_sets.end());
      for (Var v = 1; v <= F0.n_pl; ++v)
        if (F0.vt[v] == VT::und) {
          F0.vt[v] = VT::fe; ++F0.ne_d;
          F0.D[v] = emptyset;
        }
      assert(F0.n_pl == F0.na_d + F0.ne_d);
    }
  } finish(F);
  Varset A;
  Dependency dep = F.dep_sets.insert(A).first;
  std::string line;
  enum class lt { begin, e, a, d }; // line type
  lt last_line = lt::begin;
  if (in.eof()) return;
  while (true) {
    const int peek = in.peek();
    if (peek == std::char_traits<char>::eof()) return;
    if (peek != 'a' and peek != 'e' and peek != 'd') return;
    std::getline(in, line);
    std::stringstream s(line);
    {std::string skip; s >> skip;}
    if (peek == 'a') {
      if (last_line == lt::a) {
         errout << "Repeated a-line."; std::exit(code(Error::a_rep_line));
      }
      Count_t num_a = 0;
      do {
        Var v;
        if (not (s >> v)) {
          errout << "Bad a-read."; std::exit(code(Error::a_read));
        };
        if (v > F.n_pl) {
          errout << "a-variable " << v << " contradicts n=" << F.n_pl << ".";
          std::exit(code(Error::variable_value));
        }
        if (v == 0) break;
        if (F.vt[v] != VT::und) {
          errout << "Repeated a-read."; std::exit(code(Error::a_rep));
        }
        F.vt[v] = VT::fa;
        A.insert(v); ++num_a; ++F.na_d;
      } while (true);
      if (not s) {
        errout << "Bad a-line read."; std::exit(code(Error::a_line_read));
      }
      if (num_a == 0) {
        errout << "Empty a-line."; std::exit(code(Error::a_empty));
      }
      last_line = lt::a;
      const auto insert = F.dep_sets.insert(A);
      assert(insert.second);
      dep = insert.first;
    } else if (peek == 'e') {
      if (last_line == lt::e) {
        errout << "Repeated e-line."; std::exit(code(Error::e_rep_line));
      }
      Count_t num_e = 0;
      do {
        Var v;
        if (not (s >> v)) {
          errout << "Bad e-read."; std::exit(code(Error::e_read));
        };
        if (v > F.n_pl) {
          errout << "e-variable " << v << " contradicts n=" << F.n_pl << ".";
          std::exit(code(Error::variable_value));
        }
        if (v == 0) break;
        if (F.vt[v] != VT::und) {
          errout << "Repeated e-read."; std::exit(code(Error::e_rep));
        }
        F.vt[v] = VT::fe;
        F.D[v] = dep; ++num_e; ++F.ne_d;
      } while (true);
      if (not s) {
        errout << "Bad e-line read."; std::exit(code(Error::e_line_read));
      }
      if (num_e == 0) {
        errout << "Empty e-line."; std::exit(code(Error::e_empty));
      }
      last_line = lt::e;
    } else {
      assert(peek == 'd');
      Var v;
      if (not (s >> v)) {
        errout << "Bad e-read in d-line."; std::exit(code(Error::e_read));
      };
      if (v > F.n_pl) {
        errout << "e-variable " << v << " contradicts n=" << F.n_pl << ".";
        std::exit(code(Error::variable_value));
      }
      if (v == 0) {
        errout << "Empty d-line."; std::exit(code(Error::d_empty));
      }
      if (F.vt[v] != VT::und) {
        errout << "Repeated e-read in d-line."; std::exit(code(Error::e_rep));
      }
      F.vt[v] = VT::fe;
      ++F.ne_d;
      Varset A;
      do {
        Var w;
        if (not (s >> w)) {
          errout << "Bad a-read in d-line."; std::exit(code(Error::a_read));
        };
        if (w > F.n_pl) {
          errout << "a-variable " << w << " contradicts n=" << F.n_pl << ".";
          std::exit(code(Error::variable_value));
        }
        if (w == 0) break;
        if (F.vt[w] != VT::fa) {
          errout << "Not defined a."; std::exit(code(Error::d_bada));
        }
        const auto insert = A.insert(w);
        if (not insert.second) {
          errout << "Repeated a-read in d-line.";
          std::exit(code(Error::a_rep));
        }
      } while (true);
      F.D[v] = F.dep_sets.insert(A).first;
      last_line = lt::d;
    }
  }
}


// Returns false iff no (further) clause was found;
// reference-parameter C is empty iff a tautological clause was found:
bool read_clause(DClause& C) const noexcept {
  assert(in.exceptions() == 0);
  Lit x;
  assert(in.good());
  in >> x;
  if (in.eof()) return false;
  C.clear();
  Clause CA, CE; // complemented clauses
  while (true) { // reading literals into C
    if (not in) {
      errout << "Invalid literal-read."; std::exit(code(Error::literal_read));
    }
    assert(in.good());
    if (not x) break; // end of clause
    const Var v = var(x);
    if (v > F.n_pl) {
      errout << "Literal " << x << " contradicts n=" << F.n_pl << ".";
      std::exit(code(Error::variable_value));
    }
    if (at(F.vt[v]))
      if (CA.find(x) != CA.end()) { // tautology
        C.clear();
        do
          if (not (in >> x)) {
            errout << "Invalid literal-read in tautological a-clause.";
            std::exit(code(Error::literal_read));
          }
        while (x);
        return true;
      }
      else {C.P.first.insert(x); CA.insert(-x);}
    else {
      assert(et(F.vt[v]));
      if (CE.find(x) != CE.end()) { // tautology
        C.clear();
        do
          if (not (in >> x)) {
            errout << "Invalid literal-read in tautological e-clause.";
            std::exit(code(Error::literal_read));
          }
        while (x);
        return true;
      }
      else {C.P.second.insert(x); CE.insert(-x);}
    }
    in >> x;
  }
  switch (conlev) {
  case ConformityLevel::general : ;
  case ConformityLevel::normal :
    if (C.empty()) {
      errout << "Empty clause.";
      std::exit(code(Error::empty_clause));
    }
    break;
  default :
    if (C.pseudoempty()) {
      errout << "Clause without existential variables.";
      std::exit(code(Error::pseudoempty_clause));
    }
  }
  return true;
}

// Error only if announced number of clauses too small (but may be too big):
inline void add_clause(const DClause& C) {
  if (F.c + F.t >= F.c_pl) {
    errout << "More than " << F.c_pl << " clauses, contradicting cnf-header.";
    std::exit(code(Error::number_clauses));
  }
  if (C.empty()) { // means tautology here
    ++F.t; return;
  }
  const auto insert = F.F.insert(C);
  if (insert.second) {
    ++F.c;
    const Var sa = C.P.first.size(), se = C.P.second.size();
    F.la += sa; F.le += se;
    F.max_a_length = std::max(sa, F.max_a_length);
    F.max_e_length = std::max(se, F.max_e_length);
    F.max_c_length = std::max(sa+se, F.max_c_length);
    for (const Lit x : C.P.first) {
      const Var v = var(x);
      ++F.vardeg[v];
      if (F.vt[v] != VT::fa) continue;
      F.max_a_index = std::max(v, F.max_a_index);
      F.vt[v] = VT::a; ++F.na;
    }
    for (const Lit x : C.P.second) {
      const Var v = var(x);
      ++F.vardeg[v];
      if (F.vt[v] != VT::fe) continue;
      F.max_e_index = std::max(v, F.max_e_index);
      F.vt[v] = VT::e; ++F.ne;
    }
  }
}

// Counting dependency-sets, and removing unused ones:
void count_dependencies() {
  for (Var v = 1; v <= F.max_e_index; ++v) {
    if (F.vt[v] != VT::e) continue;
    const Dependency_p dp = &*F.D[v];
    ++F.dc[dp];
  }
  for (auto i = F.dep_sets.begin(); i != F.dep_sets.end();) {
    const Dependency_p dp = &*i;
    const auto find = F.dc.find(dp);
    auto j = i; ++j;
    if (find == F.dc.end()) F.dep_sets.erase(i);
    i = j;
  }
}

// Removing such a-variables from clauses, which aren't in a dependency of
// some e-variable in that clause; degrade a-variables to formal a-variables
// at the end accordingly:
void cleanup_clauses() noexcept {

}

// Shrink dependencies by removing formal a-variables:
void cleanup_dependencies() noexcept {

}

public :

ReadDimacs(std::istream& in, const ConformityLevel cl) noexcept :
  in(in), conlev(cl) {}

ClauseSet operator()() {
  read_header();
  if (in.eof()) return F;
  read_dependencies();
  if (in.eof()) return F;
  try { F.vardeg.resize(F.n_pl+1); }
  catch (const std::bad_alloc&) {
    errout << "Allocation error for degree-vector of size "<<F.n_pl<<".";
    std::exit(code(Error::allocation));
  }
  {DClause C;
   while (read_clause(C)) {
     add_clause(C);
   }
  }
  F.max_index = std::max(F.max_a_index, F.max_e_index);
  F.n = F.na + F.ne;
  F.l = F.la + F.le;
  assert(F.c == F.F.size());

  count_dependencies();
  F.count_dep = F.dep_sets.size();
  for (const Varset& D : F.dep_sets) {
    const Var size = D.size();
    F.max_s_dep = std::max(size, F.max_s_dep);
    F.min_s_dep = std::min(size, F.min_s_dep);
  }

  cleanup_clauses();
  cleanup_dependencies();

  return F;
}

}; // class ReadDimacs


// --- Translation ---

struct Encoding {

  const ClauseSet& F;

  // Vector of existential variables:
  typedef std::vector<Var> Evar_vec;
  const Evar_vec E;
  // For each e-var its index in E;
  const Evar_vec E_index;

  // Vector of dependencies as vectors:
  typedef std::vector<Var> Avar_vec;
  typedef std::vector<Avar_vec> Dep_vec;
  const Dep_vec dep;

  // Vector of Dclause-iterators:
  typedef std::vector<dclause_it> Dclause_vec;
  typedef Dclause_vec::size_type clause_index_t;
  const Dclause_vec dclauses;

  // Vector of bf-indices (for given e-index the starting point, plus at the
  // last position the one index "past-the-end"):
  typedef std::vector<Var> Var_vec;
  const Var_vec bfvar_indices;

  // The set of all partial assignments occurring:
  typedef PassSet::const_pointer Pass_p;
  typedef std::set<Pass_p> Solution_set;
  typedef std::vector<Solution_set> Solset_vec;
  typedef std::pair<PassSet, Solset_vec> All_solutions;
  const All_solutions all_solutions;

  const Var ncs, nbf, npa, n;

  typedef std::map<Pass_p,Var> EncodingPass;
  const EncodingPass enc_pass;

  Encoding(const ClauseSet& F) :
    F(F), E(extract_evar()), E_index(extract_eindices()), dep(convert_dependencies()), dclauses(list_iterators()), bfvar_indices(set_bfvar_indices()), all_solutions(set_all_solutions()), ncs(F.c), nbf(bfvar_indices.back() - F.c), npa(all_solutions.first.size()), n(ncs+nbf+npa), enc_pass(set_pass_encoding()) {}

  Var csvar(const clause_index_t C) const noexcept {
    assert(C < F.c);
    return C+1;
  }

  Var bfvar(const Var v, const Litc f) const noexcept {
    const Var i = E_index[v];
    assert(i < F.ne);
    const Var base_index = bfvar_indices[i];
    const Lit x{f};
    const Var w = var(Lit(f));
    assert(w == 0 or F.D[w]->find(w) != F.D[w]->end());
    if (w == 0) {
      assert(f.constant());
      return (f == bf(false)) ? base_index : base_index + 1;
    }
    const auto w_it = std::lower_bound(dep[i].begin(), dep[i].end(), w);
    assert(*w_it == w);
    const Var j = base_index + 2 + 2 * (w_it - dep[i].begin());
    return (x.negi()) ? j : j+1;
  }

  Var pavar(const Pass_p phi) const noexcept {
    const auto find = enc_pass.find(phi);
    assert(find != enc_pass.end());
    return find->second;
  }

private :

  Evar_vec extract_evar() const {
    Evar_vec e;
    e.reserve(F.ne);
    for (Var v = 1; v <= F.max_e_index; ++v)
      if (F.vt[v] == VT::e) e.push_back(v);
    assert(e.size() == F.ne);
    return e;
  }

  Evar_vec extract_eindices() const {
    Evar_vec ei;
    ei.reserve(F.max_e_index+1);
    for (Var i = 0; i < F.ne; ++i) ei[E[i]] = i;
    return ei;
  }

  Dclause_vec list_iterators() const {
    Dclause_vec dc;
    dc.reserve(F.c);
    for (dclause_it it = F.F.begin(); it != F.F.end(); ++it) dc.push_back(it);
    return dc;
  }

  Dep_vec convert_dependencies() const {
    Dep_vec d;
    d.resize(F.ne);
    for (Var i = 0; i < F.ne; ++i) {
      const Var v = E[i];
      d[i].assign(F.D[v]->begin(), F.D[v]->end());
    }
    return d;
  }

  Var_vec set_bfvar_indices() const {
    Var_vec ind;
    ind.resize(F.ne+1);
    Var current = F.c;
    for (Var i = 0; i < F.ne; ++i) {
      ind[i] = current;
      current += 2*F.D[E[i]]->size() + 2;
    }
    ind[F.ne] = current;
    return ind;
  }

  All_solutions set_all_solutions() const {
    All_solutions all_sol;
    all_sol.second.resize(F.c);
    for (Count_t ci = 0; ci < F.c; ++ci) {
      const DClause& C(*dclauses[ci]);
      Solution_set& pas(all_sol.second[ci]);
      for (const Lit x : C.P.second) {
        const Var v = var(x);
        Pass pa; pa[v] = bf(x.posi());
        pas.insert(&*all_sol.first.insert(pa).first);
      }
      for (const Lit x : C.P.first) {
        const Var v = var(x);
        for (const Lit y : C.P.second) {
          const Var w = var(y);;
          if (F.D[w]->find(v) != F.D[w]->end()) {
            Pass pa; pa[w] = Litc( (sign(x)==sign(y)) ? -x : x);
            pas.insert(&*all_sol.first.insert(pa).first);
          }
        }
      }
      for (const Lit x : C.P.second) {
        const Var v = var(x);
        const auto begx = F.D[v]->begin();
        const auto endx = F.D[v]->end();
        for (const Lit y : C.P.second) {
          if (not (x < y)) continue;
          const Var w = var(y);
          assert(v != w);
          std::vector<Var> I;
          std::set_intersection(begx,endx,F.D[w]->begin(),F.D[w]->end(),std::back_inserter(I));
          for (const Var u : I) {
            const Litc u1{Lit(u)}, u2{(sign(x)==sign(y))?-u1:u1};
            Pass pa; pa[v]=u1, pa[w]=u2;
            pas.insert(&*all_sol.first.insert(pa).first);
            pa.clear(); pa[v]=-u1, pa[w]=-u2;
            pas.insert(&*all_sol.first.insert(pa).first);
          }
        }
      }
    }
    return all_sol;
  }

  EncodingPass set_pass_encoding() const {
    EncodingPass ep;
    Var v = ncs + nbf;
    for (auto phi_it = all_solutions.first.begin(); phi_it != all_solutions.first.end(); ++phi_it)
      ep[&*phi_it] = ++v;
    assert(v == ncs+nbf+npa);
    return ep;
  }

};


// --- Output ---

void show_usage() {
  std::cout << "USAGE:\n"
    "> " << program << " (-v | --version)\n"
    " shows version informations and exits.\n"
    "> " << program << " (-cin | filename)\n"
    " runs the translator with input from standard input or filename.\n"
    "> " << program << " (-cin | filename) (-cout | -cerr | filename2 | -nil)\n"
      " furthermore appends the DIMACS-output to standard output or standard error or filename2, or ignores it\n "
      "(default is -cout).\n"
    "The same redirection can be done with the statistics output (as a third command-argument; default is -cout).\n"
    "For example, with\n"
    "> " << program << " -cin Out -nil\n"
    "input comes from standard input, the translation is put to file Out, and the statistics are discarded.\n"
    "While with\n"
    "> " << program << " In Out Out\n"
    "the input comes from file In, and both translations and statistics are appended to Out "
      "(first the statistics).\n";
  std::exit(0);
}

#define S(x) #x
#define STR(x) S(x)

void version_information() {
  std::cout << program << ":\n"
   " author: Oliver Kullmann\n"
   " url:\n"
   "  https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Quantification/DQCNF/AutarkiesL1.cpp\n"
   " Version: " << version << "\n"
   " Last change date: " << date << "\n"
   " Macro settings:\n"
   "  LIT_TYPE = " STR(LIT_TYPE) " (with " << std::numeric_limits<Lit_int>::digits << " binary digits)\n"
#ifdef NDEBUG
   " Compiled with NDEBUG\n"
#else
   " Compiled without NDEBUG\n"
#endif
#ifdef __OPTIMIZE__
   " Compiled with optimisation options\n"
#else
   " Compiled without optimisation options\n"
#endif
   " Compilation date: " __DATE__ " " __TIME__ "\n"
#ifdef __GNUC__
   " Compiler: g++, version " __VERSION__ "\n"
#else
   " Compiler not gcc\n"
#endif
#ifdef OKLIB
   " Provided in the OKlibrary " "http://" STR(OKLIB) "\n"
#endif
#ifdef GIT_ID
   " Git ID = " STR(GIT_ID) "\n"
#endif
  ;
  std::exit(0);
}

void output(const std::string filename, const ConformityLevel cl, const ClauseSet& F, const Encoding& enc) {
  logout <<
         "c Parameter (command line, file):\n"
         "c file_name                             " << filename << "\n"
         "c conformity_level                      " << cl << "\n"
         "c maximal_index_variables               " << F.n_pl << "\n"
         "c number_clauses                        " << F.c_pl << "\n"
         "c number_a_variables                    " << F.na_d << "\n"
         "c number_e_variables                    " << F.ne_d << "\n"
         "c number_tautological clauses           " << F.t << "\n"
         "c Actually occurring:\n"
         "c max_index_variable                    " << F.max_index << "\n"
         "c num_variables                         " << F.n << "\n"
         "c num_clauses                           " << F.c << "\n"
         "c num_a_variables                       " << F.na << "\n"
         "c num_e_variables                       " << F.ne << "\n"
         "c Additional statistics:\n"
         "c max_index_a_variable                  " << F.max_a_index << "\n"
         "c max_index_e_variable                  " << F.max_e_index << "\n"
         "c max_a_clause_length                   " << F.max_a_length << "\n"
         "c max_e_clause_length                   " << F.max_e_length << "\n"
         "c max_clause_length                     " << F.max_c_length << "\n"
         "c min_dep_size                          " << F.min_s_dep << "\n"
         "c max_dep_size                          " << F.max_s_dep << "\n"
         "c num_different_dep_sets                " << F.count_dep << "\n"
         "c num_a_literal_occurrences             " << F.la << "\n"
         "c num_e_literal_occurrences             " << F.le << "\n"
         "c num_literal_occurrences               " << F.l << "\n"
         "c Encoding:\n"
         "c ncs                                   " << enc.ncs << "\n"
         "c nbf                                   " << enc.nbf << "\n"
         "c npa                                   " << enc.npa << "\n"
         "c n                                     " << enc.n << "\n";
  logout.endl();
}



} // anonymous namespace

int main(const int argc, const char* const argv[]) {
  if (argc == 1) show_usage();
  const std::string filename = argv[1];
  if (filename == "-v" or filename == "--version") version_information();
  if (argc >= 3 and filename == std::string(argv[2])) {
      errout << "Output filename: \"" << argv[2]  << "\" identical with input filename.";
      std::exit(code(Error::file_writing));
  }
  if (argc >= 4 and filename == std::string(argv[3])) {
      errout << "Log filename: \"" << argv[3]  << "\" identical with input filename.";
      std::exit(code(Error::file_writing));
  }
  const ConformityLevel conlev = (argc >= 5) ? s2conlev(argv[4]) : ConformityLevel::normal;

  set_output(argc, argv);
  const Input in(filename);
  ReadDimacs rd(*in, conlev);
  const ClauseSet F = rd();

  const Encoding enc(F);

  output(filename, conlev, F, enc);
}
