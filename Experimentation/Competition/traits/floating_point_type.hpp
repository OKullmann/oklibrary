// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file Competition/traits/floating_point_type.hpp
  \brief Traits-metafunctions to extract the floating point type.
*/

#ifndef FLOATINGPOINTTYPE_lalHe3
#define FLOATING_POINTTYPE_lalHe3

namespace OKlib {
  namespace SATCompetition {
    namespace traits {

      /*!
        \class floating_point_type
        \brief Traits meta-function to extract the floating_point type.
      */

      template <typename T>
      struct floating_point_type {
        typedef typename T::floating_point_type type;
      };

    }

  }

}

#endif
