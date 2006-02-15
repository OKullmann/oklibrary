// Oliver Kullmann, 11.2.2006 (Swansea)

/*!
  \file MessagesBase.hpp
  \brief The base class of the messages framework

  Including this file suffices to get the message service (without the
  macro support).
*/

#ifndef MESSAGESBASE_9iJJhnb

#define MESSAGESBASE_9iJJhnb

#include <ios>
#include <iosfwd>
#include <locale>

#include "Languages.hpp"
#include "Levels.hpp"

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
    
    inline std::ostream& operator <<(std::ostream& out, const MessagesBase& m) {
      return m.print(out);
    }
    inline std::wostream& operator <<(std::wostream& out, const MessagesBase& m) {
      return m.print(out);
    }

  }

}

#endif
