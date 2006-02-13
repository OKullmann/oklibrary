// Oliver Kullmann, 10.2.2006 (Swansea)

/*!
  \file Languages_DesignStudy.hpp
  \brief Design study for the language system
  \todo Write a message object which translates language names (using a member function
  which sets the language).
  \todo How to get Umlaute ?! It seems:
   - ae = 228 (xE4), Ae = 196 (xC4)
   - oe = 246 (xF6), Oe = 214 (xD6)
   - ue = 252 (xFC), Ue = 220 (xDC)
   - ss = 223 (xDF)
   \todo fr_CA disabled due to an error in g++ (3.4.3 - 4.0.2).
*/

#ifndef LANGUAGESDESIGNSTUDY_8UyTre

#define LANGUAGESDESIGNSTUDY_8UyTre

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/size.hpp>

namespace OKlib {

  namespace Messages {

    /*!
      \def OKLIB_LANGUAGES
      \brief The list of constants naming the available languages; the names are also used to name
      the corresponding locale.

      OKLIB_LANGUAGES is a sequence in the sense of the Boost Preprocessor library.
    */

#define OKLIB_LANGUAGES (en_GB) (en_US) (en_CA) (en_AU) (fr_FR) (fr_CH) (de_DE) (de_AT) (de_CH)

    /*!
      \brief Number of available languages

      The indices of the available languages are 0, ..., number_of_languages-1.
    */

    const unsigned int number_of_languages = BOOST_PP_SEQ_SIZE(OKLIB_LANGUAGES);

    /*!
      \enum Languages
      \brief Constants of type int for all languages

      The first member is the default language en_GB with value 0.
    */

    enum Languages { BOOST_PP_SEQ_ENUM(OKLIB_LANGUAGES) };

#define OKLIB_QUOTE(r, data, elem) (BOOST_PP_STRINGIZE(elem))
#define OKLIB_QUOTED_SEQUENCE BOOST_PP_SEQ_FOR_EACH(OKLIB_QUOTE, , OKLIB_LANGUAGES)

    /*!
      \brief Array of c-strings, mapping language (indices) to names of locales
    */

    const char* const Locales[] = { BOOST_PP_SEQ_ENUM(OKLIB_QUOTED_SEQUENCE) };

#undef OKLIB_QUOTED_SEQUENCE
#undef OKLIB_QUOTE

    /*!
      \class L
      \brief L<language>, where language is an element of Languages, is a tagging class
      representing this language.
    */

    template <Languages language>
    struct L {
      static const Languages value = language;
    };

    template <>
    struct L<en_US> : L<en_GB> {
      static const Languages value = en_US;
    };
    template <>
    struct L<en_CA> : L<en_GB> {
      static const Languages value = en_CA;
    };
    template <>
    struct L<en_AU> : L<en_GB> {
      static const Languages value = en_AU;
    };

    template <>
    struct L<fr_FR> : L<en_GB> {
      static const Languages value = fr_FR;
    };
    template <>
    struct L<fr_CH> : L<fr_FR> {
      static const Languages value = fr_CH;
    };
//     template <>
//     struct L<fr_CA> : L<fr_FR> {
//       static const Languages value = fr_CA;
//     };

    template <>
    struct L<de_DE> : L<en_GB> {
      static const Languages value = de_DE;
    };
    template <>
    struct L<de_AT> : L<de_DE> {
      static const Languages value = de_AT;
    };
    template <>
    struct L<de_CH> : L<de_DE> {
      static const Languages value = de_CH;
    };

    // these should become messages :

    inline const char* translate(L<en_GB>, L<en_GB>) { return "English"; }
    inline const char* translate(L<en_GB>, L<fr_FR>) { return "Anglais"; }
    inline const char* translate(L<en_GB>, L<de_DE>) { return "Englisch"; }

    inline const char* translate(L<fr_FR>, L<en_GB>) { return "French"; }
    inline const char* translate(L<fr_FR>, L<fr_FR>) { return "Francais"; }
    inline const char* translate(L<fr_FR>, L<de_DE>) { return "Franz\xF6" "sisch"; }

    inline const char* translate(L<de_DE>, L<en_GB>) { return "German"; }
    inline const char* translate(L<de_DE>, L<fr_FR>) { return "Allemand"; }
    inline const char* translate(L<de_DE>, L<de_DE>) { return "Deutsch"; }

    

  }

}

#endif
