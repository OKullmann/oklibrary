// Oliver Kullmann, 3.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions regarding the generator LSRG for random latin squares

   - enum classes GenO, EncO, ForO, with registration
   - option_t
   - toSelection(N, string)
   - enum class (lsrg_variant) (for the variations)

   - basic_seeds(lsrg_variant, N, selection, go, so)

   - random_ls(N, selection, go, so, generator)
   - random_ls(N, selection, go, so, seed-vector)

   - lsrg_t (triple of ls_t, vec_eseed_t, gen_uint_t)
   - random_ls(lsrg_variant, N, seed-string, selection, go, so)
   - random_ls(lsrg_variant, N, seed-string, go, so)

   - enc(N, i, j, k)
   - dimacs_output(out, L)

   - Count_ls (for statistical analysis)

   - default_dimensions
   - default_filesuffix
   - default_filename.

TODOS:

1. Output as partial assignment in Dimacs format
  - In principle, one should connect to Encoding::VarEncoding in
    Generators/LatinSquares/Encoding.hpp.
  - That would handle the case of reduced forms (that seems to be the
    only relevant option here).
  - The member-function computing the Dimacs-index of the variable for
    (i,j,k) (with k written "eps" there) is
      enc(i,j,k,0)
    (0 is the index of the ls).
  - The formula used is
      enc(i,j,k,0) = 1 + i * N^2 + j * N + k.
  - Since this row-wise scheme is quite canonical, we "hardcode" it here.
  - Let's call it the "row-wise var-encoding".


*/

#ifndef LSRG_r3HkyAjxRY
#define LSRG_r3HkyAjxRY

#include <tuple>
#include <map>
#include <ostream>
#include <iomanip>
#include <string>
#include <regex>
#include <array>

#include <cstdlib>
#include <cstdint>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <Numerics/Statistics.hpp>

#include "ClauseSets.hpp"
#include "SeedOrganisation.hpp"
#include "Tests.hpp"

//Guaranteed to be included:
#include "LatinSquares.hpp"

namespace LSRG {

  namespace LS = LatinSquares;
  namespace RG = RandGen;
  namespace SO = SeedOrganisation;

  constexpr LS::ls_dim_t N_default = 10;

  enum class GenO : SO::eseed_t {majm=0, jm=1, ma=2};
  enum class EncO {ls=0, dim=1};
  enum class ForO {wc=0, nco=1, os=2};

  constexpr char sep = ',';
  typedef std::tuple<LS::StRLS, GenO, EncO, ForO> option_t;

}
namespace Environment {
  template <>
  struct RegistrationPolicies<LSRG::GenO> {
    static constexpr int size = int(LSRG::GenO::ma)+1;
    static constexpr std::array<const char*, size> string
    {"mj", "jm", "ma"};
  };
  template <>
  struct RegistrationPolicies<LatinSquares::StRLS> {
    static constexpr int size = int(LatinSquares::StRLS::both)+1;
    static constexpr std::array<const char*, size> string
      {"nos", "rs", "cs", "rcs"};
  };
  template <>
  struct RegistrationPolicies<LSRG::EncO> {
    static constexpr int size = int(LSRG::EncO::dim)+1;
    static constexpr std::array<const char*, size> string
      {"-enc", "+enc"};
  };
  template <>
  struct RegistrationPolicies<LSRG::ForO> {
    static constexpr int size = int(LSRG::ForO::os)+1;
    static constexpr std::array<const char*, size> string
      {"+co", "-co", "+se"};
  };
}
namespace LatinSquares {
  std::ostream& operator <<(std::ostream& out, const StRLS s) {
    switch (s) {
    case StRLS::none : return out << "no-std";
    case StRLS::row : return out << "row-std";
    case StRLS::column : return out << "col-std";
    default : return out << "rc-std";}
  }
}
namespace LSRG {
  std::ostream& operator <<(std::ostream& out, const GenO g) {
    switch (g) {
    case GenO::majm : return out << "ma+jm";
    case GenO::jm : return out << "jm-only";
    default : return out << "ma-only";}
  }
  std::ostream& operator <<(std::ostream& out, const EncO e) {
    switch (e) {
    case EncO::ls : return out << "as-square";
    default : return out << "Dimacs-assignment";}
  }
  std::ostream& operator <<(std::ostream& out, const ForO f) {
    switch (f) {
    case ForO::wc : return out << "with-comments";
    case ForO::nco : return out << "no-comments";
    default : return out << "seeds-only";}
  }


  enum class lsrg_variant : SO::eseed_t { basic=0, with_k=1 };
  std::ostream& operator <<(std::ostream& out, const lsrg_variant v) {
    switch (v) {
    case lsrg_variant::basic : return out << "single_ls";
    case lsrg_variant::with_k : return out << "multiple_ls";
    default : return out << "LSRG::lsrg_variant: " << SO::eseed_t(v);
    }
  }


