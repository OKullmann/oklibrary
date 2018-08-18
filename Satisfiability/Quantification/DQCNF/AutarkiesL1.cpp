// Oliver Kullmann, 28.6.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Quantification/DQCNF/AutarkiesL1.cpp
  \brief A simple translation of the level-1 autarky problem for DQCNFs
  to SAT, using the direct encoding. The special feature of this translation
  is that it allows counting the number of level-1-autarkies.

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
For log also "=" is possible, which is then the same as file-output.

Conformity-level "g" (for "general") admits c-lines and consecutive/empty
a/e-lines in the dependency-section, and also allows empty clauses.

For example:

> echo -e "p cnf 0 0\na 0\na 0\ne 0\ne 0" | ./autL1_debug -cin -nil -nil g
> echo $?
0

while at log-level n for two a-lines and two e-lines we need to have
four variables:

> echo -e "p cnf 4 0\na 1 0\ne 2 0\na 3 0\ne 4 0" | ./autL1_debug -cin -cout -nil vs
c Program autL1_debug: version 0.6.4, 15.8.2018.
c Input: -cin
p cnf 0 1
0

Level "s" (for "strict") disallows clauses without existential variables
("pseudo-empty" clauses).

The default of log-level is 0, while
log-level "1" has the original input shown, and
log-level "2" has additionally information on the encoding in
the comments-section of the translated problem.


BUGS:

RESULTS:

Running on the DQBF-instances from QBFEVAL18 on csverify, also solving the
instances (using minsat-2.2.; succesful on all instances, due to more memory):

DQCNF> time ./RunRun ./autL1 "g" ~/QBF/QBFEVAL/dqbf18                         RunRun in version 0.1.1, logfile = RunRun_2018-08-16_11-30-55

real    108m28.775s
user    105m48.456s
sys     2m40.247s

DQCNF> cat RunRun_2018-08-16_11-30-55 | ./ExtractAutL1 > RunRun_2018-08-16_11-30-55.R

In R:
> E=read.table("RunRun_2018-08-16_11-30-55.R", header=TRUE)
> summary(subset(E, select=-name))
       np                cp               ni                ci
 Min.   :    9.0   Min.   :    16   Min.   :    9.0   Min.   :    16
 1st Qu.:  139.0   1st Qu.:   853   1st Qu.:  139.0   1st Qu.:   853
 Median :  521.5   Median :  1551   Median :  521.5   Median :  1551
 Mean   :  818.4   Mean   :  3654   Mean   :  818.2   Mean   :  3654
 3rd Qu.:  878.8   3rd Qu.:  2406   3rd Qu.:  878.8   3rd Qu.:  2406
 Max.   :19097.0   Max.   :151339   Max.   :19096.0   Max.   :151339
      nai              nei                li              mind
 Min.   :  1.00   Min.   :    3.0   Min.   :    36   Min.   :  0.00
 1st Qu.: 15.00   1st Qu.:  126.5   1st Qu.:  2094   1st Qu.:  2.00
 Median : 45.50   Median :  430.0   Median :  3767   Median :  3.00
 Mean   : 57.08   Mean   :  761.2   Mean   : 17556   Mean   : 10.61
 3rd Qu.: 81.75   3rd Qu.:  710.0   3rd Qu.:  6600   3rd Qu.: 11.00
 Max.   :222.00   Max.   :19085.0   Max.   :421239   Max.   :101.00
      maxd              nd               n                 c
 Min.   :  1.00   Min.   :  2.00   Min.   :     77   Min.   :      255
 1st Qu.: 11.00   1st Qu.:  4.00   1st Qu.:  10652   1st Qu.:   138114
 Median : 45.50   Median :  4.00   Median :  80162   Median :  1541718
 Mean   : 56.12   Mean   : 69.58   Mean   : 224072   Mean   :  6183005
 3rd Qu.: 81.75   3rd Qu.:  7.00   3rd Qu.: 241889   3rd Qu.:  5275142
 Max.   :222.00   Max.   :578.00   Max.   :5273364   Max.   :185447922
       l                  sat                t
 Min.   :      582   Min.   :0.00000   Min.   :   0.000
 1st Qu.:   319876   1st Qu.:0.00000   1st Qu.:   0.060
 Median :  3323704   Median :0.00000   Median :   0.970
 Mean   : 12789023   Mean   :0.01198   Mean   :  17.261
 3rd Qu.: 10709316   3rd Qu.:0.00000   3rd Qu.:   5.133
 Max.   :374806131   Max.   :1.00000   Max.   :1326.450
> nrow(E)
[1] 334

There are four instances with level-1-autarkies:

> E[E$sat==1,]
                                                                                name
