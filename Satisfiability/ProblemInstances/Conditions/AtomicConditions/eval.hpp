// Oliver Kullmann, 8.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AtomicConditions/eval.hpp
  \brief Primary template for the evaluation function
*/

#ifndef EVAL_66ahahYt3e
#define EVAL_66ahahYt3e

namespace OKlib {
  namespace AtomicConditions {

    template <typename AC>
    inline bool eval(const AC&, const typename traits::value_type<AC>::type&);

  }
}

#endif
