// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file natural_number_type.hpp
  \brief Traits-metafunctions to extract the natural_number type.
  \todo This should go to OKlib/Variables/Traits
*/

#ifndef NATURALNUMBERTYPE_bzbXe45

#define NATURALNUMBERTYPE_bzbXe45

namespace OKlib {

  namespace Concepts {

    /*!
      \class natural_number_type
      \brief Traits meta-function to extract the natural-number type.
    */

    template <typename T>
    struct natural_number_type {
      typedef typename T::natural_number_type type;
    };

  }

}

#endif
