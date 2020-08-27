// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Statistics for the 2-sweep algorithm

*/

#ifndef STATISTICS_c1FgiM7oWX
#define STATISTICS_c1FgiM7oWX

#include <ostream>
#include <type_traits>

#include <cstdint>

namespace Statistics {

  typedef std::uint_fast64_t count_t;


  class NodeCounts {
    count_t sols, // solutions
            nds,  // inner nodes
            ucs,  // unit-clauses
            r2s,  // r2-satisfiable leaves
            r2u,  // r2-row-unsatisfiable leaves
            cu,   // r2-column-unsatisfiable leaves
            duplications; // how often solutions are multiplied

  public :
    constexpr NodeCounts(const bool root = false) noexcept : sols(0), nds(root), ucs(0), r2s(0), r2u(0), cu(0), duplications(1) {}

    count_t num_sols() const noexcept { return sols; }

    void found_uc() noexcept { ++ucs; }
    void found_r2s() noexcept { ++sols; ++r2s; }
    void found_r2u() noexcept { ++r2u; }
    void found_cu() noexcept { ++cu; }

    void set_duplication(const count_t d) noexcept { duplications = d; }

    NodeCounts& operator +=(const NodeCounts& s) noexcept {
      sols += s.duplications * s.sols;
      nds+=s.nds; ucs+=s.ucs; r2s+=s.r2s; r2u+=s.r2u; cu+=s.cu;
      return *this;
    }

    friend std::ostream& operator <<(std::ostream& out, const NodeCounts& s) {
      return out << s.duplications * s.sols << " " << s.nds << " " << s.ucs
        << " " << s.r2s << " " << s.r2u << " " << s.cu;
    }
  };

  static_assert(std::is_trivially_copyable_v<NodeCounts>);


}

#endif
