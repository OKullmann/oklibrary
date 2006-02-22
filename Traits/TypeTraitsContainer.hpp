// Oliver Kullmann, 5.6.2005 (Swansea)

/*!
  \file TypeTraitsContainer.hpp
  \brief Extending the (general) type traits from Boost by container-specific
  traits.
*/

#ifndef TYPETRAITSCONTAINER_0099io

#define TYPETRAITSCONTAINER_0099io

#include <vector>
#include <list>
#include <deque>
#include <set>

#include <Transitional/Concepts/std_Container.hpp>
#include <Transitional/Concepts/ConceptsMetafunctions.hpp>

namespace OKlib {

  namespace MetaProgramming {

    template <typename T>
    struct is_std_container :  ::boost::mpl::and_<typename OKlib::Concepts::HasConceptTag<T>::type, typename OKlib::Concepts::IsTagModel<T, OKlib::Concepts::Container_tag>::type> {};
    template <typename T>
    struct is_std_container<std::vector<T> > :  ::boost::mpl::bool_<true> {};
    template <typename T>
    struct is_std_container<std::list<T> > :  ::boost::mpl::bool_<true> {};
    template <typename T>
    struct is_std_container<std::deque<T> > :  ::boost::mpl::bool_<true> {};
    template <typename T>
    struct is_std_container<std::set<T> > :  ::boost::mpl::bool_<true> {};
    template <typename T>
    struct is_std_container<std::multiset<T> > :  ::boost::mpl::bool_<true> {};

    template <typename T>
    struct has_size_function : is_std_container<T> {};

  }

}

#endif
