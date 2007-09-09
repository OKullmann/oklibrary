// Oliver Kullmann, 8.9.2007 (Swansea)

/*!
  \file Sequences/tests/Comparisons.hpp
  \brief Generic tests for algorithms comparing sequences

  \todo How to get a kind of version number like CVS's??
  For the time being, we edit these entries by hand.

*/

#ifndef COMPARISONS_nnxcad5410o
#define COMPARISONS_nnxcad5410o

#include <vector>

#include <Transitional/TestSystem/TestBaseClass_DesignStudy.hpp>

namespace OKlib {
  namespace Programming {
    namespace Sequences {
      namespace tests {

        /*!
          \class CommonPart
          \brief Testing functors for computing longest common initial sequences
        */

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 8.9.2007 23:20:00 $", "$Revision: 1 $")

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
              if (com(v1, v2) != v1.begin())
                OKLIB_THROW("OKLIB_TEST_EQUAL");
              v1.push_back(1);
              if (com(v1, v2) != v1.begin())
                OKLIB_THROW("OKLIB_TEST_EQUAL");
              v2.push_back(1);
              if (com(v1, v2) != v1.end())
                OKLIB_THROW("OKLIB_TEST_EQUAL");
               v2.push_back(2);
              if (com(v1, v2) != v1.end())
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

