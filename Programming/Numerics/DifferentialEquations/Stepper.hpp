// Oliver Kullmann, 10.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for processing ode's

*/

#ifndef STEPPER_1BFtLs6Zgo
#define STEPPER_1BFtLs6Zgo

namespace Stepper {

  template <typename FLOAT, template <typename> class ODE>
  struct X0Y0 {
    typedef FLOAT float_t;
    typedef ODE<float_t> ode_t;
    typedef typename ode_t::F_t F_t;
    typedef typename ode_t::f_t f_t;
    typedef typename ode_t::count_t count_t;

    X0Y0(const float_t x0, const float_t y0, const F_t F, const f_t sol = f_t()) noexcept : ode(x0, y0, F, sol) {}


  private :
    ode_t ode;
  };

}

#endif
