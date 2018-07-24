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

> autL1 input [file-output] [log] [conformity-level=n] [log-level=0]

A parameter can only be used iff all parameters to the left of it are
specified.

For the input, either a filename or "-cin" (standard input) can be used.

For the two outputs, file-output and log, the default is standard output.
Other possibilities are:
 - a filename (possible equal for both)
 - "-cout" for standard output
 - "-cerr" for standard error
 - "-clog" for standard log
 - "-nil" for no output.

Conformity-level "g" (for "general") admits c-lines and repeated/empty a-lines
in the dependency-section, and also allows empty clauses.
Level "s" (for "strict") disallows clauses without existential variables
("pseudo-empty" clauses).

The default of log-level is 0, while
log-level "1" has the original input shown, and
log-level "2" has additionally information on the encoding in
the comments-section of the translated problem.


BUGS:

Running on the DQBF-instances from QBFEVAL18 (cswsok):

DQCNF> time ./Run ./autL1 "g" ~/OKplatform/QBF/EVAL18/QBFEVAL_18_DATASET/dqbf18
real    8m58.213s
user    7m32.069s
sys     1m25.115s

Out of memory:

bloem_ifm_genbuf16n.dqdimacs
bloem_ifm_genbuf16y.dqdimacs

Below, when running not with output=-nil, but into a file, there was no
OOM?


RESULTS:

On cswsok:
DQCNF> time ./RunLog ./autL1 "g" ~/OKplatform/QBF/EVAL18/QBFEVAL_18_DATASET/dqbf18
real    8m50.912s
user    7m32.028s
sys     1m17.898s

Here the same two OOM as above.

On csverify, also solving the instances, and now without OOM:

DQCNF> time ./RunRun ./autL1 "g" ~/QBF/QBFEVAL/dqbf18
RunRun in version 0.1.1, logfile = RunRun_2018-07-24_14-59-28
real    110m42.822s
user    107m49.960s
sys     2m50.464s

DQCNF> cat RunRun_2018-07-24_14-59-28 | ./ExtractAutL1 > RunRun_2018-07-24_14-59-28.R

In R:
> E=read.table("RunRun_2018-07-24_14-59-28.R", header=TRUE)
> summary(subset(E, select=-name))
      no                co               na               ne
 Min.   :    9.0   Min.   :    16   Min.   :  1.00   Min.   :    3.0
 1st Qu.:  139.0   1st Qu.:   853   1st Qu.: 15.00   1st Qu.:  127.2
 Median :  521.5   Median :  1551   Median : 45.50   Median :  431.0
 Mean   :  818.2   Mean   :  3654   Mean   : 57.09   Mean   :  761.3
 3rd Qu.:  878.8   3rd Qu.:  2406   3rd Qu.: 81.75   3rd Qu.:  710.0
 Max.   :19096.0   Max.   :151339   Max.   :222.00   Max.   :19086.0
      mind             maxd              nd               lo
 Min.   :  0.00   Min.   :  1.00   Min.   :  2.00   Min.   :    36
 1st Qu.:  2.00   1st Qu.: 11.00   1st Qu.:  4.00   1st Qu.:  2094
 Median :  3.00   Median : 45.50   Median :  4.00   Median :  3767
 Mean   : 10.61   Mean   : 56.12   Mean   : 69.58   Mean   : 17556
 3rd Qu.: 11.00   3rd Qu.: 81.75   3rd Qu.:  7.00   3rd Qu.:  6600
 Max.   :101.00   Max.   :222.00   Max.   :578.00   Max.   :421239
       n                 c                   l                  sat
 Min.   :     77   Min.   :      255   Min.   :      582   Min.   :0.00000
 1st Qu.:  10652   1st Qu.:   138114   1st Qu.:   319876   1st Qu.:0.00000
 Median :  80162   Median :  1541718   Median :  3323704   Median :0.00000
 Mean   : 224072   Mean   :  6183005   Mean   : 12789023   Mean   :0.01198
 3rd Qu.: 241889   3rd Qu.:  5275142   3rd Qu.: 10709316   3rd Qu.:0.00000
 Max.   :5273364   Max.   :185447922   Max.   :374806131   Max.   :1.00000
       t
 Min.   :   0.00
 1st Qu.:   0.11
 Median :   1.10
 Mean   :  17.52
 3rd Qu.:   5.24
 Max.   :1315.18
> nrow(E)
[1] 334

There are four instances with level-1-autarkies:

> E[E$sat==1,]
                                                     name  no  co na  ne mind
