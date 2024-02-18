// Oliver Kullmann, 18.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef PQOPTIONS_DV3Tssn5k6
#define PQOPTIONS_DV3Tssn5k6

#include <ProgramOptions/Environment.hpp>

namespace PQOptions {

  // Line-type:
  enum class LT {
    row = 0,
    column = 1,
    pdiag = 2,
    padiag = 3,
    qdiag = 4,
    qadiag = 5
  };
  constexpr int LTsize = int(LT::qadiag) + 1;
  constexpr bool rc(const LT lt) noexcept {
    return lt == LT::row or lt == LT::column;
  }
  constexpr bool pandiagonal(const LT lt) noexcept {
    return lt == LT::pdiag or lt == LT::padiag;
  }
  constexpr bool queendiagonal(const LT lt) noexcept {
    return lt == LT::qdiag or lt == LT::qadiag;
  }


  // Constraint-form:
  enum class CF {
    amo = 0,
    alo = 1,
    eo = 2
  };
  constexpr int CFsize = int(CF::eo) + 1;
  constexpr bool allows_type(const CF cf) noexcept {
    return cf != CF::alo;
  }


  // Constraint-type:
  enum class CT {
    prime = 0,
    seco = 1,
    secouep = 2
  };
  constexpr int CTsize = int(CT::secouep) + 1;
  constexpr bool has_uep(const CT ct) noexcept {
    return ct != CT::seco;
  }

}

#endif
