// Oliver Kullmann, 11.2.2006 (Swansea)

/*!
  \file Levels.hpp
  \brief Design study for the level system
*/

#ifndef LEVELS_kxbxbxvr4

#define LEVELS_kxbxbxvr4

#include <Transitional/Messages/Languages.hpp>

namespace OKlib {

  namespace Messages {

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/size.hpp>

    /*!
      \def OKLIB_STRATA
      \brief The list of constants naming the available levels of expliciteness for messages

      OKLIB_STRATA is a sequence in the sense of the Boost Preprocessor library. If a new element
      is added (for a new level; at the end or in the middle -- but not at the start, since
      otherwise it would replace the default level), then in order to make it usable, a new partial
      specialisation of ::OKlib::Messages::S needs to be created (see below), and in
      ::OKlib::Messages::Documentation::LevelNames a new print-overload for this class
      has to be created as well as extending the switch-statements of the other print-overloads.
    */

#define OKLIB_STRATA (Basic) (Full) (Extensive)

    const unsigned int number_of_levels = BOOST_PP_SEQ_SIZE(OKLIB_STRATA);

    /*!
      \enum Strata
      \brief Constants of type int for the levels

      The first member is the default level Basic with value 0.
    */

    enum Strata { BOOST_PP_SEQ_ENUM(OKLIB_STRATA) };

    template <Strata level>
    struct S {
      static const Strata value = level;
    };

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