64                              bloem_ex2.dqdimacs
187                             tentrup17_ltl2dba_theta_environment_1.dqdimacs
234                             bloem_eq1.dqdimacs
300                             bloem_ex1.dqdimacs
     np  cp  ni  ci nai nei   li mind maxd nd    n     c     l sat    t
64   60 139  60 139  10  50  323    4   10  4 2787 20920 46047   1 0.00
187 249 732 248 732   3 245 1910    1    3  4 6864 30619 72436   1 0.07
234   9  16   9  16   1   8   36    0    1  2   77   255   582   1 0.00
300  23  52  23  52   3  20  120    1    3  4  422  1912  4394   1 0.00

Autarkie-counts by sharpSAT:

bloem_ex2.dqdimacs:                                4
tentrup17_ltl2dba_theta_environment_1.dqdimacs:    256
bloem_eq1.dqdimacs:                                1     (also ctawSolver)
bloem_ex1.dqdimacs:                                2     (also ctawSolver)


The linear regression on ni versus nei says that we can well approximate ne
by just subtracting 57 from no:
> modne = lm(E$ne ~ E$no)
> summary(modne)
Residuals:
    Min      1Q  Median      3Q     Max
-165.12  -24.66   11.59   42.20   56.21
Coefficients:
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -57.215599   3.158960  -18.11   <2e-16 ***
E$ni          1.000168   0.001546  647.00   <2e-16 ***
Residual standard error: 52.9 on 332 degrees of freedom
Multiple R-squared:  0.9992,    Adjusted R-squared:  0.9992
F-statistic: 4.186e+05 on 1 and 332 DF,  p-value: < 2.2e-16
> plot(E$ni, E$nei)
> abline(modne)

For maxd we have two families, as the plot shows: one where maxd is just na,
this is the largest family, and a small family with a smaller coefficient, but
still perfect linear behaviour:
> modmaxd = lm(E$maxd ~ E$nai)
> summary(modmaxd)
Residuals:
     Min       1Q   Median       3Q      Max
-10.6196   0.3029   0.9621   1.4944   2.6085
Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept) -1.68459    0.22929  -7.347 1.59e-12 ***
E$nai        1.01268    0.00295 343.241  < 2e-16 ***
Residual standard error: 2.844 on 332 degrees of freedom
Multiple R-squared:  0.9972,    Adjusted R-squared:  0.9972
F-statistic: 1.178e+05 on 1 and 332 DF,  p-value: < 2.2e-16
> plot(E$nai, E$maxd)
> abline(modmaxd)

Concerning nd, the plot suggest that there are five families, the largest one
with constant (small) nd, the other four with linear behaviour:
> plot(E$nai, E$nd)


TODOS:

1. Clean-up handling of statistics

   And a more systematic treatment is needed, perhaps computing bundles
   of connected statistics all at once, and after the basic data (clauses,
   dependencies etc.) are established.

   For the handling of minima perhaps some helper-function is used, which
   writes "NA" under appropriate circumstances (or "NaN, which is currently
   used).

2. More statistics on dependencies:
    - average size
    - number of maximal and minimal elements
    - maximum length of a chain (height)
    - width (maximum length of an antichain); this decides also whether we
      have a PQCNF.

3. Implementing linear-size version of amo (using auxiliary variables)

    - A command-line variables specifies the number of variables from which
      on that version of amo is used.
    - See
      ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
    - Two versions: with and without unique-extension-property (the former
      when we want to count solutions); again a command-line parameter is
      needed -- or, since the special feature of this translation is the
      ability to count solutions, we actually implement only the
      counting-version.

    The new variables used here likely should come at the end.

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

  Statistics are needed to report on these reductions.

  Reductions (b), (c) need to be re-applied after autarky-reductions.

5. Determine the main parameters like number of pa-variables etc. from the
   parameters of the DQCNF.

   Use this for reserve-statements.
   And have a version which only computes the statistics (see Point 10 below),
   perhaps just based on output=nil (then actually, different from the current
   behaviour, nothing should happen).

   It seems that the class Encoding should deliver this service:
    - natural, since only here the minimal solutions per clause are
      determined;
    - might take a lot of effort, which is needed for the precise count, taking
      repetitions of partial assignments into account, but perhaps one might
      offer an upper bound, which just sums up all the possibilities per
      clause?
    - As stated at the beginning of this point, there is also the prospect of
      what can be done just from the parameters of the DQCNF. Perhaps there are
      stages of "approximation"? First from the encoding, then roughly taking
      the clauses into account, and finally the precise numbers from the
      encoding-dtails.

   First a member-function Encoding::ncl, which returns a structure with the
   (exact) numbers of clauses, as they appear in the translation.

