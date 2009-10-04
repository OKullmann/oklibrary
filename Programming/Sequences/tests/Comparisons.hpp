// Oliver Kullmann, 8.9.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Sequences/tests/Comparisons.hpp
  \brief Generic tests for algorithms comparing sequences

  \todo How to get a kind of version number like CVS's?
  <ul>
   <li> For the time being, we edit these entries by hand. </li>
   <li> See "Change dates and revision numbers" in
   Buildsystem/SourceControl/plans/general.hpp. </li>
  </ul>

*/

#ifndef COMPARISONS_nnxcad5410o
#define COMPARISONS_nnxcad5410o

#include <vector>
#include <tr1/tuple>

#include <OKlib/TestSystem/TestBaseClass_DesignStudy.hpp>

namespace OKlib {
  namespace Programming {
    namespace Sequences {
      namespace tests {

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 8.9.2007 23:20:00 $", "$Revision: 1 $")

        /*!
          \class CommonPart
          \brief Testing functors for computing longest common initial sequences

          \todo Yet only the non-const version is handled.
           <ul>
            <li> Can we check for both in one test function ? </li>
            <li> Or do we need another version CommonPart_c ? </li>
           </ul>
        */

        template <template <class R1, class R2, class IP> class CP, class ImpPol>
        OKLIB_TEST_CLASS(CommonPart) {
          OKLIB_TEST_CLASS_C(CommonPart) {}
        private :
          void test(::OKlib::TestSystem::Basic) {
            {
              typedef std::vector<int> seq_t;
              typedef CP<seq_t, seq_t, ImpPol> com_t;
              com_t com;
              seq_t v1, v2;
              if (com(v1, v2) != std::tr1::make_tuple(v1.begin(), v2.begin(), 0U))
                OKLIB_THROW("OKLIB_TEST_EQUAL");
              v1.push_back(1);
              if (com(v1, v2) != std::tr1::make_tuple(v1.begin(), v2.begin(), 0U))
                OKLIB_THROW("OKLIB_TEST_EQUAL");
              v2.push_back(1);
              if (com(v1, v2) != std::tr1::make_tuple(v1.end(), v2.end(), 1U))
                OKLIB_THROW("OKLIB_TEST_EQUAL");
              v2.push_back(2);
              if (com(v1, v2) != std::tr1::make_tuple(v1.end(), --v2.end(), 1U))
                OKLIB_THROW("OKLIB_TEST_EQUAL");
              v2.push_back(3);
              if (com(v1, v2) != std::tr1::make_tuple(v1.end(), ----v2.end(), 1U))
                OKLIB_THROW("OKLIB_TEST_EQUAL");
              v1.push_back(2);
              if (com(v1, v2) != std::tr1::make_tuple(v1.end(), --v2.end(), 2U))
                OKLIB_THROW("OKLIB_TEST_EQUAL");
              v1.push_back(3);
              if (com(v1, v2) != std::tr1::make_tuple(v1.end(), v2.end(), 3U))
                OKLIB_THROW("OKLIB_TEST_EQUAL");
            }
          }
        };

      }
    }
  }
}

# undef OKLIB_FILE_ID
#endif

