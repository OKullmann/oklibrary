// Oliver Kullmann, 11.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/MessagesBase.hpp
  \brief The base class of the messages framework

  Including this file suffices to get the message service without the
  macro support.
*/

#ifndef MESSAGESBASE_9iJJhnb
#define MESSAGESBASE_9iJJhnb

#include <ios>
#include <iosfwd>
#include <locale>

#include <OKlib/Programming/Messages/Languages.hpp>
#include <OKlib/Programming/Messages/Levels.hpp>

namespace OKlib {
  namespace Messages {

    /*!
      \class MessagesBase
      \brief The polymorphic base class for message classes

      Every message class is derived from MessagesBase.

      In each <code> std::ostream </code> object the current language (of type
      Messages::Languages) and the current level (of type Messages::Strata) are stored,
      with default values
      <code> Messages::en_GB </code> and <code> Messages::Basic </code>;
      when setting the language of a stream, then also the locale
      of this stream is set according to the the current language-value.

      Message objects (objects of some type publically derived from Messages::MessagesBase)
      can be inserted into any ostream-object, and their messages
      will be print according to the language (locale) and level of the stream.

      Public member functions are:
      <ul>
       <li> default constructor, default copy constructor, default copy assignment </li>
       <li> virtual destructor </li>
       <li> <code> static Messages::Languages language(ostream& out) </code> : current language
       of the stream (default is <code> Messages::en_GB </code>) </li>
       <li> <code> static Messages::Strata level(ostream& out) </code> : current level of the stream
         (default is <code> Messages::Basic </code>) </li>
       <li> <code> static ostream& set(ostream& out, Messages::Languages lang, const char* coding = ".utf-8") </code> :
         set the current language (more precisely, locale) of <code>out</code> together
         with the prescribed encoding </li>
       <li> <code> static ostream& set(ostream& out, Messages::Strata level) </code> :
         set the current level of <code>out</code>. </li>
      </ul>

       Pure virtual member functions:
       <ul>
        <li> <code> ostream& print(ostream& out) </code> : to be implemented by each message class;
        prints the message in the current language (more precisely, locale) and level. </li>
       </ul>

       Non-member functions:
       <ul>
        <li> <code> ostream& operator <<(ostream& out, const MessagesBase& m) </code> :
          prints the message by calling <code> m.print(out) </code>. </li>
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
