// Oliver Kullmann, 5.6.2005 (Swansea)

#ifndef TYPETRAITSTESTOBJECTS_78UiJh0

#define TYPETRAITSTESTOBJECTS_78UiJh0

#include <Transitional/Traits/TypeTraits.hpp>
#include <Transitional/Traits/TypeTraits_Tests.hpp>

namespace OKlib {

  namespace MetaProgramming {

    template class Test_is_unqualified_signed_integral_T<is_unqualified_signed_integral>;

    template class Test_is_unqualified_unsigned_integral_T<is_unqualified_unsigned_integral>;

    template class Test_is_unqualified_signedunsigned_integral_T<is_unqualified_signedunsigned_integral>;

  }

}

#endif
