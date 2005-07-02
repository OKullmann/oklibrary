// Oliver Kullmann, 2.7.2005 (Swansea)

#ifndef ORDERCONSTRUCTIONS_jdjJn6

#define ORDERCONSTRUCTIONS_jdjJn6

#include <functional>

#include <boost/functional.hpp>

namespace OKlib {

  namespace OrderRelations {

    template <class LessThanRelation>
    struct SizeLessThan : std::binary_function<
      typename boost::binary_traits<LessThanRelation>::first_argument_type,
      typename boost::binary_traits<LessThanRelation>::second_argument_type,
      bool> {
      bool operator() (const typename boost::binary_traits<LessThanRelation>::first_argument_type& arg1, const typename boost::binary_traits<LessThanRelation>::second_argument_type& arg2) const {
        typedef typename boost::binary_traits<LessThanRelation>::first_argument_type first_argument_type;
        typedef typename boost::binary_traits<LessThanRelation>::second_argument_type second_argument_type;
        typedef typename first_argument_type::size_type first_size_type;
        typedef typename second_argument_type::size_type second_size_type;
        const first_size_type& size1(arg1.size());
        const second_size_type& size2(arg2.size());
        return size1 < size2 or (size1 == size2 and LessThanRelation()(arg1, arg2));
      }
    };

  }

}

#endif
