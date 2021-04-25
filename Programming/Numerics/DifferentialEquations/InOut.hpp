// Oliver Kullmann, 25.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#ifndef INOUT_Dsmw83TydP
#define INOUT_Dsmw83TydP

#include <string>
#include <vector>
#include <ostream>
#include <iomanip>

#include <Numerics/FloatingPoint.hpp>


namespace InOut {

  enum class ParFormat { with_value=0, no_value=1 };

  template <typename FLOAT>
  struct Params {
    typedef FLOAT float_t;
    std::string name;
    float_t value;
    ParFormat format = ParFormat::with_value;

    Params(const std::string n, const float_t v) : name(n), value(v) {}
    explicit Params(const std::string n)
      : name(n), format(ParFormat::no_value) {}
  };

  template <typename FLOAT>
  using list_params_t = std::vector<Params<FLOAT>>;

  template <typename FLOAT>
  void out_params(std::ostream& out, const list_params_t<FLOAT>& lp) {
    const auto old_prec = out.precision();
    FloatingPoint::fullprec_floatg<FLOAT>(std::cout);
    for (const auto& p : lp)
      switch (p.format) {
      case ParFormat::with_value :
        out << p.name << " = " << p.value << "\n"; break;
      case ParFormat::no_value :
        out << "  " << p.name << "\n"; break;
      default :
        out << "InOut::ParFormat: " << int(p.format) << "\n";
      }
    out.precision(old_prec);
  }

}

#endif