64                              dqbf18/bloem_ex2.dqdimacs  60 139 10  50    4
187 dqbf18/tentrup17_ltl2dba_theta_environment_1.dqdimacs 248 732  3 246    1
234                             dqbf18/bloem_eq1.dqdimacs   9  16  1   8    0
300                             dqbf18/bloem_ex1.dqdimacs  23  52  3  20    1
    maxd nd   lo    n     c     l sat    t
64    10  4  323 2787 20920 46047   1 0.01
187    3  4 1910 6864 30619 72436   1 0.14
234    1  2   36   77   255   582   1 0.00
300    3  4  120  422  1912  4394   1 0.00


TODOS:

1. Clean-up handling of statistics

   And a more systematic treatment is needed, perhaps computing bundles
   of connected statistics all at once, and after the basic data (clauses,
   dependencies etc.) are established.

   For the handling of minima perhaps some helper-function is used, which
   writes "NaN" under appropriate circumstances.

2. More statistics on dependencies:
    - average size
    - number of maximal and minimal elements
    - maximum length of a chain (height)
    - width (maximum length of an antichain)

3. Implementing linear-size version of amo (using auxiliary variables)

    - A command-line variables specifies the number of variables from which
      on that version of amo is used.
    - See
      ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
    - Two versions: with and without unique-extension-property (the former
      when we want to count solutions); again a command-line parameter is
      needed.

4. Implement
    - cleanup_clauses()
    - cleanup_dependencies()

Once done, the assert
  assert(w == 0 or F.vt[w] == VT::a or F.vt[w] == VT::fa);
in function bfvar needs to be updated.

Three steps:
  (a) First remove universal literals from clauses, where they are not
      part of any dependency.
  (b) Now a-variables may have become fa-variables, and that needs to be
      updated.
  (c) Finally remove all fa-variables from dependency-sets.

5. Determine the main parameters like number of pa-variables etc. from the
   parameters of the DQCNF.

   Use this for reserve-statements.

6. Test and improve error-handling

   Give the line-numbers with the errors.

   More information should be given in case of out-of-memory.

7. Improve merging output

   We need a possibility to discard the information on the translation-
   variables (it can be big).

   Perhaps the repetition of the input should come last (before the real
   output)?

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

const std::string version = "0.5.10";
const std::string date = "24.7.2018";

const std::string program = "autL1"
#ifndef NDEBUG
  "_debug"
#endif
;
const std::string author = "\"Oliver Kullmann\"";
const std::string url = "\"https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Quantification/DQCNF/AutarkiesL1.cpp\"";

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
   by function set_output from the command-line parameters.
   In general this class wrappes output-streams. The two
   public members (besides the constructor) are
     out << x;
     out.endl();
  which send the output to the internally stored ostream *p, if set.
  Via out.nil() it can be checked, whether p has been set.
  The destructor deletes *p iff member del = true.
  Furthermore Output-objects can be compared for equality (which refers to
  the underlying pointers).
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
  bool nil() const noexcept { return p == nullptr; }
  friend bool operator ==(const Output& lhs, const Output& rhs) noexcept {
    return lhs.p == rhs.p;
  }
  friend bool operator !=(const Output& lhs, const Output& rhs) noexcept {
    return lhs.p != rhs.p;
  }
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


template <typename T>
std::ostream& operator <<(std::ostream& out, const std::set<T>& S) {
  for (const T& x : S) out << " " << x;
  return out;
}
template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& v) {
  for (const T& x : v) out << " " << x;
  return out;
}
template <typename A, typename V>
std::ostream& operator <<(std::ostream& out, const std::map<A,V>& M) {
  for (const auto& p : M) out << " (" << p.first << "," << p.second << ")";
  return out;
}


// --- Data structures for literals and variables ---

/*
  The basic classes are Var (variables) and Lit (literals), where
  Lit contains Lit_int, "Literals as integers", which are signed integers,
  while variables are unsigned.

  Polarities pos, neg are expressed via the enumeration-type Pol.

  Var is just a typedef of an unsigned integral type.

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
   - y.posi(), y.negi() (whether y is positive resp. negative)
   - y.index() (yields Lit_int)
   - for-each loop: for (Pol p : Polarities)
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

typedef Var AVar;
typedef Var EVar;

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

  friend std::ostream& operator <<(std::ostream& out, const Lit x) noexcept {
    return out << x.x;
  }
  friend std::istream& operator >>(std::istream& in, Lit& x) noexcept {
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

typedef Lit ALit;
typedef Lit ELit;


/* Literals plus true/false (the boolean functions with at most one var);
   the linear order is 0,false,true,-1,1,-2,2, ... .
   Constructors (non-converting):
    - Litc() (singular)
    - Litc(x) for Lit x
    - Litc(t) for BFt t
   And bf(b) for bool b yields Litc.
   Exactly one of x.sing(), x.constant(), x.variable() is true for Litc x.
   Operations:
    - explicit conversions to Lit and BFt
    - negation (operator - and in-place member neg())
    - boolean member functions sing(), constant(), variable()
    - ==, !=, <
    - output-streaming.
*/
class Litc  {
  ALit x;
  BFt t;
  /* Class invariants:
      - assert(t==BFt::nc or not x;);
      - exactly one of sing(), constant() or variable() is true.
  */
  constexpr Litc(const ALit x, const BFt t) noexcept : x(x), t(t) {}
public :
  Litc() = default;
  constexpr explicit Litc(const ALit x) noexcept : x(x), t(BFt::nc) {}
  constexpr explicit Litc(const BFt t) noexcept : x(0), t(t) {}

