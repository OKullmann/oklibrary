// Oliver Kullmann, 11.2.2006 (Swansea)

/*!
  \file Messages/MessagesBase.hpp
  \brief The base class of the messages framework

  Including this file suffices to get the message service without the
  macro support.
*/

#ifndef MESSAGESBASE_9iJJhnb
#define MESSAGESBASE_9iJJhnb

#include <ios>
#include <iosfwd>
#include <locale>

#include <Transitional/Messages/Languages.hpp>
#include <Transitional/Messages/Levels.hpp>

namespace OKlib {
  namespace Messages {

    /*!
      \class MessagesBase
      \brief The polymorphic base class for message classes

      Every message class is derived from MessagesBase.

      In each std::ostream object the current language (of type
      Messages::Languages) and the current level (of type Messages::Strata) are stored, with default values
      Messages::en_GB and Messages::Basic; when setting the language of a stream, then also the locale
      of this stream is set according to the the current language-value. Message objects (objects of some type
      publically derived from MessagesBase) can be inserted into any ostream-object, and their messages
      will be print according to the language (locale) and level of the stream.

      Public member functions are:
      <ul>
       <li> default constructor, default copy constructor, default copy assignment </li>
       <li> virtual destructor </li>
       <li> static Messages::Languages language(ostream& out) : current language of the stream
         (default is Messages::en_GB) </li>
       <li> static Messages::Strata level(ostream& out) : current level of the stream
         (default is Messages::Basic) </li>
       <li> static ostream& set(ostream& out, Messages::Languages lang, const char* coding = ".utf-8") :
         set the current language (more precisely, locale) of out together with the prescribed encoding </li>
       <li> static ostream& set(ostream& out, Messages::Strata level) :
         set the current level of out. </li>
      </ul>

       Pure virtual member functions:
       <ul>
        <li> ostream& print(ostream& out) : to be implemented by each message class;
        prints the message in the current language (more precisely, locale) and level. </li>
       </ul>

       Non-member functions:
       <ul>
        <li> ostream& operator <<(ostream& out, const MessagesBase& m) :
          prints the message by calling m.print(out). </li>
       </ul>
    */

    class MessagesBase {
      //! the language-index (for all ostreams)
      static int language_index() {
        static bool uninitialised = true;
        static int index;
        if (uninitialised) {
          index = std::ios_base::xalloc();
          uninitialised = false;
        }
        return index;
      }
      //! the level-index (for all ostreams)
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
      virtual ~MessagesBase() {}
      
      static ::OKlib::Messages::Languages language(std::ostream& out) {
        return static_cast< ::OKlib::Messages::Languages>(out.iword(language_index()));
      }
      static ::OKlib::Messages::Strata level(std::ostream& out) {
        return static_cast< ::OKlib::Messages::Strata>(out.iword(level_index()));
      }

      static std::ostream& set(std::ostream& out, const ::OKlib::Messages::Languages lang, const char* const coding = ".utf-8") {
        out.iword(language_index()) = lang;
        std::string locale_name(::OKlib::Messages::Locales[lang]);
        locale_name += coding;
        out.imbue(std::locale(locale_name.c_str()));
        return out;
      }
      static std::ostream& set(std::ostream& out, const ::OKlib::Messages::Strata level) {
        out.iword(level_index()) = level;
        return out;
      }
    };
    
    inline std::ostream& operator <<(std::ostream& out, const MessagesBase& m) {
      return m.print(out);
    }

  }

}

#endif
