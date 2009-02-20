// Oliver Kullmann, 5.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef TYPETRAITSTESTOBJECTS_78UiJh0
#define TYPETRAITSTESTOBJECTS_78UiJh0

#include <OKlib/traits/TypeTraits.hpp>
#include <OKlib/traits/TypeTraits_Tests.hpp>

namespace OKlib {

  namespace traits {

    template class Test_is_unqualified_signed_integral_T<is_unqualified_signed_integral>;

    template class Test_is_unqualified_unsigned_integral_T<is_unqualified_unsigned_integral>;

    template class Test_is_unqualified_signedunsigned_integral_T<is_unqualified_signedunsigned_integral>;

  }

}

#endif
