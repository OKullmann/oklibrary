// Oliver Kullmann, 23.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Refactoring/BoostPathCorrected.hpp
  \brief Adding missing functionality to Boost::filesystem::path.
  
  Making Boost::filesystem::path a model of a range.

  \todo What kind of range exactly?
  \todo Notifying the boost e-mail list about the missing functionality.
  \todo Should go to module GeneralInputOutput.
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
  struct range_const_iterator<const boost::filesystem::path> {
    typedef boost::filesystem::path::iterator type;
  };
  template <>
  struct range_size<boost::filesystem::path> {
    typedef boost::filesystem::path::iterator::difference_type type;
  };
  template <>
  struct range_size<const boost::filesystem::path> {
    typedef boost::filesystem::path::iterator::difference_type type;
  };


  namespace filesystem {

    inline boost::range_size<boost::filesystem::path>::type boost_range_size(const boost::filesystem::path& p) {
      return std::distance(p.begin(),p.end());
    }

  }

}

#endif
