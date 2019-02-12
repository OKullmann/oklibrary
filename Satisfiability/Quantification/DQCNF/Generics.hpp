// Oliver Kullmann, 12.2.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Programming tools */

#ifndef GENERICS_YeDoJP0gRe
#define GENERICS_YeDoJP0gRe

namespace Generics {

  /* Extracting the underlying code of enum-classes (scoped enums) */
  template <typename EC>
  inline constexpr int code(const EC e) noexcept {return static_cast<int>(e);}

}

#endif