  struct Dim {
    typedef LS::ls_dim_t ls_dim_t;
    typedef std::uint64_t count_t;

    static constexpr count_t max_k = 6148914691236517204UL; // (2^64-1-2)/3

    ls_dim_t N = N_default;
    lsrg_variant v{};
    count_t k = 1;

    Dim() noexcept {}
    Dim(const ls_dim_t N) noexcept : N(N) {
      assert(LS::valid(N) and v == lsrg_variant::basic and k == 1);
    }
    Dim(const ls_dim_t N, const count_t k) noexcept :
      N(N), v(lsrg_variant::with_k), k(k) {
        assert(LS::valid(N) and valid(k) and v == lsrg_variant::with_k);
      }

    static bool valid(const count_t k) noexcept {
      return k >= 1 and k < max_k;
    }
  };
  bool valid(const Dim D) noexcept {
    return LS::valid(D.N) and Dim::valid(D.k) and
      (D.v != lsrg_variant::basic or D.k == 1);
  }


  typedef std::vector<LS::Selection> selection_vt;

  inline const std::basic_regex selection1_rx("(\\d+),(\\d+),(\\d+)");
  inline const std::basic_regex selectionk_rx
    (R"#((?:([1-9]\d*)\*(?:\d+),(?:\d+),(?:\d+);)*(?:([1-9]\d*)\*(?:\d+),(?:\d+),(?:\d+));?)#");

  selection_vt toSelection(const Dim D, const std::string s, const std::string& error) {
    if (D.k == 1) {
      if (s.empty()) return {{D.N}};
      std::smatch m;
      if (not std::regex_match(s, m, selection1_rx)) {
        std::cerr << error << "Syntax error with selection:\n"
          "  \"" << s << "\"\n";
        std::exit(int(RG::Error::invalid));
      }
      assert(m.size() == 4);
      std::array<LS::ls_dim_t, 3> params;
      for (unsigned i = 0; i < 3; ++i) {
        unsigned long val;
        try { val = std::stoul(m[i+1]); }
        catch (const std::out_of_range& e) {
          std::cerr << error << "Selection argument " << i+1 <<
            " out of range:\n" << m[i+1] << "\n";
          std::exit(int(RG::Error::domain));
        }
        if (val > FloatingPoint::P232m1) {
          std::cerr << error << "Selection argument "<< i+1 <<
            " out of range:\n" << val << "\n";
          std::exit(int(RG::Error::domain));
        }
        params[i] = val;
      }
      if (not LS::Selection::check_arguments(D.N,
                                             params[0],params[1],params[2])) {
        std::cerr << error << "Selection parameters not valid for N=" << D.N
                  << ":\n  \"" << s << "\"\n";
        std::exit(int(RG::Error::domain));
      }
      return {{D.N,params[0],params[1],params[2]}};
    }
    else {
      assert(D.k >= 2);
      if (s.empty()) return selection_vt(D.k, {D.N});
      {std::smatch m;
       if (not std::regex_match(s, m, selectionk_rx)) {
         std::cerr << error
                   << "Syntax error with selection for k=" << D.k
                   << ":\n  \"" << s << "\"\n";
         std::exit(int(RG::Error::invalid));
       }
      }
      const auto parts = Environment::split(s, ';');
      const auto size = parts.size();
      assert(size != 0);
      if (size > D.k) {
        std::cerr << error
                  << size << " parts is more than k=" << D.k
                  << " for selection:\n  \"" << s << "\"\n";
         std::exit(int(RG::Error::invalid));
      }
      selection_vt res; res.reserve(D.k);
      for (Dim::count_t i = 0, sum = 0; i < size; ++i) {
        const auto subparts = Environment::split(parts[i], '*');
        assert(subparts.size() == 2);
        unsigned long long count;
        try { count = std::stoull(subparts[0]); }
        catch (const std::out_of_range& e) {
          std::cerr << error << "Count in selection argument " << i+1 <<
            " out of range:\n" << subparts[0] << "\n";
          std::exit(int(RG::Error::domain));
        }
        if (count > D.k or (sum+=count) > D.k) {
          std::cerr << error << "Counts in selection arguments amount to more"
            " than " << D.k << "\n";
          std::exit(int(RG::Error::domain));
        }
        const auto pars_s = Environment::split(subparts[1], ',');
        assert(pars_s.size() == 3);
        std::array<LS::ls_dim_t, 3> pars;
        for (unsigned j = 0; j < 3; ++j) {
          unsigned long val;
          try { val = std::stoul(pars_s[j]); }
          catch (const std::out_of_range& e) {
            std::cerr << error << "Selection argument " << j+1 <<
              " out of range in part " << i+1 << ":\n" << pars_s[j] << "\n";
            std::exit(int(RG::Error::domain));
          }
          if (val > FloatingPoint::P232m1) {
            std::cerr << error << "Selection argument " << j+1 <<
              " out of range in part " << i+1 << ":\n" << pars_s[j] << "\n";
            std::exit(int(RG::Error::domain));
          }
          pars[j] = val;
        }
        if (not LS::Selection::check_arguments(D.N,
                                               pars[0],pars[1],pars[2])) {
          std::cerr << error << "Selection part " << i+1 <<
            " not valid for N=" << D.N << ":\n  \"" << subparts[1] << "\"\n";
          std::exit(int(RG::Error::domain));
        }
        const LS::Selection sel(D.N, pars[0],pars[1],pars[2]);
        for (unsigned long long j = 0; j < count; ++j) res.push_back(sel);
      }
      if (res.size() != D.k) {
        std::cerr << error << "Counts in selection arguments amount to "
                  << res.size() << ", less than " << D.k << "\n";
        std::exit(int(RG::Error::domain));
      }
      return res;
    }
  }


