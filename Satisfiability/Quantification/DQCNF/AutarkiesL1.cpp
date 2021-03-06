// Oliver Kullmann, 28.6.2018 (Swansea)
/* Copyright 2018, 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Quantification/DQCNF/AutarkiesL1.cpp
  \brief A simple translation of the level-1 autarky problem for DQCNFs
  to SAT, using the direct encoding. The special feature of this translation
  is that it allows counting the number of level-1-autarkies.

  USAGE:

> ./AutarkiesL1 -v | --version

for information on the program, the version, and the environment.

> ./AutarkiesL1 -h | --help

for basic help-information.

> AutarkiesL1 input [file-output] [log] [conformity-level=n] [log-level=0]

For the complete documentation, see
  docus/AutarkiesL1.txt



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


-2. Problem with the order of input-clauses

    - DONE (now using colexicographical order)
      The order used here is lexicographical on the pairs A, E, which
      each component again using lexicographical order.
    - DONE On the components it should use colexicographical order.
    - DONE And also the pairs should be sorted colexicographically.
    - DONE Then for inputs produced by DQBRG the orders should be the same
      (so that no change takes place; note that DQBRG has first all
      a-variables).
    - For inputs from QBRG a change of order will take place.
      Perhaps the linear order of QCNF should also handle a- and e-variables
      separately?
    - See Normalform.cpp: the order there, here, and those produced by
      [D][Q]BRG, should all coincide.

-1. Update to new standard
    - Makefile : DONE
    - Write docus
    - Use Dimacs-output-facilities
    - Use index-facility from Environment
    - Use the new facilities for options
    - Update input/output parameters: defaults can be activated by "", except
      for output, where it means the default-output-filename.

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

     Nearly all statistics should go to Normalform.cpp now.
     AutarkiesL1 can also read the provided statistics.

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

     Now handled in Normalform.cpp.

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

     Now handled in Normalform.cpp.

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

     Now handled in Normalform.cpp.

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

     Now handled in Normalform.cpp.

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

      Now handled in Normalform.cpp.

    Using the line-numbers of the original clauses, optionally, in some
    simple syntax to be developed, the changes concerning deleted clauses,
    shortened clauses and shortened dependencies are output to a file (so
    that simple tools can independently apply the changes).

    We should detect whether the input is PQCNF (i.e., no d-lines), and
    output then accordingly. All shortened dependencies are likely optional,
    and this should be noted.

13. Report empty and unit-clauses in the translation

      Now handled in Normalform.cpp.

    Perhaps we also allow DIMACS-conformity level for the output?
    "General" allows a unit-clause, while with "normal" we output
      s UNSATISFIABLE

    Possibly unit-clauses are propagated?

14. Conformity-level vs (verystrict)

      Now handled in Normalform.cpp.

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

#include <ProgramOptions/Environment.hpp>

#include "Generics.hpp"
#include "InOut.hpp"
#include "VarLit.hpp"
#include "ClauseSets.hpp"
#include "Input.hpp"
#include "Encodings.hpp"
#include "Translations.hpp"

namespace {

// --- General input and output ---

  const Environment::ProgramInfo proginfo{
        "0.6.32",
        "2.4.2020",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Quantification/DQCNF/AutarkiesL1.cpp",
        "GPL v3"};

using Generics::code;

using InOut::Error;

InOut::Outputerr errout(proginfo.prg);

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


bool show_usage(const int argc, const char* const argv[]) {
  if (not Environment::help_header(std::cout, argc, argv, proginfo))
    return false;
  const auto& program = proginfo.prg;
  std::cout <<
    "> " << program << " [input] [output] [log] [conformity-level] [log-level]\n\n"
    "> " << program << " [-cin | filename]\n"
    " runs the translator with input from standard input (the default) or filename.\n"
    "> " << program << " [-cin | filename] [-cout | -cerr | -clog | filename2 | -nil]\n"
      " furthermore appends the DIMACS-output to standard- output/error/log or filename2, or ignores it\n "
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
    "> " << program << " In -nil [-cout]\n"
;
  return true;
}

void output(const std::string filename, const InOut::ConformityLevel cl, const ClauseSets::DClauseSet& F, const Encodings::Encoding& enc, const Translations::Translation& trans, const ClauseSets::CLS& G, const LogLevel ll, const Environment::ProgramInfo& pi) noexcept {
  logout <<
         "c Program information:\n"
         "c created_by                            \"" << pi.prg << "\"\n"
         "c version                               \"" << pi.vrs << "\"\n"
         "c date                                  \"" << pi.date << "\"\n"
         "c author                                " << pi.aut << "\n"
         "c url                                   " << pi.url << "\n"
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
    solout << "c Program " << pi.prg << ": version " << pi.vrs << ", " << pi.date << ".\n";
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

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  const std::string filename = argc>=2 ? argv[1] : "-cin";
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

  output(filename, conlev, F, enc, trans, G, loglev, proginfo);
}
