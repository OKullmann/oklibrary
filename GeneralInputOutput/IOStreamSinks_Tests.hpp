// Oliver Kullmann, 8.1.2006 (Swansea)

/*!
  \file IOStreamSinks_Tests.hpp
  \brief Testing the output devices
*/

#ifndef IOSTREAMSINKSTESTS_uuYT54

#define IOSTREAMSINKSTESTS_uuYT54

#include <limits>

#include <Transitional/Testsystem/TestBaseClass.hpp>
#include <Transitional/Testsystem/TestExceptions.hpp>

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
