// Oliver Kullmann, 23.12.2005 (Swansea)

/*!
  \file BoostPathCorrected.hpp
  \brief Adding missing functionality to Boost::filesystem::path.
  
  Making Boost::filesystem::path a model of a range.
  \todo What kind of range exactly?
  \todo Notifying the boost e-mail list about the missing functionality.
  \todo Should go to module GeneralInputOutput (?).
*/

#ifndef BOOSTPATHCORRECTED_004ttGB

#define BOOSTPATHCORRECTED_004ttGB

#include <boost/filesystem/path.hpp>

#include <boost/range/const_iterator.hpp> // Fix for erroneous Boost library filesystem ##################################################################
#include <boost/range/size_type.hpp> // Fix for erroneous Boost library filesystem ##################################################################


// Fix for erroneous Boost library filesystem ##########################################################################

namespace boost {

  template <>
  struct range_const_iterator<boost::filesystem::path> {
    typedef boost::filesystem::path::iterator type;
  };
  template <>
  struct range_size<boost::filesystem::path> {
    typedef boost::filesystem::path::iterator::difference_type type;
  };

  namespace filesystem {

    inline boost::range_size<boost::filesystem::path>::type boost_range_size(const boost::filesystem::path& p) {
      return std::distance(p.begin(),p.end());
    }

  }

}

#endif
