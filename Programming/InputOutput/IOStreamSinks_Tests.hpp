// Oliver Kullmann, 8.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/IOStreamSinks_Tests.hpp
  \brief Testing the output devices
*/

#ifndef IOSTREAMSINKSTESTS_uuYT54
#define IOSTREAMSINKSTESTS_uuYT54

#include <limits>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace GeneralInputOutput {

    /*!
      \class Test_NullStream
      \brief Test an output stream discarding all characters.
    */

    template <class NullStream>
    class Test_NullStream : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_NullStream test_type;
      Test_NullStream() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        NullStream out;
        if (not out)
          OKLIB_THROW("out not valid");
        {
          const unsigned int iterations = 100;
          for (unsigned int i = 0; i != iterations; ++i) {
            out << i;
            if (not out)
              OKLIB_THROW("out not valid");
          }
        }
        {
          typedef char char_type;
          const char_type min_char = std::numeric_limits<char_type>::min();
          const char_type max_char = std::numeric_limits<char_type>::max();
          for (char_type c(min_char);;) {
            out << c;
            if (not out)
              OKLIB_THROW("out not valid");
            if (c < max_char)
              ++c;
            else
              break;
          }
        }
      }
    };

  }
}

#endif
