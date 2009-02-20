// Oliver Kullmann, 15.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/std_Iterators.hpp
  \brief Iterator concepts from ISO/IEC 14882: 2003 (plus const-correctness). As for all concepts 
  in "std_ ...", the concepts here are not refinements of OKlib::Concepts::BasicRequirements.

  Concepts according to ISO/IEC 14882: 2003, Section 24.1 and Section 24.3.1 PLUS const-correctness.
  Definition of the concepts
   - Concepts::InputIterator
   - Concepts::MultiPassInputIterator
   - Concepts::ForwardIterator.

  \todo Write concepts for
   - OutputIterator
   - BidirectionalIterator
   - RandomAccessIterator.
*/

#ifndef STDITERATORS_pOpt5185
#define STDITERATORS_pOpt5185

#include <iterator>

#include <boost/concept_check.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits.hpp>

#include <OKlib/Concepts/ConceptsBase.hpp>
#include <OKlib/Concepts/std_Basics.hpp>
#include <OKlib/Concepts/Basics.hpp>

namespace OKlib {
  namespace Concepts {

    /*!
      \class InputIterator
      \brief Concept InputIterator according to the standard.

      Refines concept Concepts::ConstructibleCAEq. Extensions can be found in
      Concepts/Iterators.hpp (Concepts::InputIteratorDefault, making it possible
      to use the past-the-end iterator).

      \todo "reference" and "pointer" are never explained in the standard, and the
      requirements on iterator concepts don't mention them, but they suddenly show
      up in the iterator traits (24.3.1) ?
      \todo Can expressions "a -> m" be expressed ?
      \todo Create in TypeTraits.hpp a metafunction "is_derived_or_same", and use it
      for the definition of InputIterator and elsewhere. Stop --- this is now in the
      new standard library.
    */

    template <typename Iterator>
    struct InputIterator {
      typedef typename std::iterator_traits<Iterator>::value_type value_type;
      typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
      BOOST_STATIC_ASSERT((::boost::mpl::or_< ::boost::is_same<std::input_iterator_tag, iterator_category>, ::boost::is_base_and_derived<std::input_iterator_tag, iterator_category> >::value));
      typedef typename std::iterator_traits<Iterator>::difference_type difference_type;

      typedef typename std::iterator_traits<Iterator>::reference reference;
      typedef typename std::iterator_traits<Iterator>::pointer pointer;
      void constraints() {
        boost::function_requires<ConstructibleCAEq<Iterator> >();
        dummy_use_v(static_cast<value_type>(*a)); // pre-condition: a is dereferencable
        static_cast<Iterator&>(++r); // pre-condition: r is dereferencable
        (void) r++; // pre-condition: r is dereferencable
        dummy_use_v(static_cast<value_type>(*r++)); // pre-condition: r is dereferencable
      }
      const Iterator a;
      Iterator& r;
      void dummy_use_v(const value_type& v) const {}
    };
    struct InputIterator_tag : virtual ConstructibleCAEq_tag {};

    class InputIterator_Archetype {
      InputIterator_Archetype();
      typedef InputIterator_Archetype self;
    protected :
      struct convertible_to_bool {
        operator bool() { return bool(); }
      };
    public:
      typedef std::input_iterator_tag iterator_category;
      struct value_type {};
      typedef std::ptrdiff_t difference_type;
      typedef void pointer; // ToDo: left unspecified in the standard ?
      struct reference {
        operator value_type() { return value_type(); }
      };
      self& operator=(const self&) { return *this; }
      convertible_to_bool operator==(const self&) const { return convertible_to_bool(); }
      convertible_to_bool operator!=(const self&) const { return convertible_to_bool(); }
      reference operator*() const { return reference(); }
      self& operator++() { return *this; }
      struct internal {
        value_type operator* () { return value_type(); }
      };
      internal operator++(int) { return internal(); }
    };

    // --------------------------------------------------------------------------------------------------------------------

     /*!
      \class MultiPassInputIterator
      \brief Concept MultiPassInputIterator according to Boost.

      According to [The Boost Graph Library, ISBN 0-201-72914-8], Section 16.3 (page 291).
      Extension of InputIterator; added here so that ForwardIterator can refine this concept.
    */

    template <typename Iterator>
    struct MultiPassInputIterator {
      void constraints() {
        boost::function_requires<InputIterator<Iterator> >();
        boost::function_requires<EqualitySubstitutable<Iterator> >();
      }
    };
    struct MultiPassInputIterator_tag : virtual InputIterator_tag, virtual EqualitySubstitutable_tag {};

    struct MultiPassInputIterator_Archetype : InputIterator_Archetype {};

    // --------------------------------------------------------------------------------------------------------------------

    /*!
      \class ForwardIterator
      \brief Concept ForwardIterator according to the standard.

      Refines concept Concepts::MultiPassInputIterator.
    */

    template <typename Iterator>
    struct ForwardIterator {
      typedef typename std::iterator_traits<Iterator>::value_type value_type;
      typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
      BOOST_STATIC_ASSERT((::boost::mpl::or_< ::boost::is_same<std::input_iterator_tag, iterator_category>, ::boost::is_base_and_derived<std::forward_iterator_tag, iterator_category> >::value));
      typedef typename std::iterator_traits<Iterator>::difference_type difference_type;

      typedef typename std::iterator_traits<Iterator>::reference reference;
      typedef typename std::iterator_traits<Iterator>::pointer pointer;
      void constraints() {
        boost::function_requires<MultiPassInputIterator<Iterator> >();
        boost::function_requires<DefaultConstructible<Iterator> >();
        dummy_use_v(static_cast<value_type&>(*a));
        static_cast<Iterator&>(++r);
        Iterator i(static_cast<const Iterator&>(r++)); dummy_use_i(i);
        static_cast<value_type&>(*r++);
      }
      const Iterator a;
      Iterator& r;
      void dummy_use_v(const value_type& v) const {}
      void dummy_use_i(const Iterator& i) const {}
    };
  struct ForwardIterator_tag : virtual MultiPassInputIterator_tag, virtual DefaultConstructible_tag {};

    class ForwardIterator_Archetype {
      typedef ForwardIterator_Archetype self;
    protected :
      struct convertible_to_bool {
        operator bool() { return false; }
      };
    public:
      typedef std::input_iterator_tag iterator_category;
      struct value_type {};
      typedef std::ptrdiff_t difference_type;
      typedef value_type* pointer;
    public :
      struct reference {
        operator value_type&() { return v; }
      private :
        value_type v;
      };
      self& operator=(const self&) { return *this; }
      convertible_to_bool operator==(const self&) const { return convertible_to_bool(); }
      convertible_to_bool operator!=(const self&) const { return convertible_to_bool(); }
      reference operator*() const { return reference(); }
      self& operator++() { return *this; }
      struct internal {
        value_type& operator*() { return v; }
        operator const self&() { return *this; }
      private :
        value_type v;
      };
      internal operator++(int) { return internal(); }
    };


  }

}

#endif

