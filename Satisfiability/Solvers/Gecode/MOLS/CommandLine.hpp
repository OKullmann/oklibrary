// Oliver Kullmann, 27.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Reading the command-line arguments

  Namespace CommandLine, abbreviated "CL".


TODOS:

1. See the general format for the input of weights in
   Euler_BBOpt.cpp.

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
#include <optional>
#include <type_traits>
#include <algorithm>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/NumBasicFunctions.hpp>

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "Parsing.hpp"
#include "PartialSquares.hpp"
#include "Options.hpp"
#include "Solvers.hpp"
#include "LookaheadBranching.hpp"

namespace CommandLine {

  namespace CD = Conditions;
  namespace EC = Encoding;
  namespace PS = PartialSquares;
  namespace OP = Options;
  namespace SV = Solvers;
  namespace LB = LookaheadBranching;

  using size_t = CD::size_t;

  typedef std::vector<size_t> list_size_t;
  list_size_t read_N([[maybe_unused]]const int argc,
                const char* const argv[]) {
    assert(argc >= 2);
    const list_size_t res = FloatingPoint::sequences<size_t>(argv[1]);
    for (const size_t N : res) {
      if (not EC::EncCond::valid(N)) {
        std::ostringstream ss;
        ss << "ERROR[CommandLine::read_N]: value N=" << N << " is not allowed.";
        throw std::runtime_error(ss.str());
      }
    }
    return res;
  }

  constexpr char escape_char = '@';

