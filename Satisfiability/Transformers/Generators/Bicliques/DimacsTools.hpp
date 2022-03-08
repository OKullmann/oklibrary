// Oliver Kullmann, 28.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Certain tools for handling Dimacs-files

   - Imported from Random/ClauseSets.hpp:

      Var, Lit, Clause, ClauseList, dimacs_pars, DimacsClauseList
     plus var_t.


   - Reading strict Dimacs from istream:

    - read_strict_dimacs_pars
    - read_strict_literal
    - read_strict_clause
    - read_strict_Dimacs.


   - Using external SAT solvers:

    - typedef Lit_filter
    - scoped enum SolverR
    - function extract_ret(SystemCalls::ReturnValue) -> SolverR
    - class Minisat_stats
    - class Minisat_return

    - const strings input_filename, output_filename, minisat_string

    - minisat_call(string, Lit_filter) -> Minisat_return
    - minisat_call(DimacsClauseList, Lit_filter) -> Minisat_return


TODOS:

1. Read the minisat-statistics.

*/

#ifndef DIMACSTOOLS_E5ASWHPBqt
#define DIMACSTOOLS_E5ASWHPBqt

#include <istream>
#include <string>
#include <functional>
#include <filesystem>
#include <exception>
#include <utility>
#include <sstream>
#include <fstream>

#include <cassert>

#include <ProgramOptions/Strings.hpp>
#include <Transformers/Generators/Random/ClauseSets.hpp>
#include <SystemSpecifics/SystemCalls.hpp>

namespace DimacsTools {

  /* Reading strict Dimacs
      - for the parameters n, c: n is an upper bound for the occurring
        var-indices, c is the exact number of clauses.
  */
  typedef RandGen::gen_uint_t var_t;
  typedef RandGen::Var Var;
  typedef RandGen::Lit Lit;
  typedef RandGen::Clause Clause;
  typedef RandGen::ClauseList ClauseList;
  typedef RandGen::dimacs_pars dimacs_pars;
  typedef RandGen::DimacsClauseList DimacsClauseList;

  dimacs_pars read_strict_dimacs_pars(std::istream& in) noexcept {
    std::string line;
    do {
      std::getline(in, line); assert(in and not line.empty());
    } while (line.front() == 'c');
    assert(in and not line.empty() and line.starts_with("p cnf "));
    std::stringstream s(line.substr(6));
    var_t n; s >> n; var_t c; s >> c; assert(s);
    return {n,c};
  }

  Lit read_strict_literal(std::istream& in) noexcept {
    std::string s;
    in >> s;
    assert(in); assert(not s.empty());
    if (s.starts_with('-')) return {std::stoull(s.substr(1)), -1};
    else return {std::stoull(s), +1};
  }
  Clause read_strict_clause(std::istream& in) noexcept {
    assert(in);
    Clause res;
    Lit x;
    while ((x = read_strict_literal(in)).v != Var{0}) res.push_back(x);
    return res;
  }
  DimacsClauseList read_strict_Dimacs(std::istream& in) noexcept {
    assert(in);
    DimacsClauseList res;
    res.first = read_strict_dimacs_pars(in); assert(in);
    var_t c = res.first.c;
    res.second.reserve(c);
    for (; c != 0; --c) res.second.push_back(read_strict_clause(in));
    return res;
  }


  typedef std::function<bool(Lit)> Lit_filter;
  const Lit_filter triv_filter = [](Lit){return true;};

  enum class SolverR {
    unknown=0,
    sat=1,
    unsat=2,
    aborted=3
  };
  std::ostream& operator <<(std::ostream& out, const SolverR sr) {
    switch (sr) {
    case SolverR::unknown : return out << "returned undetermined";
    case SolverR::sat : return out << "returned SAT";
    case SolverR::unsat : return out << "returned UNSAT";
    case SolverR::aborted : return out << "call aborted";
    default : return out << "SolverR::UNKNOWN";}
  }
  SolverR extract_ret(const SystemCalls::ReturnValue& rv) {
    if (rv.s != SystemCalls::ExitStatus::normal) return SolverR::aborted;
    else if (rv.val == 0) return SolverR::unknown;
    else if (rv.val == 10) return SolverR::sat;
    else if (rv.val == 20) return SolverR::unsat;
    else
      throw std::runtime_error(
        "DimacsTools::extract_ret: unknown return-value "
        + std::to_string(rv.val));
  }

  struct Minisat_stats {
    SolverR sr;
    typedef SystemCalls::EReturnValue ret_t;
    Minisat_stats(const ret_t& rv) : sr(extract_ret(rv.rv)) {}
    friend std::ostream& operator <<(std::ostream& out, const Minisat_stats& s) {
      return out << s.sr << "\n";
    }
  };

  struct Minisat_return {
    typedef SystemCalls::EReturnValue ret_t;
    ret_t rv;
    Minisat_stats stats;
    Clause pa; // satisfyfing assignment, otherwise empty

    Minisat_return(const ret_t rv, const Lit_filter& f, const std::filesystem::path& out) : rv(rv), stats(rv), pa(extract_pa(stats.sr, f, out)) {}

    static Clause extract_pa(const SolverR sr, const Lit_filter& f, const std::filesystem::path& out) {
      if (sr != SolverR::sat) return {};
      auto lines = Environment::get_lines(out);
      if (lines.size() != 2)
        throw std::runtime_error("DimacsTools::Minisat_return::extract_pa: "
          "output-file has " + std::to_string(lines.size()) + " lines");
      if (lines[0] != "SAT")
        throw std::runtime_error("DimacsTools::Minisat_return::extract_pa: "
          "first output-line is \"" + lines[0] + "\"");
      std::stringstream o(std::move(lines[1]));
      Clause pa;
      for (Lit x; (x=read_strict_literal(o)).v.v != 0;)
        if (f(x)) pa.push_back(x);
      return pa;
    }
  };

  const std::string input_filename = "DimacsTools_minisatcall_in_";
  const std::string output_filename = "DimacsTools_minisatcall_out_";
  const std::string minisat_string = "minisat";

  Minisat_return minisat_call(const std::string& input,
                              const Lit_filter& f = triv_filter,
                              const std::string& options = "") {
    assert(not input.empty());
    const std::string timestamp =
      std::to_string(Environment::CurrentTime::timestamp());
    const std::string out = SystemCalls::system_filename(output_filename + timestamp);
    const std::string command = minisat_string + " " + options + " "
      + input + " " + out;
    const std::filesystem::path pout(out);
    const Minisat_return res(SystemCalls::esystem(command, ""), f, pout);
    if (not std::filesystem::remove(pout))
      throw std::runtime_error(
        "DimacsTools::minisat_call: error when removing file " + out);
    return res;
  }

  Minisat_return minisat_call(const DimacsClauseList& F,
                              const Lit_filter& f = triv_filter,
                              const std::string& options = "") {
    const std::string timestamp =
      std::to_string(Environment::CurrentTime::timestamp());
    const std::string in = SystemCalls::system_filename(input_filename + timestamp);
    {std::ofstream fin(in);
     if (not fin)
       throw std::runtime_error(
         "DimacsTools::minisat_call(F): error when creating input-file " + in);
     fin << F;
    }
    const Minisat_return res = minisat_call(in, f, options);
    const std::filesystem::path pin(in);
    if (not std::filesystem::remove(pin))
      throw std::runtime_error(
        "DimacsTools::minisat_call(F): error when removing file " + in);
    return res;
  }

}

#endif