  RG::vec_eseed_t basic_seeds(const Dim& D, const selection_vt& sel, const GenO go, const LS::StRLS so) {
    assert(valid(D));
    RG::vec_eseed_t res = SO::initial_seeding(
        SO::OKlibrary_timestamp,
        SO::Area::combinatorics,
        SO::Combinatorics::latin_squares,
        SO::lsrg_timestamp,
        SO::eseed_t(D.v));
    using SO::eseed_t;
    if (D.v == lsrg_variant::basic) {
      const eseed_t size_spec_params = 1 + 3;
      SO::add_generic_parameters(res,
                                 {eseed_t(go), eseed_t(so)}, size_spec_params);
      assert(sel.size() == 1);
      const auto& s = sel.front();
      SO::add_specific_parameters(res, {D.N, s.r,s.c,s.s});
    }
    else {
      assert(D.v == lsrg_variant::with_k);
      const eseed_t size_spec_params = 2 + 3*D.k;
      SO::add_generic_parameters(res,
                                 {eseed_t(go), eseed_t(so)}, size_spec_params);
      assert(sel.size() == D.k);
      RG::vec_eseed_t add; add.reserve(size_spec_params);
      add.push_back(D.N); add.push_back(D.k);
      for (const auto& s : sel) {
        add.push_back(s.r); add.push_back(s.c); add.push_back(s.s);
      }
      SO::add_specific_parameters(res, add);
    }
    return res;
  }


  LS::ls_t random_ls(const LS::ls_dim_t N, const LS::Selection& sel, const GenO go, const LS::StRLS so, RG::RandGen_t& g) {
    switch (go) {
    case GenO::majm :
      return LS::select(LS::standardise(
        LS::lsip2ls(LS::jm(LS::full_shuffle(
          LS::random_ma_ls(N, LS::CrRLS::with_initial_phase, g), g),
          g)), so), sel, g);
    case GenO::jm :
      return LS::select(LS::standardise(
        LS::lsip2ls(LS::jm(N, g)), so), sel, g);
    case GenO::ma :
      return LS::select(LS::standardise(
        LS::random_ma_ls(N, LS::CrRLS::with_initial_phase, g), so), sel, g);
    default : return LS::empty_ls(N);
    }
  }

  LS::ls_t random_ls(const LS::ls_dim_t N, const LS::Selection& sel,
                     const GenO go, const LS::StRLS so,
                     const RG::vec_eseed_t& seeds) {
    RG::RandGen_t g(seeds);
    return random_ls(N, {sel}, go, so, g);
  }


  typedef std::tuple<LS::ls_t, RG::vec_eseed_t, RG::gen_uint_t> lsrg_t;

  lsrg_t random_ls(const Dim& D, std::string_view seeds,
                   const LS::Selection& sel, const GenO go = GenO{},
                   const LS::StRLS so = LS::StRLS{}) {
    assert(D.v == lsrg_variant::basic);
    RG::vec_eseed_t s = basic_seeds(D, {sel}, go , so);
    const RG::gen_uint_t basic_size = s.size();
    SO::add_user_seeds(s, seeds);
    return {random_ls(D.N, sel, go, so, s), s, basic_size};
  }
  lsrg_t random_ls(const Dim& D, std::string_view seeds,
                   const GenO go = GenO{}, const LS::StRLS so = LS::StRLS{}) {
    return random_ls(D, seeds, LS::Selection(D.N), go, so);
  }
  // XXX


  constexpr std::uint64_t enc(const LS::ls_dim_t N, const LS::ls_dim_t i, const LS::ls_dim_t j, const LS::ls_dim_t k) noexcept {
    assert(LS::valid(N));
    assert(i < N and j < N and k < N);
    const std::uint64_t n = N;
    return 1 + i * n*n + j * n + k;
  }
  static_assert(enc(1,0,0,0) == 1);
  static_assert(enc(10,0,0,0) == 1);
  static_assert(enc(10,9,9,9) == 1000);