6. Test and improve error-handling

   Give the original file-line-numbers and clause-indices with the errors:
    - original line-numbers for up to (including) the p-line and the
      dependencies;
    - so the final line-number is the last dependency (or p-line);
    - then the clause-indices take over;
    - all indices start with 1 (initialised to 0).

   So when reading the input clauses, their original line-number needs to be
   stored. Tautological clauses (which are deleted) should also be recorded;
   see Point 12:
    - The class DClause could contain the original clause-index >= 1, or
      0 if not defined. DONE
    - There needs then to be a vector with the indices of the deleted
      tautological clauses.
    - When showing the original clauses, then the original indices should
      be shown (in case of repeated clauses, this needs to be indicated). DONE
    - The map new-clause-index -> old-clause-index should be made available,
      so that a simple script with inputs (input dqcnf, satisfying assignment
      for translation, map) can perform the autarky-reduction.

   More information should be given in case of out-of-memory.
   And all such errors need to be caught (so that then all functions
   are noexcept).

7. Distributing comments on the two output-streams (solout and logout)

   Currently we have the log-level (0,1,2), where level-2 actually goes to
   solout (information on the translation-variables, which can be big), if
   solout is not nil (in this case it then goes to logout).

   While log-level-1 is the input (in the new order), going to logout.

   Perhaps we can have a more powerful syntax, which allows to select the
   various pieces, and where they go, in which order.

   Perhaps what is most needed is a way to extract in a simple way the
   information on the encoding-variables, so that from a solution of
   the translation, we can compute all necessary information:
    - The meaning of the variables in the translation is given at
      log-level 2 in log-output.
    - Perhaps it should be made more machine-readable.
    - This is achieved now by using specifications of the form
      "c  cs[i]" etc. DONE

   It should also be possible to output the cleaned-up original clause-set
   (as we output it for log-level 1, but here in DQCNF-format).
   We need to store whether the input-format is DQCNF or PQCNF (and output
   it accordingly).

8. Another variation: cut out pa-variables for partial assignments with only
   one variable.

   Together with cleaning-up-or-not this yields (at least) 4 variations.
   Should become another command-line parameter.

   Perhaps four levels of preprocessing:
    - 0 : none (the default)
    - 1 : only dependency-clean-up
    - 2 : only pa-variable-clean-up
    - 3 : both clean-ups.

9. Apply autarkies found to the original problem

   Perhaps just a bash-script? No, we can do a complete package.
   But a bash-script is also useful; see Point 6 above for the information
   which clauses to remove.

   We develop this program, so that all "basic autarkies"
   can be handled; see Point 11 for autarkies with one e-variable.
   The SAT-solver name is needed as a command-line parameter, and whether
   its input is from a file or from standard input.
   Using std::system and its return-value (10 for an autarky found) should
   do the job, plus storing the solver-output in a file.

   Perhaps we require a "standardised" wrapper for solvers, so that we can
   assume some standard input- and output-behaviour: input on standard
   input, output on standard output, one or two lines:
   s (UN)SATISFIABLE
   [v solution]
   Then we just use this fixed wrapper (no further command-line input).

   Some good command-line syntax is needed, to tell the program whether to
   perform the autarky-reduction or not (which in the positive case
   is always applied until completion), and then whether
    - only 1-var-bfs autarkies
    - only 1-var autarkies
    - both types of autarkies.

10. No storing of the clauses of the translation

    Currently there is absolutely no need.
    If Point 9 is applied, then likely still we can just re-output the clauses,
    using the simple information which clauses have been satisfied.
    However, if this program performs the autarky-reductions, then we should
    keep the clauses.

11. Autarkies using one e-variable

    They are easy to handle directly. Together with a full handling (Point 9
    above), this needs then a repetition-loop, until no further changes
    (we have confluence); this includes variable-clean-up (Point 4).

12. Output a transcript of changes to original DQCNF / PQCNF

    Using the line-numbers of the original clauses, optionally, in some
    simple syntax to be developed, the changes concerning deleted clauses,
    shortened clauses and shortened dependencies are output to a file (so
    that simple tools can independently apply the changes).

    We should detect whether the input is PQCNF (i.e., no d-lines), and
    output then accordingly. All shortened dependencies are likely optional,
    and this should be noted.

13. Report empty and unit-clauses in the translation

    Perhaps we also allow DIMACS-conformity level for the output?
    "General" allows a unit-clause, while with "normal" we output
      s UNSATISFIABLE

    Possibly unit-clauses are propagated?

14. Conformity-level vs (verystrict)

    Here there must be no
     - formal variables
     - repeated literals in clauses
     - tautological clauses
     - repeated clauses
     - the number of clauses in the header must match exactly.

15. Logarithmic encoding

    To start with, a command-line parameter for the encoding is needed.

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

