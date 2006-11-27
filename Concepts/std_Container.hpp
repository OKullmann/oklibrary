// Oliver Kullmann, 3.7.2005 (Swansea)

/*!
  \file Concepts/std_Container.hpp
  \brief Container concepts from ISO/IEC 14882: 2003 (plus const-correctness).

  Basic concepts according to ISO/IEC 14882: 2003 (referred to as the "Standard"),
  Section 23.1 PLUS const-correctness.

*/

#ifndef STDCONTAINER_776YhBq
#define STDCONTAINER_776YhBq

#include <iterator>

#include <boost/concept_check.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#include <Transitional/Concepts/ConceptsBase.hpp>
#include <Transitional/Concepts/std_Basics.hpp>
#include <Transitional/Concepts/std_Iterators.hpp>
#include <Transitional/Concepts/Basics.hpp>
#include <Transitional/traits/TypeTraits.hpp>

namespace OKlib {

  namespace Concepts {

    /*!
      \class Container
      \brief Concept Container according to the standard.
      \todo Write a weakening of concept Container which does not require the value type
      to be assignable (so that std::map and std::multimap are models of this concept).
    */

    // According to Table 65 in the standard
    template <typename C>
    struct Container {
      typedef typename C::value_type value_type;
      typedef typename C::reference reference;
      typedef typename C::const_reference const_reference;
      typedef typename C::iterator iterator;
      typedef typename C::const_iterator const_iterator;
      typedef typename C::difference_type difference_type;
      typedef typename C::size_type size_type;

      typedef typename std::iterator_traits<iterator>::value_type iterator_value_type;
      typedef typename std::iterator_traits<const_iterator>::value_type const_iterator_value_type;
      typedef typename std::iterator_traits<iterator>::difference_type iterator_difference_type;
      typedef typename std::iterator_traits<const_iterator>::difference_type const_iterator_difference_type;

      void constraints() {
        boost::function_requires<FullyConstructible<C> >();
        boost::function_requires<LinearOrder<C> >();
        boost::function_requires<Assignable<value_type> >();
        boost::function_requires<CopyConstructible<value_type> >();
        boost::function_requires<Destructible<value_type> >();
        boost::function_requires<InputIterator<iterator> >();
        boost::function_requires<InputIterator<const_iterator> >();

        // unclear the meaning of reference ("lvalue of value_type") and const_reference ("const lvalue of value_type")
        BOOST_STATIC_ASSERT((boost::is_convertible<reference, value_type&>::value));
        BOOST_STATIC_ASSERT((boost::is_convertible<value_type&, reference>::value));
        BOOST_STATIC_ASSERT((boost::is_convertible<const_reference, const value_type&>::value));
        BOOST_STATIC_ASSERT((boost::is_convertible<const value_type&, const_reference>::value));

        BOOST_STATIC_ASSERT((boost::is_convertible<iterator, const_iterator>::value));

        // unclear the meaning of "iterator type pointing to value_type" resp. "iterator type pointing to const value_type
        BOOST_STATIC_ASSERT((boost::is_convertible<iterator_value_type, value_type>::value));
        BOOST_STATIC_ASSERT((boost::is_convertible<value_type, iterator_value_type>::value));
        BOOST_STATIC_ASSERT((boost::is_convertible<const_iterator_value_type, const value_type>::value));
        BOOST_STATIC_ASSERT((boost::is_convertible<value_type, const_iterator_value_type>::value));

        BOOST_STATIC_ASSERT((boost::is_same<difference_type, iterator_difference_type>::value));
        BOOST_STATIC_ASSERT((boost::is_same<difference_type, const_iterator_difference_type>::value));
        BOOST_STATIC_ASSERT(OKlib::traits::is_signed_integral<difference_type>::value);

        BOOST_STATIC_ASSERT(OKlib::traits::is_unsigned_integral<size_type>::value);

        iterator i(static_cast<iterator>(x.begin()));
        i = static_cast<iterator>(x.end());
        const_iterator ic(static_cast<const_iterator>(a.begin()));
        ic = static_cast<const_iterator>(a.end());

        static_cast<void>(x.swap(y));

        size_type s;
        s = static_cast<size_type>(x.size());
        s = static_cast<size_type>(a.size());
        s = static_cast<size_type>(x.max_size());
        s = static_cast<size_type>(a.max_size());

        bool b;
        b = static_cast<bool>(x.empty());
        b = static_cast<bool>(a.empty());
      }
      C x, y;
      const C a;
    };