  constexpr explicit operator BFt() const noexcept { return t; }
  constexpr explicit operator ALit() const noexcept { return x; }

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

  friend std::ostream& operator <<(std::ostream& out, const Litc x) noexcept {
    switch (x.t) {
    case BFt::f : return out << "bf(f)";
    case BFt::t : return out << "bf(t)";
    default : return out << "bf(" << x.x << ")";
    }
  }

};
static_assert(std::is_pod<Litc>::value, "Litc is not POD.");

inline constexpr Litc bf(const bool b) { return (b) ? Litc(BFt::t) : Litc(BFt::f); }

static_assert(ALit(Litc()) == 0_l, "Default construction of Litc does not yield singular literal.");
static_assert(BFt(Litc()) == BFt::nc, "Default construction of Litc is not nonconstant.");
// Remark: As usual, as a local variable, the declaration "Litc x;" does not
// initialise x.
static_assert(ALit(Litc(1_l)) == 1_l, "Construction of Litc does not pass literal.");
static_assert(BFt(Litc(1_l)) == BFt::nc, "Construction of Litc with literal is constant.");
static_assert(ALit(bf(false)) == 0_l, "Construction of Litc with constant does not make literal singular.");
static_assert(BFt(bf(false)) == BFt::f, "Construction of Litc with false does not yield false.");
static_assert(ALit(bf(true)) == 0_l, "Construction of Litc with constant does not make literal singular.");
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
std::ostream& operator <<(std::ostream& out, const VT t) noexcept {
  switch (t) {
  case VT::fa : return out << "fa";
  case VT::fe : return out << "fe";
  case VT::a : return out << "a";
  case VT::e : return out << "e";
  default : return out << "und";
  }
}
typedef std::vector<VT> VTvector;
inline constexpr bool et(const VT t) noexcept {return t==VT::fe or t==VT::e;}
inline constexpr bool at(const VT t) noexcept {return t==VT::fa or t==VT::a;}

typedef std::set<Var> Varset;
typedef Varset AVarset;
typedef Varset EVarset;
typedef std::set<AVarset> VarSetsystem;
typedef VarSetsystem::const_iterator Dependency;
typedef std::vector<Dependency> Dvector;
typedef VarSetsystem::const_pointer Dependency_p;
typedef std::map<Dependency_p, Count_t> DepCounts;

typedef std::set<Lit> Clause;
typedef Clause AClause;
typedef Clause EClause;
typedef std::pair<AClause,EClause> PairClause; // all-exists
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
  friend std::ostream& operator <<(std::ostream& out, const DClause& C) {
    return out << "E:" << C.P.second << "; A:" << C.P.first;
  }
};

typedef std::vector<Count_t> Degree_vec;

typedef std::vector<Clause> CLS;

typedef std::set<DClause> DCLS;
typedef DCLS::const_iterator dclause_it;

struct DClauseSet {
  DCLS F;
  VTvector vt;
  VarSetsystem dep_sets; // the occurring d-sets
  Dvector D; // for each variable its d-set
  DepCounts dc; // how often each d-set occurs
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
  Count_t t=0, empty=0, pempty=0; // number of tautological/empty/pseudoempty clauses

  friend std::ostream& operator <<(std::ostream& out, const DClauseSet& F) {
    out << "c  Variables:\nc  ";
    for (Var v = 1; v < F.vt.size(); ++v) out << " " << v << ":" << F.vt[v];
    out << "\nc  p cnf " << F.max_index << " " << F.c << "\n";
    for (Var v = 1; v < F.vt.size(); ++v)
      if (F.vt[v] == VT::e) out << "c  d " << v << *F.D[v] << " 0\n";
    for (const auto& C : F.F) out << "c  " << C << "\n";
    return out;
  }
};

typedef std::map<EVar,Litc> Pass;
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
  if (s == "g") return ConformityLevel::general;
  else if (s == "s") return ConformityLevel::strict;
  else if (s == "vs") return ConformityLevel::verystrict;
  else return ConformityLevel::normal;
}