const std::string version = "0.6.12";
const std::string date = "17.8.2018";

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
  empty_line=25,
  bad_comment=26,
  num_cls=27,
  illegal_comment=28,
  e_read_dline=29,
  e_rep_dline=30,
  a_read_dline=31,
  a_rep_dline=32,
  a_line_trail=33,
  e_line_trail=34,
  d_line_trail=35,
};
/* Extracting the underlying code of enum-classes (scoped enums) */
template <typename EC>
inline constexpr int code(const EC e) noexcept {return static_cast<int>(e);}


// Error output with ERROR-prefix, and each on a new line:
struct Outputerr {
  const std::string e = "ERROR[" + program + "]: ";
  Outputerr() noexcept { std::cerr.exceptions(std::ios::iostate(0)); }
  template <typename T>
  const Outputerr& operator <<(const T& x) const noexcept {
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
  ~Output() noexcept { if (del) delete p; }
  template <typename T>
  const Output& operator <<(const T& x) const noexcept {
    if (p) *p << x; return *this;
  }
  void endl() const noexcept { if (p) {*p << "\n"; p->flush();} }
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
  struct NoExceptions {
    NoExceptions() {}
    ~NoExceptions() noexcept {
      if (solout.p != nullptr)
        solout.p->exceptions(std::ios::iostate(0));
      if (logout.p != nullptr)
        logout.p->exceptions(std::ios::iostate(0));
    }
  } noexceptions;
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
  const std::string logname = (std::string(argv[3])=="=") ? outname : argv[3];
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
std::ostream& operator <<(std::ostream& out, const std::set<T>& S) noexcept {
  for (const T& x : S) out << " " << x;
  return out;
}
template <typename T>
std::ostream& operator <<(std::ostream& out, const std::vector<T>& v) noexcept {
  for (const T& x : v) out << " " << x;
  return out;
}
template <typename A, typename V>
std::ostream& operator <<(std::ostream& out, const std::map<A,V>& M) noexcept {
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

  constexpr bool operator ==(const Litc y) const noexcept {
    return x==y.x and t==y.t;
  }
  constexpr bool operator !=(const Litc y) const noexcept {
    return not (*this == y);
  }
  friend constexpr bool operator<(const Litc x, const Litc y) noexcept {
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
// Special output, comma-separated:
void output_clause(std::ostream& out, const Clause& C) noexcept {
  const auto begin = C.begin(), end = C.end();
  for (auto i = begin; i != end; ++i)
    if (i != begin) out << "," << *i; else out << *i;
}

typedef std::pair<AClause,EClause> PairClause; // all-exists
struct DClause {
  PairClause P; // A-E
  const Count_t index = 0;
  void clear() noexcept {P.first.clear(); P.second.clear();}
  bool pseudoempty() const noexcept {return P.second.empty();}
  bool empty() const noexcept {return P.first.empty() and P.second.empty();}
  bool operator ==(const DClause C) const noexcept {return P == C.P;}
  bool operator !=(const DClause C) const noexcept {return P != C.P;}
  friend bool operator <(const DClause& C, const DClause& D) noexcept {
    return C.P < D.P;
  }
  friend std::ostream& operator <<(std::ostream& out, const DClause& C) noexcept {
    out << "E={";
    output_clause(out, C.P.second);
    out << "} A={";
    output_clause(out, C.P.first);
    return out <<"} " << C.index;
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
  Count_t c=0; // number of clauses (without tautologies or repetitions)
  Count_t la=0, le=0, l=0, lrep=0; // number of a/e/both/repeated literal occurrences
  Count_t t=0, empty=0, pempty=0, repeated=0; // number of tautological/empty/pseudoempty/repeated clauses

  friend std::ostream& operator <<(std::ostream& out, const DClauseSet& F) noexcept {
    out << "c  List of variables:\nc   ";
    for (Var v = 1; v < F.vt.size(); ++v) out << " " << v << ":" << F.vt[v];
    out << "\nc  p cnf " << F.max_index << " " << F.c << "\n";
    for (Var v = 1; v < F.vt.size(); ++v)
      if (F.vt[v] == VT::e) out << "c  d " << v << *F.D[v] << " 0\n";
    Count_t i = 0;
    for (const auto& C : F.F) out << "c  C[" << ++i << "]: " << C << "\n";
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
ConformityLevel s2conlev(const std::string& s) noexcept {
  if (s == "g") return ConformityLevel::general;
  else if (s == "s") return ConformityLevel::strict;
  else if (s == "vs") return ConformityLevel::verystrict;
  else return ConformityLevel::normal;
}

class ReadDimacs {

std::istream& in;
DClauseSet F;
const ConformityLevel conlev;
Count_t current_line_number = 0; // starting with 1; final value the last dependency (or p-line, if no dependencies)
Count_t current_clause_index = 0; // starting with 1

// Reads header-data into F; aborts via std::exit in case of input-errors:
void read_header() noexcept {
  assert(in.exceptions() == 0);
  assert(in.good());
  std::string line;
  while (true) {
    std::getline(in, line); ++current_line_number;
    const char c = line[0];
    if (c == '\0') {
      errout << "Line" << current_line_number << "Empty line (no p-line found).";
      std::exit(code(Error::empty_line));
    }
    assert(not line.empty());
    if (c == 'p') break;
    if (c != 'c') {
      errout << "Line" << current_line_number << "Comment lines must start with \"c\".";
      std::exit(code(Error::bad_comment));
    }
  }
  assert(line[0] == 'p');
  std::stringstream s(line);
  {std::string inp; s >> inp;
   if (inp != "p") {
     errout << "Line" << current_line_number << "Syntax error in parameter line (\"p\" not followed by space).";
     std::exit(code(Error::file_pline));
   }
   assert(s);
   if (s.eof()) {
     errout << "Line" << current_line_number << "Syntax error in parameter line (p-line ends after \"p\").";
     std::exit(code(Error::file_pline));
   }
   s >> inp;
   if (not s or inp != "cnf") {
     errout << "Line" << current_line_number << "Syntax error in parameter line (no \"cnf\").";
     std::exit(code(Error::file_pline));
   }
  }
  s >> F.n_pl;
  if (not s) {
    errout << "Line" << current_line_number << "Reading error with parameter maximal-variable-index "
      "(too big or not-a-number).";
    std::exit(code(Error::file_pline));
  }
  if (not valid(F.n_pl)) {
    errout << "Line" << current_line_number << "Parameter maximal-variable-index n=" << F.n_pl <<
      " is too big for numeric_limits<Lit_int>::max=" << max_lit;
    std::exit(code(Error::num_vars));
  }
  s >> F.c_pl;
  if (not s) {
    errout << "Line" << current_line_number << "Reading error with parameter number-of-clauses "
      "(too big or not-a-number).";
    std::exit(code(Error::num_cls));
  }
  if (not s.eof()) {
    errout << "Line" << current_line_number << "Syntax error in parameter line (something after c-parameter).";
    std::exit(code(Error::file_pline));
  }
}

// Read dependency-data into F:
void read_dependencies() noexcept {
  assert(in.exceptions() == 0);
  assert(in.good());
  try { F.vt.resize(F.n_pl+1); F.D.resize(F.n_pl+1); }
  catch (const std::bad_alloc&) {
    errout << "Allocation error for vt- + dependency-vector of size " << F.n_pl;
    std::exit(code(Error::allocation));
  }
  struct Finish { // marking unset variables as fe with empty domain
    DClauseSet& F0;
    Finish(DClauseSet& F) noexcept : F0(F) {}
    ~Finish() noexcept {
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
  Dependency dep;
  try { dep = F.dep_sets.insert(A).first; }
  catch (const std::bad_alloc&) {
    errout << "Allocation error for first (empty) dependency-set.";
    std::exit(code(Error::allocation));
  }
  std::string line;
  enum class lt { begin, e, a, d }; // line type
  lt last_line = lt::begin;
  if (in.eof()) return;
  while (true) {
    const int peek = in.peek();
    if (peek == std::char_traits<char>::eof()) return;
    ++current_line_number;
    if (peek == 'c')
      if (conlev == ConformityLevel::general) {
        std::getline(in, line);
        continue;
      }
      else {
        errout << "Line" << current_line_number << "Comment after p-line.";
        std::exit(code(Error::illegal_comment));
      }
    if (peek != 'a' and peek != 'e' and peek != 'd') return;
    std::getline(in, line);
    std::stringstream s(line);
    {std::string skip; s >> skip;}
    if (peek == 'a') {
      if (conlev != ConformityLevel::general and last_line == lt::a) {
        errout << "Line" << current_line_number << "Repeated a-line.";
        std::exit(code(Error::a_rep_line));
      }
      Count_t num_a = 0;
      do {
        Var v;
        if (not (s >> v)) {
          errout << "Line" << current_line_number << "Bad a-read.";
          std::exit(code(Error::a_read));
        }
        if (v > F.n_pl) {
          errout << "Line" << current_line_number << "a-variable" << v << "contradicts n=" << F.n_pl;
          std::exit(code(Error::variable_value));
        }
        if (v == 0) break;
        if (F.vt[v] != VT::und) {
          errout << "Line" << current_line_number << "Repeated a-variable-read" << v;
          std::exit(code(Error::a_rep));
        }
        F.vt[v] = VT::fa;
        A.insert(v); ++num_a; ++F.na_d;
      } while (true);
      if (not s) {
        errout << "Line" << current_line_number << "Bad a-line read (file corrupted?).";
        std::exit(code(Error::a_line_read));
      }
      if (not s.eof()) {
        errout << "Line" << current_line_number << "Syntax error in a-line (trailing characters).";
        std::exit(code(Error::a_line_trail));
      }
      if (num_a != 0) {
        last_line = lt::a;
        try {
          const auto insert = F.dep_sets.insert(A);
          assert(insert.second);
          dep = insert.first;
        }
        catch (const std::bad_alloc&) {
          errout << "Line" << current_line_number << "Allocation error for dependency-set.";
          std::exit(code(Error::allocation));
        }
      } else {
        if (conlev != ConformityLevel::general) {
          errout << "Line" << current_line_number << "Empty a-line.";
          std::exit(code(Error::a_empty));
        }
      }
    } else if (peek == 'e') {
      if (conlev != ConformityLevel::general and last_line == lt::e) {
        errout << "Line" << current_line_number << "Repeated e-line.";
        std::exit(code(Error::e_rep_line));
      }
      Count_t num_e = 0;
      do {
        Var v;
        if (not (s >> v)) {
          errout << "Line" << current_line_number << "Bad e-read.";
          std::exit(code(Error::e_read));
        }
        if (v > F.n_pl) {
          errout << "Line" << current_line_number << "e-variable" << v << "contradicts n=" << F.n_pl;
          std::exit(code(Error::variable_value));
        }
        if (v == 0) break;
        if (F.vt[v] != VT::und) {
          errout << "Line" << current_line_number << "Repeated e-variable-read.";
          std::exit(code(Error::e_rep));
        }
        F.vt[v] = VT::fe;
        F.D[v] = dep; ++num_e; ++F.ne_d;
      } while (true);
      if (not s) {
        errout << "Line" << current_line_number << "Bad e-line read (file corrupted?).";
        std::exit(code(Error::e_line_read));
      }
      if (not s.eof()) {
        errout << "Line" << current_line_number << "Syntax error in e-line (trailing characters).";
        std::exit(code(Error::e_line_trail));
      }
      if (conlev != ConformityLevel::general and num_e == 0) {
        errout << "Line" << current_line_number << "Empty e-line.";
        std::exit(code(Error::e_empty));
      }
      last_line = lt::e;
    } else {
      assert(peek == 'd');
      Var v;
      if (not (s >> v)) {
        errout << "Line" << current_line_number << "Bad e-variable read in d-line.";
        std::exit(code(Error::e_read_dline));
      }
      if (v > F.n_pl) {
        errout << "Line" << current_line_number << "e-variable" << v << "contradicts n=" << F.n_pl;
        std::exit(code(Error::variable_value));
      }
      if (v == 0) {
        errout << "Line" << current_line_number << "Empty d-line.";
        std::exit(code(Error::d_empty));
      }
      if (F.vt[v] != VT::und) {
        errout << "Line" << current_line_number << "Repeated e-read" << v << "in d-line.";
        std::exit(code(Error::e_rep_dline));
      }
      F.vt[v] = VT::fe;
      ++F.ne_d;
      Varset A;
      do {
        Var w;
        if (not (s >> w)) {
          errout << "Line" << current_line_number << "Bad a-read in d-line.";
          std::exit(code(Error::a_read_dline));
        }
        if (w > F.n_pl) {
          errout << "Line" << current_line_number << "a-variable" << w << " in d-line contradicts n=" << F.n_pl;
          std::exit(code(Error::variable_value));
        }
        if (w == 0) break;
        if (F.vt[w] != VT::fa) {
          errout << "Line" << current_line_number << "Not a-variable" << w << "in d-line.";
          std::exit(code(Error::d_bada));
        }
        try {
          const auto insert = A.insert(w);
          if (not insert.second) {
            errout << "Line" << current_line_number << "Repeated a-variable-read in d-line.";
            std::exit(code(Error::a_rep_dline));
          }
        }
        catch (const std::bad_alloc&) {
          errout << "Line" << current_line_number << "Allocation error for insertion into dependency-set.";
          std::exit(code(Error::allocation));
        }
      } while (true);
      if (not s.eof()) {
        errout << "Line" << current_line_number << "Syntax error in d-line (trailing characters).";
        std::exit(code(Error::d_line_trail));
      }
      try { F.D[v] = F.dep_sets.insert(A).first; }
      catch (const std::bad_alloc&) {
        errout << "Line" << current_line_number << "Allocation error for insertion of dependency-set.";
        std::exit(code(Error::allocation));
      }
      last_line = lt::d;
    } // end of main if-then-else
  } // main loop
}

enum class RS { clause, none, empty, tautology, pseudoempty }; // read-status

/* Attempting to read a single clause; reference-parameter C is empty iff none
   or a tautological clause or an empty clause was found:
*/
RS read_clause(DClause& C) noexcept {
  assert(in.exceptions() == 0);
  assert(in.good());
  C.clear();
  Lit x;
  in >> x;
  if (in.eof()) return RS::none;
  ++current_clause_index;
  AClause CA; EClause CE; // complemented clauses
  Count_t lrep = 0;
  while (true) { // reading literals into C
    if (not in) {
      errout << "Clause" << current_clause_index << "Invalid literal-read at beginning of clause.";
      std::exit(code(Error::literal_read));
    }
    assert(in.good());
    if (not x) break; // end of clause
    const Var v = var(x);
    if (v > F.n_pl) {
      errout << "Clause" << current_clause_index << "Literal" << x << "contradicts n=" << F.n_pl;
      std::exit(code(Error::variable_value));
    }
    if (at(F.vt[v]))
      if (CA.find(x) != CA.end()) { // tautology via universal literals
        C.clear();
        do
          if (not (in >> x)) {
            errout << "Clause" << current_clause_index << "Invalid literal-read in a-tautological clause.";
            std::exit(code(Error::literal_read));
          }
        while (x);
        return RS::tautology;
      }
      else {
        lrep += not C.P.first.insert(x).second;
        CA.insert(-x);
      }
    else {
      assert(et(F.vt[v]));
      if (CE.find(x) != CE.end()) { // tautology via existential literals
        C.clear();
        do
          if (not (in >> x)) {
            errout << "Clause" << current_clause_index << "Invalid literal-read in e-tautological clause.";
            std::exit(code(Error::literal_read));
          }
        while (x);
        return RS::tautology;
      }
      else {
        lrep += not C.P.second.insert(x).second;
        CE.insert(-x);
      }
    }
    in >> x;
  }
  F.lrep += lrep;
  switch (conlev) {
  case ConformityLevel::general :
    if (C.empty()) return RS::empty;
    if (C.pseudoempty()) return RS::pseudoempty;
    break;
  case ConformityLevel::normal :
    if (C.empty()) {
      errout << "Clause" << current_clause_index << "Empty clause.";
      std::exit(code(Error::empty_clause));
    }
    if (C.pseudoempty()) return RS::pseudoempty;
    break;
  default :
    if (C.empty()) {
      errout << "Clause" << current_clause_index << "Empty clause.";
      std::exit(code(Error::empty_clause));
    }
    if (C.pseudoempty()) {
      errout << "Clause" << current_clause_index << "Clause without existential variables.";
      std::exit(code(Error::pseudoempty_clause));
    }
  }
  return RS::clause;
}

// Add non-tautological C to F (if not already existent):
void add_clause(const DClause& C) noexcept {
  try {
    if (F.F.insert({C.P, current_clause_index}).second) {
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
    else ++F.repeated;
  }
  catch (const std::bad_alloc&) {
    errout << "Clause" << current_clause_index << "Allocation error for insertion of clause.";
    std::exit(code(Error::allocation));
  }
}

// Counting dependency-sets, and removing unused ones:
void count_dependencies() noexcept {
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

DClauseSet operator()() noexcept {
  read_header();
  if (in.eof()) return F;
  read_dependencies();
  if (in.eof()) return F;
  try { F.vardeg.resize(F.n_pl+1); }
  catch (const std::bad_alloc&) {
    errout << "Allocation error for degree-vector of size "<< F.n_pl << ".";
    std::exit(code(Error::allocation));
  }
  {DClause C; RS status;
   while ((status = read_clause(C)) != RS::none) {
     bool add = true;
     switch (status) {
     case RS::tautology   : ++F.t; add = false; break;
     case RS::empty       : ++F.empty; break;
     case RS::pseudoempty : ++F.pempty; break;
     default : ;
     }
     if (F.c + F.repeated + F.t + ((add)?1:0) > F.c_pl) {
       errout << "Clause" << current_clause_index << "More than c=" << F.c_pl << "clauses, contradicting cnf-header.";
       std::exit(code(Error::number_clauses));
     }
     if (add) add_clause(C);
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

  Var pavar(const Pass_p& phi) const noexcept {
    const auto find = enc_pass.find(phi);
    assert(find != enc_pass.end());
    return find->second;
  }

  friend std::ostream& operator <<(std::ostream& out, const Encoding& enc) noexcept {
    out << "c cs-variables and the associated clauses:\n"
           "c ncs = " << enc.ncs << "\n";
    for (clause_index_t i = 0; i < enc.ncs; ++i)
      out << "c  cs[" << enc.csvar(i) << "]: " << *enc.dclauses[i] << "\n";
    out << "c bf-variables and the associated 1-variable partial assignments:\n"
           "c nbf = " << enc.nbf << "\n";
    for (Var vi = 0; vi < enc.F.ne; ++vi) {
      const EVar v = enc.E[vi];
      out << "c  evar[" << v << "]: ";
      const Var begin = enc.bfvar_indices[vi], end = enc.bfvar_indices[vi+1];
      out << (end-begin) << " bfs from " << begin << " to " << (end-1) << "\nc   ";
      Var j = begin;
      out << "bf[" << j++ << "]:" << Pass{{v,bf(false)}};
      out << ", bf[" << j++ << "]:" << Pass{{v,bf(true)}};
      for (const AVar w : enc.dep[vi]) {
        out << ", bf[" << j++ << "]:" << Pass{{v,Litc(Lit(w,Pol::n))}};
        out << ", bf[" << j++ << "]:" << Pass{{v,Litc(Lit(w,Pol::p))}};
      }
      out << "\n";
    }
    out << "c pa-variables and their partial assignments:\n"
           "c npa = " << enc.npa << "\n";
    {Var v = enc.ncs+enc.nbf;
     for (const Pass& phi : enc.all_solutions.first)
       out << "c  pa[" << ++v << "]:" << phi << "\n";
    }
    out << "c The clauses (see the cs-variables) and their solution-sets (as pa-variables):\n";
    for (clause_index_t ci = 0; ci < enc.ncs; ++ci) {
      out << "c  sol[" << enc.csvar(ci) << "]:";
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
LogLevel s2loglev(const std::string& s) noexcept {
  if (s == "1") return LogLevel::withinput;
  else if (s == "2") return LogLevel::withmeaning;
  else return LogLevel::normal;
}


void show_usage() noexcept {
  std::cout << "USAGE:\n"
    "> " << program << " [-v | --version]\n"
    " shows version informations and exits.\n"
    "> " << program << " [-cin | filename]\n"
    " runs the translator with input from standard input or filename.\n"
    "> " << program << " [-cin | filename] [-cout | -cerr | filename2 | -nil]\n"
      " furthermore appends the DIMACS-output to standard output or standard error or filename2, or ignores it\n "
      "(default is -cout).\n"
    "The same redirections can be done with the log-output (which might be \"=\"),\n"
    " as a third command-argument; default is -cout.\n"
    "If DIMACS- and log-output are equal (explicitly or impliclity),\n"
    " then first comes the log-output (as DIMACS-comment).\n"
    "A fourth optional argument is the conformity-level:\n"
    " g (general), n (normal), s (strict), vs (very strict).\n"
    "A fifth optional argument is the log-level (default \"0\"):\n"
    " \"1\" means with input, \"2\" has additionally explanations on translation-variables.\n"
    "\nFor example, with\n"
    "> " << program << " -cin Out -nil\n"
    "input comes from standard input, the translation is put to file Out, and the log is discarded.\n"
    "While with\n"
    "> " << program << " In Out Out\n"
    "the input comes from file In, and both translation and log are appended to Out.\n"
    "The same can be achieved with\n"
    "> " << program << " In Out =\n";
  std::exit(0);
}

#define S(x) #x
#define STR(x) S(x)

void version_information() noexcept {
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

void output(const std::string filename, const ConformityLevel cl, const DClauseSet& F, const Encoding& enc, const Translation& trans, const CLS& G, const LogLevel ll) noexcept {
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
         "c number_tautological_clauses           " << F.t << "\n"
         "c number_repeated_clauses               " << F.repeated << "\n"
         "c number_empty_clauses                  " << F.empty << "\n"
         "c number_pseudo_empty_clauses           " << F.pempty << "\n"
         "c number_repeated_literals              " << F.lrep << "\n"
         "c Actually occurring:\n"
         "c max_index_variable                    " << F.max_index << "\n"
         "c num_variables                         " << F.n << "\n"
         "c num_clauses                           " << F.c << "\n"
         "c num_a_variables                       " << F.na << "\n"
         "c num_e_variables                       " << F.ne << "\n"
         "c num_a_literal_occurrences             " << F.la << "\n"
         "c num_e_literal_occurrences             " << F.le << "\n"
         "c num_ae_literal_occurrences            " << F.l << "\n"
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

  if (code(ll) >= 1) logout <<
    "c INPUT DQCNF (without repetitions or tautolocical clauses)\n"
    "c   as list of variables, followed by p- and d-specifications,\n"
    "c   and concluded by the list of clauses, in the format\n"
    "c     C[i]: E={...} A={...} j\n"
    "c   where i the new and j is the old index (in the input):\n"
    << F;

  if (solout != logout) {
    solout << "c Program " << program << ": version " << version << ", " << date << ".\n";
    solout << "c Input: " << filename << "\n";
  }

  if (code(ll) >= 2) {
    if (not solout.nil()) {
      solout << "c MEANING of translation-variables: cs[i], bf[i], pa[i]\n";
      solout << enc;
    } else {
      logout << "c MEANING of translation-variables: cs[i], bf[i], pa[i]\n";
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
