// Oliver Kullmann, 3.1.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODO:

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

2. DONE Output options
  - DONE With or without comments:
   - "wco" (the default), "nco", "oso" (only seeds, and then just the numbers)
  - DONE Output as is or in Dimacs-format:
   - "lso" (the default), "dio"
   - DONE For now, not doing anything in case of standardisation (as we have it
     now for selection).
   - DONE Only in case of wco and lso with leading newline.
   - DONE In case of dio also outputting n = N^3.


*/

#ifndef LSRG_r3HkyAjxRY
#define LSRG_r3HkyAjxRY

#include <tuple>
#include <map>
#include <ostream>
#include <iomanip>
#include <optional>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "SeedOrganisation.hpp"
#include "Tests.hpp"
#include "Statistics.hpp"

//Guaranteed to be included:
#include "LatinSquares.hpp"

namespace LatinSquares {

  // Outputting the steps:
  void jm_next(std::ostream& out, ls_ip_t& I, RG::RandGen_t& g) noexcept {
    const ls_dim_t N = I.size(); if (N == 1) return;
    out << "jm_next input:\n" << I;
    std::uint64_t count = 0;
    triple_t r = find_zero(I, g); triple_t o = find_ones(I, r);
    out << "selected cell and indices for 1's: " << r << ", " << o << "\n";
    move(I, r, o);
    out << "after move:\n" << I;

    for (; I[o[0]][o[1]][o[2]] == -1; ++count) {
      r = o;
      const auto p = find_both_ones(I, r);
      const bool b0=RG::bernoulli(g), b1=RG::bernoulli(g), b2=RG::bernoulli(g);
      const bool c[3] = {b0,b1,b2};
      for (ls_dim_t i = 0; i < 3; ++i) o[i] = p[c[i]][i];
      out << "new selected cell and indices for 1's: "
          << r << ", " << o << "\n";
      move(I, r, o);
      out << "after move:\n" << I;
    }
  }

}

namespace LSRG {

  namespace LS = LatinSquares;
  namespace RG = RandGen;
  namespace SO = SeedOrganisation;

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


  std::optional<LS::Selection> toSelection(const LS::ls_dim_t N, const std::string_view s) {
    if (s.empty()) return {N};
    const auto split = Environment::split(s, ',');
    if (split.size() != 3) return {};
    const LS::ls_dim_t a = FloatingPoint::touint(split[0]),
      b = FloatingPoint::touint(split[1]), c = FloatingPoint::touint(split[2]);
    if (not LS::Selection::check_arguments(N,a,b,c)) return {};
    return LS::Selection{N,a,b,c};
  }


  RG::vec_eseed_t basic_seeds(const LS::ls_dim_t N, const LS::Selection& sel, const GenO go, const LS::StRLS so) {
    RG::vec_eseed_t res = SO::initial_seeding(
        SO::OKlibrary_timestamp,
        SO::Area::combinatorics,
        SO::Combinatorics::latin_squares,
        SO::lsrg_timestamp,
        SO::lsrg_variant);
    using SO::eseed_t;
    const eseed_t size_spec_params = 1 + 3;
    SO::add_generic_parameters(res,
                               {eseed_t(go), eseed_t(so)}, size_spec_params);
    SO::add_specific_parameters(res, {N, sel.r,sel.c,sel.s});
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
    return random_ls(N, sel, go, so, g);
  }

  typedef std::tuple<LS::ls_t, RG::vec_eseed_t, RG::gen_uint_t> lsrg_t;
  lsrg_t random_ls(const LS::ls_dim_t N, std::string_view seeds,
              const LS::Selection& sel, const GenO go = GenO{},
              const LS::StRLS so = LS::StRLS{}) {
    RG::vec_eseed_t s = basic_seeds(N, sel, go , so);
    const RG::gen_uint_t basic_size = s.size();
    SO::add_user_seeds(s, seeds);
    return {random_ls(N, sel, go, so, s), s, basic_size};
  }
  lsrg_t random_ls(const LS::ls_dim_t N, std::string_view seeds,
            const GenO go = GenO{}, const LS::StRLS so = LS::StRLS{}) {
    return random_ls(N, seeds, LS::Selection(N), go, so);
  }


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


  /* Analysing generators */

  struct Count_ls {
    typedef RG::gen_uint_t count_t;
    typedef FloatingPoint::float80 stats_t;
    typedef std::map<LS::ls_t, count_t> map_t;
    typedef map_t::const_iterator it_t;
    typedef RG::StatsStore<stats_t, stats_t> Statistics;

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
      assert(res1.N == count_diff and res2.N == count_diff);
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

}

#endif
