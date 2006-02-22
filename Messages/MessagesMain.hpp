// Oliver Kullmann, 11.2.2006 (Swansea)

/*!
  \file Messages.hpp
  \brief The complete messages framework (including the macro support).
*/

#ifndef MESSAGESDESIGNSTUDY_8876yHbg

#define MESSAGESDESIGNSTUDY_8876yHbg

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>

#include <Transitional/Messages/MessagesBase.hpp>

namespace OKlib {

  namespace Messages {

#define OKLIB_STRATA_T BOOST_PP_SEQ_TO_TUPLE(OKLIB_STRATA)
#define OKLIB_STRATA_L BOOST_PP_TUPLE_TO_LIST(BOOST_PP_SEQ_SIZE(OKLIB_STRATA), OKLIB_STRATA_T)
#define OKLIB_LEVEL_yUhTr6(r, lang, level) case ::OKlib::Messages::level : print(out, ::OKlib::Messages::L< ::OKlib::Messages::lang>(), ::OKlib::Messages::S< ::OKlib::Messages::level>()); break;
#define OKLIB_LANG_77TgVf(r, data, lang) case ::OKlib::Messages::lang : switch(lev) { BOOST_PP_LIST_FOR_EACH(OKLIB_LEVEL_yUhTr6, lang, OKLIB_STRATA_L) } break;

    /*!
      \def OKLIB_ACTIVE_LANGUAGES
      \brief With this macro a subset of the languages can be choosen (as compile-switch). Default is all languages.

      For example compilation either with plain make or with the buildsystem of the OKlibrary with
      CXXFLAGS='-DOKLIB_ACTIVE_LANGUAGES="(en_GB)"'
      all languages are removed from messages except of en_GB (and except of the default language --
      see the following macro).

      This list does not take inheritance relations into account; for example if you want to have all German languages,
      then use
      CXXFLAGS='-DOKLIB_ACTIVE_LANGUAGES="(de_DE)(de_CH)(de_AT)"'
      (if then a language not from the German language family is required, then because of the default language
      setting en_GB is used; so the messages occurring in the program will (only) be de_DE, de_CH, de_AT and
      en_GB).
    */

#ifndef OKLIB_ACTIVE_LANGUAGES
#define OKLIB_ACTIVE_LANGUAGES OKLIB_LANGUAGES
#endif

    /*!
      \def OKLIB_DEFAULT_LANGUAGE
      \brief The language chosen if the required language is not in the list of active languages. Default is en_GB.

      For example compilation either with plain make or with the buildsystem of the OKlibrary with
      CXXFLAGS='-DOKLIB_ACTIVE_LANGUAGES="(de_DE)" -DOKLIB_DEFAULT_LANGUAGE="de_DE"'
      results in only German messages (in the normal form) available.
    */

#ifndef OKLIB_DEFAULT_LANGUAGE
#define OKLIB_DEFAULT_LANGUAGE en_GB
#endif

    /*!
      \def OKLIB_MESSAGES_PRINT
      \brief This macro is to be used inside the definition of a message class,
      and defines the virtual function print(std::ostream&).
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
      \def OKLIB_MESSAGES
      \brief With "OKLIB_MESSAGES(M) {", where M is the name of a message class,
      we open the definition of M.
    */

#define OKLIB_MESSAGES(M) struct M : ::OKlib::Messages::MessagesBase

#define OKLIB_USING_LANG_yHbv4(r, data, lang) using ::OKlib::Messages::lang;
#define OKLIB_USING_LANGUAGES BOOST_PP_SEQ_FOR_EACH(OKLIB_USING_LANG_yHbv4, , OKLIB_LANGUAGES)

#define OKLIB_USING_LEVEL_yHbv4(r, data, level) using ::OKlib::Messages::level;
#define OKLIB_USING_LEVELS BOOST_PP_SEQ_FOR_EACH(OKLIB_USING_LEVEL_yHbv4, , OKLIB_STRATA)

#define OKLIB_USING_MESSAGES OKLIB_USING_LANGUAGES OKLIB_USING_LEVELS using ::OKlib::Messages::L; using ::OKlib::Messages::S;
  }

}

#endif
