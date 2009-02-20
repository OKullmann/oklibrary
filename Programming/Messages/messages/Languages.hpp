// Oliver Kullmann, 14.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/messages/Languages.hpp
  \brief Explanations of language names

  Defines class Messages::messages::LanguageNames for output of descriptions of languages.
*/

#ifndef LANGUAGESEXPLANATIONS_8Uh26266
#define LANGUAGESEXPLANATIONS_8Uh26266

#include <OKlib/Programming/Messages/MessagesMain.hpp>

namespace OKlib {
  namespace Messages {
    namespace messages {

      /*!
        \class LanguageNames
        \brief Outputs a description for all provided languages (more precisely, locales)

        Constructed with a language-constant.
      */

      OKLIB_USING_MESSAGES

      struct LanguageNames : ::OKlib::Messages::MessagesBase {
        OKLIB_MESSAGES_PRINT

        explicit LanguageNames(const ::OKlib::Messages::Languages lang = ::OKlib::Messages::Languages(0)) : lang(lang) {}

        ::OKlib::Messages::Languages foreign_language() const { return lang; }
        ::OKlib::Messages::Languages& foreign_language() { return lang; }
        
        void print(std::ostream& out, L<en_GB>, S<Basic>) const {
          switch (lang) {
          case en_GB :
            out << "English"; break;
          case en_US :
            out << "English (USA)"; break;
          case en_CA :
            out << "English (Canada)"; break;
          case en_AU :
            out << "English (Australia)"; break;
          case fr_FR :
            out << "French"; break;
          case fr_CH :
            out << "French (Swiss)"; break;
          case fr_CA :
            out << "French (Canada)"; break;
          case de_DE :
            out << "German"; break;
          case de_AT :
            out << "German (Austria)"; break;
          case de_CH :
            out << "German (Swiss)"; break;
          default :
            out << "Untranslated"; break;
          }
        }

        void print(std::ostream& out, L<fr_FR>, S<Basic>) const {
          switch (lang) {
          case en_GB :
            out << "Anglais"; break;
          case en_US :
            out << "Anglais (USA)"; break;
          case en_CA :
            out << "Anglais (Canada)"; break;
          case en_AU :
            out << "Anglais (Australie)"; break;
          case fr_FR :
            out << "Francais"; break;
          case fr_CH :
            out << "Francais (Suisse)"; break;
          case fr_CA :
            out << "Francais (Canada)"; break;
          case de_DE :
            out << "Allemand"; break;
          case de_AT :
            out << "Allemand (Austria ??? )"; break;
          case de_CH :
            out << "Allemand (Suisse)"; break;
          default :
            out << "???"; break;
          }
        }

        void print(std::ostream& out, L<de_DE>, S<Basic>) const {
          switch (lang) {
          case en_GB :
            out << "Englisch"; break;
          case en_US :
            out << "Englisch (USA)"; break;
          case en_CA :
            out << "Englisch (Kanada)"; break;
          case en_AU :
            out << "Englisch (Australien)"; break;
          case fr_FR :
            out << "Französisch"; break;
          case fr_CH :
            out << "Französisch (Schweiz)"; break;
          case fr_CA :
            out << "Französisch (Kanada)"; break;
          case de_DE :
            out << "Deutsch"; break;
          case de_AT :
            out << "Deutsch (Österreich)"; break;
          case de_CH :
            out << "Deutsch (Schweiz)"; break;
          default :
            out << "Ohne Übersetzung"; break;
          }
        }
        
      private :
        
        ::OKlib::Messages::Languages lang;
        
      };
    }
    
  }
  
}

#endif
