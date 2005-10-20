// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef CONCEPTSMETAFUNCTIONSTESTS_719GbWq

#define CONCEPTSMETAFUNCTIONSTESTS_719GbWq

#include "ConceptsBase.hpp"

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

namespace OKlib {

  namespace Concepts {

    template <template <class T> class ConceptTag>
    class Test_ConceptTag_T {
      struct c {};
      struct test {
        typedef c concept_tag;
      };
      BOOST_STATIC_ASSERT((::boost::is_same<c, typename ConceptTag<test>::type>::value));
    };

    // --------------------------------------------------------------------------------------------------------------------------

    template <template <class T, class concept_tag> class IsTagModel>
    class Test_IsTagModel_T_concept_tag {
      struct basis {};
      struct test1 {
        typedef basis concept_tag;
      };
      struct derived : basis {};
      struct test2 {
        typedef derived concept_tag;
      };
      BOOST_STATIC_ASSERT((IsTagModel<test1, basis>::value));
      BOOST_STATIC_ASSERT((IsTagModel<test2, basis>::value));
      BOOST_STATIC_ASSERT((IsTagModel<test2, derived>::value));
      BOOST_STATIC_ASSERT(not (IsTagModel<test1, derived>::value));
    };

    // --------------------------------------------------------------------------------------------------------------------------

    template <template <class Tag> class IsConceptTag>
    class Test_IsConceptTag_Tag {
      struct t1 {};
      struct t2 : virtual ::OKlib::Concepts::ConceptsBase_tag {};
      BOOST_STATIC_ASSERT(not IsConceptTag<t1>::value);
      BOOST_STATIC_ASSERT(not IsConceptTag< ::OKlib::Concepts::ConceptsBase_tag >::value);
      BOOST_STATIC_ASSERT(IsConceptTag<t2>::value);
    };

    // --------------------------------------------------------------------------------------------------------------------------


    template <template <class T> class HasConceptTag>
    class Test_HasConceptTag_T {
      struct c1 {};
      struct c2 : virtual ::OKlib::Concepts::ConceptsBase_tag {};
      struct test1 {
        typedef c1 concept_tag;
      };
      struct test2 {
        typedef c2 concept_tag;
      };
      struct test3 {
        typedef ::OKlib::Concepts::ConceptsBase_tag concept_tag;
      };
      BOOST_STATIC_ASSERT(not HasConceptTag<c1>::value);
      BOOST_STATIC_ASSERT(not HasConceptTag<c2>::value);
      BOOST_STATIC_ASSERT(HasConceptTag<test2>::value);
      BOOST_STATIC_ASSERT(not HasConceptTag<test1>::value);
      BOOST_STATIC_ASSERT(not HasConceptTag<test3>::value);
    };

  }

}

#endif
