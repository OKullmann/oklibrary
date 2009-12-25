// Oliver Kullmann, 30.12.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProblemInstances/Variables/traits/index_type.hpp
  \brief Traits-metafunctions to extract the index type
*/

#ifndef INDEXTYPE_772020l
#define INDEXTYPE_772020l

namespace OKlib {
  namespace Variables {
    namespace traits {

      /*!
        \class index_type
        \brief Traits meta-function to extract the index type.
      */
      
      template <typename T>
      struct index_type {
        typedef typename T::index_type type;
      };
      
    }

  }

}

#endif
