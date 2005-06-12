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

    // -------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct string_type {
      typedef typename T::string_type type;
    };

    // -------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct floating_point_type {
      typedef typename T::floating_point_type type;
    };

    // -------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct natural_number_type {
      typedef typename T::natural_number_type type;
    };

    // -------------------------------------------------------------------------------------------------------------------

  }

}

#endif
