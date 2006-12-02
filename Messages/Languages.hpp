// Oliver Kullmann, 10.2.2006 (Swansea)

/*!
  \file Messages/Languages.hpp
  \brief Definitions for identification and handling of (natural) languages supported by the library

  The message system is based on "raw" character sequences, assuming the UTF-8
  interpretation of the output. Easiest input by assuming UTF-8 encoding of the source code file.

  Defined are enumerated constants like en_GB == 0 of type Languages, an array Locales for
  translation of name-constants into name-strings (Locales[en_GB] == "en_GB"), and tagging classes
  like L<en_GB>, where derivation relations give the canonical fall-back languages (that is, locales).

  Associated message services one finds in Messages/messages/Languages.hpp.
*/

#ifndef LANGUAGES_8UyTre
#define LANGUAGES_8UyTre

#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/size.hpp>

namespace OKlib {

  namespace Messages {

    /*!
      \def OKLIB_LANGUAGES
      \brief The list of constants naming the available combinations of language and locale

      These constants are identical to locale-names used by gcc.

      OKLIB_LANGUAGES is a sequence in the sense of the Boost Preprocessor library. If a new element
      is added (for a new (localised) language; at the end or in the middle -- but not at the start, since
      otherwise it would replace the default language), then in order to make it usable, a new partial
      specialisation of ::OKlib::Messages::L needs to be created (see below), and in
      ::OKlib::Messages::messages::Languages a new print-overload for this class
      has to be created as well as extending the switch-statements of the other print-overloads.

      The message class OKlib::Messages::messages::LanguageNames outputs explanations for
      all locale-names.
    */

#define OKLIB_LANGUAGES (en_GB) (en_US) (en_CA) (en_AU) (fr_FR) (fr_CH) (fr_CA) (de_DE) (de_AT) (de_CH)

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

    //! Implementation detail (used only for OKLIB_QUOTED_SEQUENCE)
#define OKLIB_QUOTE(r, data, elem) (BOOST_PP_STRINGIZE(elem))
    //! Implementation detail (used only for the initialisation of Locales)
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

      Specialisations of L<language> stand in derivation relations to each other, where the immediate
      base class is the fall-back language (more precisely, locale) used in case L<language> is not
      implemented (for some message).
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
    template <>
    struct L<fr_CA> : L<fr_FR> {
      static const Languages value = fr_CA;
    };

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

  }

}

#endif
