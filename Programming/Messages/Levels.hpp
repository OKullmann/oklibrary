// Oliver Kullmann, 11.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/Levels.hpp
  \brief Definition of the level system (as used by messages)
*/

#ifndef LEVELS_kxbxbxvr4
#define LEVELS_kxbxbxvr4

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/size.hpp>

#include <OKlib/Programming/Messages/Languages.hpp>

namespace OKlib {
  namespace Messages {

    /*!
      \def OKLIB_STRATA
      \brief The list of constants naming the available levels of expliciteness for messages

      OKLIB_STRATA is a sequence in the sense of the Boost Preprocessor library. If a new element
      is added (for a new level; at the end or in the middle -- but not at the start, since
      otherwise it would replace the default level), then in order to make it usable, a new partial
      specialisation of ::OKlib::Messages::S needs to be created (see below), and in
      ::OKlib::Messages::Documentation::LevelNames a new print-overload for this class
      has to be created as well as extending the switch-statements of the other print-overloads.

      The standard consists of the levels Basic, Full, and Extensive.

      Associated message services one finds in Messages/messages/Levels.hpp.
    */

#define OKLIB_STRATA (Basic) (Full) (Extensive)

    const unsigned int number_of_levels = BOOST_PP_SEQ_SIZE(OKLIB_STRATA);

    /*!
      \enum Strata
      \brief Constants of type int for the levels

      Enumerated type, whose elements are named according to OKLIB_STRATA.
      The first member is the default level Basic with value 0.
    */

    enum Strata { BOOST_PP_SEQ_ENUM(OKLIB_STRATA) };

    /*!
      \class S
      \brief S<level>, where level is an element of Strata, is a tagging class representing
      this level.

      Specialisations of S<level> stand in derivation relations to each other, where the
      immediate base class is the fall-back level used in case S<level> is not implemented
      (for some message).
    */

    template <Strata level>
    struct S {
      static const Strata value = level;
    };

    template struct S<Basic>;

    template <>
    struct S<Full> : S<Basic> {
      static const Strata value = Full;
    };
    template <>
    struct S<Extensive> : S<Full> {
      static const Strata value = Extensive;
    };

  }

}

#endif
