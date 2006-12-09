// Oliver Kullmann, 8.12.2006 (Swansea)

/*!
  \file Concepts/tests/Literals.hpp
  \brief Generic tests for models of concept Literals
*/

#ifndef LITERALSTESTS_jjgdgtet4TyTr
#define LITERALSTESTS_jjgdgtet4TyTr

#include <Transitional/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <Transitional/Concepts/LibraryBasics.hpp>

#include <Transitional/Concepts/Literals.hpp>

namespace OKlib {
  namespace Concepts {
    namespace tests {
      
      /*!
        \class Literals_basic
        \brief Basic tests for models of concept Literals

        \todo Create basic semantic tests.
      */

      template <class Lit>
      class Literals_basic : public ::OKlib::TestSystem::TestBase {
        typedef ::OKlib::TestSystem::TestBase base_type;
      public :
        Literals_basic() : base_type(__FILE__, __LINE__, typeid(Literals_basic).name()) {}
      private :
        void test(::OKlib::TestSystem::Basic) {
          OKLIB_MODELS_CONCEPT_REQUIRES(Lit, Literals);
          OKLIB_MODELS_CONCEPT_TAG(Lit, Literals);
        }
      };

    }
  }
}

#endif
