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

#include "MessagesBase.hpp"

namespace OKlib {

  namespace Messages {

#define OKLIB_STRATA_T BOOST_PP_SEQ_TO_TUPLE(OKLIB_STRATA)
#define OKLIB_STRATA_L BOOST_PP_TUPLE_TO_LIST(BOOST_PP_SEQ_SIZE(OKLIB_STRATA), OKLIB_STRATA_T)
#define OKLIB_LEVEL_yUhTr6(r, lang, level) case ::OKlib::Messages::level : print(out, ::OKlib::Messages::L< ::OKlib::Messages::lang>(), ::OKlib::Messages::S< ::OKlib::Messages::level>()); break;
#define OKLIB_LANG_77TgVf(r, data, lang) case ::OKlib::Messages::lang : switch(lev) { BOOST_PP_LIST_FOR_EACH(OKLIB_LEVEL_yUhTr6, lang, OKLIB_STRATA_L) } break;

#define OKLIB_PRINT std::ostream& print(std::ostream& out) const { \
      const ::OKlib::Messages::Languages lang(language(out)); \
      const ::OKlib::Messages::Strata lev(level(out)); \
      switch (lang) { \
        BOOST_PP_SEQ_FOR_EACH(OKLIB_LANG_77TgVf, , OKLIB_LANGUAGES) \
      } \
      return out; \
    }
#define OKLIB_WPRINT std::wostream& print(std::wostream& out) const { \
      const ::OKlib::Messages::Languages lang(language(out)); \
      const ::OKlib::Messages::Strata lev(level(out)); \
      switch (lang) { \
        BOOST_PP_SEQ_FOR_EACH(OKLIB_LANG_77TgVf, , OKLIB_LANGUAGES) \
      } \
      return out; \
    }

    /*!
      \def OKLIB_MESSAGES
      \brief With "OKLIB_MESSAGES(M) {", where M is the name of a message class,
      we open the definition of M.
    */

#define OKLIB_MESSAGES(M) struct M : ::OKlib::Messages::MessagesBase

    /*!
      \def OKLIB_MESSAGES_PRINT
      \brief This macro is to be used inside the definition of a message class,
      and defines the virtual functions print(std::ostream&) and print(std::wostream&).
    */

#define OKLIB_MESSAGES_PRINT OKLIB_PRINT OKLIB_WPRINT

#define OKLIB_USING_LANG_yHbv4(r, data, lang) using ::OKlib::Messages::lang;
#define OKLIB_USING_LANGUAGES BOOST_PP_SEQ_FOR_EACH(OKLIB_USING_LANG_yHbv4, , OKLIB_LANGUAGES)

#define OKLIB_USING_LEVEL_yHbv4(r, data, level) using ::OKlib::Messages::level;
#define OKLIB_USING_LEVELS BOOST_PP_SEQ_FOR_EACH(OKLIB_USING_LEVEL_yHbv4, , OKLIB_STRATA)

#define OKLIB_USING_MESSAGES OKLIB_USING_LANGUAGES OKLIB_USING_LEVELS using ::OKlib::Messages::L; using ::OKlib::Messages::S;
  }

}

#endif
