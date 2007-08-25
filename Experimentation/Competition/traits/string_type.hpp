// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file Competition/traits/string_type.hpp
  \brief Traits-metafunctions to extract the string type.
*/

#ifndef STRINGTYPE_jjdggtRE
#define STRINGTYPE_jjdggtRE

namespace OKlib {
  namespace SATCompetition {
    namespace traits {

      /*!
        \class string_type
        \brief Traits meta-function to extract the string type.
      */

      template <typename T>
      struct string_type {
        typedef typename T::string_type type;
      };

    }

  }

}

#endif