class ReadDimacs {

std::istream& in;
DClauseSet F;
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
      " is too big for numeric_limits<Lit_int>::max=" << max_lit;
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
    errout << "Allocation error for dependency-vector of size " << F.n_pl;
    std::exit(code(Error::allocation));
  }
  struct Finish { // marking unset variables as fe with empty domain
    DClauseSet& F0;
    Finish(DClauseSet& F) : F0(F) {}
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
  AVarset A;
  Dependency dep = F.dep_sets.insert(A).first;
  std::string line;
  enum class lt { begin, e, a, d }; // line type
  lt last_line = lt::begin;
  if (in.eof()) return;
  while (true) {
    const int peek = in.peek();
    if (peek == std::char_traits<char>::eof()) return;
    if (conlev == ConformityLevel::general and peek == 'c') {
      std::getline(in, line);
      continue;
    }
    if (peek != 'a' and peek != 'e' and peek != 'd') return;
    std::getline(in, line);
    std::stringstream s(line);
    {std::string skip; s >> skip;}
    if (peek == 'a') {
      if (conlev != ConformityLevel::general and last_line == lt::a) {
         errout << "Repeated a-line."; std::exit(code(Error::a_rep_line));
      }
      Count_t num_a = 0;
      do {
        Var v;
        if (not (s >> v)) {
          errout << "Bad a-read."; std::exit(code(Error::a_read));
        };
        if (v > F.n_pl) {
          errout << "a-variable " << v << " contradicts n=" << F.n_pl;
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
      if (num_a != 0) {
        last_line = lt::a;
        const auto insert = F.dep_sets.insert(A);
        assert(insert.second);
        dep = insert.first;
      } else {
      if (conlev != ConformityLevel::general) {
          errout << "Empty a-line."; std::exit(code(Error::a_empty));
        }
      }
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
          errout << "e-variable" << v << "contradicts n=" << F.n_pl;
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
        errout << "e-variable" << v << "contradicts n=" << F.n_pl;
        std::exit(code(Error::variable_value));
      }
      if (v == 0) {
        errout << "Empty d-line."; std::exit(code(Error::d_empty));
      }
      if (F.vt[v] != VT::und) {
        errout << "Repeated e-read" << v << "in d-line."; std::exit(code(Error::e_rep));
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
          errout << "a-variable " << w << " contradicts n=" << F.n_pl;
          std::exit(code(Error::variable_value));
        }
        if (w == 0) break;
        if (F.vt[w] != VT::fa) {
          errout << "Undefined a-variable" << w; std::exit(code(Error::d_bada));
        }
        const auto insert = A.insert(w);
        if (not insert.second) {
          errout << "Repeated a-read in d-line.";
          std::exit(code(Error::a_rep));
        }
      } while (true);
      F.D[v] = F.dep_sets.insert(A).first;
      last_line = lt::d;
    } // end of main if-then-else
  } // main loop
}

enum class RS { clause, none, empty, tautology, pseudoempty }; // read-status

// Reference-parameter C is empty if no or a tautological clause was found:
RS read_clause(DClause& C) const noexcept {
  assert(in.exceptions() == 0);
  assert(in.good());
  C.clear();
  Lit x;
  in >> x;
  if (in.eof()) return RS::none;
  AClause CA; EClause CE; // complemented clauses
  while (true) { // reading literals into C
    if (not in) {
      errout << "Invalid literal-read at beginning of clause.";
      std::exit(code(Error::literal_read));
    }
    assert(in.good());
    if (not x) break; // end of clause
    const Var v = var(x);
    if (v > F.n_pl) {
      errout << "Literal " << x << " contradicts n=" << F.n_pl;
      std::exit(code(Error::variable_value));
    }
    if (at(F.vt[v]))
      if (CA.find(x) != CA.end()) { // tautology via universal literals
        C.clear();
        do
          if (not (in >> x)) {
            errout << "Invalid literal-read in tautological a-clause.";
            std::exit(code(Error::literal_read));
          }
        while (x);
        return RS::tautology;
      }
      else {C.P.first.insert(x); CA.insert(-x);}
    else {
      assert(et(F.vt[v]));
      if (CE.find(x) != CE.end()) { // tautology via existential literals
        C.clear();
        do
          if (not (in >> x)) {
            errout << "Invalid literal-read in tautological e-clause.";
            std::exit(code(Error::literal_read));
          }
        while (x);
        return RS::tautology;
      }
      else {C.P.second.insert(x); CE.insert(-x);}
    }
    in >> x;
  }
  switch (conlev) {
  case ConformityLevel::general :
    if (C.empty()) return RS::empty;
    if (C.pseudoempty()) return RS::pseudoempty;
    break;
  case ConformityLevel::normal :
    if (C.empty()) {
      errout << "Empty clause.";
      std::exit(code(Error::empty_clause));
    }
    if (C.pseudoempty()) return RS::pseudoempty;
    break;
  default :
    if (C.empty()) {
      errout << "Empty clause.";
      std::exit(code(Error::empty_clause));
    }
    if (C.pseudoempty()) {
      errout << "Clause without existential variables.";
      std::exit(code(Error::pseudoempty_clause));
    }
  }
  return RS::clause;
}