  void dimacs_output(std::ostream& out, const LS::ls_t& L) {
    assert(LS::valid(L));
    out << "v";
    using LS::ls_dim_t;
    const ls_dim_t N = L.size();
    for (ls_dim_t i = 0; i < N; ++i)
      for (ls_dim_t j = 0; j < N; ++j) {
        const ls_dim_t k = L[i][j];
        if (k != N)
          for (ls_dim_t v = 0; v < N; ++v)
            if (v == k) out << " " << enc(N,i,j,k);
            else out << " -" << enc(N,i,j,v);
      }
    out << " 0\n";
  }


  Dim read_N(const std::string s, const std::string& error) {
    if (s.empty()) return {};
    const auto c = s.find(',');
    if (c == std::string::npos) {
      const auto N = FloatingPoint::touint(s);
      if (not LS::valid(N)) {
        std::cerr << error << "N must be a positive integer in [1,"
                  << LS::max_dim-1 << "]" << ", but N=" << N << ".\n";
        std::exit(int(RG::Error::domain));
      }
      return N;
    }
    else {
      const std::string ns = s.substr(0,c);
      const auto N = FloatingPoint::touint(ns);
      if (not LS::valid(N)) {
        std::cerr << error << "N must be a positive integer in [1,"
                  << LS::max_dim-1 << "]" << ", but N=" << N << ".\n";
        std::exit(int(RG::Error::domain));
      }
      const std::string ks = s.substr(c+1, std::string::npos);
      const auto k = FloatingPoint::toUInt(ks);
      if (not Dim::valid(k)) {
        std::cerr << error << "k must be a positive integer in [1,"
                  << Dim::max_k-1 << "]" << ", but k=" << k << ".\n";
        std::exit(int(RG::Error::domain));
      }
      return {N, k};
    }
  }


  /* Analysing generators */

  struct Count_ls {
    typedef RG::gen_uint_t count_t;
    typedef FloatingPoint::float80 stats_t;
    typedef std::map<LS::ls_t, count_t> map_t;
    typedef map_t::const_iterator it_t;
    typedef GenStats::StatsStore<stats_t, stats_t> Statistics;

    map_t M;
    count_t count_diff = 0;
    const LS::ls_dim_t N;
    count_t count_all = 0;

    const LS::StRLS reduced;
    const count_t total;
    const stats_t p;

    Count_ls(const LS::ls_dim_t N, const LS::StRLS reduced) noexcept :
      N(N), reduced(reduced),
      total(LS::count_ls(N, reduced)),
      p(stats_t(1) / total) {}

    void add(const LS::ls_t& L) {
      auto& elem = M[L]; ++elem;
      if (elem == 1) ++count_diff;
      ++count_all;
    }

    stats_t p_value(const it_t it) const noexcept {
      return RG::monobit(it->second, count_all, p);
    }
    std::pair<Statistics,Statistics> evaluation() const noexcept {
      Statistics res1, res2;
      for (it_t i = M.begin(); i != M.end(); ++i) {
        res1 += stats_t(i->second) / count_all;
        res2 += p_value(i);
      }
      assert(res1.N() == count_diff and res2.N() == count_diff);
      res1.update(); res2.update();
      return {res1, res2};
    }

    friend std::ostream& operator <<(std::ostream& out, const Count_ls& c) {
      auto eval = c.evaluation();
      out <<
        "N=" << c.N << ", reduced=" << c.reduced << ", L(N)=" << c.total <<
        ", collector-T = " << FloatingPoint::coupcollprob(c.total) << "\n"
        "T=" << c.count_all << ", found=" << c.count_diff << "\n"
        "exact prob=L(N)/T = " << c.p << "\n";
      out << "frequency statistics:\n";
      eval.first.simple_output(out);
      if (c.total == c.count_diff) {
        out << "p-value statistics:\n";
        eval.second.simple_output(out, true);
      }
      return out;
    }

  };


  /* Default filename */

  std::string default_dimensions(const Dim D) {
    return std::to_string(D.N) + "_" + std::to_string(D.k);
  }

  std::string default_filesuffix(const EncO enc) {
    // bug gcc 10.1.0 with "using enum"
    switch (enc) {
    case EncO::ls : return ".ls";
    case EncO::dim : return ".dimacs";
    default : return "NOT_IMPLEMENTED";
    }
  }

  std::string default_filename(const Dim D, const EncO enc,
                               const RG::vec_eseed_t& s) {
    return SO::default_filestem(SO::Combinatorics::latin_squares) + "_" +
      default_dimensions(D) + "_" +
      SO::default_seeds(s) + default_filesuffix(enc);
  }

}

#endif
