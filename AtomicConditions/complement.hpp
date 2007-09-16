// Oliver Kullmann, 13.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AtomicConditions/complement.hpp
  \brief Primary template for the complementation function
*/

#ifndef COMPLEMENT_jJJggBop67
#define COMPLEMENT_jJJggBop67

namespace OKlib {
  namespace AtomicConditions {

    template <typename AC>
    inline void complement(AC& x) { x.complement(); }

  }
}

#endif
