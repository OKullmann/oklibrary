// Oliver Kullmann, 25.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef CONCEPTSMETAFUNCTIONS_olNb4T
#define CONCEPTSMETAFUNCTIONS_olNb4T

/*!
  \file Concepts/ConceptsMetafunctions.hpp
  \brief Metafunctions concerning the relations between concepts, concept tags and models; used mainly internally.
*/

#include <tr1/type_traits>

#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/has_xxx.hpp>

#include <OKlib/Programming/MetaProgramming/Logical.hpp>

#include <OKlib/Concepts/ConceptsBase.hpp>
#include <OKlib/Concepts/traits/concept_tag.hpp>

namespace OKlib {
  namespace Concepts {

    /*!
      \class IsTagModel
      \brief Boolean metafunction: IsTagModel<T, tag> is true iff the concept tag of T is derived from (or equal to) tag.
    */

    template <class T, class concept_tag>
    class IsTagModel {
      typedef typename ::OKlib::Concepts::traits::concept_tag<T>::type t_concept_tag;
    public :
      typedef typename std::tr1::is_base_of<concept_tag, t_concept_tag>::type type;
      OKLIB_META_VALUE_T
    };

     // -------------------------------------------------------------------------------------------------------------------

    /*!
      \class IsConceptTag
      \brief Boolean metafunction: IsConceptTag<Tag> is true iff Tag is strictly derived from ::OKlib::Concepts::ConceptsBase_tag.
    */

    template <class Tag>
    struct IsConceptTag : ::boost::is_base_and_derived< ::OKlib::Concepts::ConceptsBase_tag, Tag> {};

    /*!
      \class HasConceptTag
      \brief Boolean metafunction: HasConceptTag<T> is true iff T has a nested type concept_tag which is a concept tag.
    */

    namespace implementation_has_concept_tag {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(type)
      template <bool has_concept_tag, typename T>
      struct is_concept_tag : ::boost::mpl::bool_<false> {};
      template <typename T>
      struct is_concept_tag<true, T> : IsConceptTag<typename ::OKlib::Concepts::traits::concept_tag<T>::type> {};
    }
    template <class T>
    struct HasConceptTag : implementation_has_concept_tag::is_concept_tag<implementation_has_concept_tag::has_type< ::OKlib::Concepts::traits::concept_tag<T> >::value, T> {};

  }

}

#endif
