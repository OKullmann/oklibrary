// Oliver Kullmann, 11.2.2006 (Swansea)

/*!
  \file Levels_DesignStudy.hpp
  \brief Design study for the level system
  \todo Write a message object which explains the levels (using a member function
  for setting the level).
*/

#ifndef LEVELSDESIGNSTUDY_kxbxbxvr4

#define LEVELSDESIGNSTUDY_kxbxbxvr4

#include "Languages_DesignStudy.hpp"

namespace OKlib {

  namespace Messages {

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/size.hpp>

    /*!
      \def OKLIB_STRATA
      \brief The list of constants naming the available levels of expliciteness for messages

      OKLIB_STRATA is a sequence in the sense of the Boost Preprocessor library.
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

    // these should become messages:

      inline const char* translate(S<Basic>, ::OKlib::Messages::L< ::OKlib::Messages::en_GB>) { return "basic level (short, but still complete)"; }
      inline const char* translate(S<Full>, ::OKlib::Messages::L< ::OKlib::Messages::en_GB>) { return "full level (explicitely stating everything relevant)"; }
      inline const char* translate(S<Extensive>, ::OKlib::Messages::L< ::OKlib::Messages::en_GB>) { return "extensive level (adding to the full level examples and further explanations, considering also more special cases)"; }

      inline const char* translate(S<Basic>, ::OKlib::Messages::L< ::OKlib::Messages::de_DE>) { return "Basisniveau (kurz und b\xFC" "ndig)"; }
      inline const char* translate(S<Full>, ::OKlib::Messages::L< ::OKlib::Messages::de_DE>) { return "Normalniveau (Vollst\xE4" "ndigkeit angestrebt)"; }
      inline const char* translate(S<Extensive>, ::OKlib::Messages::L< ::OKlib::Messages::de_DE>) { return "Ausf\xFC" "hrliches Niveau (auch mit Beispielen und weiteren Erkl\xE4" "rungen)"; }

  }

}

#endif
