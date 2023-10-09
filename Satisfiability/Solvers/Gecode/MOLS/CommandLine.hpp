// Oliver Kullmann, 27.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
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
#include <memory>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/NumBasicFunctions.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>

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
  using weights_t = OP::weights_t;

  const std::string version_laMols = "0.103.1";
  const std::string date_laMols = "9.10.2023";
  constexpr int commandline_args_laMols = 14;

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
  std::vector<OPT> read_opt(const std::string opts,
                            const std::string err1, const std::string err2) {
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
  template <typename OPT>
  std::vector<OPT> read_opt([[maybe_unused]]const int argc,
                            const char* const argv[],
                            const int index, const std::string err1,
                            const std::string err2) {
    assert(argc > index);
    return read_opt<OPT>(argv[index], err1, err2);
  }

  std::tuple<list_lbro_t,
             std::unique_ptr<RandGen::RandGen_t>,
             RandGen::vec_eseed_t>
  read_lbro([[maybe_unused]]const int argc,
            const char* const argv[], const int pos,
            const RandGen::vec_eseed_t& hash) {
    assert(argc > pos);
    const std::string s = argv[pos];
    const auto find = s.find(';');
    const bool found = find != std::string::npos;
    using OP::LBRO;
    list_lbro_t list = read_opt<LBRO>(s.substr(0,find),
                                                "bro", "order-heuristics");
    const bool needs_rand = std::ranges::any_of(list,
                            [](const LBRO b){return needs_randgen(b);});
    using namespace RandGen;
    if (not needs_rand) {
      if (not found) return {list, nullptr, RandGen::vec_eseed_t{}};
      else {
        std::ostringstream ss;
        ss << "ERROR[CommandLine::read_lbro]: \";\" present, but no need for "
          "additional (seed-)parameters.\n";
        throw std::runtime_error(ss.str());
      }
    }
    vec_eseed_t seeds = found ?
      extract_seeds_hash(s.substr(find+1), hash) : vec_eseed_t{};
    return std::make_tuple(list, std::make_unique<RandGen_t>(seeds), seeds);
  }

  constexpr double default_threads = 1;
  double read_threads([[maybe_unused]]const int argc,
                      const char* const argv[], const int pos) {
    assert(argc > pos);
    const std::string x = argv[pos];
    if (x.empty()) return default_threads;
    else return FloatingPoint::to_float64(x);
  }

  constexpr unsigned default_comdist = 0;
  typedef std::vector<unsigned> list_unsigned_t;
  list_unsigned_t read_comdist([[maybe_unused]]const int argc,
                               const char* const argv[], const int pos) {
    assert(argc >= pos+1);
    const auto res = FloatingPoint::sequences<unsigned>(argv[pos]);
    if (res.empty()) return {default_comdist}; else return res;
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


  struct WeightsWithOrigin {
    const weights_t w, w0;
    const OP::SPW sp;
    WeightsWithOrigin(OP::weights_t w, OP::SPW sp) : w(w), sp(sp) {
      assert(not w.empty());
      assert(sp != OP::SPW::other);
    }
    WeightsWithOrigin(weights_t w, OP::weights_t w0) :
      w(w), w0(w0), sp(OP::SPW::other) {
      assert(not w.empty()); assert(not w0.empty());
    }
    friend std::ostream& operator <<(std::ostream& out,
                                     const WeightsWithOrigin& wo) {
      if (wo.sp != OP::SPW::other) out << Environment::W0(wo.sp);
      else Environment::out_line(out, wo.w0);
      out << " -> ";
      Environment::out_line(out, wo.w);
      return out;
    }
  };

  // Reminder: weights[0] relates to domain-size 0.
  WeightsWithOrigin weights_zero(const size_t N) {
    return {weights_t(N+1), OP::SPW::zero};
  }
  WeightsWithOrigin weights_one(const size_t N) {
    assert(N >= 1);
    weights_t res(N+1,1);
    res[0] = 0; res[1] = 0;
    return {res, OP::SPW::one};
  }
  WeightsWithOrigin weights_ap(const size_t N) {
    weights_t res; res.reserve(N+1);
    res.push_back(0);
    for (size_t i = 1; i <= N; ++i) res.push_back(i-1);
    return {res, OP::SPW::ap};
  }
  WeightsWithOrigin weights_ld(const size_t N) {
    weights_t res; res.reserve(N+1);
    res.push_back(0);
    for (size_t i = 1; i <= N; ++i)
      res.push_back(FloatingPoint::log2(i));
    return {res, OP::SPW::ld};
  }


  typedef std::vector<OP::weights_t> list_weights_t;
  struct WGenerator {
    using float80 = FloatingPoint::float80;
    using weights_t = OP::weights_t;
    using DIS = OP::DIS;
    using SPW = OP::SPW;
    using EXW = OP::EXW;

    static constexpr size_t num_levels = 19;
    static_assert(num_levels != 0);
    static constexpr float80 minval_newv = -9, minval_wdL = 0.2L;
    static constexpr float80 scaling_special = 0.1L;
    static float80 translate(size_t level, const DIS dis,
                             const EXW ew = EXW::rand) noexcept {
      assert(level < num_levels);
      if (ew == EXW::desc) level = (num_levels-1) - level;
      if (dis == DIS::newvars) return minval_newv + level;
      assert(dis == DIS::wdeltaL);
      return minval_wdL * (level + 1);
    }

    typedef FloatingPoint::Pfloat80<EXW> item_t; // either number or "r" etc.
    static bool uses_levels(const item_t x) noexcept {
      return x.v.index() == 0 and std::get<0>(x.v) != EXW::rand;
    }
    static bool is_random(const item_t x) noexcept {
      return x.v.index() == 0 and std::get<0>(x.v) == EXW::rand;
    }
    typedef std::vector<item_t> pattern_t;
    static bool uses_levels(const pattern_t& pat) noexcept {
      return std::ranges::any_of(pat, [](item_t x){return uses_levels(x);});
    }
    static bool uses_random(const pattern_t& pat) noexcept {
      return std::ranges::any_of(pat, [](item_t x){return is_random(x);});
    }

    const pattern_t pat;
    const bool leveluse = false;
    const bool randomuse = false;
    const SPW sp; // "special"

    using gen_uint_t = RandGen::gen_uint_t;
    const gen_uint_t seed = 0;
  private :
    RandGen::UniformRangeS* const U = nullptr;
  public :

    WGenerator() noexcept : pat{}, sp(SPW::other) {}
    WGenerator(const SPW sp) noexcept : pat{}, sp(sp) {
      assert(sp != SPW::other);
    }
    WGenerator(pattern_t p) noexcept
      : pat(p), leveluse(uses_levels(pat)), randomuse(uses_random(pat)),
        sp(SPW::other),
        seed(randomuse ? RandGen::timestamp_to_eseed() : 0),
        U(randomuse ?
          new RandGen::UniformRangeS(num_levels,
                                     RandGen::transform({seed})) : nullptr) {
      assert(not pat.empty());
    }
    ~WGenerator() noexcept { delete U; }

    float80 code(const EXW e) const noexcept {
      namespace FP = FloatingPoint;
      switch(e) {
      case EXW::rand : return FP::NaN;
      case EXW::asc : return FP::pinfinity;
      case EXW::desc : return FP::minfinity;
      default : assert(false); return 0; }
    }
    gen_uint_t hash_pattern() const noexcept {
      std::vector<float80> transfer; transfer.reserve(pat.size());
      for (const auto& x : pat)
        transfer.push_back(x.v.index() == 0 ? code(std::get<0>(x.v)) :
                           std::get<1>(x.v));
      return FloatingPoint::hash_UInt_range()(transfer);
    }
    std::vector<gen_uint_t> hash() const noexcept {
      std::vector<gen_uint_t> res; res.reserve(4);
      res.push_back(hash_pattern());
      res.push_back(seed);
      res.push_back(gen_uint_t(leveluse) + 2*gen_uint_t(randomuse));
      res.push_back(gen_uint_t(int(sp)));
      return res;
    }


    pattern_t fill_random(const OP::DIS dis) const {
      if (not randomuse) return pat;
      assert(U);
      pattern_t res(pat);
      for (item_t& x : res)
        if (is_random(x)) x.v = translate((*U)(), dis);
      return res;
    }
    list_weights_t transfer(const pattern_t& epat,
                            const OP::DIS dis) const {
      if (not leveluse) {
        weights_t w; w.reserve(epat.size());
        for (const item_t x : epat) {
          assert(x.v.index() == 1);
          w.push_back(std::get<1>(x.v));
        }
        return {w};
      }
      list_weights_t res; res.reserve(num_levels);
      for (size_t level = 0; level < num_levels; ++level) {
        weights_t w; w.reserve(epat.size());
        for (const item_t x : epat) {
          assert(not is_random(x));
          if (x.v.index() == 1) w.push_back(std::get<1>(x.v));
          else w.push_back(translate(level, dis, std::get<0>(x.v)));
        }
        res.push_back(std::move(w));
      }
      return res;
    }

    typedef std::vector<WeightsWithOrigin> list_weightswo_t;
    // The pattern for DIS::newvars handles domain-sizes 1,...,N-1,
    // while the one for DIS::wdL handles 3,...,N:
    list_weightswo_t adapt(const size_t start,
                           const size_t N, const OP::DIS dis) const {
      assert(start < pat.size());
      const pattern_t patdr = fill_random(dis);
      assert(patdr.size() == pat.size());
      const size_t target = dis==OP::DIS::newvars ? N-1 : N-2;
      // Expand patdr to epat, by recycling from "start":
      pattern_t epat(patdr.begin(),
                     patdr.begin() + std::min(target, pat.size()));
      for (size_t i=start; epat.size()<target;
           i = (i==pat.size()-1 ? start : i+1))
        epat.push_back(patdr[i]);
      assert(epat.size() == target);
      list_weights_t res0v = transfer(epat, dis);
      list_weightswo_t resv; resv.reserve(res0v.size());
      for (const weights_t& res0 : res0v) {
        // Interprete res0 (exponantiate and multiply), obtaining res;
        OP::weights_t res(N+1);
        if (dis == OP::DIS::wdeltaL) {
          res[2] = 1;
          for (size_t i = 0; i < target; ++i)
            res[3+i] = res[2+i] * FloatingPoint::exp2(res0[i]);
        }
        else {
          assert(dis == OP::DIS::newvars);
          res[1] = res0[0];
          for (size_t i = 1; i < target; ++i)
            res[1+i] = (i==1 ? 1 : res[i]) * FloatingPoint::exp2(res0[i]);
        }
        resv.emplace_back(std::move(res), std::move(res0));
      }
      return resv;
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
      return leveluse ? num_levels : 1;
    }

    list_weightswo_t operator ()(const size_t N, const OP::LBRT brt,
                                 const  OP::DIS dis) const noexcept {
      list_weightswo_t res; res.reserve(size(N,brt,dis));
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
        const size_t start = pat.size()-1;
        res = adapt(start, N, dis);
      }
      assert(res.size() == size(N,brt,dis));
      if (not res.empty() and res[0].sp == OP::SPW::other and
          dis == OP::DIS::newvars and pat[0].v.index() == 0) {
        list_weightswo_t nres;
        for (WeightsWithOrigin& wo : res) {
          assert(wo.sp == OP::SPW::other);
          weights_t nw = wo.w, nw0 = wo.w0;
          assert(nw.size() >= 2 and nw0.size() >= 1);
          assert(nw0[0] == nw[1]);
          nw[1] *= scaling_special; nw0[0] = nw[1];
          nres.emplace_back(std::move(nw), std::move(nw0));
        }
        res = std::move(nres);
      }
      return res;
    }
  };


  std::pair<WGenerator,bool> read_weights([[maybe_unused]]const int argc,
                          const char* const argv[], const int pos) {
    assert(argc >= pos+1);
    const auto [vecs, from_cin] = Environment::cin_or_string(argv[pos]);
    if (vecs.empty()) return {};
    {const auto sp = Environment::read<OP::SPW>(vecs);
      if (sp) return {sp.value(),false};
    }
    return {FloatingPoint::to_vec_pfloat80<OP::EXW>(vecs,
                                                    from_cin ? ' ' : ','),
        from_cin};
  }


  std::string filename(const std::string& stem,
                       const list_size_t& list_N,
                       const std::string prefix) {
    static const auto timestamp = Environment::CurrentTime::timestamp_uint();
    if (list_N.size() != 1) return {};
    const size_t N = list_N[0];
    std::ostringstream ss;
    ss << prefix << "_" + stem << "_" << N << "_" << timestamp;
    return ss.str();
  }
  const std::string prefix_solutions = "SOLUTIONS";
  std::string output_filename(const std::string& stem,
                              const list_size_t& list_N) {
    return filename(stem, list_N, prefix_solutions);
  }
  const std::string prefix_tree = "TREE";
  std::string treelogging_filename(const std::string& stem,
                                   const list_size_t& list_N) {
    return filename(stem, list_N, prefix_tree);
  }

  class delete_on_exit {
    const std::ostream* const osp;
  public :
    explicit delete_on_exit(const std::ostream* const osp) noexcept
      : osp(osp) {}
    ~delete_on_exit() noexcept { delete osp; }
  };


  void commandline_output(std::ostream& out, const int argc,
                          const char* const argv[]) {
    out << "# command-line: ";
    Environment::args_output(out, argc, argv);
    out << "\n";
  }
  void info_output(std::ostream& out,
                   const list_size_t& list_N,
                   const CD::AConditions& ac, const std::string& name_ac,
                   const std::optional<PS::PSquares>& ps0,
                   const std::string& name_ps,
                   const OP::RT rt,
                   const size_t num_runs, const double threads,
                   const std::string& outfile, const bool with_output,
                   const std::vector<size_t>& seeds) {
    assert(seeds.size() >= 3);
    out << "# N: ";
    Environment::out_line(out, list_N);
    out << "\n"
      "# k=" << ac.k << " " << "total_num_sq=" << ac.num_squares() <<
        ": \"" << name_ac << "\"\n"
      "#   num_uc=" << ac.num_ucs() << " num_eq=" << ac.eq().size() <<
      " num_peq=" << ac.peq().size() << "\n"
      "#   hash=" << seeds[1] << "\n";
    if (not ps0)
      out << "# no_ps";
    else
      out <<
        "# num_ps=" << ps0.value().psqs.size() << ": \"" << name_ps << "\"\n"
        "#   hash=" << seeds[2] << "\n";
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

  void treelogging_output(std::ostream& out, const OP::TREE to,
                          const std::string& treelogfile) {
    out << "# " << Environment::W1<OP::TREE>{to};
    if (not treelogfile.empty()) out << ": " << treelogfile;
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
  void oseed_output(std::ostream& out, const RandGen::vec_eseed_t& seeds) {
    out << "#" << std::string(spaces_algoout, ' ') << "order-seeds: ";
    if (seeds.empty()) out << "(empty)";
    else out << RandGen::ESW{seeds};
    out << "\n";
  }
  template <class VEC>
  void cd_output(std::ostream& out, const VEC& V) {
    out << "#" << std::string(spaces_algoout, ' ') << "commit-distance: ";
    Environment::out_line(out, V); out << "\n";
  }
  void wseed_output(std::ostream& out, const WGenerator& wg) {
    if (wg.randomuse)
      out << "#" << std::string(spaces_algoout, ' ') << "weight-seed=" <<
        wg.seed << "\n";
  }
  void weights_output(std::ostream& out, const WeightsWithOrigin& wo) {
    out << "#" << std::string(spaces_algoout, ' ') << "weights: ";
    const auto old = FloatingPoint::fullprec_float80(out);
    out << wo << "\n";
    out.flush();
    out.precision(old);
  }

  constexpr size_t wnds = 8;
  const Environment::wvec_t widths_la{
    8, 11,
    10, 2,
    wnds, wnds};
  void rh_genstats(std::ostream& out) {
    Environment::print1d(out,
      std::make_tuple("satc", "t",
                      "ppc", "st",
                      "nds", "lvs"),
      widths_la);
  }
  template <class STATS>
  void rs_genstats(std::ostream& out, const STATS& res) {
    const auto inds = res.S[0].N(), lvs = res.S[1].N(), nds = inds+lvs;
    Environment::print1d(out,
      std::make_tuple(
        res.b.sol_found, res.ut,
        res.gs.propagate, res.stopped,
        nds, lvs),
      widths_la);
  }

  constexpr size_t sep_spaces = 0;
  constexpr size_t precision = 3;
  constexpr size_t wN = 3, wgcd = 4;

}

#endif
