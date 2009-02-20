// Oliver Kullmann, 25.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef CONCEPTSMETAFUNCTIONSTESTOBJECTS_hnBBx36
#define CONCEPTSMETAFUNCTIONSTESTOBJECTS_hnBBx36

#include <OKlib/Concepts/traits/concept_tag.hpp>

#include <OKlib/Concepts/ConceptsMetafunctions.hpp>
#include <OKlib/Concepts/ConceptsMetafunctions_Tests.hpp>

namespace OKlib {

  namespace Concepts {

      template class Test_ConceptTag_T< ::OKlib::Concepts::traits::concept_tag>;

      template class Test_IsTagModel_T_concept_tag<IsTagModel>;

      template class Test_IsConceptTag_Tag<IsConceptTag>;

      template class Test_HasConceptTag_T<HasConceptTag>;

  }

}

#endif
