// Oliver Kullmann, 9.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/JoinedConceptTags.hpp
  \brief Concept tags for (fundamental) types in case the type models several concepts

  The problem is, that in our library every model M of a concept C
  needs a concept tag (accessed by
  OKlib::Concepts::traits::concept_tag<M>::type
  ) which is derived from OKlib::Concepts::C_tag. Now for example the model
  M1 := OKlib::Variables::Variables_int of concept Concepts::Variables and
  the model M2 := OKlib::Literals::Literals_int of concept Concepts::Literals
  are both just typedefs for the fundamental type int (one of the central
  design decisions is, for such fundamental models to support also built-in
  types), and so OKlib::Concepts::traits::concept_tag<M> cannot distinguish
  between the both. The solution is to provide a tag C1C2 with expresses
  both concepts (is derived from both concept tags). In order to avoid
  circular include-statements, the definition of C1C2 cannot be put
  into the the concept definition files for C1 or C2 (where normally
  the accompanying tags are found), but is to be defined in this file.
*/

#ifndef JOINEDCONCEPTTAGS_74TTba14kfbr
#define JOINEDCONCEPTTAGS_74TTba14kfbr

#include <OKlib/Concepts/Variables.hpp>
#include <OKlib/Concepts/Literals.hpp>

namespace OKlib {
  namespace Concepts {

    /*!
      \class VariablesLiterals_tag
      \brief Tags for types which are used as variables and as literals

      For examples int models Concepts::Variables as well as
      Concepts::Literals; more precisely, int models
      Concepts::VariablesAsIndex. The tag expressing that the type
      models both concepts now is
      VariablesLiterals_tag<Concepts::VariablesAsIndex_tag, Concepts::Literals_tag>.

      \todo It seems besides providing such a combined tag, we need also
      to provide "projections", which enable us to extract from such a
      combined tag the Var-tag and the Lit-tag? Or perhaps not, since
      all what is needed from the combined tag are the base classes
      (for tagging-polymorphism)?
    */

    template <class Var_tag, class Lit_tag>
    struct VariablesLiterals_tag : virtual Var_tag, virtual Lit_tag {
      OKLIB_MODELS_CONCEPT_TAG_T(Var_tag, OKlib::Concepts::Variables)
      OKLIB_MODELS_CONCEPT_TAG_T(Lit_tag, OKlib::Concepts::Literals)
    };

  }
}

#endif