// Error only if announced number of clauses too small (but may be too big):
inline void add_clause(const DClause& C) {
  if (F.c + F.t >= F.c_pl) {
    errout << "More than " << F.c_pl << " clauses, contradicting cnf-header.";
    std::exit(code(Error::number_clauses));
  }
  const auto insert = F.F.insert(C);
  if (insert.second) {
    ++F.c;
    const Var sa = C.P.first.size(), se = C.P.second.size();
    F.la += sa; F.le += se;
    F.max_a_length = std::max(sa, F.max_a_length);
    F.max_e_length = std::max(se, F.max_e_length);
    F.max_c_length = std::max(sa+se, F.max_c_length);
    for (const ALit x : C.P.first) {
      const Var v = var(x);
      ++F.vardeg[v];
      if (F.vt[v] != VT::fa) continue;
      F.max_a_index = std::max(v, F.max_a_index);
      F.vt[v] = VT::a; ++F.na;
    }
    for (const ELit x : C.P.second) {
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

DClauseSet operator()() {
  read_header();
  if (in.eof()) return F;
  read_dependencies();
  if (in.eof()) return F;
  try { F.vardeg.resize(F.n_pl+1); }
  catch (const std::bad_alloc&) {
    errout << "Allocation error for degree-vector of size "<<F.n_pl<<".";
    std::exit(code(Error::allocation));
  }
  {DClause C; RS status;
   while ((status = read_clause(C)) != RS::none) {
     switch (status) {
     case RS::tautology : ++F.t; break;
     case RS::empty : ++F.empty; add_clause(C); break;
     case RS::pseudoempty : ++F.pempty; add_clause(C); break;
     default : add_clause(C);
     }
   }
  }
  F.max_index = std::max(F.max_a_index, F.max_e_index);
  F.n = F.na + F.ne;
  F.l = F.la + F.le;
  assert(F.c == F.F.size());

  count_dependencies();
  F.count_dep = F.dep_sets.size();
  for (const AVarset& D : F.dep_sets) {
    const Var size = D.size();
    F.max_s_dep = std::max(size, F.max_s_dep);
    F.min_s_dep = std::min(size, F.min_s_dep);
  }

  cleanup_clauses();
  cleanup_dependencies();

  return F;
}

}; // class ReadDimacs


// --- Encoding (of variables) ---

struct Encoding {

  const DClauseSet& F;

  // Vector of existential variables:
  typedef std::vector<EVar> Evar_vec;
  const Evar_vec E;
  // For each e-var its index in E;
  const Evar_vec E_index;

  // Vector of dependencies as sorted vectors:
  typedef std::vector<AVar> Avar_vec;
  typedef std::vector<Avar_vec> Dep_vec;
  const Dep_vec dep;

  // Vector of Dclause-iterators for e-var-indices:
  typedef std::vector<dclause_it> Dclause_vec;
  typedef Dclause_vec::size_type clause_index_t;
  const Dclause_vec dclauses;

  // Vector of bf-indices (for given e-index the starting point, plus at the
  // last position the "past-the-end"-index):
  typedef std::vector<Var> Var_vec;
  const Var_vec bfvar_indices;

  // The set of all occurring partial assignments (satisfying clauses):
  typedef PassSet::const_pointer Pass_p;
  typedef std::set<Pass_p> Solution_set; // yields the pa-variables
  typedef std::vector<Solution_set> Solset_vec; // for each clause-index the set of pointers to solution-pass's
  typedef std::pair<PassSet, Solset_vec> All_solutions;
  const All_solutions all_solutions;

  const Var ncs, nbf, npa, n;

  typedef std::map<Pass_p,Var> EncodingPass;
  const EncodingPass enc_pass;

  Encoding(const DClauseSet& F) :
    F(F), E(extract_evar()), E_index(extract_eindices()), dep(convert_dependencies()), dclauses(list_iterators()), bfvar_indices(set_bfvar_indices()), all_solutions(set_all_solutions()), ncs(F.c), nbf(bfvar_indices.back() - F.c - 1), npa(all_solutions.first.size()), n(ncs+nbf+npa), enc_pass(set_pass_encoding()) {
#if not NDEBUG
    for (auto it = all_solutions.first.begin(); it != all_solutions.first.end(); ++it)
      assert(enc_pass.find(&*it) != enc_pass.end());
#endif
  }

  Var csvar(const clause_index_t C) const noexcept {
    assert(C < F.c);
    return C+1;
  }

  Var bfvar(const EVar v, const Litc f) const noexcept {
    assert(v >= 1);
    assert(v <= F.max_e_index);
    assert(not f.sing());
    const Var i = E_index[v];
    assert(i < F.ne);
    const ALit x{f};
    const AVar w = var(x);
    assert(w <= F.max_a_index);
    assert(w == 0 or F.vt[w] == VT::a or F.vt[w] == VT::fa);
    assert(w == 0 or F.D[v]->find(w) != F.D[v]->end());
    const Var base_index = bfvar_indices[i];
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

  friend std::ostream& operator <<(std::ostream& out, const Encoding& enc) {
    out << "c cs-variables and their clauses: ncs = " << enc.ncs << "\n";
    for (clause_index_t i = 0; i < enc.ncs; ++i)
      out << "c  " << enc.csvar(i) << ": " << *enc.dclauses[i] << "\n";
    out << "c bf-variables and their boolean functions: nbf = " << enc.nbf << "\n";
    for (Var vi = 0; vi < enc.F.ne; ++vi) {
      const EVar v = enc.E[vi];
      out << "c  e-var " << v << " with ";
      const Var begin = enc.bfvar_indices[vi], end = enc.bfvar_indices[vi+1];
      out << (end-begin) << " bfs from " << begin << " to " << (end-1) << ":\nc   ";
      Var j = begin;
      out << j++ << ":" << bf(false) << ", ";
      out << j++ << ":" << bf(true);
      for (const AVar v : enc.dep[vi]) {
        out << ", " << j++ << ":" << Litc(Lit(v,Pol::n)) << ", ";
        out << j++ << ":" << Litc(Lit(v,Pol::p));
      }
      out << "\n";
    }
    out << "c pa-variables and their partial assignments: npa = " << enc.npa << "\n";
    {Var v = enc.ncs+enc.nbf;
     for (const Pass& phi : enc.all_solutions.first)
       out << "c  " << ++v << ":" << phi << "\n";
    }
    out << "c The clauses and their solution-sets (as pa-variables):\n";
    for (clause_index_t ci = 0; ci < enc.ncs; ++ci) {
      out << "c  " << enc.csvar(ci) << ":";
      const Solution_set& ss = enc.all_solutions.second[ci];
      std::vector<Var> v; v.reserve(ss.size());
      for (Pass_p p : ss) v.push_back(enc.enc_pass.find(p)->second);
      std::sort(v.begin(),v.end());
      out << v << "\n";
    }
    return out;
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
      const EVar v = E[i];
      d[i].assign(F.D[v]->begin(), F.D[v]->end());
    }
    return d;
  }

  Var_vec set_bfvar_indices() const {
    Var_vec ind;
    ind.resize(F.ne+1);
    Var current = F.c+1;
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
      assert(ci < all_sol.second.size());
      Solution_set& pas(all_sol.second[ci]);
      for (const ELit x : C.P.second) { // setting e-literals to true
        const EVar v = var(x);
        Pass pa; pa[v] = bf(x.posi());
        pas.insert(&*all_sol.first.insert(std::move(pa)).first);
      }
      AVarset V;
      for (const ALit x : C.P.first) { // e-literals as negations of a-literals
        const AVar v = var(x);
        V.insert(v);
        for (const ELit y : C.P.second) {
          const EVar w = var(y);
          if (F.D[w]->find(v) != F.D[w]->end()) {
            Pass pa; pa[w] = Litc( (y.posi()) ? -x : x);
            pas.insert(&*all_sol.first.insert(std::move(pa)).first);
          }
        }
      }
      for (const ELit x : C.P.second) { // two e-literals negating each other
        const EVar v = var(x);
        const auto begx = F.D[v]->begin();
        const auto endx = F.D[v]->end();
        for (const ELit y : C.P.second) {
          if (not (x < y)) continue;
          const EVar w = var(y);
          assert(v != w);
          std::vector<AVar> I;
          std::set_intersection(begx,endx,F.D[w]->begin(),F.D[w]->end(),std::back_inserter(I));
          for (const AVar u : I) {
            if (V.find(u) != V.end()) continue;
            const Litc u1{ALit(u)}, u2{(sign(x)==sign(y))?-u1:u1};
            Pass pa; pa[v]=u1, pa[w]=u2;
            pas.insert(&*all_sol.first.insert(pa).first);
            pa.clear(); pa[v]=-u1, pa[w]=-u2;
            pas.insert(&*all_sol.first.insert(std::move(pa)).first);
          }
        }
      }
    }
    return std::move(all_sol); // to emphasise the necessity (maintaining pointers)
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


// --- Translation ---

struct Translation {

  const DClauseSet& F;
  const Encoding& enc;

  mutable Count_t c_cs=0, c_palr=0, c_parl=0, c_P=0, c_N=0, c_amo=0, litocc=0;

  Translation(const DClauseSet& F, const Encoding& enc) noexcept : F(F), enc(enc) {}

  CLS operator()() const {
    CLS G;
    // Non-triviality clause:
    {Clause C;
     for (Encoding::clause_index_t i = 0; i < enc.ncs; ++i)
       C.insert(Lit(enc.csvar(i),Pol::p));
     assert(C.size() == enc.ncs);
     G.push_back(std::move(C)); ++c_cs; litocc += enc.ncs;
    }
    // Defining the pass's:
    {for (auto it = enc.all_solutions.first.begin(); it != enc.all_solutions.first.end(); ++it) {
      const Encoding::Pass_p phi_p = &*it;
      const Pass& phi = *it;
      const Var tphi = enc.pavar(phi_p);
      // from left to right, i.e., t(phi) -> and_{v in var(phi)} t(v,phi(v)):
      {const Lit negtphi = Lit(tphi,Pol::n);
       for (const auto& pair : phi) {
         Clause C; C.insert(negtphi);
         C.insert(Lit(enc.bfvar(pair.first, pair.second), Pol::p));
         assert(C.size() == 2);
         G.push_back(std::move(C)); ++c_palr; litocc += 2;
       }
      }
      // from right to left, i.e., (and_{v in var(phi)} t(v,phi(v))) -> t(phi):
      {Clause C; C.insert(Lit(tphi,Pol::p));
       for (const auto& pair : phi)
         C.insert(Lit(enc.bfvar(pair.first, pair.second), Pol::n));
       assert(C.size() ==  1 + phi.size());
       G.push_back(std::move(C)); ++c_parl; litocc += 1 + phi.size();
      }
     }
    }
    {for (Encoding::clause_index_t i = 0; i < enc.ncs; ++i) {
       // t(C) -> P(C):
       const Var tc = enc.csvar(i);
       {Clause C; C.insert(Lit(tc,Pol::n));
        const auto& S = enc.all_solutions.second[i];
        for (const Encoding::Pass_p& phi_p : S)
          C.insert(Lit(enc.pavar(phi_p),Pol::p));
        assert(C.size() == 1 + S.size());
        G.push_back(std::move(C)); ++c_P; litocc += 1 + S.size();
       }
       // -t(C) -> N(C):
       for (const Lit x : enc.dclauses[i]->P.second) {
         const Var v = enc.E_index[var(x)];
         assert(v < F.ne);
         for (Var bfi = enc.bfvar_indices[v]; bfi < enc.bfvar_indices[v+1]; ++bfi) {
           Clause C; C.insert(Lit(tc,Pol::p)); C.insert(Lit(bfi,Pol::n));
           assert(C.size() == 2);
           G.push_back(std::move(C)); ++c_N; litocc += 2;
         }
       }
     }
    }
    // Amo-clauses for bf-variables:
    {for (Var i = 0; i < F.ne; ++i) {
       const Var beg = enc.bfvar_indices[i], end = enc.bfvar_indices[i+1];
       for (Var v = beg; v < end; ++v)
         for (Var w = v+1; w < end; ++w) {
           Clause C; C.insert(Lit(v,Pol::n)), C.insert(Lit(w,Pol::n));
           assert(C.size() == 2);
           G.push_back(std::move(C)); ++c_amo; litocc += 2;
         }
     }
    }
    return G;
  }

};

// --- Output ---

enum class LogLevel {normal=0, withinput=1, withmeaning=2};
std::ostream& operator <<(std::ostream& out, const LogLevel l) noexcept {
  switch (l) {
  case LogLevel::withinput : return out << "with_input";
  case LogLevel::withmeaning : return out << "with_var_interpretations";
  default : return out << "normal";
  }
}
// String to LogLevel:
LogLevel s2loglev(const std::string& s) {
  if (s == "1") return LogLevel::withinput;
  else if (s == "2") return LogLevel::withmeaning;
  else return LogLevel::normal;
}


void show_usage() {
  std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version informations and exits.\n"
    "> " << program << " [-cin | filename]\n"
    " runs the translator with input from standard input or filename.\n"
    "> " << program << " [-cin | filename] [-cout | -cerr | filename2 | -nil]\n"
      " furthermore appends the DIMACS-output to standard output or standard error or filename2, or ignores it\n "
      "(default is -cout).\n"
    "The same redirection can be done with the log-output, as a third command-argument; default is -cout.\n"
    "If DIMACS- and log-output are equal, then first comes the log-output (as DIMACS-comment).\n"
    "A fourth optional argument is the conformity-level: g (general), n (normal), s (strict), vs (very strict).\n"
    "A fifth optional argument is the log-level: \"1\" means with input.\n"
    "\nFor example, with\n"
    "> " << program << " -cin Out -nil\n"
    "input comes from standard input, the translation is put to file Out, and the log is discarded.\n"
    "While with\n"
    "> " << program << " In Out Out\n"
    "the input comes from file In, and both translation and log are appended to Out.\n";
  std::exit(0);
}

#define S(x) #x
#define STR(x) S(x)

void version_information() {
  std::cout << program << ":\n"
   " author: " << author << "\n"
   " url:\n  " << url << "\n"
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

void output(const std::string filename, const ConformityLevel cl, const DClauseSet& F, const Encoding& enc, const Translation& trans, const CLS& G, const LogLevel ll) {
  logout <<
         "c Program information:\n"
         "c created_by                            " << program << "\n"
         "c version                               " << version << "\n"
         "c date                                  " << date << "\n"
         "c author                                " << author << "\n"
         "c url                                   " << url << "\n"
         "c Parameter (command line, file):\n"
         "c file_name                             " "\"" << filename << "\"\n"
         "c conformity_level                      " << cl << "\n"
         "c log_level                             " << ll << "\n"
         "c maximal_index_variables               " << F.n_pl << "\n"
         "c number_clauses                        " << F.c_pl << "\n"
         "c number_a_variables                    " << F.na_d << "\n"
         "c number_e_variables                    " << F.ne_d << "\n"
         "c number_tautological clauses           " << F.t << "\n"
         "c number_empty_clauses                  " << F.empty << "\n"
         "c number_pseudo_empty_clauses           " << F.pempty << "\n"
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
         "c min_dep_size                          ";
          if (F.count_dep!=0) logout<<F.min_s_dep; else logout<<"NaN";
          logout << "\n"
         "c max_dep_size                          ";
          if (F.count_dep!=0) logout<<F.max_s_dep; else logout<<"NaN";
          logout << "\n"
         "c num_different_dep_sets                " << F.count_dep << "\n"
         "c num_a_literal_occurrences             " << F.la << "\n"
         "c num_e_literal_occurrences             " << F.le << "\n"
         "c num_ae_literal_occurrences            " << F.l << "\n"
         "c Encoding:\n"
         "c ncs                                   " << enc.ncs << "\n"
         "c nbf                                   " << enc.nbf << "\n"
         "c npa                                   " << enc.npa << "\n"
         "c n                                     " << enc.n << "\n"
         "c Translation:\n"
         "c c_cs                                  " << trans.c_cs << "\n"
         "c c_palr                                " << trans.c_palr << "\n"
         "c c_parl                                " << trans.c_parl << "\n"
         "c c_P                                   " << trans.c_P << "\n"
         "c c_N                                   " << trans.c_N << "\n"
         "c c_amo                                 " << trans.c_amo << "\n"
         "c c                                     " << G.size() << "\n"
         "c num_literal_occurrences               " << trans.litocc << "\n";

  if (code(ll) >= 1) logout << "c Input DCNF (list of variables, then list of clauses, as pairs \"E;A\"):\n" << F;

  if (solout != logout) {
    solout << "c Program " << program << ": version " << version << ", " << date << ".\n";
    solout << "c Input: " << filename << "\n";
  }

  if (code(ll) >= 2) {
    if (not solout.nil()) {
      solout << "c Information on the meaning of translation-variables:\n";
      solout << enc;
    } else {
      logout << "c The meaning of translation-variables:\n";
      logout << enc;
    }
  }

  solout << "p cnf " << enc.n << " " << G.size() << "\n";
  for (const Clause& C : G) {
    for (const Lit x : C) solout << x << " ";
    solout << "0\n";
  }
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
  const LogLevel loglev = (argc >= 6) ? s2loglev(argv[5]) : LogLevel::normal;

  set_output(argc, argv);
  const Input in(filename);
  ReadDimacs rd(*in, conlev);
  const DClauseSet F = rd();

  const Encoding enc(F);

  const Translation trans(F,enc);
  const CLS G = trans();

  output(filename, conlev, F, enc, trans, G, loglev);
}