  std::pair<CD::AConditions,std::string>
  read_ac([[maybe_unused]]const int argc, const char* const argv[]) {
    assert(argc >= 3);
    const std::string filename = argv[2];
    if (filename.empty())
      throw std::runtime_error("ERROR[CommandLine::read_ac]: "
                               "conditions-file-string empty");
    if (filename[0] == escape_char)
      return {Parsing::ReadAC()(
                Environment::unescape_eol(filename.substr(1))), filename};
    std::ifstream file(filename);
    if (not file) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_ac]: conditions-file \"" << argv[2] <<
        "\" could not be opened for reading.";
      throw std::runtime_error(ss.str());
    }
    return {Parsing::ReadAC()(file), filename};
  }

  typedef std::pair<std::optional<PS::PSquares>, std::string> ps_t;
  ps_t read_ps([[maybe_unused]]const int argc, const char* const argv[],
               const list_size_t& list_N) {
    assert(argc >= 4);
    const std::string name = argv[3];
    if (name.empty() or list_N.empty()) return {{}, {}};
    if (list_N.size() != 1) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_ps]: there must be exactly one N, but "
        "there are " << list_N.size() << ".";
      throw std::runtime_error(ss.str());
    }
    const size_t N = list_N[0];
    if (name[0] == escape_char)
      return {PS::PSquares(N,
                           Environment::unescape_eol(name.substr(1))), name};
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

  typedef std::vector<OP::LAR> list_lar_t;

  typedef std::vector<OP::LBRT> list_lbrt_t;
  typedef std::vector<OP::DIS> list_dis_t;
  typedef std::vector<OP::LBRO> list_lbro_t;

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

  double read_threads([[maybe_unused]]const int argc,
                      const char* const argv[], const int pos) {
    assert(argc >= pos+1);
    const std::string x = argv[pos];
    if (x.empty()) return 1;
    else return FloatingPoint::to_float64(x);
  }

  typedef std::vector<unsigned> list_unsigned_t;
  list_unsigned_t read_comdist([[maybe_unused]]const int argc,
                               const char* const argv[], const int pos) {
    assert(argc >= pos+1);
    const auto res = FloatingPoint::sequences<unsigned>(argv[pos]);
    if (res.empty()) return {0}; else return res;
  }

  template <typename STO>
  LB::StoppingData<STO> read_st(const std::string& sts) {
    if (sts.empty()) return {};
    const auto split = Environment::split(sts, ',');
    if (split.size() != 2) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_gcst]: a non-empty stopping-entry"
        " must have exactly two items,\n   separated by comma, but there are "
         << split.size() << " items in \"" << sts << "\"\n";
      throw std::runtime_error(ss.str());
    }
    const auto sto = Environment::read<STO>(split[0]);
    if (not sto) {
      std::ostringstream ss;
      ss << "ERROR[CommandLine::read_gcst]: " <<
        "stopping-criterion \"" << split[0] << "\" invalid.";
      throw std::runtime_error(ss.str());
    }
    if constexpr (std::is_same_v<STO, OP::STO>)
      return {sto.value(),
                    FloatingPoint::to_unsigned<unsigned long>(split[1])};
    else
      return {sto.value(),
                    FloatingPoint::toUInt(split[1])};
  }
  SV::GcStoppingData read_gcst([[maybe_unused]]const int argc,
                               const char* const argv[], const int pos) {
    assert(argc >= pos+1);
    return read_st<OP::STO>(argv[pos]);
  }

  LB::ListStoppingData read_rlast([[maybe_unused]]const int argc,
                                  const char* const argv[], const int pos) {
    assert(argc >= pos+1);
    const std::string sts = argv[pos];
    const auto split = Environment::split(sts, '|');
    LB::ListStoppingData res;
    for (const std::string& s : split) res += read_st<OP::LRST>(s);
    return res;
  }


  // Reminder: weights[0] relates to domain-size 0.
  OP::weights_t weights_zero(const size_t N) {
    return OP::weights_t(N+1);
  }
  OP::weights_t weights_one(const size_t N) {
    assert(N >= 1);
    OP::weights_t res(N+1,1);
    res[0] = 0; res[1] = 0;
    return res;
  }
  OP::weights_t weights_ap(const size_t N) {
    OP::weights_t res; res.reserve(N+1);
    res.push_back(0);
    for (size_t i = 1; i <= N; ++i) res.push_back(i-1);
    return res;
  }
  OP::weights_t weights_ld(const size_t N) {
    OP::weights_t res; res.reserve(N+1);
    res.push_back(0);
    for (size_t i = 1; i <= N; ++i)
      res.push_back(FloatingPoint::log2(i));
    return res;
  }

  typedef std::vector<OP::weights_t> list_weights_t;
  struct WGenerator {
    using weights_t = OP::weights_t;
    using SPW = OP::SPW;

    typedef weights_t pattern_t; // XXX
    const pattern_t pat;
    const SPW sp;
    const size_t rep = 1;

    WGenerator() noexcept : pat{}, sp(SPW::other) {}
    WGenerator(const SPW sp) noexcept : pat{}, sp(sp) {
      assert(sp != SPW::other);
    }
    WGenerator(pattern_t pat) noexcept : pat(pat), sp(SPW::other) {
      assert(not pat.empty());
    }

    // The pattern for DIS::newvars does not include domain-size N:
    static weights_t adapt(const pattern_t& pat,
                           const size_t N, const OP::DIS dis) {
      assert(not pat.empty());
      const size_t target = dis==OP::DIS::newvars ? N-1 : N-2;
      weights_t pat0(pat.begin(), pat.begin() + std::min(target, pat.size()));
      for (size_t i = 0; pat0.size() < target; i = (i==pat.size()-1 ? 0 : i+1))
        pat0.push_back(pat[i]);
      assert(pat0.size() == target);
      OP::weights_t res(N+1);
      if (dis == OP::DIS::wdeltaL) {
        res[2] = 1;
        for (size_t i = 0; i < target; ++i)
          res[3+i] = res[2+i] * FloatingPoint::exp2(pat0[i]);
      }
      else {
        assert(dis == OP::DIS::newvars);
        res[1] = pat0[0];
        for (size_t i = 1; i < target; ++i)
          res[1+i] = (i==1 ? 1 : res[i]) * FloatingPoint::exp2(pat0[i]);
      }
      return res;
    }

    size_t size(const size_t,
                const OP::LBRT brt, const OP::DIS dis) const noexcept {
      if (sp != SPW::other) return 1;
      if (pat.empty()) {
        if (dis == OP::DIS::wdeltaL) return 2;
        assert(dis == OP::DIS::newvars);
        if (brt == OP::LBRT::bin) return 3;
        assert(brt == OP::LBRT::enu);
        return 4;
      }
      return rep;
    }

    list_weights_t operator ()(const size_t N,
                   const OP::LBRT brt, const  OP::DIS dis) const noexcept {
      list_weights_t res; res.reserve(size(N,brt,dis));
      if (sp != SPW::other) {
        switch (sp) {
        case SPW::zero : res.push_back(weights_zero(N)); break;
        case SPW::one : res.push_back(weights_one(N)); break;
        case SPW::ap : res.push_back(weights_ap(N)); break;
        case SPW::ld : res.push_back(weights_ld(N)); break;
        default : assert(false); break;}
      }
      else if (pat.empty()) {
        if (dis == OP::DIS::wdeltaL) {
          res.push_back(weights_ap(N)); res.push_back(weights_ld(N));
        }
        else if (brt == OP::LBRT::bin) {
          res.push_back(weights_one(N));
          res.push_back(weights_ap(N)); res.push_back(weights_ld(N));
        }
        else {
          res.push_back(weights_zero(N)); res.push_back(weights_one(N));
          res.push_back(weights_ap(N)); res.push_back(weights_ld(N));
        }
      }
      else {
        assert(rep == 1);
        auto w = adapt(pat, N, dis);
        res.push_back(w);
      }
      assert(res.size() == size(N,brt,dis));
      return res;
    }
  };

  WGenerator read_weights([[maybe_unused]]const int argc,
                          const char* const argv[], const int pos) {
    assert(argc >= pos+1);
    const std::string vecs = argv[pos];
    if (vecs.empty()) return {};
    {const auto sp = Environment::read<OP::SPW>(vecs);
     if (sp) return {sp.value()};
    }
    return {FloatingPoint::to_vec_float80(argv[pos], ',')};
  }


  std::string output_filename(const std::string& stem,
                              const list_size_t& list_N) {
    if (list_N.size() != 1) return {};
    const size_t N = list_N[0];
    std::ostringstream ss;
    ss << "SOLUTIONS_" + stem << "_" << N << "_"
       << Environment::CurrentTime::timestamp_uint();
    return ss.str();
  }


  void info_output(std::ostream& out,
                   const list_size_t& list_N,
                   const CD::AConditions& ac, const std::string& name_ac,
                   const std::optional<PS::PSquares>& ps0,
                   const std::string& name_ps,
                   const OP::RT rt,
                   const size_t num_runs, const double threads,
                   const std::string& outfile, const bool with_output) {
    out << "# N: ";
    Environment::out_line(out, list_N);
    out << "\n"
      "# k=" << ac.k << " " << "total_num_sq=" << ac.num_squares() <<
        ": \"" << name_ac << "\"\n"
      "#   num_uc=" << ac.num_ucs() << " num_eq=" << ac.eq().size() <<
      " num_peq=" << ac.peq().size() << "\n";
    if (not ps0)
      out << "# no_ps";
    else
      out <<
        "# num_ps=" << ps0.value().psqs.size() << ": \"" << name_ps << "\"";
    out << "\n"
      "# num_runs=" << num_runs << "\n"
      "# threads=" << threads << "\n"
      "# rt=" << rt << "\n";
    if (with_output) out << "# output-file " << outfile << "\n";
  }

  void st_output(std::ostream& out, const SV::GcStoppingData stod) {
    if (stod)
      out << "# stopping: " << stod << "\n";
    else
      out << "# no_stopping\n";
  }
  void st_output(std::ostream& out, const LB::ListStoppingData& stod) {
    if (stod) {
      out << "# stopping: "; Environment::out_line(out, stod.list());
    }
    else
      out << "# no_stopping";
    out << "\n";
  }

  constexpr size_t spaces_algoout = 3;
  template <size_t I = 0, typename... T>
  void algo_output(std::ostream& out, const std::tuple<T...>& t) {
    constexpr size_t size = sizeof...(T);
    static_assert(I <= size);
    if constexpr (I < size) {
      out << "#" << std::string(spaces_algoout, ' ');
      Environment::out_vecpol(out, std::get<I>(t)); out << "\n";
      algo_output<I+1>(out, t);
    }
  }
  template <class VEC>
  void cd_output(std::ostream& out, const VEC& V) {
    out << "#" << std::string(spaces_algoout, ' ') << "commit-distance: ";
    Environment::out_line(out, V); out << "\n";
  }
  void weights_output(std::ostream& out, const OP::weights_t& wv) {
    out << "#" << std::string(spaces_algoout, ' ') << "weights: ";
    const auto old = FloatingPoint::fullprec_float80(out);
    Environment::out_line(out, wv); out << "\n";
    out.precision(old);
  }

}

#endif