    struct Container_tag : virtual FullyConstructible_tag, virtual LinearOrder_tag, virtual Assignable_tag {};

    class Container_Archetype {
    public :

      class value_type {
        value_type();
      };
    private :
      static value_type v;

    public :

      // ---------------------------------

      class reference {
        reference();
        reference& operator=(const reference&);
      public :
        reference(value_type&) {}
        operator value_type&() { return v; }
      };

      // ---------------------------------

      class const_reference {
        const_reference();
        const_reference& operator=(const const_reference&);
      public :
        const_reference(const value_type&) {}
        operator const value_type&() { return v; }
      };

      // ---------------------------------

      class iterator {
        iterator();
        typedef iterator self;
        struct convertible_to_bool {
          operator bool() { return bool(); }
        };
      public:
        typedef std::input_iterator_tag iterator_category;
        struct value_type {
          value_type(const Container_Archetype::value_type&) {}
          operator Container_Archetype::value_type() { return v; }
        };
      private :
        static value_type vi;
      public :
        typedef std::ptrdiff_t difference_type;
        typedef void pointer; // ToDo: left unspecified in the standard ?
        struct reference {
          operator value_type() { return vi; }
        };
        self& operator=(const self&) { return *this; }
        convertible_to_bool operator==(const self&) const { return convertible_to_bool(); }
        convertible_to_bool operator!=(const self&) const { return convertible_to_bool(); }
        reference operator*() const { return reference(); }
        self& operator++() { return *this; }
        struct internal {
          value_type operator* () { return vi; }
        };
        internal operator++(int) { return internal(); }
      };

      // ---------------------------------

      class const_iterator {
        const_iterator();
        typedef const_iterator self;
        struct convertible_to_bool {
          operator bool() { return bool(); }
        };
      public:
        const_iterator(const iterator&) {}
        typedef std::input_iterator_tag iterator_category;
        struct value_type {
          value_type(const Container_Archetype::value_type&) {}
          operator const Container_Archetype::value_type() { return v; }
        };
      private :
        static value_type vi;
      public :
        typedef std::ptrdiff_t difference_type;
        typedef void pointer; // ToDo: left unspecified in the standard ?
        struct reference {
          operator value_type() { return vi; }
        };

        self& operator=(const self&) { return *this; }
        convertible_to_bool operator==(const self&) const { return convertible_to_bool(); }
        convertible_to_bool operator!=(const self&) const { return convertible_to_bool(); }
        reference operator*() const { return reference(); }
        self& operator++() { return *this; }
        struct internal {
          value_type operator* () { return vi; }
        };
        internal operator++(int) { return internal(); }
      };

      // ---------------------------------

      typedef std::ptrdiff_t difference_type;
      typedef unsigned int size_type;

    private :
      static iterator i;
      static const_iterator ci;
    public :
      iterator begin() { return i; }
      iterator end() { return i; }
      const_iterator begin() const { return ci; }
      const_iterator end() const { return ci; }
      void swap(Container_Archetype&) {}
      size_type size() const { return size_type(); }
      size_type max_size() const { return size_type(); }

    private :
      struct convertible_to_bool {
        operator bool() { return bool(); }
      };

    public :

      convertible_to_bool empty() const { return convertible_to_bool(); }

      convertible_to_bool operator ==(const Container_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator !=(const Container_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <(const Container_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >(const Container_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <=(const Container_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >=(const Container_Archetype&) const { return convertible_to_bool(); }

    };

    // ----------------------------------------------------------------------------------------------------------------------------------

  }

}

#endif
