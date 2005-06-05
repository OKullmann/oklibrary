// Oliver Kullmann, 5.6.2005 (Swansea)

#ifndef TRAITS_khBnBt6

#define TRAITS_khBnBt6

namespace OKlib {

  namespace Concepts {

    template <class T>
    struct concept_tag {
      typedef typename T::concept_tag type;
    };

    // -------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct index_type {
      typedef typename T::index_type type;
    };

  }

}

#endif
