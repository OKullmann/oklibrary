// Oliver Kullmann, 2.7.2005 (Swansea)

#ifndef SUBSUMPTION_yhgBBv567

#define SUBSUMPTION_yhgBBv567

#include <algorithm>
#include <iterator>
#include <cassert>
#include <set>

#include <boost/utility.hpp>
#include <boost/iterator/reverse_iterator.hpp>

#include "TaggingPolymorphism.hpp"

namespace OKlib {

  namespace SetAlgorithms {

    struct size_tag : OKlib::MetaProgramming::property_tag {};
    struct use_size : size_tag {};
    struct do_not_use_size : size_tag {};

    struct uniqueness_tag : OKlib::MetaProgramming::property_tag {};
    struct hyperedges_are_unique : uniqueness_tag {};
    struct hyperedges_may_not_be_unique : uniqueness_tag {};

    struct order_tag : OKlib::MetaProgramming::property_tag {};
    struct process_only_forward_ : order_tag {};
    struct process_only_backward : order_tag {};
    struct process_both_directions : order_tag {};

    struct sorting_tag : OKlib::MetaProgramming::property_tag {};
    struct do_sorting : sorting_tag {};

    // Helper constructions

    template <typename Iterator>
    struct Get_underlying_iterator {
      typedef Iterator iterator;
      const Iterator& operator() (const Iterator& i) {
        return i;
      }
    };
    template <typename Iterator>
    struct Get_underlying_iterator<boost::reverse_iterator<Iterator> > {
      typedef Iterator iterator;
      typedef boost::reverse_iterator<Iterator> reverse_iterator;
      Iterator operator() (const reverse_iterator& i) const {
        return boost::prior(i.base());
      }
    };
    template <typename Iterator>
    struct Get_underlying_iterator<std::reverse_iterator<Iterator> > {
      typedef Iterator iterator;
      typedef std::reverse_iterator<Iterator> reverse_iterator;
      Iterator operator() (const reverse_iterator& i) const {
        return boost::prior(i.base());
      }
    };

    // ---------------------------


    template <class Container>
    struct Erase {
      typedef typename Container::iterator iterator;
      iterator operator() (Container& C, const iterator& i) const {
        return C.erase(i);
      }
    };
    template <typename T>
    struct Erase<std::set<T> > {
      typedef std::set<T> Container;
      typedef typename Container::iterator iterator;
      iterator operator() (Container& C, const iterator& i) const {
        iterator j(i); ++j;
        C.erase(i);
        return j;
      }
    };
    template <typename T>
    struct Erase<std::multiset<T> > {
      typedef std::multiset<T> Container;
      typedef typename Container::iterator iterator;
      iterator operator() (Container& C, const iterator& i) const {
        iterator j(i); ++j;
        C.erase(i);
        return j;
      }
    };


    // ToDo: These functors should move to the iterator tools ?! (On the other hand they
    // should be adaptable to the *special* situation at hand.)


    // -------------------------------------------------------------------------------------------------------------------
      
    /*!
      \class Subsumption_elimination<ContainerSets>
      \brief Functor: Eliminates all inclusions from a container.
    */

    template <class ContainerSets,
              class SizeTag = size_tag,
              class UniquenessTag = hyperedges_may_not_be_unique,
              class OrderTag = process_both_directions,
              class SortingTag = sorting_tag>
    // ToDo: Concepts etc.
    // ContainerSets supports erase without invalidating iterators.
    struct Subsumption_elimination {

      template <typename Iterator>
      void upward(ContainerSets& C, Iterator begin, const Iterator end) const {
        for (; begin != end; ++begin)
          for (Iterator j(boost::next(begin)); j != end;)
            if (std::includes(j -> begin(), j -> end(), begin -> begin(), begin -> end()))
              j = Iterator(Erase<ContainerSets>()(C, Get_underlying_iterator<Iterator>()(j)));
            else
              ++j;
      }

      void operator() (ContainerSets& C) const {
        typedef typename ContainerSets::iterator iterator;
        const iterator& end(C.end());
        upward(C, C.begin(), end);
        upward(C, boost::make_reverse_iterator(end), boost::make_reverse_iterator(C.begin()));
      }

      void if_sorted(ContainerSets& C) const {
        // Precondition:
        // The elements of the container are sorted by increasing size, and there are no duplicates.
        typedef typename ContainerSets::iterator iterator;
        const iterator& end(C.end());
        for (iterator begin(C.begin()); begin != end; ++begin) {
          typedef typename ContainerSets::size_type size_type;
          const size_type& size(begin -> size());
          for (iterator j(boost::next(begin)); j != end;) {
            assert(j -> size() >= size);
            if (j -> size() == size)
              ++j;
            else if (std::includes(j -> begin(), j -> end(), begin -> begin(), begin -> end()))
              j = Erase<ContainerSets>()(C, j);
            else
              ++j;
          }
        }
      }

    };
    
    template <class ContainerSets>
    inline void subsumption_elimination_upward(ContainerSets& C) {
      Subsumption_elimination<ContainerSets>().upward(C, C.begin(), C.end());
    }

    template <class ContainerSets>
    inline void subsumption_elimination(ContainerSets& C) {
      Subsumption_elimination<ContainerSets>()(C);
    }

    template <class ContainerSets>
    inline void subsumption_elimination_if_sorted(ContainerSets& C) {
      Subsumption_elimination<ContainerSets>().if_sorted(C);
    }

  }

}

#endif
