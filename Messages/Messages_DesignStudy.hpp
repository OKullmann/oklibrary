// Oliver Kullmann, 11.2.2006 (Swansea)

/*!
  \file Messages_DesignStudy.hpp
  \brief Design study for the messages framework

  For the definition of a message class exactly this file needs to be included.
  \todo The macro definitions should be put into a different file (which is guaranteed
  to include this file).
  \todo Write macros which create using declarations for the language and the level
  constants, and for the L- and S-templates.
*/

#ifndef MESSAGESDESIGNSTUDY_8876yHbg

#define MESSAGESDESIGNSTUDY_8876yHbg

#include <ios>
#include <iosfwd>
#include <locale>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/list/for_each.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/tuple/to_list.hpp>

#include "Languages_DesignStudy.hpp"
#include "Levels_DesignStudy.hpp"

namespace OKlib {

  namespace Messages {

    /*!
      \class MessagesBase
      \brief The polymorphic base class for message classes
    */

    class MessagesBase {
      static int language_index() {
        static bool uninitialised = true;
        static int index;
        if (uninitialised) {
          index = std::ios_base::xalloc();
          uninitialised = false;
        }
        return index;
      }
      static int level_index() {
        static bool uninitialised = true;
        static int index;
        if (uninitialised) {
          index = std::ios_base::xalloc();
          uninitialised = false;
        }
        return index;
      }

    public :

      virtual std::ostream& print(std::ostream&) const = 0;
      virtual std::wostream& print(std::wostream&) const = 0;
      virtual ~MessagesBase() {}
      
      template <class Stream>
      static ::OKlib::Messages::Languages language(Stream& out) {
        return static_cast< ::OKlib::Messages::Languages>(out.iword(language_index()));
      }
      template <class Stream>
      static ::OKlib::Messages::Strata level(Stream& out) {
        return static_cast< ::OKlib::Messages::Strata>(out.iword(level_index()));
      }

      template <class Stream>
      static Stream& set(Stream& out, const ::OKlib::Messages::Languages lang) {
        out.iword(language_index()) = lang;
        out.imbue(std::locale(::OKlib::Messages::Locales[lang]));
        return out;
      }
      template <class Stream>
      static Stream& set(Stream& out, const ::OKlib::Messages::Strata level) {
        out.iword(level_index()) = level;
        return out;
      }
    };
    
    std::ostream& operator <<(std::ostream& out, const MessagesBase& m) {
      return m.print(out);
    }
    std::wostream& operator <<(std::wostream& out, const MessagesBase& m) {
      return m.print(out);
    }

#define OKLIB_STRATA_T BOOST_PP_SEQ_TO_TUPLE(OKLIB_STRATA)
#define OKLIB_STRATA_L BOOST_PP_TUPLE_TO_LIST(BOOST_PP_SEQ_SIZE(OKLIB_STRATA), OKLIB_STRATA_T)
#define OKLIB_LEVEL(r, lang, level) case ::OKlib::Messages::level : print(out, ::OKlib::Messages::L< ::OKlib::Messages::lang>(), ::OKlib::Messages::S< ::OKlib::Messages::level>()); break;
#define OKLIB_LANG(r, data, lang) case ::OKlib::Messages::lang : switch(lev) { BOOST_PP_LIST_FOR_EACH(OKLIB_LEVEL, lang, OKLIB_STRATA_L) } break;

#define OKLIB_PRINT std::ostream& print(std::ostream& out) const { \
      const ::OKlib::Messages::Languages lang(language(out)); \
      const ::OKlib::Messages::Strata lev(level(out)); \
      switch (lang) { \
        BOOST_PP_SEQ_FOR_EACH(OKLIB_LANG, , OKLIB_LANGUAGES) \
      } \
      return out; \
    }
#define OKLIB_WPRINT std::wostream& print(std::wostream& out) const { \
      const ::OKlib::Messages::Languages lang(language(out)); \
      const ::OKlib::Messages::Strata lev(level(out)); \
      switch (lang) { \
        BOOST_PP_SEQ_FOR_EACH(OKLIB_LANG, , OKLIB_LANGUAGES) \
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

  }

}

#endif
