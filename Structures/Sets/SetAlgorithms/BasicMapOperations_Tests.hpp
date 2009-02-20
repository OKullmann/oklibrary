// Oliver Kullmann, 4.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef BASICMAPOPERATIONSTESTS_kJnNbv91
#define BASICMAPOPERATIONSTESTS_kJnNbv91

#include <map>
#include <string>

#include <boost/lexical_cast.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace SetAlgorithms {

    template <template <class Map> class MapValue>
    class Test_MapValue : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_MapValue test_type;
      Test_MapValue() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef std::map<int, int> map_type;
          typedef MapValue<map_type> evaluation_type;
          map_type m;
          m[1] = 2; m[5] = 17;
          const map_type mc(m);
          evaluation_type mc_eval(mc);
          OKLIB_TEST_EQUAL(mc_eval(1), 2);
          OKLIB_TEST_EQUAL(mc_eval(5), 17);
        }
      }

    };

  }

}

#endif
