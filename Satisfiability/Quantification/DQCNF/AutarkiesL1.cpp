// Oliver Kullmann, 28.6.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
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

1. app_tests/A1E2C1.dqdimacs with version _debug yields
autL1_debug: Encodings.hpp:106: Encodings::Encoding::Var Encodings::Encoding::bfvar(Encodings::Encoding::EVar, Encodings::Encoding::Litc) const: Assertion `w <= F.max_a_index' failed.

We have F.max_a_index = 0, since no universal variable occurs in the clauses.
The problem is that the two existential variables in a clause share
a universal variable, and thus a partial assignment satisfying the clause
can use that variable:
Translations.hpp: L56:
           C.insert(Lit(enc.bfvar(pair.first, pair.second), Pol::p));
This comes from Encoding.hpp, function
  All_solutions set_all_solutions() const {
There, in line 249 we have
              if (V.find(u) != V.end()) continue;
That means to ignore u from the intersection I of the var-domains of the
two variables v, w if it is in V, the variables of the universal literals
of the current clause, since we have already then a strictly smaller
partial assignment satisfying the clause (just assigning one of v, w suffices).
From the clause at hand we can not conclude that variable u is not to be
used, since that might come from other clauses.
The assert only becomes really valid after clean-up of universal variables
not occurring at all:
So ReadDimacs::cleanup_dependencies() needs to be performed.


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

Proposed order:

-1  update
0   tests
4   cleanup
6   error-handling
8   further cleanup
14  conformity-level
16  command-line wrapper
17  semantic basis

1   statistics
2   more statistics
7   comments
10  no clause-storage
12  transcript of changes
13  standard of output

9   autarky application

3   linear amo

5   parameter computation

11  one e-variable

15  logarithmic encoding


-1. Update to new standard

0. Extend tests

    - All examples from the report should be handled.
    - To start with, Example \label{exp:dbfq1trans} appears not to be
      treated.
    - There should be references back and forth between the Maxima-versions,
      the report, and this code.

    - Also unit-tests are needed (in program Test.cpp, using asserts).
      For this the documentation and specification of components needs to be
      improved.

1. Clean-up handling of statistics

   And a more systematic treatment is needed, perhaps computing bundles
   of connected statistics all at once, and after the basic data (clauses,
   dependencies etc.) are established.
    - For the handling of minima perhaps some helper-function is used, which
      writes "NA" under appropriate circumstances (or "NaN", which is currently
      used).
    - DClauseSet-statistics: set in the following members of Input::ReadDimacs
     - add_clause
     - count_dependencies
     - operator ()
    - The statistics-data of DClauseSet are all scalar except of the vector
      vardeg: does this make a difference?
    - As a first step, the statistics-data of DClauseSet should be concentrated
      in one member, perhaps F.s (to make access not much more painful).
    - Determining the statistics right at reading-point might be slightly
      more efficient, but that doesn't seem to matter, and a general
      function for determing all these statistics at once seems best.
    - Perhaps at construction-time of DClauseSet from DCLS F ?
      But reading needs certain aspects of the statistics, that's the
      problem!
    - So perhaps there a different "levels" of statistics:
     - F.n_pl, F.c_pl are first read, and used throughout.
     - Then, for the dependencies, F.vt, F.D are set up.
     - And F.dep_sets are set, using and updating F.vt, F.D.
     - At this time F.na_d, F.ne_d are updated (but not used).
     - Now reading a clause: F.vt is used, F.F set.
     - F.lrep, F.repeated, F.c, F.la, F.le, F.max_*, F.vardeg are updated, as
       well as F.vt.
     - The whole reading in operator() sets F.t, F.empty, F.pempty, sowie
       F.max_index, F.n, F.l, F.count_dep, F.max/min_s_dep.
     - The construction of the whole F happens in stages, using and
       constructing partial information. One needed to define these stages
       precisely.

2. More statistics on dependencies:
    - average size
    - number of maximal and minimal elements
    - maximum length of a chain (height)
    - width (maximum length of an antichain); this decides also whether we
      have a PQCNF.

3. Implementing linear-size version of amo (using auxiliary variables)

    - We need command-line arguments for the form of amo and possibly also
      where to apply it.
    - See
      ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
      for *the* basic translation of linear-size:

      It uses 3n-6 binary clauses (without uep) and < n/2 variables (also
      with uep -- doesn't use additional variables). Called
        seco(x1, ..., xn)
      there ("sequential commander"), for literals xi, it is easiest
      specified recursively:

      Base case n <= 4:
        seco(x1,x2,x3,x4) = the binomial(4,2)=6 prime-clauses for
                            amo(x1,x2,x3,x4)
      Recursion for n >= 5:
        seco(x1,...,xn) = Conjunction of
                        binomial(3,2)=3 prime-clauses for amo(x1,x2,x3)
                        and x1->w, x2->w, x3->w for the commander-variable w
                        and seco(w,x3,...,xn).
      For uep the only problem is that x1=...=xn=0 admits many solutions.
      If this needs to be excluded, then for each commander-variable w(x,y,z)
      one adds
        w -> x v y v z (i.e., the 4-clause {-w,x,y,z}).
      For ALO one takes the disjunction of the final w with the initial 3
      resp. 4 "uncommanded" x1,x2,x3,x4.

    - Two versions: with and without unique-extension-property (the former
      when we want to count solutions); a command-line parameter is
      needed -- or, since the special feature of this translation is the
      ability to count solutions, we might actually implement only the
      counting-version?
    - The new variables used here likely should come at the end.
    - Open a new file, perhaps "Translations.hpp".
    - Perhaps providing generic functions amo, alo, eo.
    - One input is, in the classical STL-style, a pair of iterators specifying
      the sequence of literals (by begind and end).
    - One might also provide an overload with a range of variables, to
      accommodate the current range
        from enc.bfvar_indices[i] to enc.bfvar_indices[i+1].
       - The problem is, how this can be distinguished from the range of
         literals, by iterators?
       - The latter would use a template parameter InputIterator.
       - While the form with an interval of variables does not use templates
         for this argument, which should make it a better fit (even together
         with the other template parameter OutputCLS.
       - This overload just calls the generic version, with an appropriate
         iterator-range; perhaps we create two helper functions, one takes
         a variable v and provides an random-access iterator lit_it(v),
         for the literals, and the other end_it(v) produces the end-iterator.
    - One parameter specifies the type of translation; an enumerated value
      seems okay.
    - For the auxiliary variables, perhaps a function object aux() (called
      without arguments, yielding a variable) is passed. Without new variables
      is realised by passing the constant zero-function.
    - A bit unfortunate that there is a coupling for the basic translation (all
      prime implicates): that is specified by the enumeration-parameter, and
      has then the special aux-object.
       - But it seems inappropriate to supply a default-value for aux (the
         zero-object), since this is only used in one case.
       - But perhaps we provide the overload amo(begin, end, F), which is just
         that basic translation.
    - How to provide the output? For our only current use, "Amo-clauses for
      bf-variables", it would be a reference to clause-set G. That should be
      also fine for the general case: a reference-parameter, where by
      push_back the clause is transferred.
    - Then there are the statistics. Perhaps a basic statistics-object is the
      output, giving
       - number of clauses
       - number of literal occurrences
       - number of new variables.
    - First we implement the framework just enough for the current case; then
      we extend the test-cases; for poor-mans unit-testing we can just
      create some Test.cpp, which has asserts in it.

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

  DONE (now Dependency is the pointer):
  It seems the distinction between Dependency and Dependency_p should
  be abandoned, and only the latter used (we don't use the iterator-property,
  or?).

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

   - Perhaps just a bash-script?
   - No, we can do a complete package.
   - But a bash-script is also useful; see Point 6 above for the information
     which clauses to remove.
   - We develop this program, so that all "basic autarkies"
     can be handled; see Point 11 for autarkies with one e-variable.
   - The SAT-solver name is needed as a command-line parameter, and whether
     its input is from a file or from standard input.
   - Using std::system and its return-value (10 for an autarky found) should
     do the job, plus storing the solver-output in a file.
   - Perhaps we require a "standardised" wrapper for solvers, so that we can
     assume some standard input- and output-behaviour: input on standard
     input, output on standard output, one or two lines:
     s (UN)SATISFIABLE
     [v solution]
     Then we just use this fixed wrapper (no further command-line input);
     let's call it "SATsolver_for_autarkies.bash".
   - Some good command-line syntax is needed, to tell the program whether to
     perform the autarky-reduction or not (which in the positive case
     is always applied until completion), and then whether
    - only 1-var-bfs autarkies
    - only 1-var autarkies
    - both types of autarkies.

   - The simplest way to handle autarky-reduction is to use that the only part
     of the translation which becomes invalid after an autarky-reduction is
     the non-triviality-clause:
    - Write the C++-program "Reduce.cpp", which calls the program for the
      initial translation F, reads off from the first clause N the
      touched-clauses-variables, runs a SAT-solver on the translation, and in
      case of an autarky found, extracts the touched (satisfied) clauses from
      the satisfying assignment, replaces the corresponding literals in N
      with spaces (in-place), and repeats the process, until F becomes
      unsatisfiable.
    - The lean kernel is then given by the touched clauses.
    - For changing F in-place, see Interfaces/InputOutput/ManipParam.cpp .

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

16. It seems time now to remove the optionality of arguments for the
    command-line, and write a bash-script for calling the program
    in a more convenient way (using then named arguments, not
    positional arguments).

17. To establish the semantic basis, backtracking algorithms for the basic
    algorithms are needed:
     - Finding just one (non-trivial) solution.
     - Or all solutions.
     - This for all basic forms of autarkies.
     - This yields then the basis for extensive testing.
     - First one needs to evaluate partial assignments on D-clause-sets:
      - ClauseSets::check(phi,F) checks the variables;
        implementation: DONE
        tests for this function are needed.
      - ClauseSets::evaluate(phi,F) assumes a fitting F, and determines the
        number of touched clauses, and amongst them the number of satisfied
        ones.
        Implementation: DONE
        Tests are needed (also for helper function eval(phi,x)).
*/

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>

#include <cstdlib>

#include "Generics.hpp"
#include "InOut.hpp"
#include "VarLit.hpp"
#include "ClauseSets.hpp"
#include "Input.hpp"
#include "Encodings.hpp"
#include "Translations.hpp"

namespace {

// --- General input and output ---

const std::string version = "0.6.26";
const std::string date = "24.10.2019";

const std::string program = "autL1"
#ifndef NDEBUG
  "_debug"
#endif
;
const std::string author = "\"Oliver Kullmann\"";
const std::string url = "\"https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Quantification/DQCNF/AutarkiesL1.cpp\"";

using Generics::code;

using InOut::Error;

InOut::Outputerr errout(program);

InOut::Output solout;
InOut::Output logout;


// --- Output ---

enum class LogLevel {normal=0, withinput=1, withmeaning=2};
std::ostream& operator <<(std::ostream& out, const LogLevel l) noexcept {
  switch (l) {
  case LogLevel::normal : return out << "normal";
  case LogLevel::withinput : return out << "with_input";
  case LogLevel::withmeaning : return out << "with_var_interpretations";
  default : return out << "LogLevel::unnamed-value=" << int(l);
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
    "> " << program << " In Out =\n"
    "To see only the log-output, use\n"
    "> " << program << " In -nil [logout=-cout]\n"
;
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
   "  LIT_TYPE = " STR(LIT_TYPE) " (with " << VarLit::digits_lit << " binary digits)\n"
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

void output(const std::string filename, const InOut::ConformityLevel cl, const ClauseSets::DClauseSet& F, const Encodings::Encoding& enc, const Translations::Translation& trans, const ClauseSets::CLS& G, const LogLevel ll) noexcept {
  logout <<
         "c Program information:\n"
         "c created_by                            \"" << program << "\"\n"
         "c version                               \"" << version << "\"\n"
         "c date                                  \"" << date << "\"\n"
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
  for (const ClauseSets::Clause& C : G) {
    for (const VarLit::Lit x : C) solout << x << " ";
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
  const InOut::ConformityLevel conlev = (argc >= 5) ? InOut::s2conlev(argv[4]) : InOut::ConformityLevel::normal;
  const LogLevel loglev = (argc >= 6) ? s2loglev(argv[5]) : LogLevel::normal;

  set_output(argc, argv, solout, logout, errout);
  const InOut::InputStream in(filename, errout);
  Input::ReadDimacs rd(*in, errout, conlev);
  const ClauseSets::DClauseSet& F = rd();

  const Encodings::Encoding enc(F);

  const Translations::Translation trans(F,enc);
  const ClauseSets::CLS G = trans();

  output(filename, conlev, F, enc, trans, G, loglev);
}
