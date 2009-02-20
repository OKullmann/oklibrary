// Oliver Kullmann, 11.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/MessagesMain.hpp
  \brief The complete messages framework (including the macro support).

  The (public) macros defined here are
   - OKLIB_USING_MESSAGES introduces using-declaration for the main messages constructs
   - OKLIB_MESSAGES_PRINT defines the basic print function in a message class.

  Besides that the (public) macros
   - OKLIB_ACTIVE_LANGUAGES
   - OKLIB_DEFAULT_LANGUAGE

  change the basic language setting, while with
   - OKLIB_USING_LANGUAGES
   - OKLIB_USING_LEVELS

  only using-declarations for the languages resp. the levels are introduced.

*/

#ifndef MESSAGESMAIN_8876yHbg
#define MESSAGESMAIN_8876yHbg

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>

#include <OKlib/Programming/Messages/MessagesBase.hpp>
// this is guaranteed to be included

namespace OKlib {
  namespace Messages {

    /*!
      \def OKLIB_ACTIVE_LANGUAGES
      \brief With this macro a subset of the languages can be choosen (as compile-switch). Default is all languages.

      For example compilation, either with plain make or with the buildsystem of the OKlibrary, with
      CXXFLAGS='-DOKLIB_ACTIVE_LANGUAGES="(en_GB)"'
      will result in all languages being removed from messages except of en_GB (and except of the default language --
      see the following macro).

      This list does not take inheritance relations into account; for example if you want to have all German languages,
      then use
      CXXFLAGS='-DOKLIB_ACTIVE_LANGUAGES="(de_DE)(de_CH)(de_AT)"'
      (if then a language not from the German language family is required, then, because of the default language
      setting, en_GB is used; so the messages occurring in the program will (only) use de_DE, de_CH, de_AT and
      en_GB).
    */

#ifndef OKLIB_ACTIVE_LANGUAGES
#define OKLIB_ACTIVE_LANGUAGES OKLIB_LANGUAGES
#endif

    /*!
      \def OKLIB_DEFAULT_LANGUAGE
      \brief The language chosen if the required language is not in the list of active languages. Default is en_GB.

      For example compilation, either with plain make or with the buildsystem of the OKlibrary, with
      CXXFLAGS='-DOKLIB_ACTIVE_LANGUAGES="(de_DE)" -DOKLIB_DEFAULT_LANGUAGE="de_DE"'
      results in only German messages (in the normal form) available.
    */

#ifndef OKLIB_DEFAULT_LANGUAGE
#define OKLIB_DEFAULT_LANGUAGE en_GB
#endif

    //! Only used in OKLIB_STRATA_L
#define OKLIB_STRATA_T BOOST_PP_SEQ_TO_TUPLE(OKLIB_STRATA)
    //! Only used in OKLIB_LEVEL_yUhTr6
#define OKLIB_STRATA_L BOOST_PP_TUPLE_TO_LIST(BOOST_PP_SEQ_SIZE(OKLIB_STRATA), OKLIB_STRATA_T)
    //! Only used in OKLIB_LANG_77TgVf
#define OKLIB_LEVEL_yUhTr6(r, lang, level) case ::OKlib::Messages::level : print(out, ::OKlib::Messages::L< ::OKlib::Messages::lang>(), ::OKlib::Messages::S< ::OKlib::Messages::level>()); break;
    //! Only used in OKLIB_MESSAGES_PRINT
#define OKLIB_LANG_77TgVf(r, data, lang) case ::OKlib::Messages::lang : switch(lev) { BOOST_PP_LIST_FOR_EACH(OKLIB_LEVEL_yUhTr6, lang, OKLIB_STRATA_L) } break;

    /*!
      \def OKLIB_MESSAGES_PRINT
      \brief This macro is to be used inside the definition of a message class,
      and defines the virtual function print(std::ostream&), which (via a nested switch-statement)
      calls for each active value lang of language and lev of level the (undefined)
      member function print(out, L<lang>, S<lev>).

      This macro should follow immediately the class definition.
    */

#define OKLIB_MESSAGES_PRINT std::ostream& print(std::ostream& out) const { \
      const ::OKlib::Messages::Languages lang(language(out)); \
      const ::OKlib::Messages::Strata lev(level(out)); \
      switch (lang) { \
        BOOST_PP_SEQ_FOR_EACH(OKLIB_LANG_77TgVf, , OKLIB_ACTIVE_LANGUAGES) \
        default : print(out, ::OKlib::Messages::L< ::OKlib::Messages::OKLIB_DEFAULT_LANGUAGE>(), ::OKlib::Messages::S< ::OKlib::Messages::Basic>()); break; \
      } \
      return out; \
    }

    /*!
      \def OKLIB_USING_LANGUAGES
      \brief Introduces using declarations for all enumerated constants denoting languages.
    */

    //! Used only in OKLIB_USING_LANGUAGES
#define OKLIB_USING_LANG_yHbv4(r, data, lang) using ::OKlib::Messages::lang;
#define OKLIB_USING_LANGUAGES BOOST_PP_SEQ_FOR_EACH(OKLIB_USING_LANG_yHbv4, , OKLIB_LANGUAGES)

    /*!
      \def OKLIB_USING_LEVELS
      \brief Introduces using declarations for all enumerated constants denoting message-levels.
    */

    //! Used only in OKLIB_USING_LEVELS
#define OKLIB_USING_LEVEL_yHbv4(r, data, level) using ::OKlib::Messages::level;
#define OKLIB_USING_LEVELS BOOST_PP_SEQ_FOR_EACH(OKLIB_USING_LEVEL_yHbv4, , OKLIB_STRATA)

    /*!
      \def OKLIB_USING_MESSAGES
      \brief Introduces using declarations for all enumerated constants denoting languages or message-levels, and for
      the class templates L<lang> and S<strata>.
    */

#define OKLIB_USING_MESSAGES OKLIB_USING_LANGUAGES OKLIB_USING_LEVELS using ::OKlib::Messages::L; using ::OKlib::Messages::S;
  }

}

#endif
