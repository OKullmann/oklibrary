// Oliver Kullmann, 27.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading the command-line arguments

*/

#ifndef COMMANDLINE_hcOzkjbxbQ
#define COMMANDLINE_hcOzkjbxbQ

#include <exception>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <ostream>
#include <utility>
#include <tuple>

#include <cassert>

#include <Numerics/NumInOut.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Options.hpp"

namespace CommandLine {

  namespace CD = Conditions;
  namespace EC = Encoding;
  namespace PR = Parsing;
  namespace PS = PartialSquares;
  namespace OP = Options;

  typedef CD::size_t size_t;

  size_t read_N([[maybe_unused]]const int argc,
                const char* const argv[]) {
    assert(argc >= 2);
    const size_t N = FloatingPoint::to_UInt(argv[1]);
    if (not EC::EncCond::valid(N)) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_N]: value N=" << N << " is not allowed.";
      throw std::runtime_error(ss.str());
    }
    return N;
  }

  std::pair<CD::AConditions,std::string>
  read_ac([[maybe_unused]]const int argc, const char* const argv[]) {
    assert(argc >= 3);
    const std::string filename = argv[2];
    std::ifstream file(filename);
    if (not file) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_ac]: conditions-file \"" << argv[2] <<
        "\" could not be opened for reading.";
      throw std::runtime_error(ss.str());
    }
    return {PR::ReadAC()(file), filename};
  }

  std::pair<PS::PSquares, std::string>
  read_ps([[maybe_unused]]const int argc, const char* const argv[],
          const size_t N) {
    assert(argc >= 4);
    const std::string name = argv[3];
    if (name.empty()) return {{N,{}}, {}};
    std::ifstream file(name);
    if (not file) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_ps]: partial-squares-file \"" << name <<
        "\" could not be opened for reading.";
      throw std::runtime_error(ss.str());
    }
    return {PS::PSquares(N, file), name};
  }

  OP::RT read_rt([[maybe_unused]]const int argc,
                 const char* const argv[]) {
    assert(argc >= 5);
    const std::string rts = argv[4];
    const auto rt0 = Environment::read<OP::RT>(rts);
    if (not rt0) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_rt]: the run-type could not be read from"
        " string \"" << rts << "\".";
      throw std::runtime_error(ss.str());
    }
    return rt0.value();
  }

  typedef std::vector<OP::BRT> list_brt_t;
  typedef std::vector<OP::PropO> list_propo_t;
  typedef std::vector<OP::BHV> list_bhv_t;
  typedef std::vector<OP::GBO> list_gbo_t;
  [[deprecated]] typedef std::vector<OP::BHO> list_bho_t;
  typedef std::vector<OP::LAR> list_lar_t;

  template <typename OPT>
  std::vector<OPT> read_opt([[maybe_unused]]const int argc,
                            const char* const argv[],
                            const int index, const std::string err1,
                            const std::string err2) {
    assert(argc > index);
    const std::string opts = argv[index];
    if (opts.empty()) return {OPT(0)};
    const bool exclude = opts[0] == '-';
    const bool sign = exclude or (opts[0] == '+');
    const auto items = Environment::split(sign ? opts.substr(1) : opts, ',');
    if (exclude) {
      std::vector<bool> excl(Environment::RegistrationPolicies<OPT>::size);
      for (const std::string& item : items) {
        const auto opt0 = Environment::read<OPT>(item);
        if (not opt0) {
          std::ostringstream ss;
          ss << "ERROR[CommandLine::read_" << err1 << "]: " << err2 <<
            "-item \"" << item << "\" invalid (in exclusion-list).";
          throw std::runtime_error(ss.str());
        }
        excl[int(opt0.value())] = 1;
      }
      std::vector<OPT> res;
      for (size_t i = 0; i < excl.size(); ++i)
        if (not excl[i]) res.push_back(OPT(i));
      return res;
    }
    else {
      std::vector<OPT> res; res.reserve(items.size());
      for (const std::string& item : items) {
        const auto opt0 = Environment::read<OPT>(item);
        if (not opt0) {
          std::ostringstream ss;
          ss << "ERROR[CommandLine::read_" << err1 << "]: " << err2 <<
            "-item \"" << item << "\" invalid.";
          throw std::runtime_error(ss.str());
        }
        res.push_back(opt0.value());
      }
      return res;
    }
  }

  auto read_weights([[maybe_unused]]const int argc,
                    const char* const argv[], const size_t N,
                    const int pos) {
    assert(argc >= pos+1);
    const auto res = FloatingPoint::to_vec_float80(argv[pos], ',');
    if (res.size() != N-1) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_weights]: For lookahead, the "
        "weights-vector must have " " size N-1=" << N-1 << ","
        " but the size is " << res.size() << ".\n";
      throw std::runtime_error(ss.str());
    }
    return res;
  }


  double read_threads([[maybe_unused]]const int argc,
                      const char* const argv[], const int pos) {
    assert(argc >= pos+1);
    const std::string x = argv[pos];
    if (x.empty()) return 1;
    else return FloatingPoint::to_float64(x);
  }


  std::string output_filename(const std::string& stem, const size_t N) {
    std::ostringstream ss;
    ss << "SOLUTIONS_" + stem << "_" << N << "_"
       << Environment::CurrentTime::timestamp_uint();
    return ss.str();
  }


  void info_output(std::ostream& out,
                   const size_t N,
                   const CD::AConditions& ac, const std::string& name_ac,
                   const PS::PSquares& ps, const std::string& name_ps,
                   const OP::RT rt,
                   const size_t num_runs, const double threads,
                   const std::string& outfile, const bool with_output) {
    out << "# N=" << N << "\n"
      "# k=" << ac.k << " " << "total_num_sq=" << ac.num_squares() <<
        ": \"" << name_ac << "\"\n"
      "#   num_uc=" << ac.num_ucs() << " num_eq=" << ac.eq().size() <<
      " num_peq=" << ac.peq().size() << "\n"
      "# num_ps=" << ps.psqs.size() << ": \"" << name_ps << "\"\n" <<
      "# num_runs=" << num_runs << "\n"
      "# threads=" << threads << "\n"
      "# rt=" << rt << "\n";
    if (with_output) out << "# output-file " << outfile << "\n";
  }

  template <size_t I = 0, typename... T>
  void algo_output(std::ostream& out, const std::tuple<T...>& t) {
    constexpr size_t size = sizeof...(T);
    static_assert(I <= size);
    if constexpr (I < size) {
      out << "#   "; Environment::out_vecpol(out, std::get<I>(t)); out << "\n";
      algo_output<I+1>(out, t);
    }
  }

}

#endif
